#include "../Common/Include/stm32l051xx.h"
#include <stdio.h>
#include <stdlib.h>
#include "../Common/Include/serial.h"
#include "lcd.h"
#include <string.h>
#include "adc.h"
#include <math.h>

#define F_CPU 32000000L
#define DEF_F 16230L // 16.25 kHz freq (frequency with highest output)

volatile int PWM_Counter = 0;
volatile unsigned char pwm1=100, pwm2=100;

void wait_1ms(void)
{
	// For SysTick info check the STM32l0xxx Cortex-M0 programming manual.
	SysTick->LOAD = (F_CPU/1000L) - 1;  // set reload register, counter rolls over from zero, hence -1
	SysTick->VAL = 0; // load the SysTick counter
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk; // Enable SysTick IRQ and SysTick Timer */
	while((SysTick->CTRL & BIT16)==0); // Bit 16 is the COUNTFLAG.  True when counter rolls over from zero.
	SysTick->CTRL = 0x00; // Disable Systick counter
}

void delayms(int len)
{
	while(len--) wait_1ms();
}

// Interrupt service routines are the same as normal
// subroutines (or C funtions) in Cortex-M microcontrollers.
// The following should happen at a rate of 1kHz.
// The following function is associated with the TIM2 interrupt 
// via the interrupt vector table defined in startup.c
void TIM2_Handler(void) 
{
	TIM2->SR &= ~BIT0; // clear update interrupt flag
	
	GPIOA->ODR ^= BIT11;
	
	GPIOA->ODR ^= BIT12;
	
}

// LQFP32 pinout
//             ----------
//       VDD -|1       32|- VSS
//      PC14 -|2       31|- BOOT0
//      PC15 -|3       30|- PB7 (horn)
//      NRST -|4       29|- PB6 
//      VDDA -|5       28|- PB5 
//LCD_RS PA0 -|6       27|- PB4 (reverse button)
// LCD_E PA1 -|7       26|- PB3 (forward button)
//LCD_D4 PA2 -|8       25|- PA15 (right button) (joystick y axis)
//LCD_D5 PA3 -|9       24|- PA14 (left button) (joystick x axis)
//LCD_D6 PA4 -|10      23|- PA13 (switch mode button) 
//LCD_D7 PA5 -|11      22|- PA12 (h bridge pin 2)
//       PA6 -|12      21|- PA11 (h bridge pin 1)
//       PA7 -|13      20|- PA10 (Reserved for RXD)
// yaxis PB0 -|14      19|- PA9  (Reserved for TXD)
// xaxis PB1 -|15      18|- PA8
//       VSS -|16      17|- VDD
//             ----------

// Joystick ADC
void ADC_Config(void)
{
	// Configure the pin used for a blinking LED: PA8 (pin 18)
	RCC->IOPENR  |= BIT0; // peripheral clock enable for port A
    GPIOA->MODER  = (GPIOA->MODER & ~(BIT17|BIT16) ) | BIT16; // Make pin PA8 output (page 200 of RM0451, two bits used to configure: bit0=1, bit1=0))
	
	// Configure the pin used for analog input: PB1 (pin 15)
	RCC->IOPENR  |= BIT1;         // peripheral clock enable for port B
	GPIOB->MODER |= (BIT2|BIT3);  // Select analog mode for PB1 (pin 15 of LQFP32 package)
	GPIOB->MODER |= (BIT0|BIT1); // analog mode for PB0
}

void initADC(void)
{
	RCC->APB2ENR |= BIT9; // peripheral clock enable for ADC (page 175 or RM0451)

	// ADC clock selection procedure (page 746 of RM0451)
	/* (1) Select PCLK by writing 11 in CKMODE */
	ADC1->CFGR2 |= ADC_CFGR2_CKMODE; /* (1) */
	
	// ADC enable sequence procedure (page 745 of RM0451)
	/* (1) Clear the ADRDY bit */
	/* (2) Enable the ADC */
	/* (3) Wait until ADC ready */
	ADC1->ISR |= ADC_ISR_ADRDY; /* (1) */
	ADC1->CR |= ADC_CR_ADEN; /* (2) */
	if ((ADC1->CFGR1 & ADC_CFGR1_AUTOFF) == 0)
	{
		while ((ADC1->ISR & ADC_ISR_ADRDY) == 0) /* (3) */
		{
			/* For robust implementation, add here time-out management */
		}
	}	

	// Calibration code procedure (page 745 of RM0451)
	/* (1) Ensure that ADEN = 0 */
	/* (2) Clear ADEN */
	/* (3) Set ADCAL=1 */
	/* (4) Wait until EOCAL=1 */
	/* (5) Clear EOCAL */
	if ((ADC1->CR & ADC_CR_ADEN) != 0) /* (1) */
	{
		ADC1->CR |= ADC_CR_ADDIS; /* (2) */
	}
	ADC1->CR |= ADC_CR_ADCAL; /* (3) */
	while ((ADC1->ISR & ADC_ISR_EOCAL) == 0) /* (4) */
	{
		/* For robust implementation, add here time-out management */
	}
	ADC1->ISR |= ADC_ISR_EOCAL; /* (5) */
}

int readADC(unsigned int channel)
{
	// Single conversion sequence code example - Software trigger (page 746 of RM0451)
	/* (1) Select HSI16 by writing 00 in CKMODE (reset value) */
	/* (2) Select the auto off mode */
	/* (3) Select channel */
	/* (4) Select a sampling mode of 111 i.e. 239.5 ADC clk to be greater than17.1us */
	/* (5) Wake-up the VREFINT (only for VRefInt) */
	// ADC1->CFGR2 &= ~ADC_CFGR2_CKMODE; /* (1) */
	ADC1->CFGR1 |= ADC_CFGR1_AUTOFF; /* (2) */
	ADC1->CHSELR = channel; /* (3) */
	ADC1->SMPR |= ADC_SMPR_SMP_0 | ADC_SMPR_SMP_1 | ADC_SMPR_SMP_2; /* (4) */
	//printf("go through readADC\n");
	if(channel==ADC_CHSELR_CHSEL17)
	{
		ADC->CCR |= ADC_CCR_VREFEN; /* (5) */
	}
	
	/* Performs the ADC conversion */
	ADC1->CR |= ADC_CR_ADSTART; /* start the ADC conversion */
	while ((ADC1->ISR & ADC_ISR_EOC) == 0) /* wait end of conversion */
	{
		/* For robust implementation, add here time-out management */
	}

	return ADC1->DR; // ADC_DR has the 12 bits out of the ADC
}

// LCD
void Configure_Pins (void)
{
	RCC->IOPENR |= BIT0; // peripheral clock enable for port A
	
	// Make pins PA0 to PA5 outputs (page 200 of RM0451, two bits used to configure: bit0=1, bit1=0)
    GPIOA->MODER = (GPIOA->MODER & ~(BIT0|BIT1)) | BIT0; // PA0
	GPIOA->OTYPER &= ~BIT0; // Push-pull
    
    GPIOA->MODER = (GPIOA->MODER & ~(BIT2|BIT3)) | BIT2; // PA1
	GPIOA->OTYPER &= ~BIT1; // Push-pull
    
    GPIOA->MODER = (GPIOA->MODER & ~(BIT4|BIT5)) | BIT4; // PA2
	GPIOA->OTYPER &= ~BIT2; // Push-pull
    
    GPIOA->MODER = (GPIOA->MODER & ~(BIT6|BIT7)) | BIT6; // PA3
	GPIOA->OTYPER &= ~BIT3; // Push-pull
    
    GPIOA->MODER = (GPIOA->MODER & ~(BIT8|BIT9)) | BIT8; // PA4
	GPIOA->OTYPER &= ~BIT4; // Push-pull
    
    GPIOA->MODER = (GPIOA->MODER & ~(BIT10|BIT11)) | BIT10; // PA5
	GPIOA->OTYPER &= ~BIT5; // Push-pull
}

// Uses SysTick to delay <us> micro-seconds. 
void Delay_us(unsigned char us)
{
	// For SysTick info check the STM32L0xxx Cortex-M0 programming manual page 85.
	SysTick->LOAD = (F_CPU/(1000000L/us)) - 1;  // set reload register, counter rolls over from zero, hence -1
	SysTick->VAL = 0; // load the SysTick counter
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk; // Enable SysTick IRQ and SysTick Timer */
	while((SysTick->CTRL & BIT16)==0); // Bit 16 is the COUNTFLAG.  True when counter rolls over from zero.
	SysTick->CTRL = 0x00; // Disable Systick counter
}

void waitms (unsigned int ms)
{
	unsigned int j;
	unsigned char k;
	for(j=0; j<ms; j++)
		for (k=0; k<4; k++) Delay_us(250);
}

void LCD_pulse (void)
{
	LCD_E_1;
	Delay_us(40);
	LCD_E_0;
}

void LCD_byte (unsigned char x)
{
	//Send high nible
	if(x&0x80) LCD_D7_1; else LCD_D7_0;
	if(x&0x40) LCD_D6_1; else LCD_D6_0;
	if(x&0x20) LCD_D5_1; else LCD_D5_0;
	if(x&0x10) LCD_D4_1; else LCD_D4_0;
	LCD_pulse();
	Delay_us(40);
	//Send low nible
	if(x&0x08) LCD_D7_1; else LCD_D7_0;
	if(x&0x04) LCD_D6_1; else LCD_D6_0;
	if(x&0x02) LCD_D5_1; else LCD_D5_0;
	if(x&0x01) LCD_D4_1; else LCD_D4_0;
	LCD_pulse();
}

void WriteData (unsigned char x)
{
	LCD_RS_1;
	LCD_byte(x);
	waitms(2);
}

void WriteCommand (unsigned char x)
{
	LCD_RS_0;
	LCD_byte(x);
	waitms(5);
}

void LCD_4BIT (void)
{
	LCD_E_0; // Resting state of LCD's enable is zero
	//LCD_RW=0; // We are only writing to the LCD in this program
	waitms(20);
	// First make sure the LCD is in 8-bit mode and then change to 4-bit mode
	WriteCommand(0x33);
	WriteCommand(0x33);
	WriteCommand(0x32); // Change to 4-bit mode

	// Configure the LCD
	WriteCommand(0x28);
	WriteCommand(0x0c);
	WriteCommand(0x01); // Clear screen command (takes some time)
	waitms(20); // Wait for clear screen command to finsih.
}

void LCDprint(char * string, unsigned char line, unsigned char clear)
{
	int j;

	WriteCommand(line==2?0xc0:0x80);
	waitms(5);
	for(j=0; string[j]!=0; j++)	WriteData(string[j]);// Write the message
	if(clear) for(; j<CHARS_PER_LINE; j++) WriteData(' '); // Clear the rest of the line
}


void Hardware_Init()
{
	// Set up output pins
	RCC->IOPENR |= BIT0; // peripheral clock enable for port A
    GPIOA->MODER = (GPIOA->MODER & ~(BIT22|BIT23)) | BIT22; // Make pin PA11 output (page 200 of RM0451, two bits used to configure: bit0=1, bit1=0)
	GPIOA->OTYPER &= ~BIT11; // Push-pull
    GPIOA->MODER = (GPIOA->MODER & ~(BIT24|BIT25)) | BIT24; // Make pin PA12 output (page 200 of RM0451, two bits used to configure: bit0=1, bit1=0)
	GPIOA->OTYPER &= ~BIT12; // Push-pull

	//Button for switching modes
	GPIOA->MODER &= ~(BIT26 | BIT27); // Make pin PA13 input button
	// Activate pull up for pin PA14:
	GPIOA->PUPDR |= BIT26;
	GPIOA->PUPDR &= ~(BIT27);

	//Button for left
	GPIOA->MODER &= ~(BIT28 | BIT29); // Make pin PA14 input button
	// Activate pull up for pin PA14:
	GPIOA->PUPDR |= BIT28;
	GPIOA->PUPDR &= ~(BIT29);

	//Button for right
	GPIOA->MODER &= ~(BIT30 | BIT31); // Make pin PA15 input button
	// Activate pull up for pin PA15:
	GPIOA->PUPDR |= BIT30;
	GPIOA->PUPDR &= ~(BIT31);

	// PB pins init
	RCC->IOPENR |= RCC_IOPENR_GPIOBEN;
	GPIOB->MODER &= ~GPIO_MODER_MODE3_0; // PB3 forward button
	GPIOB->MODER &= ~GPIO_MODER_MODE4_0; // PB4 reverse button
	GPIOB->MODER &= ~GPIO_MODER_MODE7_0; // horn
	GPIOB->PUPDR = (GPIOB->PUPDR & ~(GPIO_PUPDR_PUPD3_1 | GPIO_PUPDR_PUPD4_1 | GPIO_PUPDR_PUPD7_1))
                    | (GPIO_PUPDR_PUPD3_0 | GPIO_PUPDR_PUPD4_0 | GPIO_PUPDR_PUPD7_0);

	/* //Button for forward
	GPIOA->MODER &= ~(BIT28 | BIT29); // Make pin PA14 input button
	// Activate pull up for pin PA14:
	GPIOA->PUPDR |= BIT28;
	GPIOA->PUPDR &= ~(BIT29);

	//Button for reverse
	GPIOA->MODER &= ~(BIT30 | BIT31); // Make pin PA15 input button
	// Activate pull up for pin PA15:
	GPIOA->PUPDR |= BIT30;
	GPIOA->PUPDR &= ~(BIT31);*/

	// Set up timer
	RCC->APB1ENR |= BIT0;  // turn on clock for timer2 (UM: page 177)
	TIM2->ARR = F_CPU/(DEF_F*2)-1;
	NVIC->ISER[0] |= BIT15; // enable timer 2 interrupts in the NVIC
	TIM2->CR1 |= BIT4;      // Downcounting    
	TIM2->CR1 |= BIT7;      // ARPE enable    
	TIM2->DIER |= BIT0;     // enable update event (reload event) interrupt 
	TIM2->CR1 |= BIT0;      // enable counting    
	
	__enable_irq();
}

// Command generator 
void sendCommandGenerator(int command) {
	// turn off signal to synchronize 
	NVIC->ICER[0] |= BIT15;
	delayms(62); // bit 0 turn off signal
	NVIC->ISER[0] |= BIT15; 
	delayms(62); // bit 1 turn on signal for 62 ms

	// turn left
	if (command == 1) {
		NVIC->ICER[0] |= BIT15;
		delayms(124); // bit 0 0 
		NVIC->ISER[0] |= BIT15;

		// sequence is bit 1 0 0 1
	}
	else if (command == 2) {
		// turn off signal for 62 ms 
		NVIC->ICER[0] |= BIT15;
		delayms(62); // bit 0
		NVIC->ISER[0] |= BIT15;
		delayms(62); // bit 1
		NVIC->ICER[0] |= BIT15;
		delayms(62); // bit 0
		NVIC->ISER[0] |= BIT15;

		// sequence is 1 0 1 0
	}
	else if (command == 3) {
		// turn off signal for 62 ms
		NVIC->ICER[0] |= BIT15;
		delayms(62); // bit 0
		NVIC->ISER[0] |= BIT15;
		delayms(62); // bit 1
		delayms(62); // bit 1

		// sequence is 1 0 1 1 
	}
	else if (command == 4) {
		delayms(62); // tur
		// sequence is 1 1 0 0
	}
}

int main(void)
{
	// char buff[32];
	int npwm;
	int mode = 1;
	int mode_button;
	int current_forward, current_reverse, current_left, current_right, horn = 1;
	//int j;
	//int k;
	int x;
	int y;
	delayms(500); // Give PuTTY time to start


	// ADC initialization
	ADC_Config();
	initADC();

	Hardware_Init();

	Configure_Pins();
	LCD_4BIT();

	GPIOA->ODR |= BIT11;

	GPIOA->ODR &= ~BIT12;

	delayms(500); // Give putty a chance to start before we send characters with printf()
	
	LCDprint("Control Mode", 1, 1);

	/*printf("Servo signal generatioin using the STM32L051 using TIM2\r\n");
	printf("(outputs are PA11 and PA12, pins 21 and 22).\r\n");
	printf("By Jesus Calvino-Fraga (c) 2018-2023.\r\n\r\n");*/

	while (1)
	{
		//mode_button = (GPIOA->IDR & GPIO_IDR_ID13) ? 1 : 0;

		x=readADC(ADC_CHSELR_CHSEL9); // x-axis
		y=readADC(ADC_CHSELR_CHSEL8); // y-axis

		// y-axis
		//y = (j*3.3)/4096.0;
		//printf("x=%d V\n", x);
		//printf("y=%d V\n\r", y);
		//fflush(stdout);


		if(x>3750 || x<345){
			if(x>3700){
				current_left = 0;
			}
			else {
				current_right = 0;
			}
		}
		else if (y>3700 || y<500) {
			if (y>3700) {
				current_reverse = 0;
			} 
			else {
				current_forward = 0;
			}
		}


		// x-axis
		// x = (k*3.3)/4096.0;
		// printf("ADC[9]=%d y=%f V\n\r", k, x);
		// fflush(stdout);
		
		if (mode_button == 0) {
			if(mode == 1){
				mode = 0;
				LCDprint("Follow Mode", 1, 1);
				LCDprint(" ", 2, 1);
			}
			else {
				mode = 1;
				LCDprint("Control Mode", 1, 1);
			}			

		// Mode change 
		// Disable timer 2 interrupts in the NVIC
			NVIC->ICER[0] |= BIT15;
			delayms(55);
			NVIC->ISER[0] |= BIT15; // enable timer 2 interrupts in the NVIC
		}

		// forward button
		if (mode == 1)
		{
			// horn
			horn = (GPIOB->IDR & GPIO_IDR_ID7) ? 1 : 0;

			if (horn == 0)
			{
				LCDprint("HONK", 2, 1);
				NVIC->ICER[0] |= BIT15;
				delayms(60);
				NVIC->ISER[0] |= BIT15;
				delayms(250);
			}

			// forward
			// current_forward = (GPIOB->IDR & GPIO_IDR_ID3) ? 1 : 0; // off state (button is state 1 if not pressed, button is state 0 if pressed)

			if (current_forward == 0)
			{
				LCDprint("Forward", 2, 1);
				NVIC->ICER[0] |= BIT15;
				delayms(700);
				NVIC->ISER[0] |= BIT15;
				//delayms(250);
			}

			// while (current_forward == 0) // while button is pressed
			// {
			// 	printf("PB3\n");
			// 	current_forward = (GPIOB->IDR & GPIO_IDR_ID3)?1:0;
			// 	fflush(stdout);
			// delayms(50);
			//}

			
			// reverse button
			//current_reverse = (GPIOB->IDR & GPIO_IDR_ID4) ? 1 : 0;

			if (current_reverse == 0)
			{
				//printf("pb4\r\n");
				LCDprint("Backwards", 2, 1);
				NVIC->ICER[0] |= BIT15;
				delayms(300);
				NVIC->ISER[0] |= BIT15;
				delayms(250);
			}

			// left button
			//current_left = (GPIOA->IDR & GPIO_IDR_ID14) ? 1 : 0;

			if (current_left == 0)
			{
				LCDprint("Left", 2, 1);
				NVIC->ICER[0] |= BIT15;
				delayms(250);
				NVIC->ISER[0] |= BIT15;
				delayms(250);
			}

			// right button
			//current_right = (GPIOA->IDR & GPIO_IDR_ID15) ? 1 : 0;
			
			if (current_right == 0)
			{
				LCDprint("Right", 2, 1);
				NVIC->ICER[0] |= BIT15;
				delayms(62);
				NVIC->ISER[0] |= BIT15;
				delayms(250);
			}
		}
		
		if (horn == 1 && current_forward == 1 && current_reverse == 1 && current_left == 1 && current_right == 1) {
			LCDprint(" ", 2, 1);
		}

		current_forward = 1;
		current_reverse = 1;
		current_right = 1;
		current_left = 1;
		delayms(500);
		//delayms(100);

	}

}

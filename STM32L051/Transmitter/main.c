#include "../Common/Include/stm32l051xx.h"
#include <stdio.h>
#include <stdlib.h>
#include "../Common/Include/serial.h"

#define F_CPU 32000000L
#define DEF_F 16200L // 16.20 kHz freq (frequency with highest output)

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
//      PC15 -|3       30|- PB7
//      NRST -|4       29|- PB6
//      VDDA -|5       28|- PB5
//       PA0 -|6       27|- PB4 (reverse button)
//       PA1 -|7       26|- PB3 (forward button)
//       PA2 -|8       25|- PA15 (right button)
//       PA3 -|9       24|- PA14 (left button)
//       PA4 -|10      23|- PA13 (switch mode button)
//       PA5 -|11      22|- PA12 (h bridge pin 2)
//       PA6 -|12      21|- PA11 (h bridge pin 1)
//       PA7 -|13      20|- PA10 (Reserved for RXD)
//       PB0 -|14      19|- PA9  (Reserved for TXD)
//       PB1 -|15      18|- PA8
//       VSS -|16      17|- VDD
//             ----------




void Hardware_Init()
{
	// Set up output pins
	RCC->IOPENR |= BIT0; // peripheral clock enable for port A
    GPIOA->MODER = (GPIOA->MODER & ~(BIT22|BIT23)) | BIT22; // Make pin PA11 output (page 200 of RM0451, two bits used to configure: bit0=1, bit1=0)
	GPIOA->OTYPER &= ~BIT11; // Push-pull
    GPIOA->MODER = (GPIOA->MODER & ~(BIT24|BIT25)) | BIT24; // Make pin PA12 output (page 200 of RM0451, two bits used to configure: bit0=1, bit1=0)
	GPIOA->OTYPER &= ~BIT12; // Push-pull

	// Set up input pins
	

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
	GPIOB->PUPDR = (GPIOB->PUPDR & ~(GPIO_PUPDR_PUPD3_1 | GPIO_PUPDR_PUPD4_1))
                    | (GPIO_PUPDR_PUPD3_0 | GPIO_PUPDR_PUPD4_0);

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


int main(void)
{
    //char buff[32];
    int npwm;
	int mode = 0;
	int current_forward, current_reverse, current_left, current_right;

	int forward_button_state, reverse_button_state, left_button_state, right_button_state = 0;


	Hardware_Init();
	
	GPIOA->ODR |= BIT11;
	
	GPIOA->ODR &= ~BIT12;

	delayms(500); // Give putty a chance to start before we send characters with printf()
	
    /*printf("Servo signal generatioin using the STM32L051 using TIM2\r\n");
    printf("(outputs are PA11 and PA12, pins 21 and 22).\r\n");
    printf("By Jesus Calvino-Fraga (c) 2018-2023.\r\n\r\n");*/

	while (1)
	{
    	/*while button helde{

			set left_flag
			delay_ms(100)
		}*/

		// forward button
		current_forward=(GPIOB->IDR & GPIO_IDR_ID3)?1:0;
		if(current_forward!=forward_button_state)
		{
			forward_button_state=current_forward;
			delayms(50);
			if (forward_button_state == 0) {
				printf("PB3");
				fflush(stdout);
			}
		}

		// reverse button
		current_reverse=(GPIOB->IDR & GPIO_IDR_ID4)?1:0;
		if(current_reverse!=reverse_button_state)
		{
			reverse_button_state=current_reverse;
			delayms(50);
			if (reverse_button_state == 0) {
				fflush(stdout);
			}
		}

		// left button 
		current_left=(GPIOA->IDR & GPIO_IDR_ID14)?1:0;
		if(current_left!=left_button_state)
		{
			left_button_state=current_left;
			delayms(50);
			if (left_button_state == 0) {
				printf("Left");
				fflush(stdout);
			}
		}

		// right button
		current_right=(GPIOA->IDR & GPIO_IDR_ID15)?1:0;
		if(current_right!=right_button_state)
		{
			right_button_state=current_right;
			delayms(50);
			if (right_button_state == 0) {
				printf("Right");
				fflush(stdout);
			}
		}

		delayms(50);
	}
}

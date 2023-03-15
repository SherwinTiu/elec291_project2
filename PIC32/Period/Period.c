#include <XC.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/attribs.h>
#include "lcd.h"
 
// Configuration Bits (somehow XC32 takes care of this)
#pragma config FNOSC = FRCPLL       // Internal Fast RC oscillator (8 MHz) w/ PLL
#pragma config FPLLIDIV = DIV_2     // Divide FRC before PLL (now 4 MHz)
#pragma config FPLLMUL = MUL_20     // PLL Multiply (now 80 MHz)
#pragma config FPLLODIV = DIV_2     // Divide After PLL (now 40 MHz)
#pragma config FWDTEN = OFF         // Watchdog Timer Disabled
#pragma config FPBDIV = DIV_1       // PBCLK = SYCLK
#pragma config FSOSCEN = OFF        // Secondary Oscillator Enable (Disabled)

// Defines
#define SYSCLK 40000000L
#define Baud2BRG(desired_baud)( (SYSCLK / (16*desired_baud))-1)

#define RESISTOR 1500
 
void UART2Configure(int baud_rate)
{
    // Peripheral Pin Select
    U2RXRbits.U2RXR = 4;    //SET RX to RB8
    RPB9Rbits.RPB9R = 2;    //SET RB9 to TX

    U2MODE = 0;         // disable autobaud, TX and RX enabled only, 8N1, idle=HIGH
    U2STA = 0x1400;     // enable TX and RX
    U2BRG = Baud2BRG(baud_rate); // U2BRG = (FPb / (16*baud)) - 1
    
    U2MODESET = 0x8000;     // enable UART2
}

// Needed to by scanf() and gets()
int _mon_getc(int canblock)
{
	char c;
	
    if (canblock)
    {
	    while( !U2STAbits.URXDA); // wait (block) until data available in RX buffer
	    c=U2RXREG;
        while( U2STAbits.UTXBF);    // wait while TX buffer full
        U2TXREG = c;          // echo
	    if(c=='\r') c='\n'; // When using PUTTY, pressing <Enter> sends '\r'.  Ctrl-J sends '\n'
		return (int)c;
    }
    else
    {
        if (U2STAbits.URXDA) // if data available in RX buffer
        {
		    c=U2RXREG;
		    if(c=='\r') c='\n';
			return (int)c;
        }
        else
        {
            return -1; // no characters to return
        }
    }
}

// Use the core timer to wait for 1 ms.
void wait_1ms(void)
{
    unsigned int ui;
    _CP0_SET_COUNT(0); // resets the core timer count

    // get the core timer count
    while ( _CP0_GET_COUNT() < (SYSCLK/(2*1000)) );
}

//void waitms(int len)
//{
//	while(len--) wait_1ms();
//}

#define PIN_PERIOD (PORTB&(1<<6))

// GetPeriod() seems to work fine for frequencies between 200Hz and 700kHz.
long int GetPeriod (int n)
{
	int i;
	unsigned int saved_TCNT1a, saved_TCNT1b;
	
    _CP0_SET_COUNT(0); // resets the core timer count
	while (PIN_PERIOD!=0) // Wait for square wave to be 0
	{
		if(_CP0_GET_COUNT() > (SYSCLK/4)) return 0;
	}

    _CP0_SET_COUNT(0); // resets the core timer count
	while (PIN_PERIOD==0) // Wait for square wave to be 1
	{
		if(_CP0_GET_COUNT() > (SYSCLK/4)) return 0;
	}
	
    _CP0_SET_COUNT(0); // resets the core timer count
	for(i=0; i<n; i++) // Measure the time of 'n' periods
	{
		while (PIN_PERIOD!=0) // Wait for square wave to be 0
		{
			if(_CP0_GET_COUNT() > (SYSCLK/4)) return 0;
		}
		while (PIN_PERIOD==0) // Wait for square wave to be 1
		{
			if(_CP0_GET_COUNT() > (SYSCLK/4)) return 0;
		}
	}

	return  _CP0_GET_COUNT();
}

void Timer4us(unsigned char t) 
{
	T4CON = 0x8000; // enable Timer4, source PBCLK, 1:1 prescaler
 
    // delay 100us per loop until less than 100us remain
    while( t >= 100){
        t-=100;
        TMR4=0;
        while(TMR4 < SYSCLK/10000L);
    }
 
    // delay 10us per loop until less than 10us remain
    while( t >= 10){
        t-=10;
        TMR4=0;
        while(TMR4 < SYSCLK/100000L);
    }
 
    // delay 1us per loop until finished
    while( t > 0)
    {
        t--;
        TMR4=0;
        while(TMR4 < SYSCLK/1000000L);
    }
    // turn off Timer4 so function is self-contained
    T4CONCLR=0x8000;
}

void waitms(unsigned int ms)
{
	unsigned int j;
	unsigned char k;
	for(j=0;j<ms;j++)
		for(k=0;k<4;k++)
			Timer4us(250);
}

void LCD_pulse(void)
{
	LCD_E = 1;
	Timer4us(40);
	LCD_E = 0;
}

void LCD_byte(unsigned char x)
{
	LCD_D7=(x&0x80)?1:0;
	LCD_D6=(x&0x40)?1:0;
	LCD_D5=(x&0x20)?1:0;
	LCD_D4=(x&0x10)?1:0;
	LCD_pulse();
	Timer4us(40);
	LCD_D7=(x&0x08)?1:0;
	LCD_D6=(x&0x04)?1:0;
	LCD_D5=(x&0x02)?1:0;
	LCD_D4=(x&0x01)?1:0;
	LCD_pulse();
}

void WriteData(unsigned char x)
{
	LCD_RS = 1;
	LCD_byte(x);
	waitms(2);
}

void WriteCommand(unsigned char x)
{
	LCD_RS = 0;
	LCD_byte(x);
	waitms(5);
}

void LCD_4BIT(void)
{
	// Configure the pins used to communicate with the LCD as outputs
	LCD_RS_ENABLE = 0;
	LCD_E_ENABLE = 0;
	LCD_D4_ENABLE = 0;
	LCD_D5_ENABLE = 0;
	LCD_D6_ENABLE = 0;
	LCD_D7_ENABLE = 0;
	
	LCD_E = 0; // Resting state of LCD's enable is zero
	// LCD_RW = 0; Not used in this code.  Connect to ground.
	waitms(20);
	// First make sure the LCD is in 8-bit mdode, then change to 4-bit mode
	WriteCommand(0x33);
	WriteCommand(0x33);
	WriteCommand(0x32); // Change to 4-bit mode
	
	// Configure the LCD
	WriteCommand(0x28);
	WriteCommand(0x0c);
	WriteCommand(0x01); // Clear screen command (takes some time)
	waitms(20); // Wait for clear screen command to finish
	LATBbits.LATB0 = 	!LATBbits.LATB0;
}

void LCDprint(char * string, unsigned char line, unsigned char clear)
{
	int j;
	
	WriteCommand(line==2?0xc0:0x80);
	waitms(5);
	for(j=0;string[j]!=0;j++)
		WriteData(string[j]); //Write the message character by character
	if(clear)
		for(;j<CHARS_PER_LINE;j++)
			WriteData(' '); //Clear the rest of the line if clear is 1
}

/* Pinout for DIP28 PIC32MX130:
                                          --------
                                   MCLR -|1     28|- AVDD 
  VREF+/CVREF+/AN0/C3INC/RPA0/CTED1/RA0 -|2     27|- AVSS 
        VREF-/CVREF-/AN1/RPA1/CTED2/RA1 -|3     26|- AN9/C3INA/RPB15/SCK2/CTED6/PMCS1/RB15
   PGED1/AN2/C1IND/C2INB/C3IND/RPB0/RB0 -|4     25|- CVREFOUT/AN10/C3INB/RPB14/SCK1/CTED5/PMWR/RB14
  PGEC1/AN3/C1INC/C2INA/RPB1/CTED12/RB1 -|5     24|- AN11/RPB13/CTPLS/PMRD/RB13
   AN4/C1INB/C2IND/RPB2/SDA2/CTED13/RB2 -|6     23|- AN12/PMD0/RB12
     AN5/C1INA/C2INC/RTCC/RPB3/SCL2/RB3 -|7     22|- PGEC2/TMS/RPB11/PMD1/RB11
                                    VSS -|8     21|- PGED2/RPB10/CTED11/PMD2/RB10
                     OSC1/CLKI/RPA2/RA2 -|9     20|- VCAP
                OSC2/CLKO/RPA3/PMA0/RA3 -|10    19|- VSS
                         SOSCI/RPB4/RB4 -|11    18|- TDO/RPB9/SDA1/CTED4/PMD3/RB9
         SOSCO/RPA4/T1CK/CTED9/PMA1/RA4 -|12    17|- TCK/RPB8/SCL1/CTED10/PMD4/RB8
                                    VDD -|13    16|- TDI/RPB7/CTED3/PMD5/INT0/RB7
                    PGED3/RPB5/PMD7/RB5 -|14    15|- PGEC3/RPB6/PMD6/RB6
                                          --------
*/

// Information here:
// http://umassamherstm5.org/tech-tutorials/pic32-tutorials/pic32mx220-tutorials/1-basic-digital-io-220
void main(void)
{
	long int count;
	int exit_state = 1;
	int conversion_state = 1;
	int menu = 1;
	float T, f, capacitor;
	char toLCDPrint[16];
	char buff[17];
	float capu[5];
	float capu2[5];
	int capu_i = 0; 
	int capu2_i = 0; 
	int i;
	int expected1;
	int expected2;
	//int int_expected2;
	float diff;
	float dev_percent;

	CFGCON = 0;
  
    UART2Configure(115200);  // Configure UART2 for a baud rate of 115200
    
    ANSELB &= ~(1<<6); // Set RB6 as a digital I/O
    TRISB |= (1<<6);   // configure pin RB6 as input
    CNPUB |= (1<<6);   // Enable pull-up resistor for RB6
    
	waitms(500);
	printf("PIC32MX130 Period measurement using the core timer free running counter.\r\n"
	       "Connect signal to RB6 (pin 15).\r\n");

    LCD_4BIT();

	LCDprint("START!!", 1, 1);

    while(exit_state)
    {  
	   if(menu == 1){
			printf("\nMenu Option: \r");
	   		printf("\n1. Frequency \r");
	   		printf("\n2. Period \r");
	   		printf("\n3. Capacitance Meter 1\r");
			printf("\n4. Capacitance Meter 2\r");
			printf("\n5. Impedance \r");
	   		printf("\n6. Exit \r");

			printf("\n\n\nType desired action: \n\r");
	   		fflush(stdout); // GCC peculiarities: need to flush stdout to get string out without a '\n'
			fgets(buff, sizeof(buff)-1, stdin);
			menu = atoi(buff);
	   }

	   if(menu == 6){
			break;
	   }
	   
	   else{
			count=GetPeriod(100);
			if(count>0)
			{
				T=(count*2.0)/(SYSCLK*100.0);
				f=1/T;
				capacitor = (1.44 * T) / (3 * 1.5);
        	    //capacitor = (f * (3 * RESISTOR)) / 1.44;
				//cap_value = (1.44 * period * 1000) / (3 * 1.5);

				if(menu == 1){
					printf("\nThe frequency is %f Hz\r\n", f);
					LCDprint("Frequency     ", 1, 1);
					sprintf(toLCDPrint, "%.2f Hz", f);
        		    LCDprint(toLCDPrint, 2, 1);
				}

				else if(menu == 2){
					printf("\nThe period is %f \n\r", T);
					LCDprint("Period        ", 1, 1);
					sprintf(toLCDPrint, "%.5f ", T);
        		    LCDprint(toLCDPrint, 2, 1);
				}
				else if(menu == 3){
					printf("\nType 0 for nF, 1 for uF: \r\n");
	   				fflush(stdout); // GCC peculiarities: need to flush stdout to get string out without a '\n'
					fgets(buff, sizeof(buff)-1, stdin);
					conversion_state = atoi(buff);
					LCDprint("Capacitance", 1, 1);

					if(conversion_state == 0){
						//cap_value = (1.44 * period * 1000) / (3 * 1.5);
						printf("Capacitance = %f nf \r\n", capacitor * 1000000);
						sprintf(toLCDPrint, "%.6f nF", capacitor * 1000000);
        		    	LCDprint(toLCDPrint, 2, 1);

						capu[capu_i] = capacitor * 1000000;
						capu_i++;
					}
					else{
						printf("Capacitance = %f uf \r\n", capacitor * 1000);
						sprintf(toLCDPrint, "%.6f uF", capacitor * 1000);
        		    	LCDprint(toLCDPrint, 2, 1);
					}
				}

				else if(menu == 4){
					printf("\nType 0 for nF, 1 for uF: \r\n");
	   				fflush(stdout); // GCC peculiarities: need to flush stdout to get string out without a '\n'
					fgets(buff, sizeof(buff)-1, stdin);
					conversion_state = atoi(buff);
					LCDprint("Capacitance", 1, 1);

					if(conversion_state == 0){
						printf("Capacitance = %f nf \r\n", capacitor * 1000000);
						sprintf(toLCDPrint, "%.6f nF", capacitor * 1000000);
        		    	LCDprint(toLCDPrint, 2, 1);

						capu2[capu2_i] = capacitor * 1000000;
						capu2_i++;
					}
					else{
						printf("Capacitance = %f uf \r\n", capacitor * 1000);
						sprintf(toLCDPrint, "%.6f uF", capacitor * 1000);
        		    	LCDprint(toLCDPrint, 2, 1);
					}
				}

				else{
					LCDprint("Impedance     ", 1, 1);
					printf("\nThe impedance of the capacitor is - 1/%.4f j, in uOhms\r\n", capacitor * 1000 * f);
					sprintf(toLCDPrint, "-1/%.2fj uOhms", capacitor * 1000 * f);
        		    LCDprint(toLCDPrint, 2, 1);
				}
			}
			else
			{
				printf("NO SIGNAL         \r");
        	    LCDprint("No Signal!!!   ", 1, 1);
			}
	   }
		printf("\nType 0 to exit, 1 to go to menu, 3 to do capacitance 1 measurements: \r\n");
	   	fflush(stdout); // GCC peculiarities: need to flush stdout to get string out without a '\n'
		fgets(buff, sizeof(buff)-1, stdin);
		exit_state = atoi(buff);
		printf("\nExit state: %d\n\r", exit_state);
		menu = exit_state;

		fflush(stdout);
		waitms(200);
    }

	printf("\nInput expected capacitance 1 in nF: \n\r");
	fflush(stdout); // GCC peculiarities: need to flush stdout to get string out without a '\n'
	fgets(buff, sizeof(buff)-1, stdin);
	expected1 = atoi(buff);

	printf("\nInput expected capacitance 2 in nF (101 if none): \n\r");
	fflush(stdout); // GCC peculiarities: need to flush stdout to get string out without a '\n'
	fgets(buff, sizeof(buff)-1, stdin);
	expected2 = atoi(buff);




	// Only prints the first deviation percentage of capacitor 1 measurement
	dev_percent = ((expected1 - capu[0]) / expected1) * 100;
	sprintf(toLCDPrint, "%.4f", dev_percent);
	LCDprint("Dev Percent 1", 1, 1);
    LCDprint(toLCDPrint, 2, 1);


	if(expected2 == 101) {
		printf("\nDeviations\n\r");
    	printf("# Capacitor nF\n\r");
		//printf(expected1);
    	for(i = 0; i <= 5; i++){
			diff = capu[i] - expected1;
        	printf("%d. %.4f  ||  %.4f  \n\r", i + 1, capu[i], diff);
    	}
	}

	else {
		printf("\nDeviations\n\r");
    	printf("# Capacitor nF ; Capacitor 2 \n\r");
		for(i = 0; i <= 5; i++){
			diff = capu[i] - expected1;
    	    printf("%d. %.4f  ||  %.4f  ||  %.4f  ||  %.4f \n\r", i + 1, capu[i], diff, capu2[i], capu2[i] - expected2);
    	    //printf("Testing: expect %f, actual %f\n", expected, cap0[i]);
    	    //printf("Testing: expect %f, actual %f\n\n", expected2, cap1[i]);
    	}
	}

	waitms(3000);

	printf("\n\nExited!\r");
	LCD_4BIT();
    LCDprint("T", 1, 1);
    waitms(500);
    LCDprint("Th", 1, 1);
    waitms(500);
    LCDprint("Tha", 1, 1);
    waitms(500);
    LCDprint("Than", 1, 1);
    waitms(500);
    LCDprint("Thank", 1, 1);

    waitms(500);
    LCDprint("Thank y", 1, 1);
    waitms(500);
    LCDprint("Thank yo", 1, 1);
    waitms(500);
    LCDprint("Thank you", 1, 1);

    waitms(500);
    LCDprint("Thank you :", 1, 1);
    waitms(500);
    LCDprint("Thank you :)", 1, 1);

    waitms(1000);
    LCDprint("Have a nice day!", 2, 1);
}

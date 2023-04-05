#include <XC.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>


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

 
// Configuration Bits (somehow XC32 takes care of this)
#pragma config FNOSC = FRCPLL       // Internal Fast RC oscillator (8 MHz) w/ PLL
#pragma config FPLLIDIV = DIV_2     // Divide FRC before PLL (now 4 MHz)
#pragma config FPLLMUL = MUL_20     // PLL Multiply (now 80 MHz)
#pragma config FPLLODIV = DIV_2     // Divide After PLL (now 40 MHz) 
#pragma config FWDTEN = OFF         // Watchdog Timer Disabled
#pragma config FPBDIV = DIV_1       // PBCLK = SYCLK
#pragma config FSOSCEN = OFF        // Turn off secondary oscillator on A4 and B4

// Defines
#define SYSCLK 40000000L
#define FREQ 100000L // We need the ISR for timer 1 every 10 us
#define Baud2BRG(desired_baud)( (SYSCLK / (16*desired_baud))-1)


volatile int ISR_pwm1=150, ISR_pwm2=150, ISR_cnt=0, ISR_frc, ISR_cnt2=0 , ISR_cnt_measure_pulse=0; 
long int time_ISR = 0;
long int Prev_V_ISR, Peak_V_ISR = 0;
volatile int bitone, bittwo, bitthree;
volatile int movement_instruction_ISR = 0; 
volatile int movement_flag = 0;
volatile int movement_flag_follow = 0;
volatile int pulse_time_average = 0.0;
int mode;


volatile int entered_if_statement = 0;
long int timer_count = 0;
long int adc_four;
int startflag1, startflag2, bitflag1,bitflag2,bitflag3 = 0;
int num_bits_read = 0;


int ADCRead(char analogPIN)
{
    AD1CHS = analogPIN << 16;    // AD1CHS<16:19> controls which analog pin goes to the ADC
 
    AD1CON1bits.SAMP = 1;        // Begin sampling
    while(AD1CON1bits.SAMP);     // wait until acquisition is done
    while(!AD1CON1bits.DONE);    // wait until conversion done
 
    return ADC1BUF0;             // result stored in ADC1BUF0
}

// The Interrupt Service Routine for timer 1 is used to generate one or more standard
// hobby servo signals.  The servo signal has a fixed period of 20ms and a pulse width
// between 0.6ms and 2.4ms.
void __ISR(_TIMER_1_VECTOR, IPL5SOFT) Timer1_Handler(void)
{
	IFS0CLR=_IFS0_T1IF_MASK; // Clear timer 1 interrupt flag, bit 4 of IFS0

	ISR_cnt++;
	ISR_cnt_measure_pulse++;
	/*if(ISR_cnt==ISR_pwm1)
	{
		LATAbits.LATA3 = 0;
	}
	if(ISR_cnt==ISR_pwm2)
	{
		LATBbits.LATB4 = 0;
	}*/
	//if(ISR_cnt == 100){
	//}

	if(ISR_cnt_measure_pulse >= 1000){

		ISR_cnt_measure_pulse = 0;
		pulse_time_average = MeasurePulse();
	}


	if(ISR_cnt>=100)
	{
		/*20msCount++;*/
		ISR_cnt=0; // 2000 * 10us=20ms
		//LATAbits.LATA3 = 1;
		//LATBbits.LATB4 = 1;
		ISR_frc++;
	}



}

void delay_ms (int msecs)
{	
	int ticks;
	ISR_frc=0;
	ticks=msecs;
	while(ISR_frc<ticks);
}



__ISR(_TIMER_2_VECTOR, IPL5SOFT) Timer2_Handler(void){

	IFS0CLR=_IFS0_T2IF_MASK; // Clear timer 2 interrupt flag, bit 4 of IFS0

	ISR_cnt2++;

	if(ISR_cnt2 == 1000 && movement_flag != 1 && mode == 1){
		
		if(ADCRead(4) * 3290.0 / 1023.0 - 150 < 0.85 * (Prev_V_ISR-150)){
		    T2CONbits.ON = 1;
			_CP0_SET_COUNT(0);
			Peak_V_ISR = Prev_V_ISR;
			
			while(ADCRead(4) * 3290.0 / 1023.0 - 150 < 0.85 * (Peak_V_ISR -150) && movement_flag != 1){
				if ((_CP0_GET_COUNT() / (SYSCLK/(2*1000))) * 1000 > 350000){
					Peak_V_ISR = -100000;
					break;
				}
			}
			//while(ADCRead(4) * 3290.0 / 1023.0 - 150 < 0.85 * (Peak_V_ISR -150) && movement_flag != 1);


			//while(ADCRead(4) * 3290.0 / 1023.0 - 150 < 100 && movement_flag != 1);

            T2CONbits.ON = 0;
			
			time_ISR = (_CP0_GET_COUNT() / (SYSCLK/(2*1000))) * 1000; // TIME in uS
			//time_ISR = 2 * (_CP0_GET_COUNT() / (SYSCLK/(2*1000))) * 1000; // TIME in uS

			
			printf("Time ISR: %d\r\n", time_ISR);
			
			T2CONbits.ON = 1;

			if(time_ISR <= 75000){                            //no signal
				//movement_instruction_ISR = 0;
				time_ISR = 0;
			}

			else if(time_ISR > 80000 && time_ISR <= 120000 ){  //go backward ///was 75000 and 125000 
				movement_instruction_ISR = 2;
				time_ISR = 0;
			}

			else if(time_ISR > 130000  && time_ISR <= 170000 ){ //go left ///was 125000 and 175000
				movement_instruction_ISR = 3;
				time_ISR = 0;
			}

			else if(time_ISR > 180000 && time_ISR <= 220000 ){ //go forward //was 175000 and 225000
				movement_instruction_ISR = 1;
				time_ISR = 0;
			}

			else if(time_ISR > 230000  && time_ISR <= 270000 ){ //go right ///was 225000 and 275000
				movement_instruction_ISR = 4;
				time_ISR = 0;
			}

			else if(time_ISR > 270500  && time_ISR <= 340000 ){ //switch mode ///was 275000 and 325000
				movement_instruction_ISR = 5;
				time_ISR = 0;
			}

			else if(time_ISR >= 350000 ){                      //no signal /// was 325000
				//movement_instruction_ISR = 0;
				time_ISR = 0;
			}
			
			else{
				time_ISR = 0;
			}

			//
			//printf("movement instruction: %d\r\n", time_ISR);

		}

		///Switch Modes while in follow mode 
		/*else if(ISR_cnt2 == 1000 && movement_flag_follow != 1 && mode == 0){

			if(ADCRead(4) * 3290.0 / 1023.0 - 150 < 0.85 * (Prev_V_ISR-150)){
				T2CONbits.ON = 1;
				_CP0_SET_COUNT(0);
				Peak_V_ISR = Prev_V_ISR;
				
				//while(ADCRead(4) * 3290.0 / 1023.0 - 150 < 0.75 * (Peak_V_ISR -150) && movement_flag != 1);
				while(ADCRead(4) * 3290.0 / 1023.0 - 150 < 0.85 * (Peak_V_ISR -150) && movement_flag != 1);


				//while(ADCRead(4) * 3290.0 / 1023.0 - 150 < 100 && movement_flag != 1);

				T2CONbits.ON = 0;
				
				time_ISR = (_CP0_GET_COUNT() / (SYSCLK/(2*1000))) * 1000; // TIME in uS
				//time_ISR = 2 * (_CP0_GET_COUNT() / (SYSCLK/(2*1000))) * 1000; // TIME in uS

				
				printf("Time ISR: %d\r\n", time_ISR);
				
				T2CONbits.ON = 1;
			}

			if(time_ISR > 280000 && time_ISR <= 340000){ //switch mode ///was 275000 and 325000
					movement_instruction_ISR = 5;
					time_ISR = 0;
			}
			
			
		}*/

		else{
			Prev_V_ISR = ADCRead(4) * 3290.0 / 1023.0;
			time_ISR = 0;
		}
	}


	if(ISR_cnt2 >= 1000){
			ISR_cnt2=0; // 1000 * 10us=10ms
	}

	
		
		
	
}

void SetupTimer1 (void)
{
	// Explanation here: https://www.youtube.com/watch?v=bu6TTZHnMPY
	__builtin_disable_interrupts();
	PR1 =(SYSCLK/FREQ)-1; // since SYSCLK/FREQ = PS*(PR1+1)
	TMR1 = 0;
	T1CONbits.TCKPS = 0; // 3=1:256 prescale value, 2=1:64 prescale value, 1=1:8 prescale value, 0=1:1 prescale value
	T1CONbits.TCS = 0; // Clock source
	T1CONbits.ON = 1;
	IPC1bits.T1IP = 5;
	IPC1bits.T1IS = 0;

	PR2 =(SYSCLK/FREQ)-1; // since SYSCLK/FREQ = PS*(PR1+1)
	TMR2 = 0;
	T2CONbits.TCKPS = 0; // 3=1:256 prescale value, 2=1:64 prescale value, 1=1:8 prescale value, 0=1:1 prescale value
	T2CONbits.TCS = 0; // Clock source
	T2CONbits.ON = 1;
	IPC2bits.T2IP = 5;
	IPC2bits.T2IS = 0;
	
	IFS0bits.T1IF = 0;
	IEC0bits.T1IE = 1;

	IFS0bits.T2IF = 0;
	IEC0bits.T2IE = 1;
	
	INTCONbits.MVEC = 1; //Int multi-vector
	__builtin_enable_interrupts();
}

// Use the core timer to wait for 1 ms.
void wait_1ms(void)
{
    unsigned int ui;
    _CP0_SET_COUNT(0); // resets the core timer count

    // get the core timer count
    while ( _CP0_GET_COUNT() < (SYSCLK/(2*1000)) );
}

void waitms(int len)
{
	while(len--) wait_1ms();
}

#define PIN_PERIOD (PORTB&(1<<5))

// GetPeriod() seems to work fine for frequencies between 200Hz and 700kHz.

 
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

void uart_puts(char * s)
{
	while(*s)
	{
		putchar(*s);
		s++;
	}
}

char HexDigit[]="0123456789ABCDEF";
void PrintNumber(long int val, int Base, int digits)
{ 
	int j;
	#define NBITS 32
	char buff[NBITS+1];
	buff[NBITS]=0;

	j=NBITS-1;
	while ( (val>0) | (digits>0) )
	{
		buff[j--]=HexDigit[val%Base];
		val/=Base;
		if(digits!=0) digits--;
	}
	uart_puts(&buff[j+1]);
}

// Good information about ADC in PIC32 found here:
// http://umassamherstm5.org/tech-tutorials/pic32-tutorials/pic32mx220-tutorials/adc
void ADCConf(void)
{
    AD1CON1CLR = 0x8000;    // disable ADC before configuration
    AD1CON1 = 0x00E0;       // internal counter ends sampling and starts conversion (auto-convert), manual sample
    AD1CON2 = 0;            // AD1CON2<15:13> set voltage reference to pins AVSS/AVDD
    AD1CON3 = 0x0f01;       // TAD = 4*TPB, acquisition time = 15*TAD 
    AD1CON1SET=0x8000;      // Enable ADC
}


void ConfigurePins(void)
{
    // Configure pins as analog inputs
    ANSELBbits.ANSB2 = 1;   // set RB2 (AN4, pin 6 of DIP28) as analog pin
    TRISBbits.TRISB2 = 1;   // set RB2 as an input
    ANSELBbits.ANSB3 = 1;   // set RB3 (AN5, pin 7 of DIP28) as analog pin
    TRISBbits.TRISB3 = 1;   // set RB3 as an input

	// Pin RB14
    ANSELBbits.ANSB14 = 0; // DC
    TRISBbits.TRISB14 = 1; //pin 3 echo (TRIS = DIGITAL, ANSEL = ANALOG) input

    // Pin RB15 
    ANSELBbits.ANSB15 = 0; // DC
    TRISBbits.TRISB15 = 0; //output
	
	//ANSELAbits.ANSA1 = 1;   // the button input for the switch mode
	//Button tx	`o switch modes
	//ANSELA &= ~(1<<1); // Set RA1 as a digital I/O
    //TRISA |= (1<<1);   // configure pin RA1 as input
    //CNPUA |= (1<<1);   // Enable pull-up resistor for RA1
	ANSELAbits.ANSA1 = 0;
	TRISAbits.TRISA1 = 1;
	CNPUA |= (1<<1);   // Enable pull-up resistor for RA1
    
	// Configure digital input pin to measure signal period
	ANSELB &= ~(1<<5); // Set RB5 as a digital I/O (pin 14 of DIP28)
    TRISB |= (1<<5);   // configure pin RB5 as input
    CNPUB |= (1<<5);   // Enable pull-up resistor for RB5
    
    // We can do the three lines above using this instead:
    // ANSELBbits.ANSELB5=0;  Not needed because RB5 can not be analog input?
    // TRISBbits.TRISB5=1;
    // CNPUBbits.CNPUB5=1;
    
    // Configure output pins
	TRISAbits.TRISA0 = 0; // pin  2 of DIP28
	//TRISAbits.TRISA1 = 0; // pin  3 of DIP28
	TRISBbits.TRISB0 = 0; // pin  4 of DIP28
	TRISBbits.TRISB1 = 0; // pin  5 of DIP28
	//TRISAbits.TRISA2 = 0; // pin  9 of DIP28
	//TRISAbits.TRISA3 = 0; // pin 10 of DIP28
	//TRISBbits.TRISB4 = 0; // pin 11 of DIP28
	INTCONbits.MVEC = 1;

	//Configure output pins for motor
	TRISBbits.TRISB0 = 0; // pin4
	TRISBbits.TRISB1 = 0; // pin5
	TRISAbits.TRISA2 = 0; // pin9
	TRISAbits.TRISA4 = 0; // pin12


	// Pin RA3 as pushbutton
	//ANSELAbits.ANSA3 = 0;
	//TRISAbits.TRISA3 = 1;
	//CNPUA |= (1<<3);   // Enable pull-up resistor for RA1
}

void PrintFixedPoint (unsigned long number, int decimals)
{
	int divider=1, j;
	
	j=decimals;
	while(j--) divider*=10;
	
	PrintNumber(number/divider, 10, 1);
	uart_puts(".");
	PrintNumber(number%divider, 10, decimals);
}

// movements
void go_forward(){
	LATBbits.LATB0 = 0; //pin 4
	LATBbits.LATB1 = 1; //pin 5
	LATAbits.LATA2 = 0; //pin 9
	LATAbits.LATA4 = 1; //pin 12
	movement_flag = 1;
	movement_flag_follow = 1;
}

void go_backward(){
	LATBbits.LATB0 = 1; //pin 4
	LATBbits.LATB1 = 0; //pin 5
	LATAbits.LATA2 = 1; //pin 9
	LATAbits.LATA4 = 0; //pin 12
	movement_flag = 1;
	movement_flag_follow = 1;
	
}

void turn_left(){
	LATBbits.LATB0 = 0; //pin 4
	LATBbits.LATB1 = 1; //pin 5
	LATAbits.LATA2 = 1; //pin 9
	LATAbits.LATA4 = 0; //pin 12
	movement_flag = 1;
	movement_flag_follow = 1;
}

void turn_right(){
	LATBbits.LATB0 = 1; //pin 4
	LATBbits.LATB1 = 0; //pin 5
	LATAbits.LATA2 = 0; //pin 9
	LATAbits.LATA4 = 1; //pin 12
	movement_flag = 1;
	movement_flag_follow = 1;
	

}

void stop_motors(){
	//printf("\n\r0ol");
	//all motor pins off
	LATBbits.LATB0 = 1; //pin 4
	LATBbits.LATB1 = 1; //pin 5
	LATAbits.LATA2 = 1; //pin 9
	LATAbits.LATA4 = 1; //pin 12
if(mode == 1){
	movement_flag = 0;
}

else if(mode == 0){
	movement_flag_follow = 0;
}
}

void stop_motors2(){
	//printf("\n\r0ol");
	//all motor pins off
	LATBbits.LATB0 = 1; //pin 4
	LATBbits.LATB1 = 1; //pin 5
	LATAbits.LATA2 = 1; //pin 9
	LATAbits.LATA4 = 1; //pin 12
	movement_flag = 0;


}



/*int mode_handler(int instruction, int mode){
	/*if(instruction == 5){
		printf("\n\rmode before: %d", mode);
		mode = !mode;
		printf("\n\rmode after: %d", mode);
		return mode;
	}

	
	if((PORTA&(1<<1)?'1':'0') == 1){

		if(mode == 0){
			mode = 1;
		}
		if(mode == 1){
			mode = 0;
		}


	if(ADCRead(4) * 3290.0 / 1023.0 - 150 < 0.85 * (v1-150)){
		T2CONbits.ON = 1;
		_CP0_SET_COUNT(0);
		Peakv1 = Prev1;
		
		//while(ADCRead(4) * 3290.0 / 1023.0 - 150 < 0.75 * (Peak_V_ISR -150) && movement_flag != 1);
		while(ADCRead(4) * 3290.0 / 1023.0 - 150 < 0.85 * (Peak_V_ISR -150) );


		//while(ADCRead(4) * 3290.0 / 1023.0 - 150 < 100 && movement_flag != 1);

		T2CONbits.ON = 0;
		
		time_ISR = (_CP0_GET_COUNT() / (SYSCLK/(2*1000))) * 1000;

	}

	printf("mode: %d", mode);

	//mode = 0;
	return mode;

	
}*/

long int real_time_average_V1(){
	int count1 = 0;
	long int sum_V1 = 0;
	while(count1 < 10){
		
		sum_V1 += ADCRead(4);
		count1++;
		delay_ms(10);
	}

	/*if(sum_V1 * 3290L / 1023L / 20L < 300 && mode == 0 && mode == 0){
		mode = 1;
		printf("mode: %d\r\n", mode);
	}*/
	return sum_V1 * 3290L / 1023L / 20L;
}

long int real_time_average_V2(){
	int count2 = 0;
	long int sum_V2 = 0;
	while(count2 < 10){
		
		sum_V2 += ADCRead(5);
		count2++;
		delay_ms(10);
	}
	return sum_V2 * 3290L / 1023L / 20L;
}

int reverseCommand(int command) {
	if (command == 1) {
		return 2;
	}
	else if (command == 2) {
		return 1;
	}
	else if (command == 3) {
		return 4;
	}
	else if (command == 4) {
		return 3;
	}

	return;
}


void goHome(int* go_home_array, int size) {
	while (size-1 >= 0) {
		    if(go_home_array[size-1] == 1)
			{
				go_forward();
				//printf("going forward\r\n");
				delay_ms(700);
				//printf("stopped moving\r\n");
				stop_motors2();

			}
			
			else if(go_home_array[size-1] == 2)
			{
				go_backward();
				//printf("going back\r\n");
				delay_ms(700);
				//printf("stopped moving\r\n");
				stop_motors2();
			}

			else if(go_home_array[size-1] == 3)
			{
				turn_left();
				//printf("going left\r\n");
				delay_ms(700);
				//printf("stopped moving\r\n");
				stop_motors2();
			}

			else if(go_home_array[size-1] == 4)
			{
				turn_right();
				//printf("going right\r\n");
				delay_ms(700);
				//printf("stopped moving\r\n");
				stop_motors2();
			}
			size--;
			delay_ms(50);
	} 
	return;
}

// Calculation angle for robot to turn in Go Home Fast
double angle_diff(double curr_x, double curr_y, double robot_angle) {
	double angle_to_origin = atan2(-curr_y, -curr_x)*(18000/314); // gives angle in degrees

	// Calculate difference between orientation angle of robot and angle_to_origin
	double diff = robot_angle*(18000/314) - angle_to_origin;

	while (diff > 180) {
		diff -= 360;
	}

	while (diff < -180) {
		diff += 360;
	}

	return diff;
}

// Go Home Fast Movement
void goHomeFast(double angle, double distance) {
	// Turn first 
	if (angle < 0) {
		turn_right();
		//printf("going right\r\n");
		delay_ms(angle*700/90);
		//printf("stopped moving\r\n");
		stop_motors();
	}
	else if (angle > 0) {
		turn_left();
		//printf("going right\r\n");
		delay_ms(angle*700/90);
		//printf("stopped moving\r\n");
		stop_motors();
	}
	delay_ms(30);

	// Move in path
	go_forward();
	//printf("going forward\r\n");
	delay_ms((int) 700 * distance);
	//printf("stopped moving\r\n");
	stop_motors();
}

float MeasurePulse()
{
    float pw;
    uint8_t overflow_count;
    _CP0_SET_COUNT(0);
    LATBbits.LATB15 = 1;
    while(_CP0_GET_COUNT() < (SYSCLK/(2100000))); //delay for 10us
    LATBbits.LATB15 = 0;

    // Reset the counter
    TMR1 = 0;
    overflow_count = 0;

    while (PORTBbits.RB14 == 0);
        //uart_puts("\r\necho is 0");
     // Wait for the signal
    T1CONbits.ON = 1;           // Start the timer
    _CP0_SET_COUNT(0);
    while (PORTBbits.RB14)
    {
        //uart_puts("\r\necho is 1");
        if (IFS0bits.T1IF) // Did the 16-bit timer overflow?
        {
            IFS0bits.T1IF = 0;
            overflow_count++;
            if (overflow_count > 5) break;
        }
    }
	T1CONbits.ON = 0; // Stop timer 2, the 24-bit number [overflow_count-TMR2] has the period!
    pw = (overflow_count 65536.0 + TMR1) * (1 / (float)SYSCLK);
    //printf("\r\npulse width: %f", pw);
    return pw; //pulse width
}


// In order to keep this as nimble as possible, avoid
// using floating point or printf() on any of its forms!
void main(void)
{
	volatile unsigned long t=0;
    long int adcval1, adcval2;
    long int v1,v2, Prev1, Peakv1;
	unsigned long int count, f;
	long int time;
	unsigned char LED_toggle=0;
	int movement_instruction = 9;
	//int mode;
	float left_right_difference;
	long int sampleV_arr1[20];
	long int sampleV_arr2[20];
	int array_count = 0;
	//int movement_instruction_ISR=0;

	int reversed_command;
	int go_home[100];
	int go_home_count = 0;


	// Go Home Fast variables
	double curr_x = 0; // current x coordinate of robot
	double curr_y = 0; // current y coordinate of robot
	double orientation_angle = 0; // robots current angle of orientation relative to the x axis 

	CFGCON = 0;
  
    UART2Configure(115200);  // Configure UART2 for a baud rate of 115200
    ConfigurePins();
	ADCConf(); // Configure ADC
    SetupTimer1();
    
    waitms(50); // Give PuTTY time to start
	uart_puts("\x1b[2J\x1b[1;1H"); // Clear screen using ANSI escape sequence.
	uart_puts("\r\nPIC32 multi I/O example.\r\n");
	uart_puts("Measures the voltage at channels 4 and 5 (pins 6 and 7 of DIP28 package)\r\n");
	uart_puts("Measures period on RB5 (pin 14 of DIP28 package)\r\n");
	uart_puts("Toggles RA0, RA1, RB0, RB1, RA2 (pins 2, 3, 4, 5, 9, of DIP28 package)\r\n");
	uart_puts("Generates Servo PWM signals at RA3, RB4 (pins 10, 11 of DIP28 package)\r\n\r\n");

	//Prev_V_ISR = ADCRead(4) * 3290.0 / 1023.0;
	
	//set motors off initially
	LATAbits.LATA2 = 1;
	LATAbits.LATA3 = 1;
	LATBbits.LATB4 = 1; 
	LATAbits.LATA4 = 1;
	mode = 1;


	/*
	in timer runnning in background:
	when voltage drops below threshold,
	set flag up
	when voltage rises above thresshold,
	set flag down
	record how much time passes between up flag and down flag
	determine instruction
	in main follow mode:
	if instruction = 5, switch modes, else stay
	in main  control mode:
	if instruction = 5, switch mode
	else execute other instructions
	
	*/

	while(1)
	{
    	
		//adcval1 = ADCRead(4); // note that we call pin AN4 (RB2) by it's analog number (receiver pin left)
		//uart_puts("ADC[4]=0x");
		//PrintNumber(adcval1, 16, 3);
		uart_puts("\r\n, V_left=");
		
			v1 = real_time_average_V1();
			//printf(" V1_test =  %d ,", v1);
			PrintFixedPoint(v1, 3);
			uart_puts("V ");
		

		//uart_puts("ADC[5]=0x");
		//PrintNumber(adcval2, 16, 3);
		uart_puts(", V_right=");


		v2 = real_time_average_V2();
		PrintFixedPoint(v2, 3);
	    uart_puts("V ");


		left_right_difference = v1 - v2;
		//printf(" left_diff : %f \r\n", left_right_difference);
		//***********************************************************

	/*if(mode == 0 && movement_flag_follow == 0){
		if(ADCRead(4) * 3290.0 / 1023.0 - 150 < 0.85 * (Prev1-150)){
			T2CONbits.ON = 1;
			_CP0_SET_COUNT(0);
			Peakv1 = Prev1;
			
			//while(ADCRead(4) * 3290.0 / 1023.0 - 150 < 0.75 * (Peak_V_ISR -150) && movement_flag != 1);
			while(ADCRead(4) * 3290.0 / 1023.0 - 150 < 0.85 * (Peakv1 -150) ){
				printf("hello");
			}


			//while(ADCRead(4) * 3290.0 / 1023.0 - 150 < 100 && movement_flag != 1);

			T2CONbits.ON = 0;
			
			time = (_CP0_GET_COUNT() / (SYSCLK/(2*1000))) * 1000 + 210000;
			printf("time: %d\r\n", time);

			if(time > 280000 && time <= 320000){ //switch mode ///was 275000 and 325000
				mode = 1;
			}
		}
	}*/	

		/*1. 0 means go forward
  		  2. 1 means go backward
  		  3. 2 means turn left
  		  4. 3 means turn right
  		  5. 4 means horn*/
		//printf("\n\r%f", left_right_difference);
		
		//if following mode (mode = 0)
		if(mode == 0 && movement_instruction_ISR == 5){
			mode = 1;
            movement_instruction_ISR = 0;
		}

		if(mode == 0 /*&& v1 != 0*/){
			//printf("\r\nDifference: %f", left_right_difference); 

			//mode = mode_handler(movement_instruction, mode);

			
			//movement_flag = 1;

			//printf("\n\r%f", left_right_difference);
			if(left_right_difference > (v1+v2)/2*0.14 ){ //if left - right is positive then turn left to align ///was 0.12
				turn_left();
				//delay_ms(10);	
				//stop_motors();
				//printf("Turning left...Difference: %f", left_right_difference);                                  
			}

			else if(left_right_difference <  -((v1+v2)/2*0.15)){ //if left - right is positive then turn left to align ///was 0.12
				turn_right();
				//delay_ms(10);
				//stop_motors();
				//printf("Turning left...Difference: %f", left_right_difference);                                  
			}

			else{
				stop_motors();

				//here we need to have an algorithm where it moves back/forward so that left - right = 0.2 ish 
				if(v1 < 650){ // 0.530 too big, 460
					go_forward();
					//delay_ms(10);
			    	//stop_motors();
					//delay_ms(100);
					//stop_motors();
				}

				else if(v1 > 800){ // 750
					go_backward();
					//delay_ms(10);
					//stop_motors();
				}

			}

			Prev1 = v1;

		}
		//if control mode (mode = 1)
		while(mode == 1){

			while(1){
				printf("\r\nT=%5.3fms d=%7.3fcm", pulse_time_average*1000.0, pulse_time_average*34300.0/2.0);
				waitms(10);
			}
			
			//printf("RA1: %d", PORTA&(1<<1));
			// go home 
			/*if (PORTAbits.RA1 == 1) {
					printf("\r\nGoing Home");
					goHome(go_home, go_home_count);
				}*/
			
			//printf("ADC four: %d\r\n", adc_four);
			
			//v1 = real_time_average_V1();
			
			while(movement_instruction_ISR == 0) {
				// if (PORTAbits.RA1 == 0) {
				// 	//printf("\r\nGoing Home");
				// 	goHome(go_home, go_home_count);
				// 	//go_home = [];
				// 	go_home[0] = 0;
				// 	go_home_count = 0;
				// }

				if (PORTAbits.RA1 == 0) {
					goHomeFast(angle_diff(curr_x, curr_y, orientation_angle), sqrt(pow(curr_x, 2) + pow(curr_y, 2)));
				}
				//delay_ms(10);
			}

			//printf("\n\r %d", movement_instruction_ISR);
			//printf("\r\nBit3: %d", bitthree);
			//printf("\r\ntimer count: %d", timer_count);


			/*if(ADCRead(4) * 3290.0 / 1023.0 < 200 ){

			
				//printf("entered if statemtn\r\n");
				
				while(ADCRead(4) * 3290.0 / 1023.0 < 200);
				
				//delay_ms(93);
				
				if(ADCRead(4) * 3290.0 / 1023.0 < 200 && timer_count < 93){
						bitone = 0;
				}
				else{
						bitone = 1;
				}
				//delay_ms(62);
				if(ADCRead(4) * 3290.0 / 1023.0 < 200 && timer_count >= 93 && timer_count < 155){
					bittwo = 0;
				}
				else{
					bittwo = 1;
				}
				//delay_ms(62);
				if(ADCRead(4)  * 3290.0 / 1023.0 < 200 && timer_count >= 155 && timer_count <= 217){
					bitthree = 0;
				}
				else{
					bitthree = 1;
				}
			
			
			}*/
			
			if(movement_instruction_ISR  == 1)
			{
				go_forward();
				//printf("going forward\r\n");
				delay_ms(700);
				//printf("stopped moving\r\n");
				stop_motors();

				// Go Home Fast stuff
				curr_x += cos(orientation_angle);
				curr_y += sin(orientation_angle);

				reversed_command = reverseCommand(movement_instruction_ISR);
				go_home[go_home_count] = reversed_command;
				movement_instruction_ISR = 0;
				go_home_count++;

			}
			
			else if(movement_instruction_ISR == 2)
			{
				go_backward();
				//printf("going back\r\n");
				delay_ms(700);
				//printf("stopped moving\r\n");
				stop_motors();

				// Go Home Fast stuff
				curr_x += cos(3.14/2 + orientation_angle);
				curr_y += sin(3.14/2 + orientation_angle);

				reversed_command = reverseCommand(movement_instruction_ISR);
				go_home[go_home_count] = reversed_command;
				go_home_count++;
				movement_instruction_ISR = 0;

			}

			else if(movement_instruction_ISR == 3)
			{
				turn_left();
				//printf("going left\r\n");
				delay_ms(700);
				//printf("stopped moving\r\n");
				stop_motors();

				// Go Home Fast stuff
				orientation_angle += 3.14/2;

				reversed_command = reverseCommand(movement_instruction_ISR);
				go_home[go_home_count] = reversed_command;
				go_home_count++;
				movement_instruction_ISR = 0;
			}

			else if(movement_instruction_ISR == 4)
			{
				turn_right();
				//printf("going right\r\n");
				delay_ms(700);
				//printf("stopped moving\r\n");
				stop_motors();

				// Go Home Fast stuff
				orientation_angle -= 3.14/2;

				reversed_command = reverseCommand(movement_instruction_ISR);
				go_home[go_home_count] = reversed_command;
				go_home_count++;
				movement_instruction_ISR = 0;
			}

			else if(movement_instruction_ISR == 5)
			{
				//printf("stopped moving\r\n");
				stop_motors();
				mode = 0;
				delay_ms(500);

				movement_instruction_ISR = 0;
			}

			else{
				stop_motors();
				delay_ms(200);
				movement_instruction_ISR = 0;
			}
			
		}
	
	delay_ms(50);

	}
}



/*void main(void)
{
	CFGCON = 0;
  
    UART2Configure(115200);  // Configure UART2 for a baud rate of 115200
    
    ANSELB &= ~(1<<15); // Set RB15 as a digital I/O
    TRISB |= (1<<15);   // configure pin RB15 as input
    CNPUB |= (1<<15);   // Enable pull-up resistor for RB15
 
    delayms(500);
    printf("\r\nPIC32 push button test. Connect push button between RB15 (pin 26) and ground.\r\n");
    
    while(1)
    {
        printf("\rRB15(pin 26): %c", PORTB&(1<<15)?'1':'0');
        fflush(stdout);
        delayms(300);
    }
}
*/
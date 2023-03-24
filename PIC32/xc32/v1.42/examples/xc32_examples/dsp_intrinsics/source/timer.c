/*********************************************************************
 *
 *                  Using DSP Intrinsics for PIC32
 *
 *	This example demonstrates how PIC32 MZ EF can be programmed to
 * efficiently use the DSP extension.
 *
 *
 *
 *********************************************************************
 * FileName:        timer.c
 * Dependencies:	xc.h
 *
 * Processor:       PIC32 MZ EF
 *
 * Compiler:        MPLAB XC32
 *                  MPLABX IDE
 * Company:         Microchip Technology Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the “Company”) for its PIC32 Microcontroller is intended
 * and supplied to you, the Company’s customer, for use solely and
 * exclusively on Microchip PIC32 Microcontroller products.
 * The software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *
 ********************************************************************/

#include <xc.h>

static unsigned long start_time_val, stop_time_val;

unsigned long timer_count(void)
{
	return (TMR2);
}

void timer_start(void)
{
    TMR2 = 0;
    TMR3 = 0;
    start_time_val = timer_count();
    T2CONbits.TON = 1;
}

void timer_stop(void)
{
    T2CONbits.TON = 0;
    stop_time_val = timer_count();
}

unsigned long timer_get_time(void)
{
   return stop_time_val - start_time_val; 
}

void timer_init()
{
 
    /* Ensure Timer 2 is in reset state */
    T2CON = 0;

    /* Reset Timer 2 interrupt flag */
    IFS0bits.T2IF = 0;

    /* Disable Timer 2 interrupt */
    IEC0bits.T2IE = 1;
        
    /* Set Timer 2 and Timer 3 period register */
    PR2 = 0xFFFF;
    PR3 = 0xFFFF;

    /* Set the timer prescaler for 1:16 */
    T2CONbits.TCKPS = 4;

    /* 32 bit mode */
    T2CONbits.T32 = 1;

    /* Select internal timer clock */
    T2CONbits.TCS = 0;

    /* Reset TIMER2 and TIMER3*/
    TMR2 = 0;
    TMR3 = 0;
    /* Enable TIMER2*/
    T2CONbits.TON = 1;
}

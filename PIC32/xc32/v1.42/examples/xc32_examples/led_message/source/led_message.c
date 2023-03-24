/*********************************************************************
 *
 *                  Message via 8 LEDs
 *
 *********************************************************************
 * FileName:        led_message.c
 * Dependencies:	xc.h
 *
 *
 * Processor:       PIC32
 *
 * Compiler:        MPLAB XC32
 *                  MPLAB IDE v8.0+
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
 * $Id: led_message.c 9560 2008-07-04 10:10:53Z deshmukha $
 *
 ********************************************************************/

/*
** Message in a bottle
**
** Explorer16 version (long delays)
**
** Thanks to Lucio DiJasio for letting us use this example.
**
** Run this example on Explorer-16 board with PIC32MX PIM.
** Hold the board vertically from the PICtail connector size
** and wave the board back-and-forth to see message "HELLO" on LEDs
*/

#include <xc.h>

// Config settings
// POSCMOD = HS, FNOSC = PRIPLL, FWDTEN = OFF
// PLLIDIV = DIV_2, PLLMUL = MUL_16
// PBDIV = 8 (default)
// Main clock = 8MHz /2 * 16    = 80MHz
// Peripheral clock = 80MHz /8  =  10MHz

// Configuration Bit settings
// SYSCLK = 80 MHz (8MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 10 MHz
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
// WDT OFF
// Other options are don't care
//
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_8



// 1. define timing constant
#define SHORT_DELAY (50*8)
#define LONG_DELAY	(400*8)

// 2. declare and initialize an array with the message bitmap
char bitmap[30] = {
    0xff,	// H
    0x08,
    0x08,
    0xff,
    0,
    0,
    0xff,	// E
    0x89,
    0x89,
    0x81,
    0,
    0,
    0xff,	// L
    0x80,
    0x80,
    0x80,
    0,
    0,
    0xff,	// L
    0x80,
    0x80,
    0x80,
    0,
    0,
    0x7e,	// O
    0x81,
    0x81,
    0x7e,
    0,
    0
    };

// 3. the main program
main()
{
    // disable JTAG port
    DDPCONbits.JTAGEN = 0;

    // 3.1 variable declarations
    int i;           // i will serve as the index

    // 3.2 initialization
    TRISA = 0;      // all PORTA as output
    T1CON = 0x8030; // TMR1 on, prescale 1:256 PB

    // 3.3 the main loop
    while( 1)
    {
        // 3.3.1 display loop, hand moving to the right
        for( i=0; i<30; i++)
        {	// 3.3.1.1 update the LEDs
            PORTA = bitmap[i];

            // 3.3.1.2 short pause
            TMR1 = 0;
            while ( TMR1 < SHORT_DELAY)
            {
            }
        } // for i

        // 3.3.2 long pause, hand moving back to the left
        PORTA = 0;      // turn LEDs off
        TMR1 = 	0;
        while ( TMR1 < LONG_DELAY)
        {
        }
    } // main loop
} // main

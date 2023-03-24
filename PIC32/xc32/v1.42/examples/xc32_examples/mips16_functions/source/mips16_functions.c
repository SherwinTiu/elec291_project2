/*********************************************************************
 *
 *                  Declaring mips16e functions
 *
 *	This example demonstrates how a source file of mixed MIPS32 and MIPS16
 *  functions could be written.
 *
 *
 *
 *********************************************************************
 * FileName:        mips16_functions.c
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
 * $Id: mips16_functions.c 9560 2008-07-04 10:10:53Z deshmukha $
 *
 ********************************************************************/

#include <xc.h>			// Required for SFR defs


// Configuration Bit settings
// SYSCLK = 80 MHz (8MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 40 MHz
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
// WDT OFF
// Other options are don't care
//
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_2


// Use of mips16 attribute will ensure that following function is always
// compiled in MIPS16 instruction set.

int __attribute__((mips16,noinline)) MIPS16Function(int a, int b);

// Use of nomips16 attribute will ensure that the following function
// is always compiled in mips32 instruction set even if this file
// is built with -mips16 command-line option.
int __attribute__((nomips16,noinline)) NotAMIPS16Function(int a, int b);

int main(void)
{
	int i, j;

	// Prepare some dummy parameters - don't care what they are.
	i = 123;
	j = 456;

	// Call the MIPS16 function.
	MIPS16Function(i, j);

	// Call an always MIPS32 function.
	NotAMIPS16Function(i, j);


	while(1);
}

// A MIPS16 function
int __attribute__ ((mips16,noinline)) MIPS16Function(int a, int b)
{
	return (a*b + a + b);
}

// A MIPS16 function
int __attribute__ ((nomips16,noinline)) NotAMIPS16Function(int a, int b)
{
	return (a*b + a + b);
}

/*********************************************************************
 *
 *                  Using a custom ld file
 *
 *	This example shows an application can use a modified custom ld file
 *
 *
 *
 *********************************************************************
 * FileName:        custom_ld_file.c
 * Dependencies:	xc.h

 *
 *
 * Processor:       PIC32
 *
 * Compiler:        MPLAB XC32
 *                  MPLAB X 1.40
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
 * MPLAB XC32 uses two part default linker script file. See C32 User's
 * guide to understand the structure of default linker script file.
 *
 * For simple changes such as changing memory allocation and defining
 * a new memory section, only procdef.ld file needs modification.
 * Steps required to customize procdef.ld file are:
 *
 * 1. Depending on the processor you are using, copy the procdef.ld file
 *    from <xc32-install-path>/pic32mx/lib/proc/<processor> directory into
 *    your MPLAB X project directory.
 * 2. Modify the file as per your requirements.
 * 3. Rebuild your project.
 *
 * See procdef.ld in ../ directory to learn how changes can be made.

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


// Allocate myMemory array in my_space section. Because this array is at a global level,
// it will be initialized to all zeros.
// If all zeros are not desired, you will have to declare a pointer to my_reserved_flash_addr
// variable as defined in procdef.ld file.
const unsigned char myMemory[1024] __attribute__((section (".my_space"),space(prog))) = {1};

// You may also use the address() attribute to locate a function or variable at an absolute
// address in the kseg0_program_mem or kseg1_data_mem memory region. Use the space(prog)
// or space(data) attribute along with the address attribute.
const unsigned char in_prog __attribute__((address(0x9D000100),space(prog))) = 2;
unsigned char in_data __attribute__((address(0xA0004000),space(data))) = 3;

// This is how you can get uninitialized array pointing to my_space
// extern const char my_reserved_flash_addr[];
// unsigned char *myMemory = my_reserved_flash_addr;

void foo(int index);

int main(void)
{
	int index;

	foo(99);

    while(1); /* Don't return from main */
	return 0;
}


void foo(int index)
{
	int i;

	// Do something
	i = 20 + myMemory[index];
}

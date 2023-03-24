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
 * FileName:        defines.h
 * Dependencies:	stdfix.h
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

#ifndef _DEFINES_H
#define _DEFINES_H

#include <stdfix.h>

typedef short v2q15 __attribute__ ((vector_size(4)));
typedef int i32;
typedef long long a64;

#define BUFSIZE 2048

#endif
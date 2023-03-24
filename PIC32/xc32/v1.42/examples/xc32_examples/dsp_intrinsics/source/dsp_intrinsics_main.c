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
 * FileName:        dsp_intrinsics_main.c
 * Dependencies:	xc.h
 *                  stdio.h
 *                  string.h
 *					limits.h
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
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "defines.h"
#include "filter.h"
#include "timer.h"

// Configuration Bit settings for PIC32 MZ EF (32MZ2048EFM144) Starter Kit
// SYSCLK = 200 MHz (8MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
#pragma config FPLLIDIV = DIV_1, FPLLMULT = MUL_50, FPLLODIV = DIV_2, FWDTEN = OFF
#pragma config FPLLRNG = RANGE_5_10_MHZ, FPLLICLK = PLL_FRC, FNOSC = SPLL
#pragma config FSOSCEN = OFF, IESO = ON, POSCMOD = OFF, FCKSM = CSECMD
#pragma config FDMTEN = OFF, DMTCNT = DMT31, ICESEL = ICS_PGx2
#pragma config BOOTISA = MICROMIPS

// 16-bit data buffers for Q15 data
short coeff[BUFSIZE] __attribute__ ((aligned(4)));
short delay[BUFSIZE] __attribute__ ((aligned(4)));

// 32-bit integer buffers
i32 data_32[BUFSIZE];
i32 coeff_32[BUFSIZE];

// 64-bit accumulators
a64 res1, res2;

static char outputbuffer[1024];

/*
 * Initialize 16-bit data buffers with random values.
 */
static void init_data_16(short *coeff, short *delay, int buflen)
{
    int i;
    
    for (i = 0; i < buflen; i++) {
        coeff[i] = rand() % SHRT_MAX;
        delay[i] = rand() % SHRT_MAX;
    }
}

/*
 * Initialize 32-bit buffers with random values.
 */
static void init_data_32(i32 *coeff, i32 *data, int buflen)
{
    int i;
    
    for (i = 0; i < buflen; i++) {
        coeff[i] = rand() % INT_MAX;
        data[i] = rand() % INT_MAX;
    }
}

int main(int argc, char** argv)
{  
    volatile unsigned long time;
    
    // Init data buffers
    init_data_16(coeff, delay, BUFSIZE);
    init_data_32(coeff_32, data_32, BUFSIZE);
    
    // Init timer
    timer_init();
    
    // Run filter example without DSP Intrinsics
    timer_start();
    res1 = FIR_Filter_Traditional_16(delay, coeff, BUFSIZE);
    timer_stop();  
    time = timer_get_time();    
    sprintf(outputbuffer, "16-bit without DSP Intrinsics: timer ticks %lu\n", time);
    
    // Run filter example with DSP Intrinsics
    timer_start();
    res2 = FIR_Filter_Intrinsics_16(delay, coeff, BUFSIZE);
    timer_stop();
    time = timer_get_time();
    sprintf(outputbuffer + strlen(outputbuffer), "16-bit with DSP Intrinsics: timer ticks: %lu\n", time);  
    
    // Check results are the same
    if (res1 != res2) {
        sprintf(outputbuffer + strlen(outputbuffer), "Error: Results do not match!\n");
    }
    
    // Run filter example without DSP Intrinsics (32-bit)
    timer_start();
    res1 = FIR_Filter_32(data_32, coeff_32, BUFSIZE);
    timer_stop();
    time = timer_get_time();
    sprintf(outputbuffer + strlen(outputbuffer), "32-bit without DSP Intrinsics: timer ticks %lu\n", time);
    
    // Run filter example with DSP Intrinsic (32-bit)
    timer_start();
    res2 = FIR_Filter_Intrinsics_32(data_32, coeff_32, BUFSIZE);
    timer_stop();
    time = timer_get_time();
    sprintf(outputbuffer + strlen(outputbuffer), "32-bit with DSP Intrinsics: timer ticks %lu\n", time);  

    // Check results are the same
    if (res1 != res2) {
        sprintf(outputbuffer + strlen(outputbuffer), "Error: Results do not match!\n");
    }
    
    return (EXIT_SUCCESS);
}


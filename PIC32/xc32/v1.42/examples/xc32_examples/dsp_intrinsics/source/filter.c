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
 * FileName:        filter.c
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

#include "defines.h"

/*
 * FIR filter example operating on 16-bit Q15 data.
 * 
 * C code non-optimized for DSP.
 */
long long FIR_Filter_Traditional_16(short *delay, short *coeff, int buflen)
{
    int i;
    short x, y;
    
    // 64-bit accumulator for result
    long long ac0 = 0;
    
    for (i = 0; i < buflen; i++) {
        x = coeff[i];
        y = delay[i];
        
        // check saturation condition
        if ((unsigned short)x == 0x8000 && (unsigned short)y == 0x8000) {
            ac0 += 0x7fffffff;
        } else {
            // multiply (Q15 x Q15) needs left shift
            // result is added to accumulator variable
            ac0 += ((x * y) << 1);
        }
    }
    return ac0;
}

/*
 * FIR filter example operating on 16-bit Q15 data.
 * 
 * Uses intrinsics supported by MIPS32 DSP ASE.  
 */
a64 FIR_Filter_Intrinsics_16(short *delay, short *coeff, int buflen)
{
    int i;
    
    // cast arrays to 32-bit vector type v2q15 (holds two 16-bit Q15 values)
    v2q15 *my_delay = (v2q15 *)delay;
    v2q15 *my_coeffs = (v2q15 *)coeff;
    
    // 64-bit accumulator for result
    a64 ac0 = 0;
    
    for (i = 0; i < buflen/2; i++) {
        // Call "Dot Product with Accumulate/Subtract" DSP Intrinsic.
        // MIPS32 DSP ASE instructions running on vector type operands are 
        // generated which leads to more efficient execution.
        // Check generated assembly to see the machine instructions.
        ac0 = __builtin_mips_dpaq_s_w_ph (ac0, my_delay[i], my_coeffs[i]);
    }
    
    return ac0;
}

/*
 * FIR filter example operating on 32-bit integer data.
 * 
 * C code gets optimized and generates "Multiply Add" (MADD) from MIPS32 
 * DSP ASE instructions.
 */
a64 FIR_Filter_32(i32 *data, i32 *coeff, int buflen)
{
    int i;
    
    // 64-bit accumulator for result
    a64 acc = 0;
    
    for (i = 0; i < buflen; i++) {
        // cast 32-bit operand to 64-bit before multiply
        // in order to get compiler to generate the MADD instruction
        acc += (long long)data[i] * coeff[i];
    }
    
    return acc;
}

/*
 * FIR filter example operating on 32-bit integer data.
 * 
 * Uses intrinsics supported by MIPS32 DSP ASE.
 */
a64 FIR_Filter_Intrinsics_32(i32 *data, i32 *coeff, int buflen)
{
    int i;
  
    // 64-bit accumulator for result
    a64 acc = 0;
    
    for (i = 0; i < buflen; i++) {
        // Call "Signed Multiply and Add" DSP Intrinsic 
        // for 32-bit operands and 64-bit accumulator
        // Check generated assembly to see the machine instructions.
        acc = __builtin_mips_madd (acc, data[i], coeff[i]);
    }
    
    return acc;
}


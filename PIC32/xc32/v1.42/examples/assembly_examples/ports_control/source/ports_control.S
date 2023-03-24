/*********************************************************************
 *
 *                  Example Assembly Project
 *
 *********************************************************************
 * FileName:        ports_control.S
 *
 * Processor:       PIC32MX
 * 
 * Assembler/Compiler/Linker:  MPLAB XC32
 *
 * Company:         Microchip Technology Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the “Company”) for its PICmicro® Microcontroller is intended and
 * supplied to you, the Company’s customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
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
 *********************************************************************
 *
 * $Id: $
 *
 * Description:
 * This project demonstrates how to build projects with only
 * assembly language source files.
 * This example uses "S" extension so that the 'C' pre-processor
 * can be used.
 *
 * This example provides simple assembly application. It uses
 * standard C Startup code probided by the C32 compiler.
 *
 ********************************************************************/
#include <xc.h>

#define IOPORT_BIT_7 (1 << 7)  

   /* define all global symbols here */
   .global main

   /* define which section (for example "text")
     * does this portion of code resides in. Typically,
     * all your code will reside in .text section as
     * shown below.
    */
   .text

   /* This is important for an assembly programmer. This
     * directive tells the assembler that don't optimize
     * the order of the instructions as well as don't insert
     * 'nop' instructions after jumps and branches.
    */
   .set noreorder

/*********************************************************************
 * main()
 * This is where the PIC32 start-up code will jump to after initial
 * set-up.
 ********************************************************************/

.ent main /* directive that marks symbol 'main' as function in ELF
           * output
           */

main:

   /* Call function to clear bit relevant to pint 7 in port A.
     * The 'jal' instruction places the return address in ra.
     */
   ori      a0, zero, IOPORT_BIT_7
   jal      mPORTAClearBits
   nop

   /* endless loop */
endless:
   j      endless
   nop

.end main /* directive that marks end of 'main' function and registers
           * size in ELF output
           */

/*********************************************************************
 * mPORTAClearBits(int bits)
 * This function clears the specified bites in IOPORT A.
 *
 * pre-condition: ra contains return address
 * Input: Bit mask in a0
 * Output: none
 * Side effect: clears bits in IOPORT A
 ********************************************************************/
.ent mPORTAClearBits 
mPORTAClearBits:
   /* function prologue - save registers used in this function 
     * on stack and adjust stack-pointer
     */
   addiu   sp, sp, -4
   sw      s0, 0(sp)

   la      s0, LATACLR
   sw      a0, 0(s0)      /* clear specified bits */
   
   /* function epilogue - restore registers used in this function
     * from stack and adjust stack-pointer
     */
   lw      s0, 0(sp)
   addiu   sp, sp, 4

   /* return to caller */
   jr      ra
   nop
.end mPORTAClearBits
   


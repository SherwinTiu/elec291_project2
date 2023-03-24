/*********************************************************************
 *
 *                  Example Assembly Project
 *
 *********************************************************************
 * FileName:        example.S
 *
 * Processor:       PIC32MX
 * 
 * Assembler/Compiler/Linker:  Microchip MPLAB XC32 v1.00+
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
 * Description:
 * This project demonstrates how to build projects with only
 * assembly language source files.
 * This example uses "S" file-name extension so that the 
 * 'C' pre-processor can be used.
 *
 ********************************************************************/
#include <xc.h>

/* One second time period for core running at 8MHz */
#define TIMER_PERIOD (8000000)

#define IOPORT_BIT_7 (1 << 7)
#define CT_INT_ON (1 << 15)
#define CT_INT_PRIOR_3 (3)
  

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

   jal      mPORTAOutputConfig
   nop

   jal      mPORTAToggleBits
   nop

   jal      INTEnableSystemMultiVectoredInt
   nop

   lui      a0, (TIMER_PERIOD >> 16)
   ori      a0, a0, (TIMER_PERIOD & 0xFFFF)
   jal      OpenCoreTimer
   nop

   lui      a0, ((CT_INT_ON | CT_INT_PRIOR_3) >> 16)
   ori      a0, a0, ((CT_INT_ON | CT_INT_PRIOR_3) & 0xFFFF)
   jal      mConfigIntCoreTimer
   nop

   /* endless loop */
endless:
   j      endless
   nop

.end main /* directive that marks end of 'main' function and registers
           * size in ELF output
           */


/*********************************************************************
 * This is the actual interrupt handler that gets installed
 * in the interrupt vector table. It jumps to the core-timer
 * interrupt handler function.
 *
 * Note: The ".section .vector_0" is not allocatable. Hence to force
 * this section to be allocatable, use the "ax" directive.
 ********************************************************************/
.section .vector_0,code
   j      CoreTimerIntHandler
   nop   


/*********************************************************************
 * CoreTimerIntHandler()
 * Interrupt handler function for core-timer. The function
 * clears the interrupt flag, toggles the LED and updates the
 * core-timer register with new time period.
 *
 * pre-condition: A jump to ISR is registered in vector table
 * Input: none
 * Output: none
 * Side effect: toggles LED
 ********************************************************************/
.text
.ent CoreTimerIntHandler 
CoreTimerIntHandler:
   /* interrupt prologue */
   rdpgpr      sp, sp
   mfc0        k0, $13          /* read CAUSE register */
   mfc0        k1, $14          /* read EPC register */
   srl         k0, k0,0xA      /* align RIPL to bit 0 */
   addiu       sp, sp, -16      /* 4 words space on stack */
   sw          k1, 12(sp)      /* save EPC on stack */
   mfc0        k1, $12         /* read STATUS register */
   sw          k1, 8(sp)         /* save STATUS on stack */
   ins         k1, k0, 10, 6      /* insert RIPL to IPL field */
   ins         k1, zero, 1, 4
   mtc0        k1, $12         /* write STATUS register */
   sw          ra, 4(sp)         /* save Return Addr on stack */
   sw          a0, 0(sp)         /* save A0 on stack */

   /* clear interrupt flag */
   jal      mCTClearIntFlag
   nop

   /* set up a0 with time period and then call the
     * update core-timer routine for next interrupt.
     */
   lui      a0, (TIMER_PERIOD >> 16)
   ori      a0, a0, (TIMER_PERIOD & 0xFFFF)   
   jal      UpdateCoreTimer
   nop

   /* set up a0 with IOPORT A bit 7 position.
    * toggle the bit which toggles LED.
     */
   ori      a0, zero, IOPORT_BIT_7
   jal      mPORTAToggleBits
   nop
   
   /* interrupt epilogue */
   lw          a0, 0(sp)         /* restore A0 from stack */
   lw         ra, 4(sp)         /* restore Return Addr from stack */
   di                         /* disable interrupts */
   lw          k0, 12(sp)      /* restore EPC from stack */
   mtc0        k0, $14
   lw          k0, 8(sp)         /* restore STATUS from stack */
   addiu       sp, sp, 16      /* adjust stack */
   mtc0        k0, $12
   ei                        /* enable interrupts */

   /* return from interrupt */
   eret        
.end CoreTimerIntHandler



/*********************************************************************
 * mPORTAClearBits(int bits)
 * This function clears the specified bits in IOPORT A.
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

/*********************************************************************
 * mPORTAOutputConfig(int bits)
 * This function sets the specified bits as output for port A.
 *
 * pre-condition: ra contains return address
 * Input: Bit mask in a0
 * Output: none
 * Side effect: sets bits in IOPORT A as output
 ********************************************************************/
.ent mPORTAOutputConfig 
mPORTAOutputConfig:
   /* function prologue - save registers used in this function 
     * on stack and adjust stack-pointer
     */
   addiu   sp, sp, -4
   sw      s0, 0(sp)

   la      s0, TRISACLR
   sw      a0, 0(s0)      /* config specified bits */
   
   /* function epilogue - restore registers used in this function
     * from stack and adjust stack-pointer
     */
   lw      s0, 0(sp)
   addiu   sp, sp, 4

   /* return to caller */
   jr      ra
   nop
.end mPORTAOutputConfig

/*********************************************************************
 * mPORTAToggleBits(int bits)
 * This function toggles the specified bits of port A.
 *
 * pre-condition: ra contains return address
 * Input: Bit mask in a0
 * Output: none
 * Side effect: sets bits in IOPORT A as output
 ********************************************************************/
.ent mPORTAToggleBits 
mPORTAToggleBits:
   /* function prologue - save registers used in this function 
     * on stack and adjust stack-pointer
     */
   addiu   sp, sp, -4
   sw      s0, 0(sp)

   la      s0, LATAINV
   sw      a0, 0(s0)      /* toggle specified bits */
   
   /* function epilogue - restore registers used in this function
     * from stack and adjust stack-pointer
     */
   lw      s0, 0(sp)
   addiu   sp, sp, 4

   /* return to caller */
   jr      ra
   nop
.end mPORTAToggleBits

/*********************************************************************
 * mCTClearIntFlag()
 * This function clears interrupt flag.
 *
 * pre-condition: ra contains return address
 * Input: none
 * Output: none
 ********************************************************************/
.ent mCTClearIntFlag
mCTClearIntFlag:
   /* function prologue - save registers used in this function 
     * on stack and adjust stack-pointer
     */
   addiu   sp, sp, -8
   sw      s0, 0(sp)
   sw      s1, 4(sp)

   addiu   s1, zero, 1
   la      s0, LATAINV
   sw      s1, 0(s0)      /* toggle specified bits */
   
   /* function epilogue - restore registers used in this function
     * from stack and adjust stack-pointer
     */
   lw      s1, 4(sp)
   lw      s0, 0(sp)
   addiu   sp, sp, 8

   /* return to caller */
   jr      ra
   nop
.end mCTClearIntFlag

/*********************************************************************
 * mCTSetIntPriority(unsigned long priority)
 * This function sets specified interrupt priority.
 *
 * pre-condition: ra contains return address
 * Input: priority value in a0
 * Output: none
 ********************************************************************/
.ent mCTSetIntPriority
mCTSetIntPriority:
   /* function prologue - save registers used in this function 
     * on stack and adjust stack-pointer
     */
   addiu   sp, sp, -8
   sw      s0, 0(sp)
   sw      s1, 4(sp)

   addiu   s1, zero, (7 << 2)
   la      s0, IPC0CLR
   sw      s1, 0(s0)

   sll     s1, a0, 2
   la      s0, IPC0SET
   sw      s1, 0(s0)
   
   /* function epilogue - restore registers used in this function
     * from stack and adjust stack-pointer
     */
   lw      s1, 4(sp)
   lw      s0, 0(sp)
   addiu   sp, sp, 8

   /* return to caller */
   jr      ra
   nop
.end mCTSetIntPriority

/*********************************************************************
 * mCTIntEnable()
 * This function enables interrupts in interrupt ctl register.
 *
 * pre-condition: ra contains return address
 * Input: none
 * Output: none
 ********************************************************************/
.ent mCTIntEnable
mCTIntEnable:
   /* function prologue - save registers used in this function 
     * on stack and adjust stack-pointer
     */
   addiu   sp, sp, -8
   sw      s0, 0(sp)
   sw      s1, 4(sp)

   addiu   s1, zero, 1

   la      s0, IEC0CLR
   sw      s1, 0(s0)

   la      s0, IEC0SET
   sw      s1, 0(s0)
   
   /* function epilogue - restore registers used in this function
     * from stack and adjust stack-pointer
     */
   lw      s1, 4(sp)
   lw      s0, 0(sp)
   addiu   sp, sp, 8

   /* return to caller */
   jr      ra
   nop
.end mCTIntEnable


/*********************************************************************
 * OpenCoreTimer (unsigned long period)
 * This function clears COUNT register and sets the specified
 * period value in the COMPARE register.
 *
 * pre-condition: ra contains return address
 * Input: period value in a0
 * Output: none
 ********************************************************************/
.ent OpenCoreTimer
OpenCoreTimer:
   mtc0   $0, $9
   mtc0   a0, $11

   /* return to caller */
   jr      ra
   nop
.end OpenCoreTimer

/*********************************************************************
 * mConfigIntCoreTimer (unsigned long config)
 * This function configures the core-timer.
 *
 * pre-condition: ra contains return address
 * Input: config value in a0
 * Output: none
 ********************************************************************/
.ent mConfigIntCoreTimer
mConfigIntCoreTimer:

   /* function prologue */
   addiu   sp, sp, -4
   sw      ra, 0(sp)

   jal      mCTClearIntFlag
   nop

   jal      mCTSetIntPriority
   nop   

   jal      mCTIntEnable
   nop   

   /* function epilogue */
   lw      ra, 0(sp)
   addiu   sp, sp, 4

   /* return to caller */
   jr      ra
   nop
.end mConfigIntCoreTimer

/*********************************************************************
 * INTEnableSystemMultiVectoredInt()
 * This function enables multi-vector interrupts.
 *
 * pre-condition: ra contains return address
 * Input: none
 * Output: none
 ********************************************************************/
.ent INTEnableSystemMultiVectoredInt
INTEnableSystemMultiVectoredInt:

   /* function prologue - save registers used in this function 
     * on stack and adjust stack-pointer
     */
   addiu   sp, sp, -8
   sw      s0, 0(sp)
   sw      s1, 4(sp)

   mfc0    s0, $13
   lui     s1, 0x0080
   or      s0, s0, s1
   mtc0    s0, $13

   /* function epilogue - restore registers used in this function
     * from stack and adjust stack-pointer
     */
   lw      s1, 4(sp)
   lw      s0, 0(sp)
   addiu   sp, sp, 8
   
   /* return to caller */
   jr      ra
   ei      /* enable system-wide interrupts */
.end INTEnableSystemMultiVectoredInt

/*********************************************************************
 * UpdateCoreTimer(unsigned int period)
 * This function updates the COMPARE register with specified
 * period value.
 *
 * pre-condition: ra contains return address
 * Input: a0 has period value
 * Output: none
 ********************************************************************/
.ent UpdateCoreTimer
UpdateCoreTimer:
   /* function prologue - save registers used in this function 
     * on stack and adjust stack-pointer
     */
   addiu   sp, sp, -4
   sw      s0, 0(sp)

   mfc0    s0, $11
   addu    s0, s0, a0 /* note we use 'addu' instruction and
                           * not 'add' to avoid overflow trap
                           */
   mtc0    s0, $11

   /* function epilogue - restore registers used in this function
     * from stack and adjust stack-pointer
     */
   lw      s0, 0(sp)
   addiu   sp, sp, 4
   
   /* return to caller */
   jr      ra
   nop
.end UpdateCoreTimer









   


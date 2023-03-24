/*
 * This software is developed by Microchip Technology Inc. and its
 * subsidiaries ("Microchip").
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1.      Redistributions of source code must retain the above copyright
 *         notice, this list of conditions and the following disclaimer.
 * 2.      Redistributions in binary form must reproduce the above
 *         copyright notice, this list of conditions and the following
 *         disclaimer in the documentation and/or other materials provided
 *         with the distribution.
 * 3.      Microchip's name may not be used to endorse or promote products
 *         derived from this software without specific prior written
 *         permission.
 *
 * THIS SOFTWARE IS PROVIDED BY MICROCHIP "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL MICROCHIP BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING BUT NOT LIMITED TO
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWSOEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *-------------------------------------------------------------------------*/
#include <xc.h>
#include <cp0defs.h>
#include <sys/attribs.h>
#include <iostream>

// Configuration Bit settings
// SYSCLK = 80 MHz (8MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 40 MHz
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
// WDT OFF
// Other options are don't care
//
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_1

#define CORE_TICK_RATE          1000000u

using namespace std;
volatile unsigned long my_count = 0;

// An ISR must be in the "C" namespace.
extern "C"
void __ISR(_CORE_TIMER_VECTOR, IPL2SOFT) CoreTimerHandler(void)
{
    unsigned long old_count, period;
    old_count = _CP0_GET_COUNT();
    
    // clear the interrupt flag
    IFS0CLR = _IFS0_CTIF_MASK;

    // .. things to do
    cout << "Executing CoreTimerHandler #" << my_count++ << endl;
    
    // update the period
    period = CORE_TICK_RATE;
    period += old_count;
    _CP0_SET_COMPARE(period);
}

int __attribute__((nomips16)) main (void)
{
    unsigned int cause_val;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Add code to configure cache, wait states, and peripheral bus clock
    // See http://www.microchip.com/mplabharmony

    // Set stdout to go to UART 1 instead of UART 2
    __XC_UART=1;

    cout << "Beginning " __FILE__ " test" << endl;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Configure the core timer
	// clear the count reg
    _CP0_SET_COUNT(0);
    // set up the period in the compare reg
    _CP0_SET_COMPARE(CORE_TICK_RATE);

    // The Core timer should halt when we are halted at a debug breakpoint.
    _CP0_BIC_DEBUG(_CP0_DEBUG_COUNTDM_MASK);

    // set up the core timer interrupt with a prioirty of 2 and zero sub-priority
    IFS0CLR = _IFS0_CTIF_MASK;
    IPC0CLR = _IPC0_CTIP_MASK;
    IPC0SET = (2 << _IPC0_CTIP_POSITION);
    IPC0CLR = _IPC0_CTIS_MASK; 
    IPC0SET = (0 << _IPC0_CTIS_POSITION);
    IEC0CLR = _IEC0_CTIE_MASK;
    IEC0SET = (1 << _IEC0_CTIE_POSITION);

    // enable multi-vector interrupts
    // set the CP0 cause IV bit high
    cause_val = _CP0_GET_CAUSE();
    cause_val |= _CP0_CAUSE_IV_MASK;
    _CP0_SET_CAUSE(cause_val);
      
    INTCONSET = _INTCON_MVEC_MASK;
    // set the CP0 status IE bit high to turn on interrupts
    __asm__ volatile ("ei");

    while(1) {
       __asm__ volatile ("nop");
    }
}

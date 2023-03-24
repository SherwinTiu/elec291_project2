/* This example shows how to place PIC32 configuration
 * bits in an assembly file.
 * Typically this file should be a separate stand-alone file than
 * your source code files.
 */

/* The following setting sets up clock for 80MHz on Explorer 16+PIC32 platform */ 
 
 /*
    Configuration Bit settings - 0xBFC02FF4
    PLL Input Divider - 2x Divider
    PLL Multiplier - 20x Multiplier
    System PLL Output Clock Divider - PLL Divide by 1
 */
 .section .config_BFC02FF4,code
 .word 0xfff8ffd9
 
 /*
    Configuration Bit settings - 0xBFC02FF8
    Oscillator Selection Bits - Primary Osc w/PLL (XT+,HS+,EC+PLL)
    Secondary Oscillator Enable - Disabled
    Internal/External Switch Over - Disabled
    Primary Oscillator Configuration - HS osc mode
    CLKO Output Signal Active on the OSCO Pin - Enabled
    Peripheral Clock Divisor - PB_Clk is Sys_Clk/1
    Clock Switching and Monitor Selection - Clock switching disabled; fail safe clock monitor disabled
    Watchdog Timer Postscaler - 1:1
    Watchdog Timer Enable - WDT Disabled (SWDTEN Bit Controls)
 */
 .section .config_BFC02FF8,code
 .word 0xFF60CE5B
 
 /*
    Configuration Bit settings - 0xBFC02FFC
    ICE/ICD Comm Channel Select - ICE EMUC2/EMUD2 pins shared with PGC2/PGD2
    Boot Flash Write Protect - Boot Flash is writable
    Code Protect - Protection Disabled
 */
 .section .config_BFC02FFC,code
 .word 0x7FFFFFFA

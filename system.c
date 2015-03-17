/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 03/13/15     1.0_DW0a    Created.
/******************************************************************************/

/******************************************************************************/
/* Contains oscillator initialization functions.
 *
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>        /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>       /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>   /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */
#include <stdio.h>       /* For true/false definition */

#endif

#include "system.h"

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/

/******************************************************************************/
/* ConfigureOscillator
 *
 * The function waits here until the oscillator is ready.
/******************************************************************************/
void ConfigureOscillator(void)
{
    OSCCONbits.IRCF = 0xE;//8MHz
    #ifdef SysFreq32
    /* Turn on PLL */
    OSCCONbits.SPLLEN = 1;    
    #else
    /* Turn off PLL */
    OSCCONbits.SPLLEN = 0; 
    #endif

    while(!(OSCSTAT & HFIOFS));//Wait for High frequency inernal oscillator to be ready

    #ifdef SysFreq32
    while(!(OSCSTATbits.PLLR));//Wait for PLL to be ready
    #endif
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/
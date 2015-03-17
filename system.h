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
/* Files to Include                                                           */
/******************************************************************************/
#ifndef SYSTEM_H
#define	SYSTEM_H

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>    /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#endif

#include "user.h"

/******************************************************************************/
/* System frequency
 *
 * This is the CPU clock frequency.
 *
 * For this system the clock is either 32MHz and the internal PLL is used or
 *  8MHz and the PLL is not used. This is based off of the macro 'SysFreq32'
 *  found in user.h
 *
/******************************************************************************/
#ifdef SysFreq32
#define SYS_FREQ        32000000L
#else
#define SYS_FREQ        8000000L
#endif

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/

#define FCY             SYS_FREQ/4

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/

void ConfigureOscillator(void); /* Handles clock switching/osc initialization */

#endif	/* SYSTEM_H */
/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/
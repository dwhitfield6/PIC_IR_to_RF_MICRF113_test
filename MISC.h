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
#ifndef MISC_H
#define	MISC_H

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

#include "system.h"

/******************************************************************************/
/* Delay constant
 *
 * This parameter is set to calibrate the delayUS() function. Parameter found
 *  from guess and check.
/******************************************************************************/
/* inverse relationship on timing */
#define delayConst 31

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/

#ifdef SysFreq32
#define SYS_FREQ_US 32
#else
#define SYS_FREQ_US 8
#endif

#define TRUE 1
#define FALSE 0
#define YES 1
#define NO 0

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/

void delayUS(long US);
void cleanBuffer(unsigned char* data, int count);
void cleanBuffer16bit(unsigned int* data, int count);

#endif	/* MISC_H */
/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/
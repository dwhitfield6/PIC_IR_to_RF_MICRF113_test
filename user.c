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
/* Contains Functions for PIC initialization
 *
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
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
#include <stdio.h>         /* For sprintf definition */

#endif

#include "user.h"

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/

extern const unsigned char Version[];

/******************************************************************************/
/* Functions
/******************************************************************************/

/******************************************************************************/
/* InitApp
 *
 * The function initializes the application. It sets the pins and pull-ups.
/******************************************************************************/
void InitApp(void)
{
    BiGreen_TRIS    = OUTPUT;
    BiRed_TRIS      = OUTPUT;
    Red_TRIS        = OUTPUT;
    RFtx_TRIS       = OUTPUT;

    IRl_TRIS        = INPUT;
    IRh_TRIS        = INPUT;
    ANSELC = 0; // All port C is digital
    SLRCONCbits.SLRC3 = 0; // Maximum slew rate on RF pin
    LATC =0;

    /* Turn On global Interrupts and Peripheral */
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/

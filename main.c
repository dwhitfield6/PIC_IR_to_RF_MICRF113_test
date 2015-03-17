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
/* Contains main function.
 *
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#define USE_OR_MASKS

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "user.h"
#include "system.h"
#include "MISC.h"
#include "RF.h"
#include "Timer.h"
#include "IR.h"

/******************************************************************************/
/* Version number                                                             */
/******************************************************************************/

const unsigned char Version[] = {"1.0_DW0a"};

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

void main(void)
{
    unsigned char i=0;

    /* Initialize system */
    ConfigureOscillator();
    InitApp();
    init_Timers();
    init_IR();

    //blink LEDs
    LATC |= Red;
    for(i =0;i<10;i++)
    {
       LATC |= BiGreen;
       LATC &= ~BiRed;
       delayUS(40000);
       LATC |= BiRed;
       LATC &= ~BiGreen;
       delayUS(40000);
    }
    LATC &= ~(BiRed + Red);

    while(1)
    {
        if(IR_New_Code)
        {
            /* There was an IR code received */
            UseIRCode(&IR_New_Code,IR_NEC);
        }
    }
}


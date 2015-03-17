/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 01/21/15     1.2         Added log.
/******************************************************************************/

/******************************************************************************/
/* Contains RF functions.
 *
 * Chip PT2260a
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "RF.h"
#include "Timer.h"
#include "MISC.h"
#include "IR.h"

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/

unsigned char RFsendCode[RFmaxSize];
volatile unsigned char RFsendFlag = 0;
unsigned char RFcodePlace = 0;
unsigned char RFcodeBit = 0;
volatile unsigned char Sendcount =0;
volatile unsigned char Sent = 1;

/******************************************************************************/
/* Functions
 *
/******************************************************************************/

/******************************************************************************/
/* SendRF
 *
 * The function sends the RF channel code.
/******************************************************************************/
unsigned char SendRF(unsigned char* Code, unsigned char size, unsigned char RepeatAmount)
{
    unsigned char i;
    if(Sent == YES)
    {
        /* Previous send finished */
        Sent = NO;
        Sendcount = RepeatAmount;
        RFsendFlag = 1;
        RFcodeBit = 0;
        RFcodePlace = 1;


        if(size >= RFmaxSize)
        {
            return FAIL;
        }
        for(i=0;i<size;i++)
        {
            RFsendCode[i] = Code[i];
        }
        RFsendCode[i] = EndofRFcode;

        Timer2Reset();
        Set_Timer2(Sync);
        Timer2ON();
        return PASS;        
    }
    return SENDING;
}

/******************************************************************************/
/* GetRFstatus
 *
 * The function returns the status of the RF transmitter.
/******************************************************************************/
unsigned char GetRFstatus(void)
{
    if(!RFsendFlag)
    {
        return PASS;
    }
    return SENDING;
}

/******************************************************************************/
/* SendRF_wait
 *
 * Sends the code and waits here until finished. Send the 'amount' of repeats
 *  passed in as amount.
/******************************************************************************/
void SendRF_wait(unsigned char* Code, unsigned char size, unsigned char amount)
{
    while(SendRF(Code, size, amount) == SENDING);
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/
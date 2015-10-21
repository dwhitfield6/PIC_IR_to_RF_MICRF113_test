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
/* Contains Interupt service routine.
 *
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#define USE_OR_MASKS
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

#endif

#include "Timer.h"
#include "RF.h"
#include "MISC.h"
#include "IR.h"

/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/

/* Baseline devices don't have interrupts. Note that some PIC16's
 * are baseline devices.  Unfortunately the baseline detection macro is
 * _PIC12 */
#ifndef _PIC12

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/

volatile unsigned int TMR1_overflow = 0;
volatile unsigned int TMR4_overflow = 0;
volatile unsigned char LEDsdone;
/******************************************************************************/
/* ISR                                                                */
/******************************************************************************/
void interrupt isr(void)
{

    /* Dont allow nested ISRs */
    INTCONbits.GIE = 0;
    if(PIR1bits.TMR2IF && PIE1bits.TMR2IE)
    {
        /* Timer 2 interrupt */
        /* This is used for the RF Code */
        PIE1bits.TMR2IE = 0;//disable timer 2 interupt

        if(RFsendCode[RFcodeBit] != EndofRFcode)
        {
            /* In the middle of sending a code */
            if(RFsendFlag)
            {
                if(RFsendCode[RFcodeBit] == 0)
                {
                    if(RFcodePlace == 1)
                    {
                        RFon();
                        Set_Timer2(Short);
                        RFcodePlace++;
                        Timer2Reset();
                    }
                    else if(RFcodePlace == 2)
                    {
                        RFoff();
                        Set_Timer2(Long);
                        RFcodePlace++;
                    }
                    else if(RFcodePlace == 3)
                    {
                        RFon();
                        Set_Timer2(Short);
                        RFcodePlace++;
                        Timer2Reset();
                    }
                    else if(RFcodePlace == 4)
                    {
                        RFoff();
                        Set_Timer2(Long);
                        RFcodePlace = 1;
                        RFcodeBit++;
                    }
                    else
                    {
                        /* Not supposd to get here */
                        RFcodePlace = 1;
                        RFsendFlag = 0;
                    }
                }
                else if(RFsendCode[RFcodeBit] == 1)
                {
                    if(RFcodePlace == 1)
                    {
                        RFon();
                        Set_Timer2(Long);
                        RFcodePlace++;
                    }
                    else if(RFcodePlace == 2)
                    {
                        RFoff();
                        Set_Timer2(Short);
                        RFcodePlace++;
                        Timer2Reset();
                    }
                    else if(RFcodePlace == 3)
                    {
                        RFon();
                        Set_Timer2(Long);
                        RFcodePlace++;
                    }
                    else if(RFcodePlace == 4)
                    {
                        RFoff();
                        Set_Timer2(Short);
                        RFcodePlace = 1;
                        RFcodeBit++;
                        Timer2Reset();
                    }
                    else
                    {
                        /* Not supposd to get here */
                        RFcodePlace = 1;
                        RFsendFlag = 0;
                    }
                }
                else
                {
                    /* The bit is an f (floating)*/
                    if(RFcodePlace == 1)
                    {
                        RFon();
                        Set_Timer2(Short);
                        RFcodePlace++;
                        Timer2Reset();
                    }
                    else if(RFcodePlace == 2)
                    {
                        RFoff();
                        Set_Timer2(Long);
                        RFcodePlace++;
                    }
                    else if(RFcodePlace == 3)
                    {
                        RFon();
                        Set_Timer2(Long);
                        RFcodePlace++;
                    }
                    else if(RFcodePlace == 4)
                    {
                        RFoff();
                        Set_Timer2(Short);
                        RFcodePlace = 1;
                        RFcodeBit++;
                        Timer2Reset();
                    }
                    else
                    {
                        /* Not supposd to get here */
                        RFcodePlace = 1;
                        RFsendFlag = 0;
                    }
                }
            }
        }
        else
        {
            /* We finished sending one code and now need to sync */
            if(RFcodePlace == 1)
            {
                RFon();
                Set_Timer2(Short);
                RFcodePlace++;
                Timer2Reset();
            }
            else if( RFcodePlace == 2)
            {
                RFoff();
                Set_Timer2(Sync);
                RFcodePlace++;
                Sendcount--;
                if(Sendcount)
                {
                    /* Send another repeat */
                    RFcodePlace = 1;
                    RFcodeBit = 0;
                }
            }
            else
            {
                /* Finished sending all of the repeats */
                Timer4Reset();
                PIR2bits.TMR4IF = 0;//Clear Flag
                TMR4_overflow = 0;
                Set_Timer4(0xFF);
                Timer4ON();
                Timer2OFF();
                RFsendFlag = 0;
            }
        }
        PIR1bits.TMR2IF = 0; //Clear Flag
        if(RFsendFlag)
        {
            /* We are still in the middle of sending */
            PIE1bits.TMR2IE = 1;//Enable timer 2 interupt
        }
    }
    else if(PIR1bits.TMR1IF && PIE1bits.TMR1IE)
    {
        /* IR receiver Timeout or Repeat Code */
        PIE1bits.TMR1IE = 0;
        Reset_Timer1();
        #ifdef SysFreq32
        /* Set the buffer contents to 0 */
        if(TMR1_overflow)
        {
            TMR1_overflow = 0;
            IR_New_Code = 0;
            IRrawCodeNum = 0;
        }
        else
        {
            TMR1_overflow = 1;
            PIR1bits.TMR1IF = 0;
            PIE1bits.TMR1IE = 1;
            /* Clear Flag */
        }
        #else
        Stop_Timer1();
        IR_New_Code = 0;
        IR_NEC = 0;
        IRrawCodeNum = 0;
        #endif
        PIR1bits.TMR1IF = 0;
    }
    else if(IOCCFbits.IOCCF4 && INTCONbits.IOCIE)
    {
        /* Interrupt by the IR receiver */        
        /* disable negative edge interrupt */
        IOCCPbits.IOCCP4 = 0;
        /* disable negative edge interrupt */
        IOCCNbits.IOCCN4 = 0;

        IRrawCount = TMR1L;
        IRrawCount += ((unsigned int)TMR1H << 8);

        Stop_Timer1();
        Reset_Timer1();
        PIR1bits.TMR1IF = 0;
        Start_Timer1();
        PIE1bits.TMR1IE = 1;
        if(IRrawCodeNum < MaxScanEdgeChange)
        {
            /* We are still receiving an NEC code */
            if(IRrawCodeNum != 0)
            {
                #ifdef SysFreq32
                if(TMR1_overflow)
                {
                    IRRawCode[IRrawCodeNum - 1] = (IRrawCount >> 1) + 32768;
                    TMR1_overflow = 0;
                }
                else
                {
                    IRRawCode[IRrawCodeNum - 1] = IRrawCount >> 1;
                }
                #else
                IRRawCode[IRrawCodeNum - 1] = IRrawCount;
                #endif
                if( IRRawCode[IRrawCodeNum - 1] >= Repeatlower && IRRawCode[IRrawCodeNum - 1] <= Repeatupper )
                {
                    /* Repeat Code */
                    IR_New_Code = 2;
                    IRrawCodeNum =0;
                    IRRawCode[0] = Repeatlower;

                }
            }
            else
            {
                TMR1_overflow = 0;
                Stop_Timer1();
                Start_Timer1();
            }
            IRrawCodeNum++;
        }
        if(IRrawCodeNum == MaxScanEdgeChange)
        {
            /* Turn off timer interrupt because we did not Timeout */
            /* We received a full NEC code */
            PIR1bits.TMR1IF = 0;
            PIE1bits.TMR1IE = 0;
            Stop_Timer1();
            if(!IR_New_Code)
            {
                /* Convert from raw data to an NEC code */
                IR_New_Code = IRrawToNEC(IRRawCode, &IR_NEC, YES);
            }
            IRrawCodeNum = 0;
            TMR1_overflow = 0;
            IRreceived = TRUE;
        }

        /* enable negative edge interrupt */
        IOCCPbits.IOCCP4 = 1;
        /* enable negative edge interrupt */
        IOCCNbits.IOCCN4 = 1;
        /* Clear flags */
        IOCCFbits.IOCCF4 = 0;
        INTCONbits.IOCIF = 0;        
    }
    else if(PIR2bits.TMR4IF && PIE2bits.TMR4IE)
    {
        /* This is the timeout of the wait period for sending the RF data */
        Timer4OFF();
        if(TMR4_overflow < RFsendWaitTime)
        {
            Timer4Reset();
            Timer4ON();
            TMR4_overflow++;
        }
        else
        {
            /*
             * We waited the specified amount of timer to allow
             *  for another RF send
             */
            Sent = YES;
        }
    }
    else if(PIR2bits.TMR6IF && PIE2bits.TMR6IE)
    {
        /* This is the timeout of the wait period for the LEDs */
        Timer6OFF();
        LATC &= ~(BiGreen + BiRed + Red);
        LEDsdone = TRUE;
    }
    else
    {
        /* We should Never get here */
        NOP();
    }
    INTCONbits.GIE = 1;
}
#endif
/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/


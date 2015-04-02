/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 03/11/15     1.0         Initial Coding.
/******************************************************************************/

/******************************************************************************/
/* Contains functions for IR receiver.
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

#include "user.h"          /* User funct/params, such as InitApp */
#include "IR.h"          /* User funct/params, such as InitApp */
#include "Timer.h"          /* User funct/params, such as InitApp */
#include "MISC.h"          /* User funct/params, such as InitApp */
#include "RF.h"          /* User funct/params, such as InitApp */

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/

unsigned int BlueLEDcount = IR_LED_TIMEOUT;
unsigned char IRfault = 0;
extern unsigned char IRpinOLD;
unsigned int IRRawCode[IR_SIZE];
unsigned long IR_NEC;
unsigned int IRrawCount = 0;
unsigned char IRrawCodeNum = 0;
unsigned char IR_New_Code  = 0;
volatile unsigned char LEDsdone = TRUE;
volatile unsigned char IRreceived = FALSE;

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/

/******************************************************************************/
/* init_IR
 *
 * The function initializes IR receiver.
/******************************************************************************/
void init_IR(void)
{
    Enable_IR();
    /* Enable IOC interrupts */
    INTCONbits.IOCIE = 1;
    Timer1Init();
}

/******************************************************************************/
/* Enable_IR
 *
 * The function enables IR receive.
/******************************************************************************/
void Enable_IR(void)
{
    /* enable positive edge interrupt */
    IOCCPbits.IOCCP4 = 1;
    /* enable negative edge interrupt */
    IOCCNbits.IOCCN4 = 1;
}

/******************************************************************************/
/* Disable_IR
 *
 * The function disables IR receive.
/******************************************************************************/
void Disable_IR(void)
{
    /* Disable positive edge interrupt */
    IOCCPbits.IOCCP4 = 0;
    /* Disable negative edge interrupt */
    IOCCNbits.IOCCN4 = 0;
}

/******************************************************************************/
/* ReadIRpin
 *
 * The function reads the IR pin and return a 0 if low and 1 if High.
/******************************************************************************/
unsigned char ReadIRpin(void)
{
    //READ RC5
    if((PORTC & IRl) == IRl)
    {
        return (1);
    }
    else
    {
        return 0;
    }
}

/******************************************************************************/
/* IRrawToNEC
 *
 * The function converts from raw counts to NEC code.
/******************************************************************************/
unsigned char IRrawToNEC(unsigned int* Raw, unsigned long* NEC, unsigned char Invert)
{
    /* the Protocol is as follows
     *
     * 1. A 9ms leading pulse burst (16 times the pulse burst length used for
     *   a logical data bit)
     *
     * 2. A 4.5ms space
     *
     * 3. The 8-bit address for the receiving device
     *
     * 4. The 8-bit logical inverse of the address
     *
     * 5. The 8-bit command
     *
     * 6. The 8-bit logical inverse of the command
     *
     * 7. A final 562.5µs pulse burst to signify the end of message transmission.
     *
     * 8. A 40 mS pause signifies the end of entire transmission
     *
     * 9. A 9ms leading pulse burst
     *
     * 10. A 2.25ms space
     *
     * 11.A 562.5µs pulse burst to mark the end of the space
     *  (and hence end of the transmitted repeat code).
     */
    unsigned char i;
    unsigned char Pause =0;
    unsigned char first =0;
    unsigned char StartBit=IR_SIZE;
    unsigned long NECtemp = 0;

    /* Start Bit begin*/
    for (i =0; i < IR_SIZE; i++)
    {
        if(Raw[i] >= Repeatlower && Raw[i] <= Repeatupper )
        {
            /* This is the Repeat bit*/
            return 2;
        }
        else if(Raw[i] >= StartbitHIGHlower && Raw[i] <= StartbitHIGHupper )
        {
            /* This is the Start bit*/
            StartBit = i;
            break;
        }
    }
    if(IR_SIZE - StartBit < MaxScanEdgeChange)
    {
        /* No start bit found  or found too late in buffer */
        return 0;
    }

    StartBit++;
    if(Raw[StartBit] < StartbitLOWlower || Raw[StartBit] > StartbitLOWupper )
    {
        /* Start bit is wrong */
        return 0;
    }
    /* Start bit finish */
    
    StartBit++;
    for(i = StartBit; i < IR_SIZE;i++)
    {
        if(Raw[StartBit] >= DataShortlower && Raw[StartBit] <= DataShortupper )
        {
            if(Pause != 3)
            {
                if(!first)
                {
                    /* This is the beginning of the bit*/
                    first = 1;
                }
                else
                {
                    first = 0;
                    NECtemp <<= 1;
                    if(!Invert)
                    {
                        NECtemp++;
                    }
                    /* We just got 2 562.5 uS Pulses in a row */
                }
            }
            else
            {
                /* Finished Sequence */
                *NEC = NECtemp;
                return 1;
            }
        }
        else if(Raw[StartBit] >= DataLonglower && Raw[StartBit] <= DataLongupper)
        {
            if(first)
            {
                /* This is a logical Zero */
                NECtemp <<= 1;
                if(Invert)
                {
                    NECtemp++;
                }
                first = 0;
            }
            else
            {
                /* we just got 2 longs in a row which should never happen*/
                return 0;
            }
        }
        else if(Raw[StartBit] == 0 && first == 1)
        {
            /*
             * We got the final 562.5µs pulse burst to signify the end of
             *  message transmission and the rest of the buffer is empty.
             */
            *NEC = NECtemp;
            return 1;
        }
        else if(Raw[StartBit] >= PauseBurstlower && Raw[StartBit] <= PauseBurstupper)
        {
            /* First part of ending */
            Pause = 1;
        }
        else if(Pause == 1 && Raw[StartBit] >= StartbitHIGHlower && Raw[StartBit] <= StartbitHIGHupper)
        {
            /* second part of ending */
            Pause = 2;
        }
        else if(Pause == 2 && Raw[StartBit] >= PauseSpacelower && Raw[StartBit] <= PauseSpaceupper)
        {
            /* third part of ending */
            /* the only thing left is one final short pause */
            Pause = 3;
        }
        else if(Raw[StartBit] >= Repeatlower && Raw[StartBit] <= Repeatupper)
        {
            /* repeat character */
            *NEC = NECtemp;
            return 2;
        }
        else
        {
            return 0;
        }
        StartBit++;
    }
    /* Cant get here but do this to make compiler happy */
    return 0;
}

/******************************************************************************/
/* UseIRCode
 *
 * The function is called after the IR receiver either gets a repeat code or
 *  a new code.
/******************************************************************************/
void UseIRCode(unsigned char* Code, unsigned long NEC)
{
    Timer6OFF();
    Timer6Reset();
    if(*Code == 2)
    {
        LATC |= Red;
        LEDsdone = FALSE;
    }
    if(NEC == 0x1CE350AF)
    {
        LATC |= BiGreen;
        LEDsdone = FALSE;
        SendRF(ChannelF, 12, 10);
    }
    else
    {
        LATC |= BiRed;
        LEDsdone = FALSE;
    }
    while(!Sent);
    Set_Timer6(LEDtimeout);
    PIR2bits.TMR6IF = 0;//Clear Flag
    Timer6ON();
    *Code = 0;
}
/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/
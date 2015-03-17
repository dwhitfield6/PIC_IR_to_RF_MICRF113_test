/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 03/11/15     1.0_DW0a    Initial coding.
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#ifndef RF_H
#define	RF_H

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
/* Defines                                                                    */
/******************************************************************************/

#define f 3
#define FAIL 0
#define PASS 1
#define SENDING 2
#define EndofRFcode 99

/******************************************************************************/
/* Rf Codes
 *
 * This contains the RF codes that are sent from the system.
/******************************************************************************/

unsigned char ChannelF[] = {f,0,f,f,f,f,0,0,1,0,0,0};

/******************************************************************************/
/* RFmaxSize
 *
 * This is the size of the RF send buffer. A code can not be more bytes than
 *  this value.
/******************************************************************************/
#define RFmaxSize 20

/******************************************************************************/
/* RFsendWaitTime
 *
 * This is the amount of delay that the system waits before it allows another
 *  RF code to send. This is dependant upon the toggle speed of the end system.
/******************************************************************************/
#define RFsendWaitTime 6

/******************************************************************************/
/* RF bit Timing for PT2260a
 *  The Timer is clocked at 2 MHz with a post scaler of 15.
/******************************************************************************/
/* Short bit/pause */
/* Nominal 128 cycles or about 240uS*/
#define Short 8

/* Long bit/pause */
/* Nominal 384 cycles or about 240uS*/
#define Long 24

/* Sync bit/pause */
/* Nominal 4096 cycles or about 240uS*/
#define Sync 240

/******************************************************************************/
/* Macro Functions                                                            */
/******************************************************************************/

/******************************************************************************/
/* RFon
 *
 * The function sets the RF pin high.
/******************************************************************************/
#define RFon() (LATC |= RFtx)

/******************************************************************************/
/* RFoff
 *
 * The function sets the RF pin low.
/******************************************************************************/
#define RFoff() (LATC &= ~RFtx)

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/

extern unsigned char RFsendCode[RFmaxSize];
extern volatile unsigned char RFsendFlag;
extern unsigned char RFcodePlace;
extern unsigned char RFcodeBit;
extern volatile unsigned char Sendcount;
volatile unsigned char Sent;

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/

unsigned char SendRF(unsigned char* Code, unsigned char size, unsigned char RepeatAmount);
void SendRF_wait(unsigned char* Code, unsigned char size, unsigned char amount);
unsigned char GetRFstatus(void);

#endif	/* RF_H */
/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/
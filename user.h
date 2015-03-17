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
#ifndef USER_H
#define	USER_H

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

/******************************************************************************/
/* SysFreq32
 *
 * If this is defined then the system runs at 32MHz and if it is not defined
 *  then the system runs at 8MHz.
/******************************************************************************/

//#define SysFreq32

/******************************************************************************/
/* LEDtimeout
 *
 * This sets the timing for when the LED turns off after an IR code is
 *  recieved.
/******************************************************************************/
/* 8 bit */
#define LEDtimeout 0xFF

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/

#ifndef SysFreq32
#define SysFreq8
#endif

#define ON 1
#define OFF 0
#define OUTPUT 0
#define INPUT 1

/******************************************************************************/
/* Pin defines                                                                */
/******************************************************************************/

#define BiGreen_TRIS	TRISCbits.TRISC0
#define BiGreen 0b00000001 //RC0

#define BiRed_TRIS	TRISCbits.TRISC1
#define BiRed 0b00000010 //RC1

#define Red_TRIS	TRISCbits.TRISC2
#define Red 0b00000100 //RC2

#define IRh_TRIS	TRISCbits.TRISC4
#define IRh 0b00010000 //RC4 used as IOC (Interrupt on change)

#define IRl_TRIS	TRISCbits.TRISC5
#define IRl 0b00100000 //RC5

#define RFtx_TRIS	TRISCbits.TRISC3
#define RFtx 0b00001000 //RC3

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/

void InitApp(void);        
void Init_System (void);

#endif	/* USER_H */
/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/

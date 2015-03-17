/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 01/21/15     1.2         Created log.
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#ifndef TIMER_H
#define	TIMER_H

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
/* Macro Functions                                                            */
/******************************************************************************/

/******************************************************************************/
/* Timer2Reset
 *
 * The function resets Timer 2.
/******************************************************************************/
#define Timer2Reset() TMR2 = 0

/******************************************************************************/
/* Set_Timer2
 *
 * The function sets Timer 2.
/******************************************************************************/
#define Set_Timer2(time) (PR2 = time)

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/

void init_Timers(void);
void Timer2Init(void);
void Timer2ON(void);
void Timer2OFF(void);
void Timer1Init(void);
void Stop_Timer1(void);
void Start_Timer1(void);
void Reset_Timer1(void);
void Timer4Init(void);
void Timer4ON(void);
void Timer4OFF(void);
void Timer4Reset(void);
void Set_Timer4(unsigned char time);
void Timer6Init(void);
void Timer6ON(void);
void Timer6OFF(void);
void Timer6Reset(void);
void Set_Timer6(unsigned char time);

#endif	/* TIMER_H */
/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/

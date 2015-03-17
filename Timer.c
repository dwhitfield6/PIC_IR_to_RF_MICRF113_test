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
/* Contains Timer functions.
 *
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

#include "Timer.h"
#include "user.h"

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/

extern volatile unsigned int TMR1_overflow;

/******************************************************************************/
/* Functions
 *
/******************************************************************************/

/******************************************************************************/
/* Timer2Init
 *
 * The function initializes timer 2. The interrupt is enabled.
/******************************************************************************/
void init_Timers(void)
{
    Timer1Init();
    Timer2Init();
    Timer4Init();
    Timer6Init();
}

/******************************************************************************/
/* Timer1Init
 *
 * The function initializes timer 1. The interrupt is enabled.
/******************************************************************************/
void Timer1Init(void)
{
    /* Clocked by FOSC/4 */
    T1CONbits.TMR1CS = 0x0;
    /* PreScaler is 8*/
    T1CONbits.T1CKPS = 0x3;
    /* generate software interrupt only */
    CCP1CONbits.CCP1M = 0b1010;
    /* disable overflow interrupt */
    PIE1bits.TMR1IE = 0;
    /* Turn Off timer */
    Reset_Timer1();
    Stop_Timer1();
}

/******************************************************************************/
/* Stop_Timer1
 *
 * The function starts timer 1.
/******************************************************************************/
void Stop_Timer1(void)
{
    /*turn off timer 1*/
    T1CONbits.TMR1ON =0;
}

/******************************************************************************/
/* Start_Timer1
 *
 * The function starts timer 1.
/******************************************************************************/
void Start_Timer1(void)
{
    /*turn on timer 1*/
    T1CONbits.TMR1ON =1;
}

/******************************************************************************/
/* Reset_Timer1
 *
 * The function resets the counter on Timer 1.
/******************************************************************************/
void Reset_Timer1(void)
{
    /*
     * Reset count to 2 because when the TMR1 register is written to,
     * the increment is inhibited for the following two instruction cycles
     */
    TMR1H = 0;
    TMR1L = 0;
}

/******************************************************************************/
/* Timer2Init
 *
 * The function initializes timer 2. The interrupt is disabled.
 * This is used for the RF transmittion
/******************************************************************************/
void Timer2Init(void)
{
    /* Clocked by FOSC/4 */
    /* PostScaler is 15 */
    T2CONbits.T2OUTPS = 0xE;
    #ifdef SysFreq32
    /* PreScaler is 16*/
    T2CONbits.T2CKPS = 0x2;
    #else
    /* PreScaler is 4*/
    T2CONbits.T2CKPS = 0x1;
    #endif
    /* generate software interrupt only */
    CCP2CONbits.CCP2M = 0b1010;
    PR2 = 0xFF;
    Timer2OFF();         //timer2 off
}

/******************************************************************************/
/* Timer2ON
 *
 * This function turns on Timer 2.
/******************************************************************************/
void Timer2ON(void)
{
    T2CONbits.TMR2ON = 1;
    PIE1bits.TMR2IE = 1;//enable timer 2 interupt
}

/******************************************************************************/
/* Timer2OFF
 *
 * This function turns off Timer 2.
/******************************************************************************/
void Timer2OFF(void)
{
    T2CONbits.TMR2ON = 0;
    PIR1bits.TMR2IF = 0;//Clear Flag
    PIE1bits.TMR2IE = 0;//disable timer 2 interupt
}

/******************************************************************************/
/* Timer4Init
 *
 * The function initializes timer 4. The interrupt is disabled.
/******************************************************************************/
void Timer4Init(void)
{
    /* Clocked by FOSC/4 */
    /* PostScaler is 16*/
    T4CONbits.T4OUTPS = 0xF;
    #ifdef SysFreq32
    /* PreScaler is 64*/
    T4CONbits.T4CKPS = 0x3;
    #else
    /* PreScaler is 16*/
    T4CONbits.T4CKPS = 0x2;
    #endif
    PR4 = 0xFF;
    Timer4OFF();         //timer4 off
}

/******************************************************************************/
/* Timer4ON
 *
 * This function turns on Timer 4.
/******************************************************************************/
void Timer4ON(void)
{
    T4CONbits.TMR4ON = 1;
    PIE2bits.TMR4IE = 1;//enable timer 2 interupt
}

/******************************************************************************/
/* Timer4OFF
 *
 * This function turns off Timer 4.
/******************************************************************************/
void Timer4OFF(void)
{
    T4CONbits.TMR4ON = 0;
    PIR2bits.TMR4IF = 0;//Clear Flag
    PIE2bits.TMR4IE = 0;//disable timer 2 interupt
}

/******************************************************************************/
/* Timer4Reset
 *
 * The function resets Timer 4.
/******************************************************************************/
void Timer4Reset()
{
    TMR4 = 0;
}

/******************************************************************************/
/* Set_Timer4
 *
 * The function sets Timer 4.
/******************************************************************************/
void Set_Timer4(unsigned char time)
{
    PR4 = time;
}

/******************************************************************************/
/* Timer6Init
 *
 * The function initializes timer 6. The interrupt is disabled.
/******************************************************************************/
void Timer6Init(void)
{
    /* Clocked by FOSC/4 */
    /* PostScaler is 16*/
    T6CONbits.T6OUTPS = 0xF;
    #ifdef SysFreq32
    /* PreScaler is 64*/
    T6CONbits.T6CKPS = 0x3;
    #else
    /* PreScaler is 16*/
    T6CONbits.T6CKPS = 0x2;
    #endif
    PR6 = 0xFF;
    Timer6OFF();         //timer6 off
}

/******************************************************************************/
/* Timer6ON
 *
 * This function turns on Timer 6.
/******************************************************************************/
void Timer6ON(void)
{
    T6CONbits.TMR6ON = 1;
    PIE2bits.TMR6IE = 1;//enable timer 6 interupt
}

/******************************************************************************/
/* Timer6OFF
 *
 * This function turns off Timer 6.
/******************************************************************************/
void Timer6OFF(void)
{
    T6CONbits.TMR6ON = 0;
    PIR2bits.TMR6IF = 0;//Clear Flag
    PIE2bits.TMR6IE = 0;//disable timer 6 interupt
}

/******************************************************************************/
/* Timer6Reset
 *
 * The function resets Timer 6.
/******************************************************************************/
void Timer6Reset()
{
    TMR6 = 0;
}

/******************************************************************************/
/* Set_Timer6
 *
 * The function sets Timer 6.
/******************************************************************************/
void Set_Timer6(unsigned char time)
{
    PR6 = time;
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/

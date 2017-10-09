/*
 * AUTHOR: STEPHEN SZYMCZAK
 * PROJECT: BUTTON INTERRUPT FOR FR5994
 */
#include <msp430.h>

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
    P1DIR |= BIT0;  //SET BIT0 (LED1) AS OUTPUT
    P1OUT &= ~BIT0; //SET LED1 OFF

    P5DIR &= ~BIT5; //SET P1.5 AS INPUT

    P5REN |= BIT5; //ENABLED PULL UP OR DOWN FOR P5.5
    P5OUT |= BIT5; //SPECIFIED AS A PULLUP FOR P5.5

    P5IE |= BIT5;   //SET P5.5 INTERRUPT ENABLED (S2)
    P5IFG &= ~BIT5; //P5.5 IFG CLEARED

    __enable_interrupt();   //ENABLE ALL INTERRUPTS
    for(;;)
    {}

}

#pragma vector=PORT5_VECTOR
__interrupt void PORT_5(void)
{
    P1OUT ^= BIT0; //TURN ON LED
    P5IFG &= ~BIT5; //P5.5 IFG CLEARED
    P5IES ^= BIT5; //TOGGLE INTERRUPT EDGE
}

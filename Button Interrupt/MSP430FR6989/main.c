/*
 * AUTHOR: STEPHEN SZYMCZAK
 * PROJECT: BUTTON INTERRUPT FOR FR6989
 */
#include <msp430.h>

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
    P1DIR |= BIT0;  //SET BIT0 (LED1) AS OUTPUT
    P1OUT &= ~BIT0; //SET LED1 OFF

    P1DIR &= ~BIT1; //SET P1.1 AS INPUT

    P1REN |= BIT1; //ENABLED PULL UP OR DOWN FOR P1.1
    P1OUT |= BIT1; //SPECIFIED AS A PULLUP FOR P1.1

    P1IE |= BIT1;   //SET P1.1 INTERRUPT ENABLED (S2)
    P1IFG &= ~BIT1; //P1.1 IFG CLEARED

    __enable_interrupt();   //ENABLE ALL INTERRUPTS
    for(;;)
    {}

}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{
    P1OUT ^= BIT0; //TURN ON LED
    P1IFG &= ~BIT1; //P1.5 IFG CLEARED
    P1IES ^= BIT1; //TOGGLE INTERRUPT EDGE
}

/*
 * AUTHOR: STEPHEN SZYMCZAK
 * PROJECT: 8 BIT COUNTER FOR FR5994
 */
#include <msp430.h>
int rollover1 = 0;
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode

    //set all p3 pins as outputs and make sure they are all off
    P3DIR = 0xff;
    P3OUT &= ~0xff;

    //set up timer
    TA0CCTL0 = CCIE;                             // CCR0 interrupt enabled
    TA0CCR0 = 10000;                             //overflow every 10ms
    TA0CTL = TASSEL_2 + MC_1;                  // SMCLK, up mode
    __enable_interrupt();   //ENABLE ALL INTERRUPTS
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
      if(rollover1>=50) // 10ms * num, where rollover>=num (10ms * 100 = 1s blink period)
      {
          P1OUT ^= BIT0;                            // Toggle P1.0
          P3OUT++;  //meat of the program, outputs to LEDs as the register counts up.
          rollover1 = 0;
      }
      rollover1++;
}

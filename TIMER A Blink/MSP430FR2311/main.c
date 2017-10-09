/*AUTHOR: Stephen Szymczak
PROJECT: timer a blink for FR2311

**********************************************************************************/

#include <msp430.h>

int rollover1 = 0;
int rollover2 = 0;
int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  PM5CTL0 &= ~LOCKLPM5;                     // Disable the GPIO power-on default high-impedance mode
                                              // to activate previously configured port settings
  P1DIR |= 0x01;                            // P1.0 output
  P1OUT &= ~BIT0;                           //LED 1 OFF
  P2DIR |= BIT0;                            // P2.0 output
  P2OUT &= ~BIT0;                           //LED 2 OFF
  TB0CCTL0 |= CCIE;                             // TBCCR0 interrupt enabled
  TB0CCR0 = 10000;
  TB0CTL = TBSSEL__SMCLK | MC__UP;             // SMCLK, UP mode

  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt

}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer_A (void)
{


      if(rollover1>=32) // 10ms * num, where rollover>=num (10ms * 100 = 1s blink period)
      {
          P1OUT ^= BIT0;                            // Toggle P1.0
          rollover1 = 0;
      }

      if(rollover2>=9) // 10ms * num, where rollover>=num (10ms * 100 = 1s blink period)
      {
          P2OUT ^= BIT0;                            // Toggle P2.0
          rollover2 = 0;
      }
      rollover1++;
      rollover2++;

}


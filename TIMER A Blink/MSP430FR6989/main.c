/*AUTHOR: Stephen Szymczak
PROJECT: timer a blink for FR6989

**********************************************************************************/

#include <msp430.h>

int rollover1 = 0;
int rollover2 = 0;
int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                              // to activate previously configured port settings
  // LED SETUP:
  P1DIR |= 0x01;                            // P1.0 output
  P1OUT &= ~BIT0;                           //LED 1 OFF
  P9DIR |= BIT7;                            // P9.7 output
  P9OUT &= ~BIT7;                           //LED 2 OFF

  //TIMER SETUP:
  TA0CCTL0 = CCIE;                             // CCR0 interrupt enabled
  TA0CCR0 = 10000;                             //overflow every 10ms
  TA0CTL = TASSEL_2 + MC_1;                  // SMCLK, up mode

  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt

}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{


      if(rollover1>=1) // 10ms * num, where rollover>=num (10ms * 100 = 1s blink period)
      {
          P1OUT ^= BIT0;                            // Toggle P1.0
          rollover1 = 0;
      }

      if(rollover2>=9) // 10ms * num, where rollover>=num (10ms * 100 = 1s blink period)
      {
          P9OUT ^= BIT7;                            // Toggle P9.7
          rollover2 = 0;
      }
      rollover1++;
      rollover2++;

}


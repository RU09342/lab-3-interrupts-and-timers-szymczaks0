/*AUTHOR: Stephen Szymczak
PROJECT: timer a blink for G2553

**********************************************************************************/

#include <msp430.h>

int rollover1 = 0;
int rollover2 = 0;
int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT



  P1DIR |= 0x01;                            // P1.0 output
  P1OUT &= ~BIT0;                           //LED 1 OFF
  P1DIR |= BIT6;                            // P1.6 output
  P1OUT &= ~BIT6;                           //LED 2 OFF
  CCTL0 = CCIE;                             // CCR0 interrupt enabled
  CCR0 = 10000;                             //overflow every 10ms
  TACTL = TASSEL_2 + MC_1;                  // SMCLK, up mode

  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt

}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
      if(rollover1>=32) // 10ms * num, where rollover>=num (10ms * 100 = 1s blink period)
      {
          P1OUT ^= BIT0;                            // Toggle P1.0
          rollover1 = 0;
      }

      if(rollover2>=9) // 10ms * num, where rollover>=num (10ms * 100 = 1s blink period)
      {
          P1OUT ^= BIT6;                            // Toggle P1.6
          rollover2 = 0;
      }

      rollover1++;
      rollover2++;
}


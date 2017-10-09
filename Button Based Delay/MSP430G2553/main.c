/*AUTHOR: Stephen Szymczak
PROJECT: button delay for G2553
**********************************************************************************/

#include <msp430.h>

int rollover1 = 0;
int rollover2 = 0;
int delay = 20; //initialize delay to 20, so that the led blinks at 10hz initially
int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  //LED setup:
  P1DIR |= 0x01;                            // P1.0 output
  P1OUT &= ~BIT0;                           //LED 1 OFF
  P1DIR |= BIT6;                            // P1.6 output
  P1OUT &= ~BIT6;                           //LED 2 OFF

  //Button setup:
  P1DIR &= ~BIT3; //SET P1.3 AS INPUT
  P1REN |= BIT3; //ENABLED PULL UP OR DOWN FOR P1.3
  P1OUT |= BIT3; //SPECIFIED AS A PULLUP FOR P1.3

  //Timer A0 setup:
  CCTL0 = CCIE;                             // CCR0 interrupt enabled
  CCR0 = 10000;                             //overflow every 10ms
  TACTL = TASSEL_2 + MC_1;                  // SMCLK, up mode

  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt

}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0 (void)
{


      if(rollover1>=delay/2) // 10ms * delay/2 = blink period. logic behind "/2" is explained near the delay++;
      {
          P1OUT ^= BIT0;                            // Toggle P1.0
          rollover1 = 0;
      }

      if(rollover2>=10) // 10ms * num, where rollover>=num (ex: 10ms * 10 = 100ms blink period)
      {
          P1OUT ^= BIT6;                            // Toggle P1.6
          rollover2 = 0;
      }
      rollover1++;
      rollover2++;
      if(!(P1IN & BIT3)) // period = time button pressed; 1 period = 50% on / 50% off
      delay++;          //  without the "/2" in "if(rollover1>=delay/2)" the LED would be on for the amount of time the button is held which results in a period twice as long as button held
                        // in order to make the LED on for half and off for half the time the button is held, delay is simply divided by 2 in the if statement
}

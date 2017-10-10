/*AUTHOR: Stephen Szymczak
PROJECT: button delay with reset for FR5994

**********************************************************************************/

#include <msp430.h>

int rollover1 = 0;
int rollover2 = 0;
int delay = 20;
int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                              // to activate previously configured port settings
  //LED setup
  P1DIR |= 0x01;                            // P1.0 output
  P1OUT &= ~BIT0;                           //LED 1 OFF
  P1DIR |= BIT1;                            // P1.1 output
  P1OUT &= ~BIT1;                           //LED 2 OFF

  //Button setup:
  P5DIR &= ~BIT5; //SET P5.5 AS INPUT
  P5REN |= BIT5; //ENABLED PULL UP OR DOWN FOR P5.5
  P5OUT |= BIT5; //SPECIFIED AS A PULLUP FOR P5.5
  P5DIR &= ~BIT6; //SET P5.6 AS INPUT
  P5REN |= BIT6; //ENABLED PULL UP OR DOWN FOR P5.6
  P5OUT |= BIT6; //SPECIFIED AS A PULLUP FOR P5.6

  //Timer setup:
  TA0CCTL0 = CCIE;                             // CCR0 interrupt enabled
  TA0CCR0 = 10000;                             //overflow every 10ms
  TA0CTL = TASSEL_2 + MC_1;                  // SMCLK, up mode

  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt

}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{


      if(rollover1>=delay/2) // 10ms * num, where rollover>=num (10ms * 100 = 1s blink period)
      {
          P1OUT ^= BIT0;                            // Toggle P1.0
          rollover1 = 0;
      }

      if(rollover2>=10) // 10ms * num, where rollover>=num (10ms * 100 = 1s blink period)
      {
          P1OUT ^= BIT1;                            // Toggle P1.1
          rollover2 = 0;
      }
      rollover1++;
      rollover2++;
      if(!(P5IN & BIT5)) delay++;   // period = time button pressed; 1 period = 50% on / 50% off
                                    // without the "/2" in "if(rollover1>=delay/2)" the LED would be on for the amount of time the button is held which results in a period twice as long as button held
                                    // in order to make the LED on for half and off for half the time the button is held, delay is simply divided by 2 in the if statement

      else if(!(P5IN & BIT6)) // Reset button
          {
              delay = 20; // reset delay back to 20
              rollover1 = 0; // this syncs the two leds with each other
              rollover2 = 0;
          }
      else;
}



/*
 * P5IN = 0110 0000
 * BIT5 = 0010 0000
 * BIT6 = 0100 0000
 *
 */

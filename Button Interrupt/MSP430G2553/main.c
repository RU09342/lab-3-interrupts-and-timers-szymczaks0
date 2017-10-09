/*AUTHOR: STEPHEN SZYMCZAK
 * PROJECT: BUTTON INTERRUPT FOR G2553
 */
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  P1DIR |= BIT0;                            // Set P1.0 to output direction
  P1IE |=  BIT3;                            // P1.3 interrupt enabled
  P1IES |= BIT3;                            // P1.3 Hi/lo edge
  P1REN |= BIT3;                            // Enable Pull Up on SW2 (P1.3)
  P1IFG &= ~BIT3;                           // P1.3 IFG cleared


  __bis_SR_register(LPM4_bits + GIE);       // Enter LPM4 w/interrupt
}

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    __delay_cycles(40000);  //bounce management
  P1OUT ^= BIT0;                            // P1.0 = toggle

  if (!(BIT3 & P1IN))
          {
              __delay_cycles(40000);    //bounce management
          }
  P1IFG &= ~BIT3;                           // P1.3 IFG cleared

}

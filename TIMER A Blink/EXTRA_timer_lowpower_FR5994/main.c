/*AUTHOR: Stephen Szymczak
PROJECT: timer LOW POWER for FR5994

**********************************************************************************/

#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  PM5CTL0 &= ~LOCKLPM5;                     // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
  PADIR = 0xFF;//lines 14 through 38 disable all unused pins to save power.
  PBDIR = 0xFF;
  PCDIR = 0xFF;
  PDDIR = 0xFF;
  P1DIR = 0xFF;
  P2DIR = 0xFF;
  P3DIR = 0xFF;
  P4DIR = 0xFF;
  P5DIR = 0xFF;
  P6DIR = 0xFF;
  P7DIR = 0xFF;
  P8DIR = 0xFF;

  PAOUT = 0x00;
  PBOUT = 0x00;
  PCOUT = 0x00;
  PDOUT = 0x00;
  P1OUT = 0x00;
  P2OUT = 0x00;
  P3OUT = 0x00;
  P4OUT = 0x00;
  P5OUT = 0x00;
  P6OUT = 0x00;
  P7OUT = 0x00;
  P8OUT = 0x00;// end power saving code
  P1DIR |= BIT0;                            // P1.0 output
  P1OUT &= ~BIT0;                           //LED 1 OFF

  TA0CCTL0 = CCIE;                             // CCR0 interrupt enabled
  TA0CCR0 = 10000;                             //overflow every 10ms
  TA0CTL = TASSEL_2 + MC_1 + ID_3;                  // SMCLK, up mode

  __bis_SR_register(LPM3_bits + GIE);       // Enter LPM3 w/ interrupt
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
          P1OUT ^= BIT0;                            // Toggle P1.0
          __bis_SR_register_on_exit(LPM3_bits + GIE); // reenable lpm3

}


#### Stephen Szymczak
# LAB 3: TIMER A Blink
  Similar to how the button interrupt code utilizes PORTx interrupt routine, timer have interrupts and interrupt routines as well. There are a lot of options for utilizing timers and their associated ISRs. In this lab, timers will be used to blink two LEDs at differing rates.
## Timers are different between processors in more complex ways than ports were.
  Previously, the only differences between processors that had to be accounted for were their PORT allocations. For example, P1.1 might be a button on one board, an LED on another, or an unnassigned pin on another. Now that timers are being utilized, the differences between processors become slightly more complex. For example, Timer A exists on all processors except the FR2311 which only has a Timer B. Furthermore, syntax differs between processors as well. The use of the following functions will be explained in more detail, but here is an example of how two processors differ with syntax:
  ```c
  TA0CTL = TASSEL_2 + MC_1;         //setting up a timer for the FR5994
  TB0CTL = TBSSEL__SMCLK | MC__UP;  //has significantly different syntax than the FR2311
  TACTL = TASSEL_2 + MC_1;          //and different still for the G2553
  ```
## Basics of Timer A
##### Note that for the following basics, assume the NAMES of these registers only apply to FR5994 as it is being used here as an example.

  Taking the first line of code above ```  TA0CTL = TASSEL_2 + MC_1; ``` we see on the left TA0CTL which is the Timer A0 control register, this register, as it may sound, controls the functionality of Timer A0. 
  
  On the right we have constants being assigned to the TA0 register. TASSEL\_2 sets the bits in the TA0 register that turn on SMCLK, a constant ~1Mhz signal. 
  
  Also on the right, MC\_1 sets the timer to UP mode. Up mode counts up to a predetermined amount using a count register TA0R, then resets to 0. 
  
  This predetermined reset point is stored in a register called TA0CCR0 or the capture/compare register 0. So, if TA0CCR0 = 10000, every time the timer counting register TA0R reaches 10000, the count will reset:
  ```c
  TA0CCR0 = 10000; //at TA0R == 10000 -> TA0R = 0
  ```
  TA0CCR0 can also be used to activate a timer ISR. In the same way Timer A0 has a control register TA0CTL, TA0CCR0 also has a control register TA0CCTL0. Setting certain bits in TA0CCTL0 can turn on interrupts for CCR0:
  ```c
  TA0CCTL0 = CCIE;                             // CCR0 interrupt enabled with CCIE (Capture Compare Interrupt Enable)
  ```
  Like the button interrupt utilizing PORTx\_VECTOR to setup a button press ISR, we have the following for Timer A0:
  ```c
  #pragma vector=TIMER0_A0_VECTOR //TIMER0 specifies we are using timer 0, A0 specifies it is an interrupt for TA0CCR0
  ```
  If we write an ISR using this setup, the ISR will be entered whenever TA0R = TA0CCR0.
  
 ### To summarize the above functionality:
  * TA0CCR0 is the register that holds the value at which TA0R will count up to in up mode.
  * TA0CCTL0 controls functionality for TA0CCR0
  * TA0R is the register that increments every clock cycle, based on which clock is chosen
  * SMCLK is the chosen clock that increments TA0R
  * MC\_UP sets the TA0CTL register to up mode
  * TA0CTL is the timer A0 control register that dictates how Timer A0 will function
#### As mentioned before, the NAMES of these registers change from board to board, and Timer A must be substituted for Timer B on the FR2311 as it does not have a Timer A.
## Implementation
```c
TA0CCTL0 = CCIE;                             // CCR0 interrupt enabled
  TA0CCR0 = 10000;                             //overflow every 10ms
  TA0CTL = TASSEL_2 + MC_1;                  // SMCLK, up mode
/// ...
#pragma vector=TIMER0_A0_VECTOR
```
This code sets TA0CCR0 to interrupt enabled, sets the value for TA0CCR0 to 10000, and turns on Timer A0 driven by SMCLK and in up mode.

If we want to blink two LEDs, we enter the ISR and pop in two LED toggle lines, assuming the LEDs' pins were already setup the ISR would look like this:
```c
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
          P1OUT ^= BIT0;                            // Toggle P1.0
          P1OUT ^= BIT1;                            // Toggle P1.1
}
```
While this functionally will blink the LEDs, it blinks them far too fast, so if we want to slow down the blink rate we have two options:
  1.  set up TA0CTL with a pre-divider.
  2.  use counting logic to slow down how frequently the LED pins will toggle.
While using slightly more power, option 2 was easier to implement for the purpose of having TWO LEDs blink at different rates:
```c
int rollover = 0;
int main(void)
//...
//inside ISR:
if(rollover1>=32) // 10ms * num, where rollover>=num (10ms * 100 = 1s blink period)
      {
          P1OUT ^= BIT0;                            // Toggle P1.0
          rollover1 = 0;
      }
      if(rollover2>=9) // 10ms * num, where rollover>=num (10ms * 100 = 1s blink period)
      {
          P1OUT ^= BIT1;                            // Toggle P1.1
          rollover2 = 0;
      }
      rollover1++;
      rollover2++;
//...
```
32 and 9 were arbitrary numbers that showcase that the LEDs can toggle at rates that aren't even multiples of each other. Furthermore, it can be noted that 32 and 9 could be replaced with variables that change with button presses. This exact functionality is implemented for the button delay portion of this lab.

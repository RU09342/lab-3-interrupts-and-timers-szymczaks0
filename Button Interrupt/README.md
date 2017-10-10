#### Stephen Szymczak
# LAB 3: Button Interrupt
  Polling is a very power inefficient way of detecting a button press. In polling, the processor is constantly checking to see if the button has been pressed, which means the processor is constantly executing code and thus using power. An interrupt solves this issue. The interrupt serves as a way to detect a button press at any time in the code, eliminating polling. After an interrupt occurs, an interrupt function is entered called an "interrupt service routine" or "ISR".
  
## Usage:
While polling requires less setup: the button's pin simply needs to be set as an input, and maybe have a pull-up resistor, a button interrupt requires some additional specification to work properly:
```c
P5DIR &= ~BIT5; //SET P5.5 AS INPUT
P5REN |= BIT5; //ENABLED PULL UP OR DOWN FOR P5.5
P5OUT |= BIT5; //SPECIFIED AS A PULLUP FOR P5.5
P5IE |= BIT5;   //SET P5.5 INTERRUPT ENABLED (S2)
P5IFG &= ~BIT5; //P5.5 IFG CLEARED
```
This code is the basic setup to have a pin set as an input (P5DIR &= ~BIT5;), have a pull up resistor (P5REN |= BIT5; & P5OUT |= BIT5;), enable interrupt capability for this pin (P5IE |= BIT5;), and clear the interrupt flag (P5IFG &= ~BIT5;) to be sure an interrupt doesn't occur the instant the code begins. Of course, P5.5 is specific to one processor (the FR5994) and is replaced with different values depending on what pins the buttons are connected to on different processors:
```c
PxDIR &= ~BITy; //Px.y
```
With the button set up to cause an interrupt, the ISR must now be defined:
```c
#pragma vector=PORT5_VECTOR
__interrupt void PORT_5(void)
{
    P1OUT ^= BIT0; //TURN ON LED
    P5IFG &= ~BIT5; //P5.5 IFG CLEARED
    P5IES ^= BIT5; //TOGGLE INTERRUPT EDGE
}
```
Notice that since the FR5994 has it's buttons on PORT5 that the interrupt vector is ``` PORT5_Vector ```. This is renamed for the different boards as ``` #pragma vector=PORTx_VECTOR ``` where x is the port number that the button set up for interrupts is on.

``` PORT_5 ``` on the line reading ``` __interrupt void PORT_5(void) ``` is an arbitrary variable name, and was named so for ease of reading the code.

The flag is cleared again inside the ISR ``` P5IFG &= ~BIT5; ``` and the interrupt edge is toggled by XORing the interrupt edge select with BIT5 ``` P5IES ^= BIT5; ```. This is done so that as long as the button is being held, a new interrupt occurs nearly instantaneously, causing the LED to stay on as long as the button is pressed.

# Extra Work: 8-bit counter
For this project, an 8bit LED counter was designed using a timer ISR. The code is very simple. A breadboard was required.

## Implementation
Port 3 was used for the LED output:
```c
P3DIR = 0xff;
P3OUT &= ~0xff;
```
Using the timer interrupt code from Timer A blink, the entire P3OUT register is incremented every time the interrupt is entered:
```c
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
```
For the breadboard, LEDs and resistors were used. 

[here](https://imgur.com/a/HSeqj) is a gif demonstrating the counter. Note that LSB is on the left and the MSB is on the right. Binary counting is usually done right to left. If you wanted to recreate this with proper binary counting (LSB on the right), wire the pins to the LEDs the opposite way that I did.

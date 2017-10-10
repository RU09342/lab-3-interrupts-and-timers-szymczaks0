#### Stephen Szymczak

# Lab 3: Button Based Delay

  Taking heavily from the code used to implement timer A blink, it is now modified to adjust the blink rate of an LED by way of a button press. The goal is to have (time button is pressed) = (period of blink). Period of blink meaning one cycle of being on and off.
## Implementation  
  Recall the following delay code that was used in A blink for the FR5994:
 ```c
  if(rollover1>=32) // 10ms * num, where rollover>=num (10ms * 100 = 1s blink period)
      {
          P1OUT ^= BIT0;                            // Toggle P1.0
          rollover1 = 0;
      }
 ``` 
 This if statement acts as a clock divider on our output (toggling the LED). If we replace 32 with a variable 'delay' we can control how much the clock divider divides. This is done by checking if the button is pressed every time the timer ISR is entered:
 ```c
  int rollover1 = 0;
  int delay = 20;
  //...
  //within the ISR:
  if(rollover1>=delay/2) // 10ms * num, where rollover>=num (10ms * 100 = 1s blink period)
        {
            P1OUT ^= BIT0;                            // Toggle P1.0
            rollover1 = 0;
        }
  rollover1++;
  if(!(P5IN & BIT5)) delay+= 2;
 ```
  This is essentially all the code that is needed to cause a delay with a nearly 1:1 ratio with duration of the button press.
  ```c
  int rollover1 = 0;
  int delay = 20;
  ```
  This code sets up the rollover and delay variables. delay is initialized at 20 because it will be divided by 2 within the if statement divider code ``` if(rollover1>=delay/2) ```. rollover1 >= 10 causes a blink to occur every 10 times the ISR is entered. The ISR is entered every time CCR0 is reached. CCR0 is reached in 10ms and the math is as follows:

  CCR0/SMCLK = 10000/1Mhz = 10ms -> 10ms * 10 = 100ms
 
 So, when delay is 20, the blink rate is once per 100ms.
 
 Every time the ISR is entered and the button is pressed, 'delay' increases by 1. This means that the button has been registered as being pressed for 10ms (CCR0/SMCLK = 10000/1Mhz = 10ms). 'delay' is now 21. 
 
 21/2 = 10.5 -> 10.5 * 10ms = 105ms.
 
 Now, the button has been pressed for 10ms. The new blink toggle is once every 105ms, this means the time that the LED is on has increased by 5 ms and the time off by 5ms, thus the overall period has increased by 10ms(two iterations of the ISR). Thus for every 10ms that the button is pressed, the LED's period will increase by 10ms.
 
 ###### Note: the time it takes to run through the code makes the resolution of this method limited by 10ms steps.
 ###### Note: there is a rollover2 and a second LED with identical code except it is at a constant 100ms cycle for comparison purposes:
 ```c
 if(rollover2>=10) // 10ms * num, where rollover>=num (10ms * 100 = 1s blink period)
      {
          P1OUT ^= BIT1;                            // Toggle P1.1
          rollover2 = 0;
      }
 rollover2++;
 ```
 ###### Note: This code is specifically for the FR5994, but the logical structure of it is the same for all of the boards. Timer & Port syntax varies across the boards.
 # Extra Work
 For the FR5994, a manual reset was implemented for this project. this was done by polling for another button inside the ISR in the same fashion that the delay button was checked. This reset manually resets the rollover count to 0 and the delay back to 20. Furthermore, a second LED was used as a baseline 100ms blink rate to make the changes in blink rate more noticeable. The noteable changes in code are given below:
 ```c
 else if(!(P5IN & BIT6))  //reset button
          {
              delay = 20; //resets delay
              rollover1 = 0; //resets both rollovers for each LED to synchronize them
              rollover2 = 0;
          }
      else;
 ```
 
 

#include <msp430.h> 

#define SW		BIT3					// Switch -> P1.3
#define GREEN	BIT6					// Green LED -> P1.6
#define RED     BIT0 					//czerwona lampka

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;			// Stop watchdog timer


  P1DIR |= RED + GREEN;            // P1.0 and P1.6 pins output the rest are input 
  P1REN |= SW;                   // Enable internal pull-up/down resistors
  P1OUT |= SW;                   //Select pull-up mode for P1.3
  P1IE |= SW;                       // P1.3 interrupt enabled
  P1IES |= SW;                     // P1.3 Hi/lo edge
  P1IFG &= ~SW;                  // P1.3 IFG cleared
__bis_SR_register(LPM4_bits + GIE);	  
  while(1)                          
  {}
} 
  
#pragma vector=TIMERA0_VECTOR 
__interrupt void Timer_A (void) 
{   
   P1OUT ^= RED;                          // Toggle P1.0 
} 
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{    
   P1OUT ^= GREEN;                      // Toggle P1.6
   P1IFG &= ~SW;                     // P1.3 IFG cleared 
}
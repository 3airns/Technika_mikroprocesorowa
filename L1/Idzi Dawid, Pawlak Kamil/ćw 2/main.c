#include <msp430.h> 

#define SW   BIT3                    // Switch -> P1.3
#define LED BIT0                    // Red LED -> P1.0
#define LED2 BIT6                   // green LED -> P1.0
int p,i;

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

    P1DIR |= LED2+ LED;                  // Set LED pin -> Output
    P1DIR &= ~SW;                       // Set SW pin -> Input
    P1REN |= SW;                        // Enable Resistor for SW pin
    P1OUT |= SW;                        // Select Pull Up for SW pin

    P1IES &= ~SW;                       // Select Interrupt on Rising Edge
    P1IE |= SW;                         // Enable Interrupt on SW pin

    __bis_SR_register(LPM4_bits + GIE); // Enter LPM4 and Enable CPU Interrupt
}
p=1;
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{

    if(!(P1IN & SW)) // jesli przycisk wcisniety
           {
               p=p+1;    //inkrementuj zmienn¹ p
               for(i = 0; i < 10000; i++);
           }

           switch(p) { //otwarcie petli switch
           case 1: // warunek 1
               P1OUT &=~BIT0+BIT6; //œwieci tylko zielona dioda
               P1OUT |=BIT6;
               break;

           case 2:
               P1OUT &=~BIT6; //swieci sie tylko czerwona dioda
               P1OUT |=BIT0;
               break;

           case 3:
               P1OUT |=BIT0+BIT6;  // œwiec¹ siê obie diody
               break;

           case 4:
               P1OUT &=~BIT0+BIT6; //nie swieci siê zadna dioda

               //for (;;)
               //{
                 //  P1OUT|=BIT0;
                  // __delay_cycles(1000);
                  // P1OUT&=~BIT0;
              // }
               break;

           case 5:  // zerowanie programu - swego rodzaju reset
              p=0;
               break;

           default:
               P1OUT &=~BIT0+BIT6; // opcja defaultowa
               }

}

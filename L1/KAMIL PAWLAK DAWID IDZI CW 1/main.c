#include <msp430.h> 
#define SW  BIT3                    // Switch -> P1.3
#define LED BIT0                    // Red LED -> P1.0
#define LED2 BIT6                    // green LED -> P1.0
int p,i;
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;       // Stop watchdog timer

    P1DIR |= LED+LED2;                   // Set LED pin -> Output
    P1DIR &= ~SW;                   // Set SW pin -> Input
    P1REN |= SW;                    // Enable Resistor for SW pin
    P1OUT |= SW;                    // Select Pull Up for SW pin
    p=1;

//p1out &=~bit - wylacz
//p1out |=bit - wlacz
//p1out ^=bit - zmiana stanu


    while(1)
    {
        if(!(P1IN & SW))
        {
            p=p+1;0;
            for(i = 0; i < 10000; i++);
        }

        switch(p) {
        case 1:
            P1OUT &=~BIT0+BIT6;
            P1OUT |=BIT6;
            break;

        case 2:
            P1OUT &=~BIT6;
            P1OUT |=BIT0;
            break;

        case 3:
            P1OUT |=BIT0+BIT6;
            break;

        case 4:
            P1OUT &=~BIT0+BIT6;

            //for (;;)
            //{
              //  P1OUT|=BIT0;
               // __delay_cycles(1000);
               // P1OUT&=~BIT0;
           // }
            break;

        case 5:
            p=0;
            break;

        default:
            P1OUT &=~BIT0+BIT6;
            }
     }
}

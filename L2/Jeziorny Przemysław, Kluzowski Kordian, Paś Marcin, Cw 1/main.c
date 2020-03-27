#include <msp430.h>
#define SW  BIT3                    // przycisk = P1.3
#define LED BIT0                    // czerwona dioda LED = P1.0
#define LED2 BIT6                   // zielona dioda LED = P1.0
int a,i;
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;       // Stop watchdog timer

    P1DIR |= LED+LED2;              // Set LED pin -> Output
    P1DIR &= ~SW;                   // Set SW pin -> Input
    P1REN |= SW;                    // Enable Resistor for SW pin
    P1OUT |= SW;                    // Select Pull Up for SW pin
    a=0;

//p1out &=~bit - wylacz
//p1out |=bit  - wlacz
//p1out ^=bit  - zmiana stanu

    while(1)
    {
        if(!(P1IN & SW)) {

            a=a+1;
            for(i = 0; i < 10000; i++);
     }
        if (a==1) {

            P1OUT |= BIT0;
            P1OUT ^= BIT0;
            P1OUT |= BIT6;
            __delay_cycles(1000000);

     }
        if (a==2) {

            P1OUT |= BIT6;
            P1OUT ^= BIT6;
            P1OUT |= BIT0;
            __delay_cycles(1000000);

    }
        if (a==3) {

            P1OUT |= BIT0;
            P1OUT |= BIT6;
            __delay_cycles(1000000);

    }
        if (a==4) {
            P1OUT &=~BIT0+BIT6;
            __delay_cycles(1000000);

    }
        if(a==5) {
            a=0;

    }

}
}

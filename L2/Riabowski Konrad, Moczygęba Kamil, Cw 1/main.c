#include <msp430.h>

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;       // Stop watchdog timer

    P1DIR |= (BIT0+BIT6);           // P1.0 (Red LED), P1.1 (Green LED)

    P1DIR &= ~BIT3;             // P1.3 (SW2) -> Input
    P1REN |= BIT3;              // P1.3 Pull Up/Down Enable
    P1OUT |= BIT3;              // P1.3 Pull Up Enable

    while(1)
    {
        volatile unsigned long i;

        if(P1IN & BIT3)                 // If SW is NOT pressed
        {
            for(i = 0; i < 1000; i++)      //counter 0-5
            {
                P1OUT &= ~BIT6;             //Green LCED -> OFF
                P1OUT |= BIT0;              //Red LED -> ON

                P1OUT &= ~BIT0;             //Red LED -> OFF
                P1OUT |= BIT6;              //Green LCED -> ON
            }
        }
        else
        {

            P1OUT ^= BIT0;

        {
    }
}

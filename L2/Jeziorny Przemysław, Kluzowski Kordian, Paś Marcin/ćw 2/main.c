#include <msp430.h> 

#define SW      BIT3                    // Zdefiniowanie przycisku
#define GREEN   BIT6                    // Zdefiniowanie zielonej diody
#define RED     BIT0                    // Zdefiniowanie czerwonej diody
int a;                                  // Zdefiniowanie zmiennej a

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

    P1DIR |= GREEN+RED;                 // Wyb�r kierunku rejestru(wyjscie)
    P1DIR &= ~SW;                       // Wyb�r kierunku rejestru(wyjscie)

    P1REN |= SW;                        // Enable Resistor for SW pin
    P1OUT |= SW;                        // Stan wyjsciowy przycisku - stan wysoki

    P1IES &= ~SW;                       // Wyb�r zbocza sygna�u wyzwalaj�cego przerwanie(zbocze opadaj�ce)
    P1IE |= SW;                         // W��czenie przerwania

    __bis_SR_register(LPM4_bits + GIE); // Enter LPM4 and Enable CPU Interrupt
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    while(1)
        {
            if(!(P1IN & SW)) {
                a=a+1;
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
        P1IFG &= ~SW;                    // Flaga przerwania (1-''wejscie''do procedury obs�ugi przerwania)
}

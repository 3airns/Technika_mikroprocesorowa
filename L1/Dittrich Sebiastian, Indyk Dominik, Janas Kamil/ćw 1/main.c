#include <msp430.h>

#define SW BIT3         // Przycisk P1.3
#define LEDG BIT0        // Zielona dioda P1.0
#define LEDR BIT6       // Czerwona dioda P1.6

int x,i;
void main (void)
{
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

    P1DIR |= LEDR+LEDG;
    P2DIR |= BIT5+BIT3+BIT1;
    P1DIR &= ~SW;
    P1REN |= SW;
    P1OUT |= SW;

    x=1;

    while(1)
    {
        if(!(P1IN & SW))                // Jeśli przycisk jest wciśnięty
        {
            __delay_cycles(20000);      // Poczekaj 20ms
            if(!(P1IN & SW))            // Sprawdza czy przycisk jest dalej wciśniety
            {                           // Ignoruj wciśnięcie, które jest krótsze niż 20ms
                while(!(P1IN & SW));    // Jak puścimy przycisk
                x = x+1;0;              // Zmień wartość x
        }
        }
        switch(x){
        case 1:                         // Świeci się dioda P1.6 i P2.1

            P1OUT &= ~(LEDG+LEDR);      // Negacja diody P1.0 i P1.6
            P2OUT &= ~(BIT3+BIT5);      // Negacja diody P2.3 I P2.5
            P2OUT |= BIT1;              // Ustawienie logicznej 1 na P2.1 (dioda czerwona)
            P1OUT |= LEDR;              // Ustawienie logicznej 1 na P1.6 (dioda czerwona)
            break;
        case 2:                         // Świeci się dioda P1.0 i P2.3

            P1OUT &= ~LEDR;             // Negacja diody P1.6
            P2OUT &= ~BIT1;             // Negacja diody P2.1
            P1OUT |= LEDG;              // Ustawienie logicznej 1 na P1.0 (dioda zielona)
            P2OUT |= BIT3;              // Ustawienie logicznej 1 na P2.3 (dioda zielona)
            break;
        case 3:                         // Migająca dioda P2.5 (niebieska), stałe świecenie P1.0 i P1.6
            P2OUT &= ~BIT3;             // Negacja diody P2.3
            P1OUT |=LEDR+LEDG;          // Ustawienie logicznej 1 na P1.0 i P1.6
            P2OUT |= BIT5;              // Ustawienie logicznej 1 na P2.5 (dioda niebieska)
            for(i = 0; i<20000; i++);   // Opóźnienie zmiany stanu P2.5 20ms
            P2OUT &= ~BIT5;             // Negacja diody P2.5 (dioda niebieska)
            for(i = 0; i<20000; i++);   // Opóźnienie zmiany stanu P2.5 20ms
            break;
        case 4:                         // Miganie na przemienne diód czerwonych i zielonych
                                        // P2.1 i P1.6 po zmianie P2.3 i P1.0 opóźnienie 20 ms
            P1OUT &= ~LEDR;             // Negacja diody P1.6 (czerwonej)
            P1OUT |= LEDG;              // Ustawienie logicznej 1 dla diody P1.0 (zielonej)
            P2OUT &= ~BIT1;             // Negacja diody P2.1 (czerwonej)
            P2OUT |= BIT3;              // Ustawienie logicznej 1 dla diody P2.3 (zielonej)
            for(i = 0; i<20000; i++);   // Opóźnienie zmiany stanu 20ms
            P1OUT &= ~LEDG;             // Negacja diody P1.0 (zielonej)
            P1OUT |= LEDR;              // Ustawienie logicznej 1 na diodę P1.6 (czerwonej)
            P2OUT &= ~BIT3;             // Negacja diody P2.3 (zielonej)
            P2OUT |= BIT1;              // Ustawienie logicznej 1 na diodę P2.1 (czerwonej)
            for(i = 0; i<20000; i++);   // Opóźnienie zmiany stanu 20ms
            break;
        case 5:
            x=0;                        // Reset instrukcji Switch case
            break;
        default:                        // Wyłączenie wszystkich diód
            P1OUT &= ~LEDG+LEDR;        // Negacja P1.0 i P1.6
            P2OUT &= ~BIT1+BIT3+BIT5;   // Negacja P2.1; P2.3; P2.5
        }
    }
}

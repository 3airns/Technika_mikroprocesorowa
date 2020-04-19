#include <msp430.h>

#define SW BIT3         // Przycisk P1.3
#define LEDG BIT0        // Zielona dioda P1.0
#define LEDR BIT6       // Czerwona dioda P1.6

int x=0;

void main (void){
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

    P1DIR |= LEDR+LEDG;
    P2DIR |= BIT5+BIT3+BIT1;
    P1DIR &= ~SW;
    P1REN |= SW;
    P1OUT |= SW;
    P1IES &= ~SW;                       // Select Interrupt on Rising Edge
    P1IE |= SW;                         // Enable Interrupt on SW pin

    __bis_SR_register(LPM4_bits + GIE); // Glowny wlacznik przerwan maskowanych
}

#pragma vector=PORT1_VECTOR

    __interrupt void Port_1(void){
    x = x+1;0;              // Zmien wartosc

    switch(x){
        case 1:                         // Swieci sie dioda P1.6 i P2.1

            P1OUT &= ~(LEDG+LEDR);      // Negacja diody P1.0 i P1.6
            P2OUT &= ~(BIT3+BIT5);      // Negacja diody P2.3 I P2.5
            P2OUT |= BIT1;              // Ustawienie logicznej 1 na P2.1 (dioda czerwona)
            P1OUT |= LEDR;              // Ustawienie logicznej 1 na P1.6 (dioda czerwona)
            break;
        case 2:                         // Swieci sie dioda P1.0 i P2.3

            P1OUT &= ~LEDR;             // Negacja diody P1.6
            P2OUT &= ~BIT1;             // Negacja diody P2.1
            P1OUT |= LEDG;              // Ustawienie logicznej 1 na P1.0 (dioda zielona)
            P2OUT |= BIT3;              // Ustawienie logicznej 1 na P2.3 (dioda zielona)
            break;
        case 3:                         // Swieci sie dioda P2.5 (niebieska), P2.1 (czerwona), P1.0 (zielony) i P1.6 (czerwony)
            P2OUT &= ~BIT3;             // Negacja diody P2.3
            P1OUT |=LEDR+LEDG;          // Ustawienie logicznej 1 na P1.0 i P1.6
            P2OUT |= BIT5+BIT1;         // Ustawienie logicznej 1 na P2.5 (dioda niebieska), P.2.1 (dioda czerwona)
            break;
        case 4:                         // Swieci dioda P2.5 (niebieska)
            P1OUT &= ~(LEDR+LEDG);      // Negacja diody P1.6 (czerwonej), P1.0 (zielonej)
            P2OUT &= ~BIT1;             // Negacja diody P2.1 (czerwonej)
            break;
        case 5:
            P2OUT &= ~BIT5;             // Zanegowanie diody P2.5 (niebieska)
            x=0;                        // Reset instrukcji Switch case
            break;
        default:                        // Wylaczenie wszystykich diód
            P1OUT &= ~LEDG+LEDR;        // Negacja P1.0 i P1.6
            P2OUT &= ~BIT1+BIT3+BIT5;   // Negacja P2.1; P2.3; P2.5
    }
            P1IFG &= ~SW;               // Czyszczenie flagi przerwania na przycisku
                                        // Bit 1 : Flaga ustawiana z pojawieniem sie zbocza Hi-Lo
}

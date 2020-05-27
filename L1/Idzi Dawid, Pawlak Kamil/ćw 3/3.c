#include "msp430g2553.h"

#define RED BIT6                        // Red LED -> P1.6
#define GRN BIT0                        // Green LED -> P1.0
#define SW BIT3                         // Switch -> P1.3
int licznik;                            // deklaracja zmiennej licznik jako integer

void main(void){
    WDTCTL = WDTPW + WDTHOLD;           // Stop watchdog timer

    P1DIR |= RED;                       // Set LED pin -> Output
    P1OUT &= ~RED;                      // Wy³¹czenie diody zielonej
    P1DIR |= GRN;                       // Set LED pin -> Output
    P1OUT &= ~GRN;                      // wy³¹czenie diody czerwonej
    P1DIR &= ~SW;                       // Ustawienie wejœcia na pinie P1.3 (SW2)
    P1REN |= SW;                        // w³¹czenie rezystora podci¹gaj¹cego
    P1OUT |= SW;                        // ustawienie rezystora podci¹gaj¹cego jako pull-up

    TA0CCTL0 |= CCIE;                   // Ustawienie wywo³ania przerwania timera0 po odliczeniu ustawionego czasu - timer 0
    TA0CTL |= TASSEL_1  + TACLR ;       // Ustawienie liczenia - "w górê", Zegar -> ACLK, Zerowanie stanu czasu - timer 0
    TA1CCTL0 |= CCIE;                   // Ustawienie wywo³ania przerwania timera1 po odliczeniu ustawionego czasu - timer 1
    TA1CTL |= TASSEL_1  + TACLR ;       // Ustawienie liczenia - "w górê", Zegar -> ACLK, Zerowanie stanu czasu - timer 1

    TA0CCR0 = 15000;                    // Ustawienie Timera 0 na 3 sekundy
    TA1CCR0 = 5000;                     // Ustawienie Timera 1 na 1 sekundê

    P1IES &= ~SW;                       // Wybranie zbocza narastajacego na wyzwalanie przerwania (puszczenie przycisku)
    P1IE |= SW;                         // W³¹czenie przerwania na przycisku

    __bis_SR_register(LPM0_bits + GIE); // Wejœcie w tryb LPM3 (Low-power Mode) oraz w³¹czenie przerwania CPU
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void){          // Przerwanie wykonane przez naciœniêcie przycisku
    licznik=0;                          // ustawienie wartoœci zmiennej licznik na "0"
    TACTL |= MC_1;                      // Uruchomienie przerwania timera 0
    P1IFG &= ~SW;                       // Czyszczenie flagi przerwania na Przycisku
}

#pragma vector = TIMER0_A0_VECTOR       // Przerwanie po odliczeniu czasu timera 0
__interrupt void CCR0_ISR(void){

    if (licznik==0){
        P1OUT |= GRN;                   // Zapalenie diody zielonej
    }
    else if (licznik<4){
        P1OUT ^= GRN;                   // prze³¹czenie stanu diody zielonej
        P1OUT ^= RED;                   // prze³¹czenie stanu diody czerwonej
    }
    else{
        licznik=0;                      // ustawienie wartoœci zmiennej licznik na "0"
        TA0CTL &= ~MC_3;                // Zatrzymanie timera 0
        TA1CTL |= MC_1;                 // Uruchomienie timera1
        P1OUT &= ~GRN;                  // Wy³¹czenie led zielonej
        P1OUT &= ~RED;                  // wy³¹czeenie led czerwonej
    }

    licznik++;                          // inkrementacja zartoœci zmiennej licznik
}

#pragma vector = TIMER1_A0_VECTOR       // Przerwanie po odliczeniu czasu timera 1
__interrupt void CCR1_ISR(void){
    while(licznik<=4){                  // wykonanie 4 zmian stanu diody czerownej
        P1OUT ^= RED;                   // zmiana stanu diody czerwonej
        __delay_cycles(200000);         // opó¿nienie wykonania pêtli while
        licznik++;                      // inkrementacja zmiennej licznik
    }

    TA1CTL &= ~MC_3;                    // Zatrzymanie wykonania timera 1
}

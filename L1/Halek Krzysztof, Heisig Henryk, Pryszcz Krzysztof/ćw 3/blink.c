#include "msp430g2553.h"

#define RED BIT6                        // Red LED -> P1.6
#define GRN BIT0                        // Green LED -> P1.0
#define SW BIT3                         // Switch -> P1.3
int licznik,zerojeden=0;                // deklaracja zmiennej licznik jako integer

void main(void){
    //Konfiguracja watchdoga
    WDTCTL = WDTPW + WDTHOLD;           // Stop watchdog timer

    //Konfiguracja I/O
    P1DIR |= RED;                       // Set LED pin -> Output
    P1OUT &= ~RED;                      // Wy��czenie diody zielonej
    P1DIR |= GRN;                       // Set LED pin -> Output
    P1OUT &= ~GRN;                      // wy��czenie diody czerwonej
    P1DIR &= ~SW;                       // Ustawienie wej�cia na pinie P1.3 (SW2)
    P1REN |= SW;                        // w��czenie rezystora podci�gaj�cego
    P1OUT |= SW;                        // ustawienie rezystora podci�gaj�cego jako pull-up

    //Konfiguracja Timer 0
    TACCTL0 |= CCIE;                    // Ustawienie wywo�ania przerwania timera0 po odliczeniu ustawionego czasu
    TACTL |= TASSEL_2  + TACLR + ID_3;  // Zegar SMCLK, Zerowanie stanu czasu
    TA0CCR0 = 37500;                    // Ustawienie czas na 0,3 sekundy

    //Konfiguracja Timer 1
    TA1CCTL0 |= CCIE;                   // Ustawienie wywo�ania przerwania timera1 po odliczeniu ustawionego czasu
    TA1CTL |= TASSEL_2  + TACLR + ID_3; // Zegar SMCLK, Zerowanie stanu czasu
    TA1CCR0 = 12500;                    // Ustawienie czasu na 0,1 sekundy

    //konfiguracja przerwania z przycisku
    P1IES &= ~SW;                       // Wybranie zbocza narastajacego na wyzwalanie przerwania (puszczenie przycisku)
    P1IE |= SW;                         // W��czenie przerwania na przycisku

    //wej�cie w tryb oszcz�dzania energii
    __bis_SR_register(LPM1_bits + GIE); // Wej�cie w tryb LPM1 (Low-power Mode) oraz w��czenie przerwania CPU
}

//kod po wywo�aniu przerwaania uruchomiony przyciskiem
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void){          // Przerwanie wykonane przez naci�ni�cie przycisku
    licznik=0;                          // ustawienie warto�ci zmiennej licznik na "0"
    TACTL |= MC_1;                      // Uruchomienie przerwania timera 0
    P1IFG &= ~SW;                       // Czyszczenie flagi przerwania na Przycisku
}

//kod po wywo�aniu przerwaania uruchomiony przez timer 0
#pragma vector = TIMER0_A0_VECTOR       // Przerwanie po odliczeniu czasu timera 0
__interrupt void CCR0_ISR(void){

    zerojeden++;                        // inkrementacja zmiennej zliczaj�cej wyzwalanie przerwania

    if(zerojeden==10){                  // kod wykonany po 10 przerwaniach ( 10*0,3s=3s )
        if (licznik==0){
            P1OUT |= GRN;               // Zapalenie diody zielonej po 3 sekundach wykonania kodu
        }
        else if (licznik<4){
            P1OUT ^= GRN;               // zmiana stanu na przeciwny diody zielonej
            P1OUT ^= RED;               // zmiana stanu na przeciwny diody czerwonej
        }
        else{
            licznik=0;                  // zerowanie zmiennej "licznik"
            TACTL &= ~MC_3;             // Zatrzymanie timera 0
            TA1CTL |= MC_1;             // Uruchomienie liczenia timera 1 "w g�r�"
            P1OUT &= ~GRN;              // Wy��czenie led zielonej
            P1OUT &= ~RED;              // wy��czeenie led czerwonej
            zerojeden=0;                // zerowanie zmiennej "zerojeden"
        }

        licznik++;                      // inkrementacja zarto�ci zmiennej licznik
        zerojeden=0;                    // zerowanie zmiennej "zerojeden"
    }
}

//kod po wywo�aniu przerwaania uruchomiony przez timer 1
#pragma vector = TIMER1_A0_VECTOR
__interrupt void CCR1_ISR(void){
    zerojeden++;                        // inkrementacja zmiennej zliczaj�cej wyzwalanie przerwania
    if (zerojeden==10){                 // kod wykonany po 10 przerwaniach ( 10*0,1s=1s )
        while(licznik<=4){              // wykonanie 4 zmian stanu diody czerownej
            P1OUT ^= RED;               // zmiana stanu diody czerwonej
            __delay_cycles(150000);     // op�nienie wykonania p�tli while
            licznik++;                  // inkrementacja zmiennej licznik
        }
        TA1CTL &= ~MC_3;                // Zatrzymanie wykonania timera 1
    }
}

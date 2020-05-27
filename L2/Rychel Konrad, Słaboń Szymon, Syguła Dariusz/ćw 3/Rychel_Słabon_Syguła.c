#include "msp430g2553.h"

#define RED_LED BIT6                        // Red LED -> P1.6
#define GREEN_LED BIT0                        // Green LED -> P1.0
#define SW BIT3                         // Switch -> P1.3


void main(void){
    WDTCTL = WDTPW + WDTHOLD;           // Stop watchdog timer

// Ustawianie diod i przycisku

    P1DIR |= RED_LED;                       
    P1OUT &= ~RED_LED;                     
    P1DIR |= GREEN_LED;                       
    P1OUT &= ~GREEN_LED;                      
    P1DIR &= ~SW;                       
    P1REN |= SW;                        // wlaczenie rezystora podciagajacego
    P1OUT |= SW;                        // ustawienie rezystora podciagajacego jako pull-up
	
	P1IES &= ~SW;                       // Wybranie wybranie puszczenia przycisku jako wyzwalanie przerwania
    P1IE |= SW;                         // Wlczenie przerwania na przycisku

    TA0CCTL0 |= CCIE;                   					// Ustawienie wywo³ania przerwania timera0 po odliczeniu ustawionego czasu - timer 0
    TA0CTL |= TASSEL_2  + TACLR + ID_3; 	// Ustawienie liczenia - "w górê", Zegar -> ACLK, Zerowanie stanu czasu - timer 0
    TA1CCTL0 |= CCIE;                 				 	// Ustawienie wywo³ania przerwania timera1 po odliczeniu ustawionego czasu - timer 1
    TA1CTL |= TASSEL_2  + TACLR + ID_3; 	// Ustawienie liczenia - "w górê", Zegar -> ACLK, Zerowanie stanu czasu - timer 1

    TA0CCR0 = 37500;                    // Ustawienie Timera 0 na 3 sekundy
    TA1CCR0 = 12500;                    // Ustawienie Timera 1 na 1 sekundê

    __bis_SR_register(LPM0_bits + GIE); 			// Wejscie w tryb LPM3 (Low-power Mode) oraz wlaczenie przerwania CPU
}

unsigned int licznik=0;							// deklaracja zmiennej licznik jako integer
unsigned int takt=0;                              // deklaracja zmiennej licznik jako takt
unsigned int y;                            			// deklaracja zmiennej y

 // Przerwanie wykonane przez uruchomienie przycisku
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void){         
    TACTL |= MC_1;                      // Uruchomienie przerwania timera 0
    P1IFG &= ~SW;                       // Czyszczenie flagi przerwania na przycisku
}


 // Przerwanie wykonane przez timer 1
#pragma vector = TIMER1_A0_VECTOR       			// Przerwanie po odliczeniu czasu timera 1
__interrupt void CCR1_ISR(void){
    takt++;                           								    // inkrementacja zmiennej zliczajacej ilosc taktowan
    if (takt==10){
        for(y=0; y<3; y++){                  					// wykonanie 4 zmian stanu diody czerownej

            P1OUT ^= RED_LED;                   				// zmiana stanu diody czerwonej
            __delay_cycles(100000);         					// opóznienie wykonania petli for

        }
            P1OUT &= ~RED_LED;                              // zmiana stanu diody czerwonej
        TA1CTL &= ~MC_3;                    					// zatrzymanie wykonania timera 1
		__bic_SR_register_on_exit( LPM4_bits );			// zakonczenie funkcji przerwania
    }
}

 // Przerwanie wykonane przez timer 0
#pragma vector = TIMER0_A0_VECTOR       // Przerwanie po odliczeniu czasu timera 0
__interrupt void CCR0_ISR(void){

    takt++;                            // inkrementacja zmiennej zliczajacej ilosc taktowan

    if(takt==10){                      // wykonanie kodu po zliczeniu 10 przerwan
        if (licznik==0)
		{
            P1OUT |= GREEN_LED;                   // zapalenie diody zielonej
        }
		
        else if (licznik!=5)
		{
            P1OUT ^= GREEN_LED;                   // przelaczenie stanu diody zielonej
            P1OUT ^= RED_LED;                   // przelaczenie stanu diody czerwonej
        }
		
        else if(licznik==5)
		{
            licznik=0;                      // ustawienie wartsci zmiennej licznik na "0"
            TA0CTL &= ~MC_3;                // Zatrzymanie timera 0
            TA1CTL |= MC_1;                 // Uruchomienie timera 1
            P1OUT &= ~GREEN_LED;                  // wylaczenie led zielonej
            P1OUT &= ~RED_LED;                  // wylazenie led czerwonej
            takt=0;
        }

        licznik++;                          // inkrementacja wartosci zmiennej licznik
        takt=0;
    }
}



#include <msp430.h>
/* --------- Adresowanie symboliczne -----------------------------------
 Przypisanie adresów symbolicznych do bitów
 --------------------------------------------------------------*/

#define Przycisk      BIT3                        // Przyciskitch -> P1.3
#define LedRed        BIT0                        // LedRed LED -> P1.0
#define LedGreen      BIT6                        // LedGreen LED -> P1.6

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;
    /* --------- Hardware -----------------------------------
     Ustawienie wejsc wyjsc rezystor�w itd
     --------------------------------------------------------------*/

    P1DIR |= LedRed+LedGreen;                 // Ustawienie ledow jako wyjscia
    P1OUT &= ~LedRed;                        // wyłącz led czerwony
    P1OUT &= ~LedGreen;                     // wyłącz led zielony
    P1DIR &= ~Przycisk;                       // ustawienie przycisku jako wejscie
    P1REN |= Przycisk;                        // włączenie rezystora
    P1OUT |= Przycisk;                        // ustawienie rezystora jako pull up
    P1OUT &= LedRed;                        //w�acza diode czerwon�
    /* --------- zmiennej -----------------------------------
     inicjalizacja zmiennych pomocniczych
     --------------------------------------------------------------*/
    int przyciskPressed = 0;
    int Sekwencja = 0;

    /* --------- która sekwencja -----------------------------------------
     Czesc programu odpowiadająca za zlicznie ilości wciśnieć przycisku
     ----------------------------------------------------------------------*/
    if(!(P1IN & Przycisk)){                     // je�li przycisk zostanie wcisniety

        przyciskPressed++;                     // dodaj jeden do zmiennej
        __delay_cycles(20000);                 //czekaj 20ms

    }
    else{                                      //jeśli poprzedni if nie jest speśniony
        przyciskPressed=0;                     //ustaw wartość zmiennej na 0
    }

    if(przyciskPressed == 1){                   //jesli zostało wykryte zbocze
                                                //narastające(if...else powyżej) dodaje jeden do zmiennej
        Sekwencja++;

    }

    /* --------- Sekwencje mrugania -----------------------------------------------
     W zależności od ktoraSekwencja  taka sekwencja mrugania jest wykonywana
     dla 0 led wyłączone
     ---------------------------------------------------------------------------------*/
     if(Sekwencja == 0){                   // zerowa sekwencja
       P1OUT &= ~LedRed;                        // wyłącz led czerwony
       P1OUT &= ~LedGreen;                     // wyłącz led zielony
     }

    if(Sekwencja == 1){                    // pierwsza sekwencja

        P1OUT &= LedRed;                        // w��cza czerwona diode
    }

    if(Sekwencja == 2){                    // druga sekwencja
        P1OUT &= ~LedRed;                        // wyłącz led czerwony
        P1OUT &= LedGreen;                      // w��cza diode zielona
        }

    if(Sekwencja == 3){                   // trzecia sekwencja

        P1OUT &= LedRed;                // zapalenie diody czerwonej
        }

    if(Sekwencja == 4){                   // czwarta sekwencja
        P1OUT &= ~LedRed;
        P1OUT &= ~LedGreen;                      // wy��cza obie diody
        __delay_cycles(500000);                 //czekaj 500ms
        P1OUT &= LedRed;
        P1OUT &= LedGreen;                      // wy��cza obie diody
        __delay_cycles(500000);                 //czekaj 500ms


        }

    if(Sekwencja == 5){                  // pi�te wci�ni�cie przycisku zeruje sekwencje
        Sekwencja = 0;

        }






}

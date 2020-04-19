#include <msp430.h>
#define SW  BIT3                    // definiujemy zmienn¹ SW która bêdzie odpowiada³a za Bit3. BIT3 odpowaida za fizyczny pin P1.3 na p³ytce prototypowej
#define RED BIT1                    // Zdefiniujemy bit3 jako kolor czerwony diody RGB
#define GRN BIT3                    // Zdefiniujemy bit1 jako kolor zielony diody RGB
#define BLU BIT5                    // Zdefiniujemy bit5 jako kolor niebieski diody RGB
unsigned int i,a=1;                 // rezerwujemy zmienn¹ "i" oraz "a" jako unsigned integer


void wykonaj(){                     // rozpoczêcie nieskoñczonej pêtli programu, który przy ka¿dym naciœniêciu przycisku SW2 inkrementuje wartoœæ zmiennej "a"

    if(a==1){                       // warunek, który jest spe³niony jeœli zmienna "a" jest równa 1
        P2OUT |= GRN;
        P2OUT |= RED;               // ustawienie wyjœæ dla odpowiednich pinów do których jest pod³¹czona dioda RGB
        P2OUT &= ~BLU;              // w tej konfiguracji zaœwiecony jest kolor czerwony i zielony
    }

    if(a==2){                       // warunek, który jest spe³niony jeœli zmienna "a" jest równa 2
        P2OUT &= ~GRN;
        P2OUT |= RED;               // ustawienie wyjœæ dla odpowiednich pinów do których jest pod³¹czona dioda RGB
        P2OUT |= BLU;               // w tej konfiguracji zaœwiecony jest kolor niebieski i zielony
    }

    if(a==3){                       // warunek, który jest spe³niony jeœli zmienna "a" jest równa 3
        P2OUT |= GRN;
        P2OUT |= RED;               // ustawienie wyjœæ dla odpowiednich pinów do których jest pod³¹czona dioda RGB
        P2OUT |= BLU;               // w tej konfiguracji zaœwiecony jest kolor czerwony, zielony i niebieski
    }

    if(a==4){                       // warunek, który jest spe³niony jeœli zmienna "a" jest równa 4
        P2OUT |= GRN;
        P2OUT &= ~RED;              // ustawienie wyjœæ dla odpowiednich pinów do których jest pod³¹czona dioda RGB
        P2OUT |= BLU;               // w tej konfiguracji zaœwiecony jest kolor zielony i niebieski
    }

    if(a==5){                       // warunek, który jest spe³niony jeœli zmienna "a" jest równa 5
        P2OUT |= GRN;
        P2OUT &= ~RED;              // ustawienie wyjœæ dla odpowiednich pinów do których jest pod³¹czona dioda RGB
        P2OUT &= ~BLU;              // w tej konfiguracji zaœwiecony jest kolor zielony
    }

    if(a==6){                       // warunek, który jest spe³niony jeœli zmienna "a" jest równa 6
        P2OUT &= ~GRN;
        P2OUT |= RED;               // ustawienie wyjœæ dla odpowiednich pinów do których jest pod³¹czona dioda RGB
        P2OUT &= ~BLU;              // w tej konfiguracji zaœwiecony jest kolor czerwony
    }

    if(a==7){                       // warunek, który jest spe³niony jeœli zmienna "a" jest równa 7
        P2OUT &= ~GRN;
        P2OUT &= ~RED;              // ustawienie wyjœæ dla odpowiednich pinów do których jest pod³¹czona dioda RGB
        P2OUT |= BLU;               // w tej konfiguracji zaœwiecony jest kolor niebieski
    }

    if(a>=8){                       // Jeœli zmienna "a" jest wiêksza lub równa 8
        a = 1;                      // nastêpuje ustawienie wartoœci zmiennej "a" na 1
        wykonaj();                  // ponowne wykonanie funkcji aby zrealizowaæ kombinacjê dla "a = 1"
    }
}

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;       // Zatrzymanie zegara watchdog'a

    P2DIR |= (RED+GRN+BLU);         // Ustawienie wyjœæ na pinach P2.3 (Czerwona LED), P2.1 (Zielona LED), P2.5(Niebieska LED)
    P1DIR &= ~SW;                   // Ustawienie wejœcia na pinie P1.3 (SW2)
    P1REN |= SW;                    // w³¹czenie rezoystora podci¹gaj¹cego
    P1OUT |= SW;                    // ustawienie rezystora podci¹gaj¹cego jako pull-up

    P1IES &= ~SW;                   // Wybranie zbocza narastajacego na wyzwalanie przerwania (puszczenie przycisku)
    P1IE |= SW;                     // W³¹czenie przerwania na przycisku

    __bis_SR_register(LPM4_bits + GIE); // Wejœcie w tryb LPM4 (Low-power Mode) oraz w³¹czenie przerwania CPU
}

#pragma vector=PORT1_VECTOR

__interrupt void Port_1(void){

    a=a+1;                          // inkrementacja zmiennej "a"
    wykonaj();                      // Wykonanie funkcji w której bêd¹ ustawione odpowiednie stany na wyjœciach w zale¿noœci od zmiennej "a"
    P1IFG &= ~SW;                   // Czyszczenie flagi przerwania na Przycisku
}

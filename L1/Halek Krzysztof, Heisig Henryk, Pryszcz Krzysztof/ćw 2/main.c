#include <msp430.h>
#define SW  BIT3                    // definiujemy zmienn� SW kt�ra b�dzie odpowiada�a za Bit3. BIT3 odpowaida za fizyczny pin P1.3 na p�ytce prototypowej
#define RED BIT1                    // Zdefiniujemy bit3 jako kolor czerwony diody RGB
#define GRN BIT3                    // Zdefiniujemy bit1 jako kolor zielony diody RGB
#define BLU BIT5                    // Zdefiniujemy bit5 jako kolor niebieski diody RGB
unsigned int i,a=1;                 // rezerwujemy zmienn� "i" oraz "a" jako unsigned integer


void wykonaj(){                     // rozpocz�cie niesko�czonej p�tli programu, kt�ry przy ka�dym naci�ni�ciu przycisku SW2 inkrementuje warto�� zmiennej "a"

    if(a==1){                       // warunek, kt�ry jest spe�niony je�li zmienna "a" jest r�wna 1
        P2OUT |= GRN;
        P2OUT |= RED;               // ustawienie wyj�� dla odpowiednich pin�w do kt�rych jest pod��czona dioda RGB
        P2OUT &= ~BLU;              // w tej konfiguracji za�wiecony jest kolor czerwony i zielony
    }

    if(a==2){                       // warunek, kt�ry jest spe�niony je�li zmienna "a" jest r�wna 2
        P2OUT &= ~GRN;
        P2OUT |= RED;               // ustawienie wyj�� dla odpowiednich pin�w do kt�rych jest pod��czona dioda RGB
        P2OUT |= BLU;               // w tej konfiguracji za�wiecony jest kolor niebieski i zielony
    }

    if(a==3){                       // warunek, kt�ry jest spe�niony je�li zmienna "a" jest r�wna 3
        P2OUT |= GRN;
        P2OUT |= RED;               // ustawienie wyj�� dla odpowiednich pin�w do kt�rych jest pod��czona dioda RGB
        P2OUT |= BLU;               // w tej konfiguracji za�wiecony jest kolor czerwony, zielony i niebieski
    }

    if(a==4){                       // warunek, kt�ry jest spe�niony je�li zmienna "a" jest r�wna 4
        P2OUT |= GRN;
        P2OUT &= ~RED;              // ustawienie wyj�� dla odpowiednich pin�w do kt�rych jest pod��czona dioda RGB
        P2OUT |= BLU;               // w tej konfiguracji za�wiecony jest kolor zielony i niebieski
    }

    if(a==5){                       // warunek, kt�ry jest spe�niony je�li zmienna "a" jest r�wna 5
        P2OUT |= GRN;
        P2OUT &= ~RED;              // ustawienie wyj�� dla odpowiednich pin�w do kt�rych jest pod��czona dioda RGB
        P2OUT &= ~BLU;              // w tej konfiguracji za�wiecony jest kolor zielony
    }

    if(a==6){                       // warunek, kt�ry jest spe�niony je�li zmienna "a" jest r�wna 6
        P2OUT &= ~GRN;
        P2OUT |= RED;               // ustawienie wyj�� dla odpowiednich pin�w do kt�rych jest pod��czona dioda RGB
        P2OUT &= ~BLU;              // w tej konfiguracji za�wiecony jest kolor czerwony
    }

    if(a==7){                       // warunek, kt�ry jest spe�niony je�li zmienna "a" jest r�wna 7
        P2OUT &= ~GRN;
        P2OUT &= ~RED;              // ustawienie wyj�� dla odpowiednich pin�w do kt�rych jest pod��czona dioda RGB
        P2OUT |= BLU;               // w tej konfiguracji za�wiecony jest kolor niebieski
    }

    if(a>=8){                       // Je�li zmienna "a" jest wi�ksza lub r�wna 8
        a = 1;                      // nast�puje ustawienie warto�ci zmiennej "a" na 1
        wykonaj();                  // ponowne wykonanie funkcji aby zrealizowa� kombinacj� dla "a = 1"
    }
}

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;       // Zatrzymanie zegara watchdog'a

    P2DIR |= (RED+GRN+BLU);         // Ustawienie wyj�� na pinach P2.3 (Czerwona LED), P2.1 (Zielona LED), P2.5(Niebieska LED)
    P1DIR &= ~SW;                   // Ustawienie wej�cia na pinie P1.3 (SW2)
    P1REN |= SW;                    // w��czenie rezoystora podci�gaj�cego
    P1OUT |= SW;                    // ustawienie rezystora podci�gaj�cego jako pull-up

    P1IES &= ~SW;                   // Wybranie zbocza narastajacego na wyzwalanie przerwania (puszczenie przycisku)
    P1IE |= SW;                     // W��czenie przerwania na przycisku

    __bis_SR_register(LPM4_bits + GIE); // Wej�cie w tryb LPM4 (Low-power Mode) oraz w��czenie przerwania CPU
}

#pragma vector=PORT1_VECTOR

__interrupt void Port_1(void){

    a=a+1;                          // inkrementacja zmiennej "a"
    wykonaj();                      // Wykonanie funkcji w kt�rej b�d� ustawione odpowiednie stany na wyj�ciach w zale�no�ci od zmiennej "a"
    P1IFG &= ~SW;                   // Czyszczenie flagi przerwania na Przycisku
}

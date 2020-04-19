
#include <msp430.h>
/* --------- Adresowanie symboliczne -----------------------------------
 Przypisanie adresów symbolicznych do bitów
 --------------------------------------------------------------*/

#define przycisk      BIT3                    // Przycisk -> P1.3
#define GREEN   BIT6                    // LedRed LED -> P1.0
#define RED     BIT0                    // LedGreen LED -> P1.6
#define Time1   100000
#define Time2   50000

volatile int flag=0;                   

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer
    /* --------- Hardware -----------------------------------
         Ustawienie wejsc wyjsc rezystorów itd
         --------------------------------------------------------------*/

    P1DIR |= GREEN + RED;               // Ustawienie ledow jako wyjscia
    P1DIR &= ~przycisk;                       // ustawienie przycisku jako wejscie
    P1REN |= przycisk;                        // wÅ‚Ä…czenie rezystora
    P1OUT |= przycisk;                        // ustawienie rezystora jako pull up
    P1IES &= ~przycisk;
    P1IE |= przycisk;                            //W³¹czana jest obs³uga przerwañ dla wejœcia

    __bis_SR_register( GIE);             // ustawia bity rejestru stanu

    while(1)                                                             // pêtla while
     {
          P1OUT ^= RED;
          if(flag%2) __delay_cycles(Time1);                                 // Warunek warunkujacy swiecenie diody czerwonej
          else __delay_cycles(Time2);
     };
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    P1OUT ^= GREEN;                                                            // Zmiana stanu Green LED
    flag++;                                                                    // inkremenentacja zmieonnej globalnej
    P1IFG &= ~przycisk;                                                              // Zerowana jest flaga przerwania:
    __bic_SR_register_on_exit( LPM4_bits );                                    //instrukcja która nakazuje zakoñczenie procedury obs³ugi przerwania
}

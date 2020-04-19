
#include <msp430.h>


#define SW      BIT3                    // Podstawowe Definiowanie konkretnych bitów
#define GREEN   BIT6
#define RED     BIT0
#define Time1   62000
#define Time2   32000

volatile int flag=0;                    // ustawianie zmiennej globalnej flag i nadanie jej wartosci 1
                                        // slowo kluczowe volatile mowi kompilatorowi o mozliwosci
                                        // zamiany wartosci zmiennej z zewnatrz
                                        // Dzieki  slowu kompilator nie dokonuje optymalizacji kodu
                                        // (w normalnym warunkach przeypisalby jej wartosc constant w procesie optymalizacji)
                                        // i za kazdym razem sprawdza rzeczywista wartosc zmiennej



void main(void) {
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

    P1DIR |= GREEN + RED;               // Diody Led jako wyjscie
    P1DIR &= ~SW;                       //Switch jako wejscie

    P1REN |= SW;                        //Aktywowany jest rezystor podci¹gaj¹cy typu pull-up
    P1OUT |= SW;

    P1IES &= ~SW;
    P1IE |= SW;                            //W³¹czana jest obs³uga przerwañ dla wejœcia

    __bis_SR_register( GIE);             // odblokowanie obslugi przerwan

    while(1)                                                             // pêtla g³ówna programu
     {
          P1OUT ^= GREEN;
          if(flag%2) __delay_cycles(Time1);                                 // Warunek warunkujacy swiecenie diody czerwonej
          else __delay_cycles(Time2);
     };
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{

    P1OUT ^= RED;                                                              // Zmiana stanu Green LED
    flag++;                                                                    // inkremenentacja zmieonnej globalnej
    P1IFG &= ~SW;                                                              // Zerowana jest flaga przerwania:
    __bic_SR_register_on_exit( LPM4_bits );                                     //instrukcja która nakazuje zakoñczenie procedury obs³ugi przerwania

}

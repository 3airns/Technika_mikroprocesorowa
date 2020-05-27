#include <msp430.h>
// Definicja zmiennych

#define SW      BIT3
#define GREEN   BIT6
#define RED     BIT0
#define Time1   20000
#define Time2   40000

volatile int flag=0;


void main(void) {
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

    P1DIR |= GREEN + RED;               // Ustawienie wyjœæ
    P1DIR &= ~SW;                       //Ustawienie wejœæ

    P1REN |= SW;                        //Aktywacja rezystora pull-up
    P1OUT |= SW;

    P1IES &= ~SW;
    P1IE |= SW;                            //W³¹czana jest obs³uga przerwañ dla wejœcia

    __bis_SR_register( GIE);             // Odblokowanie obslugi przerwan

    while(1)
     {
          P1OUT ^= RED;
          if(flag%2) __delay_cycles(Time1);
          else __delay_cycles(Time2);
     };
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    P1OUT ^= GREEN;
    flag++;
    P1IFG &= ~SW;                                    // Zerowana jest flaga przerwania:
    __bic_SR_register_on_exit( LPM4_bits );         //instrukcja która nakazuje zakoñczenie procedury obs³ugi przerwania
}

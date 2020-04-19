#include <msp430.h>


#define     DELAY1  2000000                        // opóźnienie

void main( void )                                  // program główny
{
  WDTCTL = WDTPW + WDTHOLD;                        // zatrzymaj układ Watchdog


    P1OUT = 0x00;                                  //stan niski wejscia
    P2OUT = 0x00;                                  //stan niski wejscia

    P1DIR = 0xff;                                  //kierunek z wejscia na wyjscie
    P2DIR = 0xff;                                  //kierunek z wejscia na wyjscie

        //microswitch P1.3
    P1OUT |=   BIT3;                               // wejście "pull-up"
    P1REN |=   BIT3;                               // włącz rezystor podciągający
    P1DIR &=~  BIT3;                               // ustaw linie w kierunku wejścia

        //obsługa przerwań
    P1IES |=   BIT3;                               // ustaw zbocze wyzwalające ( 1->0 )
    P1IFG &=~  BIT3;                               // zeruj flagę przerwania
    P1IE  |=   BIT3;                               // włącz obsługę przerwania
    __bis_SR_register(GIE);                        // odblokowanie przerwań maskowalnych

             while(1)                              // pętla główna programu
              {



                  P1OUT |=  BIT0;                  // włącz diodę  LED ZIELONY
                __delay_cycles(DELAY1);            // czekaj
                  P1OUT &=~ BIT0;                  // wyłącz diodę LED ZIELONY

                  P2OUT |=  BIT3;                  // włącz diodę  RGB ZIELONY
                __delay_cycles(DELAY1);            // czekaj
                  P2OUT &=~ BIT3;                  // wyłącz diodę RGB ZIELONY



              }

     __bis_SR_register( LPM4_bits );

}

#pragma vector=PORT1_VECTOR                         // procedura obsługi przerwania
__interrupt void Port_1(void)                       // port P1
{

        P1OUT &=~ BIT0;                            // wyłącz diodę LED ZIELONY
        P2OUT &=~ BIT3;                            // wyłącz diodę RGB ZIELONY
        P1OUT ^= BIT6;                             // włącz diodę  LED CZERWONA
        __delay_cycles(DELAY1/6);                  // czekaj
        P1IFG &= ~BIT6;                            // włącz diodę  LED CZERWONA

}

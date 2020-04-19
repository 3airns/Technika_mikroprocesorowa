#include <msp430.h>


#define SW  BIT3                    			// Przycisk -> P1.3
#define RED_LED BIT0                    // Czerwony LED-> P1.0
#define GREEN_LED   BIT6             // Zielony LED -> P1.0

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;           // Zatrzymywanie Watchdog timer'a

    P1DIR |= GREEN_LED + RED_LED;               // Klasyczne definiowanie

    P1OUT |= RED_LED;
    P1DIR &= ~SW;

    P1REN |= SW;
    P1OUT |= SW;

    P1IES &= ~SW;												// Zerowanie flagi przerwań 
    P1IE |= SW;														// Włączanie obsługi przerwań

    __bis_SR_register( GIE);    							    // Odblokowanie obsługi przerwań

    while(1)                                                             
     {                                                                     
          P1OUT ^= RED_LED;								// Przełączanie czerwonej diody
       __delay_cycles(32000);								// Opuźnienie przełączania
     };

}

// Funkcja przerwania
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{

    P1OUT ^= GREEN_LED;                     				// Przełączanie zilonej diody
    P1IFG &= ~SW;                       		  			    // Zerowanie flagi przerwania
    __bic_SR_register_on_exit( LPM4_bits );			// Zakończenie funkcji przerwania

}
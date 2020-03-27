#include <msp430.h>

#define SW  BIT3                    			// Przycisk -> P1.3
#define RED_LED BIT0                    // Czerwony LED-> P1.0
#define GREEN_LED   BIT6             // Zielony LED -> P1.0

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;       // Zatrzymywanie Watchdog timer'a

    P1DIR |= RED_LED+GREEN_LED;          // Ustawianie diod na wyjściu
    P1DIR &= ~SW;                 					  // Ustawianie przycisku na wejściu
    P1REN |= SW;                    					  // Przyłączanie rezystora sterującego do przycisku
    P1OUT |= SW;										  // Włączanie przycisku
    P1OUT &= ~GREEN_LED;					  // Wyłączanie zielonej diody
    P1OUT &= ~RED_LED;						  // Wyłączanie czerwonej diody
	
    unsigned int flag = 0;							   	 // Definiowanie zmiennej bez znaku
    while(1)
    {
        if(!(P1IN & SW))           						 // Jeżeli przycisk jest wciśnięty
        {

		flag ++;												// Inkrementacja wartości zmiennej flag
		__delay_cycles(320000);						// Opuźnienie
        }
        switch (flag)										
        case 1:
        {
            {
				P1OUT &= ~GREEN_LED;			// Wyłączanie zielonej diody
				P1OUT ^= RED_LED;					// Przełączanie czerwonej diody
				
				volatile unsigned long i;				// Defniowanie długiej zmiennej bez znaku
				for(i = 0; i < 20000; i++);			// Opóźnianie inkrementacji
                break;
            };
            case 2:
            {
				P1OUT ^= GREEN_LED;				// Przełączanie zielonej diody
				P1OUT ^= RED_LED;					// Przełączanie czerwonej diody
				
				volatile unsigned long i;
				for(i = 0; i < 20000; i++);			// Opuźnienie
                break;
            };
            case 3:
            {
				P1OUT &= ~RED_LED;				// Wyłączanie czerwonej diody
				P1OUT ^= GREEN_LED;				// Przełączanie zielonej diody
				
				volatile unsigned long i;
				for(i = 0; i < 20000; i++);			// Opuźnienie
                break;
            };
            case 4:
            {
                P1OUT &=~RED_LED;				// Wyłączanie czerwonej diody
                P1OUT &=~GREEN_LED;			// Wyłączanie zielonej diody
				
				volatile unsigned long i;
				for(i = 0; i < 20000; i++);			// Opuźnienie
                break;
            };
            default :
            {
                flag=0;										
                __delay_cycles(1000);
            }
        }
    }
}

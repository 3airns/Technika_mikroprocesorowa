
#include <msp430.h>

#define SW		BIT3						// Przycisk -> P1.3
#define RED		BIT0						// LED czerwona -> P1.0
#define GREEN 	BIT6						// LED zielona -> P1.6

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;			// Sprawdzanie na bledy w kodzie

    P1DIR |= RED+GREEN;					// Ustalenie pinów LED  -> Wyjscie
    P1DIR &= ~SW;						// Ustawianie przycisku SW -> wejscie
    P1REN |= SW;						// Wlacz rezystor dla pinu SW
    P1OUT |= SW;						// Select Pull Up for SW pin

    volatile unsigned int flag = 0;
    while(1)
    {
    	if(!(P1IN & SW))				// Gdy przycisk jest wcisniety
    	{
    		__delay_cycles(20000);		// Czeka 20ms - czekamy 20ms na ustabilizowanie siê przycisku
    		if(!(P1IN & SW))			// Sprawdzanie czy jest nadal przycisk wcisniety
    		{							// Ignoruj gdy sygnal wcisniecia przycisku krotszy niz 20ms
    			while(!(P1IN & SW));	// Poczekaj na puszczenie przycisku
    			flag = !flag;			// Zmien wartosc flagi
    		}
    	}
    	if(flag)						// Sprawdz wartosc flagi
    	{
    		P1OUT &= ~GREEN;			// Zielona LED -> Wylacz

    				volatile unsigned int i;	// Zadeklarowanie zmiennej i

    				P1OUT ^= 0x01;				// Przelaczenie czerwonej LED P1.0

    				i = 20000;					// Okreslenie czasu czestotliwosci mrugania
    				do i--;
    				while(i != 0);


    						
    	}
    	else
    	{
    		P1OUT &= ~RED;				// Czerwona LED -> Wylacz
    				
    		volatile unsigned int i;	// Zadeklarowanie zmiennej i
    		
    		    				P1OUT ^= BIT6;				// Czerwona LED -> Wylacz

    		    				i = 10000;					// Okreslenie czasu czestotliwosci mrugania
    		    				do i--;
    		    				while(i != 0);				
    	}
    }
}



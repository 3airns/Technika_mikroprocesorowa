// Program wykonuje się w pętli, domyslnie miga dioda czerwona, po nacisnieciu dioda czerowna gasnie
// i zaczyna migać dioda zielona. Po kolejnym naciśnieciu zaczyna migac dioda czerwona. 
#include <msp430.h> 

#define SW		BIT3						// przycisk -> P1.3
#define RED		BIT0						// czerwony led -> P1.0
#define GREEN 	BIT6						// zielony led -> P1.6

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;			// Stop watchdog timer

    P1DIR |= RED+GREEN;					// Set LED pins -> Output
    P1DIR &= ~SW;						// Set SW pin -> Input
    P1REN |= SW;						// Enable Resistor for SW pin
    P1OUT |= SW;						// Select Pull Up for SW pin

    volatile unsigned int flag = 0;//zmienna do stanu
	volatile unsigned long i = 0;//zmienna do timera
    while(1)
    {
    	if(!(P1IN & SW))				//  gdy przycisk jest wciśniety
    	{
    		__delay_cycles(20000);		// odczekanie 20ms ( drgania styku)
    		if(!(P1IN & SW))			// sprawdz czy przycisk jest nadal wcisnięty
    		{							// Ignores presses shorter than 20ms
    			while(!(P1IN & SW));	// zaczekaj na puszczenie przycisku
    			flag = !flag;			// negacja zmiennej
    		}
    	}
    	if(flag)						
    	{
		while(!(P1IN & SW)){ // czekaj dopoki przycisk jest nie wcisniety
		
    	P1OUT |= BIT0;				//czerwony LED -> ON

    	for(i = 0; i<10000; i++);	//opoznienie

    	P1OUT &= ~BIT0;				//czerwony LED -> OFF
   
    	for(i = 0; i<10000; i++);	//opoznienie
}
	}
	
	
	
    	}
    	else
    	{
		while(!(P1IN & SW)){ // czekaj dopoki przycisk jest nie wcisniety
		
    	P1OUT |= BIT6;				//zielony LED -> ON

    	for(i = 0; i<10000; i++);	//opoznienie

    	P1OUT &= ~BIT6;						//zielony LED -> OFF
   
    	for(i = 0; i<10000; i++);	//opoznienie
}
			
    	}
    }
}
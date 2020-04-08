#include <msp430.h>

#define SW  BIT3						// definiujemy zmienną SW która będzie odpowiadała za Bit3. BIT3 odpowaida za fizyczny pin P1.3 na płytce prototypowej
#define RED BIT3						// Zdefiniujemy bit3 jako kolor czerwony diody RGB
#define GRN BIT1						// Zdefiniujemy bit1 jako kolor zielony diody RGB
#define BLU BIT5						// Zdefiniujemy bit5 jako kolor niebieski diody RGB

unsigned int i,a;						// rezerwujemy zmienną "i" oraz "a" jako unsigned integer

void main(void) {
	WDTCTL = WDTPW | WDTHOLD;			// Zatrzymanie zegara watchdog'a

	P2DIR |= (RED+GRN+BLU);				// Ustawienie wyjść na pinach P2.3 (Czerwona LED), P2.1 (Zielona LED), P2.5(Niebieska LED)
	P1DIR &= ~SW;						// Ustawienie wejścia na pinie P1.3 (SW2) 
	P1REN |= SW;						// włączenie rezoystora podciągającego
	P1OUT |= SW;						// ustawienie rezystora podciągającego jako pull-up
	
	a=1;								// zapisanie wartości 1 dla zmiennej "a"
	
	while(1){							// rozpoczęcie nieskończonej pętli programu, który przy każdym naciśnięciu przycisku SW2 inkrementuje wartość zmiennej "a" 

		if(!(P1IN & SW)){				// wykonanie kodu jeźeli przycisk SW2 jest naciśnięty
			a=a+1;						// inkrementacja zmiennej "a"
			for(i = 0; i<10000; i++);	// opóźnienie, aby przy jednym naciśnięciu inkrementacja zmiennej "a" była wykonana jeden raz
		}

		if(a==1){						// warunek, który jest spełniony jeśli zmienna "a" jest równa 1
			P2OUT |= GRN;				
			P2OUT |= RED;				// ustawienie wyjść dla odpowiednich pinów do których jest podłączona dioda RGB 
			P2OUT &= ~BLU;				// w tej konfiguracji zaświecony jest kolor czerwony i zielony
		}

		if(a==2){						// warunek, który jest spełniony jeśli zmienna "a" jest równa 2
			P2OUT &= ~GRN;
			P2OUT |= RED;				// ustawienie wyjść dla odpowiednich pinów do których jest podłączona dioda RGB
			P2OUT |= BLU;				// w tej konfiguracji zaświecony jest kolor niebieski i zielony
		}

		if(a==3){						// warunek, który jest spełniony jeśli zmienna "a" jest równa 3
			P2OUT |= GRN;
			P2OUT |= RED;				// ustawienie wyjść dla odpowiednich pinów do których jest podłączona dioda RGB
			P2OUT |= BLU;				// w tej konfiguracji zaświecony jest kolor czerwony, zielony i niebieski
		}

		if(a==4){						// warunek, który jest spełniony jeśli zmienna "a" jest równa 4
			P2OUT |= GRN;
			P2OUT &= ~RED;				// ustawienie wyjść dla odpowiednich pinów do których jest podłączona dioda RGB
			P2OUT |= BLU;				// w tej konfiguracji zaświecony jest kolor zielony i niebieski
		}

		if(a==5){						// warunek, który jest spełniony jeśli zmienna "a" jest równa 5
			P2OUT |= GRN;				
			P2OUT &= ~RED;				// ustawienie wyjść dla odpowiednich pinów do których jest podłączona dioda RGB
			P2OUT &= ~BLU;				// w tej konfiguracji zaświecony jest kolor zielony
		}

		if(a==6){						// warunek, który jest spełniony jeśli zmienna "a" jest równa 6
			P2OUT &= ~GRN;
			P2OUT |= RED;				// ustawienie wyjść dla odpowiednich pinów do których jest podłączona dioda RGB
			P2OUT &= ~BLU;				// w tej konfiguracji zaświecony jest kolor czerwony
		}

		if(a==7){						// warunek, który jest spełniony jeśli zmienna "a" jest równa 7
			P2OUT &= ~GRN;
			P2OUT &= ~RED;				// ustawienie wyjść dla odpowiednich pinów do których jest podłączona dioda RGB
			P2OUT |= BLU;				// w tej konfiguracji zaświecony jest kolor niebieski
		}

		if(a>=8){						// Jeśli zmienna "a" jest większa lub równa 8 
			a = 1;						// następuje ustawienie wartości zmiennej "a" na 1
		}
	}
}

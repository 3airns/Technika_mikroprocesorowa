// Program z funkcj� obs�ugi przerwa�
// Wykonane przez : Tacica, Tiszbierek, Szczeka�a AiR ns semestr VI

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//**************************************************************************Definicje dla procesora

#define LED_1 (1<<PB0)  // definicja pinu, do kt�rego jest pod��czona LED(czerwona)
#define LED_1_TOG PORTB ^= LED_1  // makrodefinicja zmiany stanu diody LED1

#define LED_2 (1<<PA0)    // definicja pinu, do kt�rego jest pod��czona led2(niebieska)
#define LED_2_TOG PORTA ^= LED_2 // makrodefinicja zmiany stanu diody LED2

#define SW (1<<PD2)    //definicja, do kt�rego jest pod��czony przycisk SW

//*******************************************************************************P�tla g��wna main()

int main(void)
{
	int x=0;
//************************************************************************************Inicjalizacja
	DDRB |= LED_1;		// Rejestr kierunku PB0 - wyjscie
	DDRA |= LED_2;      // Rejestr kierunku PA0 - wyjscie
	DDRD &=~ SW;        // Rejestr kierunku PD2 - wej�cie

	PORTA |= LED_2;     // Wy��cznie diody LED_2 (pod��czona anod� do pinu)
	PORTB |= LED_1;     // Wy��cznie diody LED_1 (pod��czona anod� do pinu)
    PORTD |= SW ;       // Podci�gniecie pinu pod VCC (wewn�trzny rezystor)

	GICR = 1<<INT0;		// Zezwolenie na przerwania INT0 na wejsciu PD2
	MCUCR &=~ 1<<ISC01 | 1<<ISC00; // INT0 wyzwalane niskim stanem na wejsciu PD2
	poczatek:;          // etykieta
	x=0 ;          // Tworzenie zmiennej x typu int warto�c pocz�tkowa 0

	sei();			// W��cznie globalnych przerwa�
//****************************************************************************P�tla g��wna programu
	while(x<10){   // P�tla While zliczaj�ca od x do 10

		LED_2_TOG;  // Zmiana stanu diody na przeciwny
     _delay_ms(700);   // Przerwa 700 ms
     x+=1;           // Inkrementacja wartosci x
     if(x==10) goto poczatek;  // sprawdzanie wyra�enia x==10 jesli tak skok do poczatek
}

}
ISR(INT0_vect)  // Procedura obs�ugi przerwa�
{
	for(int i=0 ; i<12 ; i = i++)  // p�tla for i=0 ; wykonana 12 razy ; i++ inkrementacja i
	{
	LED_1_TOG;		// Zmiana stanu LED_1 na przeciwny
	_delay_ms(250);  	// Odczekanie 250 ms
	}


}
//***********************************************************************************Koniec main()



// Program z funkcj¹ obs³ugi przerwañ
// Wykonane przez : Tacica, Tiszbierek, Szczeka³a AiR ns semestr VI
//*************************************************************************************************************************************************************
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
//**************************************************************************************************************************************Definicje dla procesora
#define SW (1<<PD2)                    // Definicja pinu do którego, jest pod³¹czony przycisk
#define SW_DOWN !(PIND & SW)          // Definicja marko sprawdzaj¹ce czy na przycisku jest stan niski
#define SW_UP (PIND & SW)            // Definicja makro sprawdzaj¹cego czy na przycisku jest stan wysoki
#define LED_1 (1<<PA1)              // Definicja pinu do którego, jest pod³¹czona dioda czerwona
#define LED_1_TOG PORTA ^= LED_1   // Definicja zmiany stanu na dodzie czerwonej
#define LED_2 (1<<PA0)            // Definicja pinu do którego, jest pod³¹czona dioda niebieska
#define LED_2_TOG PORTA ^= LED_2 // Definicja zmiany stanu na diodzie niebieskiej
//******************************************************************************************************************************************Tworzenie zmiennych
bool x=0;                      // Tworzenie zmiennej typu bool x dla w³¹czania uk³adu, wartoœci pocz¹tkowo 0
unsigned char counter=0;      //  Tworzenie zmiennej typu unsigned char dla coounter, wartoœc pocz¹tkowa 0
int t2=0;                    //   Tworzenie zmennej typu int t2 dla timera t2, wartoœci¹ pocz¹tkow¹ 0
//***********************************************************************************************************************************Procedury obs³ugi przerwañ
ISR(TIMER1_COMPA_vect)                        // Procedura obs³ugi przerwañ kana³u A
{
   if ((x==1)&&(counter<5))                 // Instrukcja if z warunkami jej wykonania, kiedy  ( x==1 po naciœnieciu i zwolnieniu przyciska) & >>
       {                                   //    >> & ( counter<5 ) licznik zwiksza sie o 1 z zboczem opadaj¹cym LED_2 kiedy t1 konczy liczyc 3s
	   LED_2_TOG;                         // Zmiana stanu na dziodzie niebieskiej //
	   LED_1_TOG;                        // Zmiana stanu na dziodzie czerwonej
	   counter = TCNT0;                 // Nadpisywanie wartoœci counter wartoœci¹ z rejestru licznika/timera T0
	   TCNT1 = 0;                      // Zerowanie rejestru licznika/timera T1
       }
}

ISR(TIMER1_COMPB_vect)   // Procedura obs³ugi przerwañ kana³u B
{
	if ((x==1)&&(counter>=5)&&(t2<15625)) // instrukcja if z warunkami kiedy, (( x==1 & counter>=5)podobnie jak w wczeœniejszym przypadku ) & (t2<15632) >>
		{                                // >> wartoœc 15632 = 1 sekundzie. czêstotlioœc taktowania 16Mhz/preskalar 1024 = 15625 impulsów zegarowych na sekunde
		LED_1_TOG;                      // zmiana stanu na diodzie czerwonej
		t2=t2+TCNT1;                   // nadpisywanie wartosci t2 wartoscia z rejestru licznika T1, która przy kazdym przerwaniu wynosi 5208
		TCNT1 = 0;                    // zerowanie rejestru licznika T1
        }
}
//*******************************************************************************************************************************************Pêtla g³ówna main()
int main( void )
{
	DDRD &=~ SW;                 // Rejestr kierunku SW PD2 - wejœcie
	DDRA |= LED_1;              // Rejestr kierunku LED_1 dioda czerwona na wyjscie
    DDRA |= LED_2;             // Rejest kierunku LED_2 dioda niebieska na wyjscie
	PORTD |= SW;              // Podci¹gniecie pinu pod VCC (wewnêtrzyn rezystor)
	PORTA &=~ LED_1;         // Wy³¹cznie diody LED_1 dioda czerwona (pod³¹czona anod¹ do pinu)
    PORTA &=~ LED_2;        // Wy³¹czenie diody LED_2 dioda niebieska (pod³¹czona anod¹ do pinu)

//*****************************************************************************************************************Ustawienie rejestrów kontroli liczniaka T1
	TCCR1A = (1<<FOC1A) | (1<<FOC1B)  ; // Rejestr kontroli licznika/timera 1 TCCR1A, (1<<FOC1A | 1<,FOC1B) Wymyszenie trybu Output Compare dla kana³u A | B
	TCCR1B = (1<<CS10) | (1<<CS12) ;   // Rejestr kontroli licznika/timera 1 TCCR1B, (1<<CS10) | (1<<CS12) Ustawienie prestkalara na 1024

	OCR1A = 46875;                   // Rejestr OCR1A wartoœc, która stale jest porównywana z rejestrem licznika TCNT1, kiedy OCR1A==TCNT1 nastêpuje przerwanie
    OCR1B = 5208;                   // Rejestr OCR1B wartoœc, która stale jest porównywana z rejestrem licznika TCNT1, kiedy OCR1B==TCNT1 nastêpuje przerwanie

    TIMSK=(1<<OCIE1A) | (1<<OCIE1B);  // Rejestr maskowania przerwañ liczników, (1<<OCIE1A) | (1<<OCIE1B) odblokowanie przerwañ output compare A | B
//*****************************************************************************************************************Ustawienie rejestrów kontroli liczniaka T0
    TCCR0 =  (1<<CS01) | (1<<CS02); //  Rejestr kontroli licznika/ timera 0  (1<<CS01) | (1<<CS02) w³¹czenie zewnêtrzenego Ÿród³a zegara z pinu T0 >>
                                   // zliczaj¹ce impulsy na zbocze opadaj¹ce. Utworzone dla licznika zliczaj¹cego do 5
     sei();                       // W³¹czenie globalnych przerwañ

//*****************************************************************************************************************************************Pêtla g³ówna programu
while(1)
 {
    if((x==1)&&(counter==5)&&(t2>=15625))   // instrukcja wy³¹czaj¹ca program kiedy, x==1, licznik zliczy³ do 5 oraz t2= 15625(1s)
            {
	          PORTA &=~ LED_1;            // Wy³¹cznie diody LED_1 dioda czerwona (pod³¹czona anod¹ do pinu)
	          PORTA &=~ LED_2;           // Wy³¹czenie diody LED_2 dioda niebieska (pod³¹czona anod¹ do pinu)
	          x=0;                      // Wyzerowanie zmiennej x wpisaniem do niej wartoœci 0
            }

    if (SW_DOWN)                     // instrukcja z makro sprawdzaj¹ce czy na przycisku jest stan niski
       {
    	_delay_ms(100);            // odczekanie 100ms w celu upewnienia sie czy przycisk wciœniety by wyelminowac drgania styków
    	if (SW_UP)                // instrukcja z makro sprawdzaj¹ce czy na przycisku jest stan wysoki aby rozpocz¹c program po zwolnieniu przycisku
    	   {                     //*************************************************************************W instukcji ustawiane sa warunki pocz¹tkowe programu
	     x=1;                   //  Nadpisanie zmiennej x wartoœci 1 niezbêdn¹ do w³acznie programu
	     t2=0;                 // Wyzerowanie zmiennej t2 wpisaniem do niej wartosci 0
	     counter=0;           // Wyzerowanie zmiennej counter wpisaniem do niej wartosci 0
	     TCNT0 = 0;          // Wyzerowanie rejestru kontrolnego licznika/timera T0
	     TCNT1 = 0;         // Wyzerowanie rejestru kontrolnego licznika/timera T1
         PORTA &=~ LED_1;  // Wy³¹cznie diody LED_1 dioda czerwona
         PORTA |= LED_2;  // Wy³¹czenie diody LED_2 dioda niebieska
            }
       }
 }                     //************************************************************************************************************Koniec pêtli g³ównej programu
}                     //**************************************************************************************************************Koniec pêtli g³ównej main ()

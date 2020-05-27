// Program z funkcj� obs�ugi przerwa�
// Wykonane przez : Tacica, Tiszbierek, Szczeka�a AiR ns semestr VI
//*************************************************************************************************************************************************************
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
//**************************************************************************************************************************************Definicje dla procesora
#define SW (1<<PD2)                    // Definicja pinu do kt�rego, jest pod��czony przycisk
#define SW_DOWN !(PIND & SW)          // Definicja marko sprawdzaj�ce czy na przycisku jest stan niski
#define SW_UP (PIND & SW)            // Definicja makro sprawdzaj�cego czy na przycisku jest stan wysoki
#define LED_1 (1<<PA1)              // Definicja pinu do kt�rego, jest pod��czona dioda czerwona
#define LED_1_TOG PORTA ^= LED_1   // Definicja zmiany stanu na dodzie czerwonej
#define LED_2 (1<<PA0)            // Definicja pinu do kt�rego, jest pod��czona dioda niebieska
#define LED_2_TOG PORTA ^= LED_2 // Definicja zmiany stanu na diodzie niebieskiej
//******************************************************************************************************************************************Tworzenie zmiennych
bool x=0;                      // Tworzenie zmiennej typu bool x dla w��czania uk�adu, warto�ci pocz�tkowo 0
unsigned char counter=0;      //  Tworzenie zmiennej typu unsigned char dla coounter, warto�c pocz�tkowa 0
int t2=0;                    //   Tworzenie zmennej typu int t2 dla timera t2, warto�ci� pocz�tkow� 0
//***********************************************************************************************************************************Procedury obs�ugi przerwa�
ISR(TIMER1_COMPA_vect)                        // Procedura obs�ugi przerwa� kana�u A
{
   if ((x==1)&&(counter<5))                 // Instrukcja if z warunkami jej wykonania, kiedy  ( x==1 po naci�nieciu i zwolnieniu przyciska) & >>
       {                                   //    >> & ( counter<5 ) licznik zwiksza sie o 1 z zboczem opadaj�cym LED_2 kiedy t1 konczy liczyc 3s
	   LED_2_TOG;                         // Zmiana stanu na dziodzie niebieskiej //
	   LED_1_TOG;                        // Zmiana stanu na dziodzie czerwonej
	   counter = TCNT0;                 // Nadpisywanie warto�ci counter warto�ci� z rejestru licznika/timera T0
	   TCNT1 = 0;                      // Zerowanie rejestru licznika/timera T1
       }
}

ISR(TIMER1_COMPB_vect)   // Procedura obs�ugi przerwa� kana�u B
{
	if ((x==1)&&(counter>=5)&&(t2<15625)) // instrukcja if z warunkami kiedy, (( x==1 & counter>=5)podobnie jak w wcze�niejszym przypadku ) & (t2<15632) >>
		{                                // >> warto�c 15632 = 1 sekundzie. cz�stotlio�c taktowania 16Mhz/preskalar 1024 = 15625 impuls�w zegarowych na sekunde
		LED_1_TOG;                      // zmiana stanu na diodzie czerwonej
		t2=t2+TCNT1;                   // nadpisywanie wartosci t2 wartoscia z rejestru licznika T1, kt�ra przy kazdym przerwaniu wynosi 5208
		TCNT1 = 0;                    // zerowanie rejestru licznika T1
        }
}
//*******************************************************************************************************************************************P�tla g��wna main()
int main( void )
{
	DDRD &=~ SW;                 // Rejestr kierunku SW PD2 - wej�cie
	DDRA |= LED_1;              // Rejestr kierunku LED_1 dioda czerwona na wyjscie
    DDRA |= LED_2;             // Rejest kierunku LED_2 dioda niebieska na wyjscie
	PORTD |= SW;              // Podci�gniecie pinu pod VCC (wewn�trzyn rezystor)
	PORTA &=~ LED_1;         // Wy��cznie diody LED_1 dioda czerwona (pod��czona anod� do pinu)
    PORTA &=~ LED_2;        // Wy��czenie diody LED_2 dioda niebieska (pod��czona anod� do pinu)

//*****************************************************************************************************************Ustawienie rejestr�w kontroli liczniaka T1
	TCCR1A = (1<<FOC1A) | (1<<FOC1B)  ; // Rejestr kontroli licznika/timera 1 TCCR1A, (1<<FOC1A | 1<,FOC1B) Wymyszenie trybu Output Compare dla kana�u A | B
	TCCR1B = (1<<CS10) | (1<<CS12) ;   // Rejestr kontroli licznika/timera 1 TCCR1B, (1<<CS10) | (1<<CS12) Ustawienie prestkalara na 1024

	OCR1A = 46875;                   // Rejestr OCR1A warto�c, kt�ra stale jest por�wnywana z rejestrem licznika TCNT1, kiedy OCR1A==TCNT1 nast�puje przerwanie
    OCR1B = 5208;                   // Rejestr OCR1B warto�c, kt�ra stale jest por�wnywana z rejestrem licznika TCNT1, kiedy OCR1B==TCNT1 nast�puje przerwanie

    TIMSK=(1<<OCIE1A) | (1<<OCIE1B);  // Rejestr maskowania przerwa� licznik�w, (1<<OCIE1A) | (1<<OCIE1B) odblokowanie przerwa� output compare A | B
//*****************************************************************************************************************Ustawienie rejestr�w kontroli liczniaka T0
    TCCR0 =  (1<<CS01) | (1<<CS02); //  Rejestr kontroli licznika/ timera 0  (1<<CS01) | (1<<CS02) w��czenie zewn�trzenego �r�d�a zegara z pinu T0 >>
                                   // zliczaj�ce impulsy na zbocze opadaj�ce. Utworzone dla licznika zliczaj�cego do 5
     sei();                       // W��czenie globalnych przerwa�

//*****************************************************************************************************************************************P�tla g��wna programu
while(1)
 {
    if((x==1)&&(counter==5)&&(t2>=15625))   // instrukcja wy��czaj�ca program kiedy, x==1, licznik zliczy� do 5 oraz t2= 15625(1s)
            {
	          PORTA &=~ LED_1;            // Wy��cznie diody LED_1 dioda czerwona (pod��czona anod� do pinu)
	          PORTA &=~ LED_2;           // Wy��czenie diody LED_2 dioda niebieska (pod��czona anod� do pinu)
	          x=0;                      // Wyzerowanie zmiennej x wpisaniem do niej warto�ci 0
            }

    if (SW_DOWN)                     // instrukcja z makro sprawdzaj�ce czy na przycisku jest stan niski
       {
    	_delay_ms(100);            // odczekanie 100ms w celu upewnienia sie czy przycisk wci�niety by wyelminowac drgania styk�w
    	if (SW_UP)                // instrukcja z makro sprawdzaj�ce czy na przycisku jest stan wysoki aby rozpocz�c program po zwolnieniu przycisku
    	   {                     //*************************************************************************W instukcji ustawiane sa warunki pocz�tkowe programu
	     x=1;                   //  Nadpisanie zmiennej x warto�ci 1 niezb�dn� do w�acznie programu
	     t2=0;                 // Wyzerowanie zmiennej t2 wpisaniem do niej wartosci 0
	     counter=0;           // Wyzerowanie zmiennej counter wpisaniem do niej wartosci 0
	     TCNT0 = 0;          // Wyzerowanie rejestru kontrolnego licznika/timera T0
	     TCNT1 = 0;         // Wyzerowanie rejestru kontrolnego licznika/timera T1
         PORTA &=~ LED_1;  // Wy��cznie diody LED_1 dioda czerwona
         PORTA |= LED_2;  // Wy��czenie diody LED_2 dioda niebieska
            }
       }
 }                     //************************************************************************************************************Koniec p�tli g��wnej programu
}                     //**************************************************************************************************************Koniec p�tli g��wnej main ()

/* PROGRAM SYGNALIZATORA OPTYCZNEGO 06062020*/
/* wykorzystanie przerwania w MSP430*/
/* AiR_ns 2019/2020  */
/* Mateusz Dziembowski */

#include <msp430.h>
#include <intrinsics.h>
#include <stdio.h>

unsigned int b=0;             // zmienna inkrementowana w przerwaniu
unsigned int i;                        // zmienna pomocnicza
unsigned int j;                        // zmienna pomocnicza
unsigned int p=0;
unsigned int t_on=30000;               // czas stanu wysokiego w pulsach
unsigned int t_off=30000;              // czas stanu niskiego w pulsach

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;
    P1DIR |= BIT0;                  // ustawienie  P1.0 (czerwona-LED) jako wyjœcie
    P1DIR |= BIT6;                  // ustawienie  P1.6 (zielona-LED) jako wyjœcie
    P1OUT &= ~BIT6;                 // ustawienie  P1.6 na off
    P1OUT &= ~BIT0;                 // ustawienie  P1.0 na off
    P1DIR &= ~BIT3;                 // ustawienie  P1.3 (przycisk S2) jako wejœcie
    P1REN |=  BIT3;                 // wlaczenie rezystora pull-up na P1.3 (przycisk S2)

    P1OUT |= BIT3;                  // gdy przycisk jest w górze
    P1IES &= BIT3;                  // wybrane zbocze narastaj¹ce do wyzwolenia przerwania
    P1IE |= BIT3;                   // w³¹czenie przerwania

   _bis_SR_register(GIE);

while(1)
    {      /*-------warunki i akcje ---------------------------------------*/

          if (b==1)                   // pierwsze nacisniecie - dioda 2 on
          { P1OUT |=BIT6; }
          if (b==2)                   // drugie nacisniecie - dioda 1 on
          { P1OUT |=BIT0; }
          if (b==3)                  // trzecie nacisniecie - obie diody off i przypisanie b=0;
          { P1OUT &= ~BIT6;P1OUT &= ~BIT0; }
 i=0; j=0;
          if (b==4)                   // czwarte nacisniecie - pulsowanie diody 2 (zielona)
          {
              for(i;i<t_on;i++)       // czas wlaczenia
              {P1OUT |=BIT6; }          // czas wylaczenia
              for(j;j<t_off;j++)
              {P1OUT &= ~BIT6; }

          }
          if (b==5)                    // piate nacisniecie - pulsowanie diody 1 (czerwona)
          {
              for(i;i<t_on;i++)       // czas wlaczenia
              {P1OUT |=BIT0;}
              for(j;j<t_off;j++)       // czas wylaczenia
              {P1OUT &= ~BIT0;}

          }
          if (b==6)                    // szóste nacisniecie - pulsowanie obu diód synchronicznie
          {
              for(i;i<t_on;i++)       // czas wlaczenia
              {P1OUT |=BIT6;P1OUT |=BIT0;}
              for(j;j<t_off;j++)       // czas wylaczenia
              {P1OUT &= ~BIT6;P1OUT &= ~BIT0;}

          }
          if (b==7)                    // siódme nacisniecie - pulsowanie obu diód naprzemiennie
          {
              for(i;i<t_on;i++)       // czas wlaczenia
              {P1OUT |=BIT6;P1OUT &= ~BIT0;}
              for(j;j<t_off;j++)       // czas wylaczenia
              {P1OUT &= ~BIT6;P1OUT |=BIT0;}

          }
      }
}
#pragma vector=PORT1_VECTOR

__interrupt void Port_1(void)
{

    b=b+1;                          // inkrementacja zmiennej "b"
    if (b==8)                       // wylaczenie
    {   P1OUT &= ~BIT6; P1OUT &= ~BIT0; b=0;}
    P1IFG &= ~BIT3;                   // czyszczenie pamiêci przerwania
    __delay_cycles(30000);
}

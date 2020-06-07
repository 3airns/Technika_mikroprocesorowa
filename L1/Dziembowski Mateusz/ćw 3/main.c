/* PROGRAM SYGNALIZATORA OPTYCZNEGO 06062020*/
/* wykorzystanie timerów w MSP430*/
/* AiR_ns 2019/2020  */
/* Mateusz Dziembowski */

#include <msp430.h>
#include <intrinsics.h>
#include <stdio.h>

unsigned int b=0;                    // zmienna inkrementowana w przerwaniu
unsigned int i;                        // zmienna pomocnicza
unsigned int j;                        // zmienna pomocnicza
unsigned int p=0;
unsigned int t_on=30000;               // czas stanu wysokiego w pulsach
unsigned int t_off=30000;              // czas stanu niskiego w pulsach
int czasomierz_1=0;
int czasomierz_2=0;


int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;
    P1DIR |= BIT0;                  // ustawienie  P1.0 (czerwona-LED) jako wyjœcie
    P1DIR |= BIT6;                  // ustawienie  P1.6 (zielona-LED) jako wyjœcie
    P1OUT &= ~BIT6;                 // ustawienie  P1.6 na off
    P1OUT &= ~BIT0;                 // ustawienie  P1.0 na off
    P1DIR &= ~BIT3;                 // ustawienie  P1.3 (przycisk S2) jako wejœcie
    P1REN |=  BIT3;                 // wlaczenie rezystora pull-up na P1.3 (przycisk S2)
/* ustawienia dla przerwania na przycisku */
    P1OUT |= BIT3;                  // gdy przycisk jest w górze
    P1IES &= BIT3;                  // wybrane zbocze narastaj¹ce do wyzwolenia przerwania
    P1IE |= BIT3;                   // w³¹czenie przerwania na przycisku
/* ustawienia timerów */
    TA0CCTL0 |= CCIE;               // ustawienie przerwania timer_0
    TA0CTL |= TASSEL_2  + TACLR ;   // liczenie w góre
    TA1CCTL0 |= CCIE;               // ustawienie przerwania timer_0
    TA1CTL |= TASSEL_2  + TACLR ;   //liczenie w góre
    TA0CCR0 = 1;                    // rejestr wartoœci dla timer_0
    TA1CCR0 = 1;                    // rejestr wartoœci dla timer_1

   _bis_SR_register(GIE);          // wlaczenie przerwan CPU

while(1)

    { /*-------warunki i akcje poza timerami---------------------------------------*/

          if (b==1)                   // pierwsze nacisniecie - dioda 2 on
          { P1OUT |=BIT6;P1OUT &= ~BIT0;}
          if (b==2)                   // drugie nacisniecie - dioda 1 on
         { P1OUT |=BIT0; P1OUT &= ~BIT6;}
          if (b==3)                  // trzecie nacisniecie - obie diody off i przypisanie b=0;
          { P1OUT &= ~BIT6;P1OUT &= ~BIT0;}
          if (b>=4%b<=7)                   // od 4 do 7  naciœniêcia
          {TA0CTL |= MC_1; }        // wlaczenie timera 0


      }
}
#pragma vector=PORT1_VECTOR

__interrupt void Port_1(void)
{
    TA0CTL &= ~MC_3;                // wy³¹czenie timera_0
    TA1CTL &= ~MC_3;                // wy³¹czenie timera_1
    b=b+1;                          // inkrementacja zmiennej "b"
    if (b==8)                       // wylaczenie
    {   P1OUT &= ~BIT6; P1OUT &= ~BIT0; b=0;}
    P1IFG &= ~BIT3;                   // czyszczenie pamiêci przerwania
    __delay_cycles(10000);            // trochê opóŸnienia
}
/*-------TIMER_0---------------------------------------*/
#pragma vector = TIMER0_A0_VECTOR
__interrupt void CCR0_ISR(void)
{
    czasomierz_1++;
     /*-------warunki i akcje w pierwszym timerze---------------------*/
     if (b==4) {P1OUT |=BIT6;   P1OUT &= ~BIT0;}  // wszystkie wariancje
     if (b==5) {P1OUT &= ~BIT6; P1OUT |=BIT0;  }
     if (b==6) {P1OUT |=BIT6;P1OUT |=BIT0;}
     if (b==7){P1OUT |=BIT6;P1OUT &= ~BIT0;}
      if (czasomierz_1==t_on)
          {
          czasomierz_1=0;
            TA0CTL &= ~MC_3;   // wy³¹czenie wlasne timera_0
            TA1CTL |= MC_1;    // w³¹czenie timera_1
            __delay_cycles(4);
          }
 }
/*-------TIMER_1---------------------------------------*/
#pragma vector = TIMER1_A0_VECTOR       // Przerwanie po odliczeniu timer0
__interrupt void CCR1_ISR(void)
{
    czasomierz_2++;
     /*-------warunki i akcje w drugim timerze---------------------*/
     if (b==4){P1OUT &= ~BIT6;P1OUT &= ~BIT0;}        // wszystkie wariancje
     if (b==5) {P1OUT &= ~BIT6; P1OUT &= ~BIT0;  }
     if (b==6) {P1OUT &= ~BIT6;P1OUT &= ~BIT0;}
     if (b==7) {P1OUT &= ~BIT6;P1OUT |=BIT0;}
      if (czasomierz_2==t_off)
          {
          czasomierz_2=0;
            TA1CTL &= ~MC_3;// wy³¹czenie wlasne timera_1
            __delay_cycles(4);
          }
 }

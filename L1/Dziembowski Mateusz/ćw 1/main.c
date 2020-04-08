/* PROGRAM SYGNALIZATORA OPTYCZNEGO 04032020*/
/* AiR_ns 2019/2020  */
/* Mateusz Dziembowski */

#include <msp430.h>
#include <intrinsics.h>

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
      int a=0;                      // zmienna pomocnicza
      int b=0;                      // zmienna pomocnicza
      int i;                        // zmienna pomocnicza
      int j;                        // zmienna pomocnicza
      int czulosc=1000;             // ustalenie dlugosci nacisniecia przycisku do dzialania
      int t_on=30000;               // czas stanu wysokiego w pulsach
      int t_off=30000;              // czas stanu niskiego w pulsach


    while(1)
    {
        /* --------- petla do-while -----------------------------------
       pomocniczo do interpretacji nacisniecia i jego dlugosci na s2
       --------------------------------------------------------------*/
      do
      {
          if( (P1IN & BIT3 ) == 0)      // gdy S2 wcisniety gdy bit 3 == 0
            {  a = a+1;                 // inkrementacja zmiennej a gdy wcisniety s2
                if (a==czulosc)
                    {b=b+1;}           // zwiêkszanie wartoœci zmiennej b gdy inkrementowane a
                                       //      jest równe ustawionej czulosci
            }
          else
            {   a=0; }              // wyzerowanie a , gdy s2 nie jest wcisniety
       }
      while((P1IN & BIT3 ) == 0);

      /*-------warunki i akcje ---------------------------------------*/

        if (b==1)                   // pierwsze nacisniecie - dioda 2 on
        { P1OUT |=BIT6; }
        if (b==2)                   // drugie nacisniecie - dioda 1 on
        { P1OUT |=BIT0; }
        if (b==3)                  // trzecie nacisniecie - obie diody off i przypisanie b=0;
        { P1OUT &= ~BIT6;P1OUT &= ~BIT0; }
        i=0;
        j=0;
        if (b==4)                    // czwarte nacisniecie - pulsowanie diody 2 (zielona)
        {   /*for(i;i<t_on;i++)       // czas wlaczenia
            {P1OUT |=BIT6;}          // czas wylaczenia
            for(j;j<t_off;j++)
            {P1OUT &= ~BIT6;}*/

            P1OUT |=BIT6;
            _delay_cycles(300000);
            P1OUT &= ~BIT6;
            _delay_cycles(300000);


        }
        if (b==5)                    // piate nacisniecie - pulsowanie diody 1 (czerwona)
        {   for(i;i<t_on;i++)       // czas wlaczenia
            {P1OUT |=BIT0;}
            for(j;j<t_off;j++)       // czas wylaczenia
            {P1OUT &= ~BIT0;}
        }
        if (b==6)                    // szóste nacisniecie - pulsowanie obu diód synchronicznie
        {   for(i;i<t_on;i++)       // czas wlaczenia
            {P1OUT |=BIT6;P1OUT |=BIT0;}
            for(j;j<t_off;j++)       // czas wylaczenia
            {P1OUT &= ~BIT6;P1OUT &= ~BIT0;}
        }
        if (b==7)                    // siódme nacisniecie - pulsowanie obu diód naprzemiennie
        {   for(i;i<t_on;i++)       // czas wlaczenia
            {P1OUT |=BIT6;P1OUT &= ~BIT0;}
            for(j;j<t_off;j++)       // czas wylaczenia
            {P1OUT &= ~BIT6;P1OUT |=BIT0;}
        }
        if (b==8)                    // wylaczenie
        {  P1OUT &= ~BIT6; P1OUT &= ~BIT0; b=0;}

    }


	return 0;
}

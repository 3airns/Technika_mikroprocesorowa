/* PROGRAM KOMUNIKACJI UART 06062020*/
/* AiR_ns 2019/2020  */
/* Mateusz Dziembowski */

#include <msp430.h>

/* ----- wypiswanie liczby--------*/
void wyswietl_liczbe(unsigned int liczba)
{
    char buf[6];
    char* str = &buf[5];

    *str = '\0';

    do
    {
        unsigned long m = liczba;
        liczba /= 10;
        char c = (m - 10 * liczba) + '0';
        *--str = c;
    } while (liczba);

    wyswietl_tekst(str);
}
/* ----- wypiswanie tekstu --------*/
void wyswietl_tekst(char* text)
{
    unsigned int i = 0;
    while (text[i] != '\0')
    {
        while (!(IFG2 & UCA0TXIFG));      // sprawdzenie czy  TX nie jest zajęte
        UCA0TXBUF = text[i];            //
        i++;
    }
}


void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;
    if (CALBC1_1MHZ == 0xFF)              // sprawdzenie kalibracji
    {
        while (1);                       //
    }
    DCOCTL = 0;                         // nastawa DCO Clock Frequency Control
    BCSCTL1 = CALBC1_1MHZ;              // nastawa DCO na 1MHz Basic Clock System Control 1
    DCOCTL = CALDCO_1MHZ;               // DCO Clock Frequency Control

    P1SEL = BIT1 + BIT2;               // ustawienie Rx/Tx na  P1.1,P1.2
    P1SEL2 = BIT1 + BIT2;

    UCA0CTL1 |= UCSSEL_2;               // USCI A0 Control Register 1
    UCA0BR0 = 104;                      // USCI A0 Baud Rate 0
    UCA0BR1 = 0;                        // USCI A0 Baud Rate 1
    UCA0MCTL = UCBRS_1;                 // USCI A0 Modulation Control
    UCA0CTL1 &= ~UCSWRST;               // USCI A0 Control Register 1

    unsigned int c = 0;
    do
    {   c++;
        wyswietl_liczbe(c);                 //wypisz liczbe
        wyswietl_tekst("\t oczy mnie bola\r\n");  //wypisz dany tekst



    }while (c<=2);

}


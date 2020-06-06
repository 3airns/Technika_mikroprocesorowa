#include <msp430.h>

void print(char *text)
{
    unsigned int i = 0;                  // ZMIENNA

    while(text[i] != '\0')
    {
        while (!(IFG2&UCA0TXIFG));      // SPRAWDZ CZY TX PRACUJE
        UCA0TXBUF = text[i];            // TX -> ODEBRANE DANE
        i++;
    }
}

void printNumber(unsigned int num)
{
    char buf[6];
    char *str = &buf[5];

    *str = '\0';

    do
    {
        unsigned long m = num;           // ZMIENNA

        num /= 10;
        char c = (m - 10 * num) + '0';
        *--str = c;
    } while(num);

    print(str);
}

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;           // Stop Watchdog
    if (CALBC1_1MHZ==0xFF)              // SPRAWDZ CZY STAŁA KABLIBRACJI ZOSTAŁA SKASOWANA
    {
        while(1);                       // NIE ŁADUJ PROGRAMU
    }
    DCOCTL = 0;                         // NAJNIZSZE USTAWIENIE DCO
    BCSCTL1 = CALBC1_1MHZ;              // USTAW DCO NA 1 MHz
    DCOCTL = CALDCO_1MHZ;

    P1SEL = BIT1 + BIT2 ;               // USTAWIENIA FUNKCJI UART RX/TX
    P1SEL2 = BIT1 + BIT2;

    UCA0CTL1 |= UCSSEL_2;               // UART -> SMCLK
    UCA0BR0 = 104;                      // USTAWIENIA SZYKOSCI TRANSMISJI AN 1MHz 9600
    UCA0BR1 = 0;                        // USTAWIENIA SZYKOSCI TRANSMISJI AN 1MHz 9600
    UCA0MCTL = UCBRS_1;                 // USTAWIENIA MODULACJI 1MHz 9600
    UCA0CTL1 &= ~UCSWRST;               // INICJALIZACJA MODUŁU UART

    unsigned int count = 12;             // ZMIENNA
   // unsigned int licz = 0;              // ZMIENNA

    while(1)
    {



        if (count >= 30)              // ZEROWANIE LICZNIKA GDY WARTOSC LICZBOWA OSIAGNIE 30
           {
            print("wartosc powyzej 30 zerowanie licznika");
            count = 0;
           }
        else
            count =  count +1;
            printNumber(count);
            print("\r\n");


        __delay_cycles(500000);
    }
}

#include <msp430.h>

void print(char* text)
{
    unsigned int i = 0;
    while (text[i] != '\0')
    {
        while (!(IFG2 & UCA0TXIFG));      // sprawdzenie czy if TX is ongoing
        UCA0TXBUF = text[i];            // TX -> Received Char + 1
        i++;
    }
}

void printNumber(unsigned int num)
{
    char buf[6];
    char* str = &buf[5];

    *str = '\0';

    do
    {
        unsigned long m = num;
        num /= 10;
        char c = (m - 10 * num) + '0';
        *--str = c;
    } while (num);

    print(str);
}

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;           // Stop watchdog
    if (CALBC1_1MHZ == 0xFF)              // Sprawd�, czy sta�a kalibracji zosta�a skasowana
    {
        while (1);                       //
    }
    DCOCTL = 0;                         // Select lowest DCO settings
    BCSCTL1 = CALBC1_1MHZ;              // Set DCO to 1 MHz
    DCOCTL = CALDCO_1MHZ;

    P1SEL = BIT1 + BIT2;               // Select UART RX/TX function on P1.1,P1.2
    P1SEL2 = BIT1 + BIT2;

    UCA0CTL1 |= UCSSEL_2;               // UART Clock -> SMCLK
    UCA0BR0 = 104;                      // Baud Rate Setting for 1MHz 9600
    UCA0BR1 = 0;                        // Baud Rate Setting for 1MHz 9600
    UCA0MCTL = UCBRS_1;                 // Modulation Setting for 1MHz 9600
    UCA0CTL1 &= ~UCSWRST;               // Initialize UART Module

    unsigned int count = 0;
    while (1)
    {
        printNumber(count);				//wypisz liczb�
        print("\r\n");					//wypisz pust� lini�
        print("\tTesty rakiety PK DI 2001 powiodly sie!\r\n"); //wypisz dany tekst 
        count++;						//inkrementuj liczb�
        __delay_cycles(10000);
    }
}

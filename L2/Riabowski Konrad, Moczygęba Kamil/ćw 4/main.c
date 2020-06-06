#include <msp430.h>

void print(char *text)
{
    unsigned int i = 0;
    while(text[i] != '\0')
    {
    while (!(IFG2&UCA0TXIFG));     // Check if TX is ongoing
    UCA0TXBUF = text[i];            // TX -> Received Char + 1
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
        unsigned long m = num;
        num /= 10;
        char c = (m - 10 * num) + '0';
        *--str = c;
    } while(num);
    print(str)
}

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;          // Stop Watchdog
    if (CALBC1_1MHZ==0xFF)              // Kalibracja
    {while(1)}
        DCOCTL = 0;
        BCSCTL1 = CALBC1_1MHZ;
        DCOCTL = CALDCO_1MHZ;
        P1SEL = BIT1 + BIT2 ;
        P1SEL2 = BIT1 + BIT2;
        UCA0CTL1 |= UCSSEL_2;               // UART Clock -> SMCLK
        UCA0BR0 = 104;                      // Baud Rate Setting for 1MHz 9600
        UCA0BR1 = 0;                        // Baud Rate Setting for 1MHz 9600
        UCA0MCTL = UCBRS_1;                 // Modulation Setting for 1MHz 9600
        UCA0CTL1 &= ~UCSWRST;               // Initialize UART Module
        IE2 |= UCA0RXIE;
    __bis_SR_register(LPM0_bits + GIE)
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    while (!(IFG2&UCA0TXIFG));
    UCA0TXBUF = UCA0RXBUF;
    printNumber(UCA0TXBUF);                   // Wypisz liczbe
    print("\tOK\tTekst do wyswietlenia\r\n")   //Wyswietl tekst
}

#include <msp430.h>
// WYSYLANIE ZNAKU
void print(char *text){
    unsigned int i = 0;
    while(text[i] != '\0')
    {
    	// DOPÓKI TX JEST GOTOWY
        while (!(IFG2&UCA0TXIFG));
        // ZWRACAJ ZNAKI
        UCA0TXBUF = text[i];
        i++;
    }
}
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;           // Stop Watchdog
#define RED BIT6                        // Red LED P1.6
#define GREEN BIT0                      // Green LED P1.0
    P1DIR |= RED;                       // USTAWIENIE DIODY CZERWONEJ
    P1OUT &= ~RED;                      // WYLACZENIE DIODY CZERWONEJ
    P1DIR |= GREEN;                     // USTAWIENIE DIODY ZIELONEJ
    P1OUT &= ~GREEN;                    // WYLACZENIE DIODY ZIELONEJ
    // KONFIGURACJA UART:
    if (CALBC1_1MHZ==0xFF)              // SPRAWDZAJ CZY KALIBRACJA JEST STALE USUNIETA
    {
        while(1);
    }
    DCOCTL = 0;                         // WYBIERZ NAJMNIEJSZA WARTOSC
    BCSCTL1 = CALBC1_1MHZ;              // USTAW WARTOSC 1MHZ
    DCOCTL = CALDCO_1MHZ;
    // USTAWIENIE PINOW RX I TX:
    P1SEL = BIT1 + BIT2 ;
    P1SEL2 = BIT1 + BIT2;
    // TRANSMISJA UART:
    UCA0CTL1 |= UCSSEL_2;               // UART Clock -> SMCLK
    UCA0BR0 = 104;                      // Baud Rate Setting for 1MHz 9600
    UCA0BR1 = 0;                        // Baud Rate Setting for 1MHz 9600
    UCA0MCTL = UCBRS_1;                 // Modulation Setting for 1MHz 9600
    UCA0CTL1 &= ~UCSWRST;               // Initialize UART Module
    //USTAWIENIA PRZERWANIA:
    IE2 |= UCA0RXIE;
    __bis_SR_register(LPM0_bits + GIE); // Enter LPM0, Enable Interrupt
}
// WCISNIECIE PRZYCISKU SPOWODUJE PRZERWANIE
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
	//ODBIERAJ ZNAKI DOPOKI:
    while (!(IFG2&UCA0TXIFG));
            UCA0TXBUF = UCA0RXBUF;
            if (UCA0TXBUF=='d' || UCA0TXBUF=='D'){
            print("\t- W grupie jest osoba, której imię zaczyna się na literę D to Dominik\r\n");
            print("\t Ta osoba podlega ocenie.\r\n");
            P1OUT |=GREEN;
            P1OUT &= ~RED;
            }
            else if (UCA0TXBUF=='s' || UCA0TXBUF=='S'){
            print("\t- W grupie jest osoba, której imię zaczyna się na literę S to Sebastian\r\n");
            print("\t Ta osoba podlega ocenie.\r\n");
            P1OUT |=GREEN;
            P1OUT &= ~RED;
            }
            else if (UCA0TXBUF=='k' || UCA0TXBUF=='K'){
            print("\t- W grupie jest osoba, której imię zaczyna się na literę K to Kamil\r\n");
            print("\t Ta osoba podlega ocenie.\r\n");
            P1OUT |=GREEN;
            P1OUT &= ~RED;
            }
            else{
            print("\t Nie ma w grupie osoby , której imię zaczyna się na literę\r\n");
            print("\t- To nie dla tej osoby ocena\r\n");
            P1OUT &= ~GREEN;
            P1OUT |=RED;
}
}

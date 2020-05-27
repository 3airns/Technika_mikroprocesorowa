#include <msp430.h>
#define RED BIT6                        // Red LED -> P1.6
#define GRN BIT0                        // Green LED -> P1.0
#define SW BIT3                         // Switch -> P1.3

int flaga = 1;                          // ustawienie zmiennej flaga


void print(char *text){                 // funkcja wysy³aj¹ca string na wyjœcie Tx znak po znaku
    unsigned int i = 0;
    while(text[i] != '\0')
    {
        while (!(IFG2&UCA0TXIFG));      // Check if TX is ongoing
        UCA0TXBUF = text[i];            // TX -> Received Char + 1
        i++;
    }
}

void printNumber(unsigned int num){     // funkcja pobieraj¹ca string i zamieniaj¹ca go na int
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

    print(str);
}

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;           // Stop Watchdog

    //Konfiguracja I/O
    P1DIR |= RED;                       // Set LED pin -> Output
    P1OUT &= ~RED;                      // Wy³¹czenie diody zielonej
    P1DIR |= GRN;                       // Set LED pin -> Output
    P1OUT &= ~GRN;                      // wy³¹czenie diody czerwonej
    P1DIR &= ~SW;                       // Ustawienie wejœcia na pinie P1.3 (SW2)
    P1REN |= SW;                        // w³¹czenie rezystora podci¹gaj¹cego
    P1OUT |= SW;                        // ustawienie rezystora podci¹gaj¹cego jako pull-up

    // Konfiguracja zegara dla UART
    if (CALBC1_1MHZ==0xFF)              // Check if calibration constant erased
    {
        while(1);                       // do not load program
    }
    DCOCTL = 0;                         // Select lowest DCO settings
    BCSCTL1 = CALBC1_1MHZ;              // Set DCO to 1 MHz
    DCOCTL = CALDCO_1MHZ;
    // Ustawienie pinów P1.1 i P1.2 jako Rx i Tx
    P1SEL = BIT1 + BIT2 ;               // Select UART RX/TX function on P1.1,P1.2
    P1SEL2 = BIT1 + BIT2;

    // Konfiguracja parametrów transmisji UART
    UCA0CTL1 |= UCSSEL_2;               // UART Clock -> SMCLK
    UCA0BR0 = 104;                      // Baud Rate Setting for 1MHz 9600
    UCA0BR1 = 0;                        // Baud Rate Setting for 1MHz 9600
    UCA0MCTL = UCBRS_1;                 // Modulation Setting for 1MHz 9600
    UCA0CTL1 &= ~UCSWRST;               // Initialize UART Module
    // przerwanie uruchamiane poprzez odebranie sygna³u na pinie Rx
    IE2 |= UCA0RXIE;                    // Enable RX interrupt

    //konfiguracja przerwania z przycisku
    P1IES &= ~SW;                       // Wybranie zbocza narastajacego na wyzwalanie przerwania (puszczenie przycisku)
    P1IE |= SW;                         // W³¹czenie przerwania na przycisku

    __bis_SR_register(LPM0_bits + GIE); // Enter LPM0, Enable Interrupt
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void){          // Przerwanie wykonane przez naciœniêcie przycisku
    if (flaga==0){
        flaga=1;                        // zmienna flaga odpowiada za wybranie trybu pracy
    }
    else{
        flaga=0;
    }
    P1IFG &= ~SW;                       // Czyszczenie flagi przerwania na Przycisku
}

#pragma vector=USCIAB0RX_VECTOR         // Przerwanie uruchamiane po odebraniu znaku
__interrupt void USCI0RX_ISR(void)
{
    while (!(IFG2&UCA0TXIFG));          // Uruchomienie odbieranie znaków
        if (flaga==1){
            UCA0TXBUF = UCA0RXBUF;      // echo - wypisanie do terminala odebranego znaku
            print("\t- wprowadzony znak\r\n");  //wunkcja wypisania stringa na terminal
            printNumber(UCA0RXBUF);     // funkcja zamiany wprowadzonego stringa jako int i wypisanie go na terminal
            print("\t- kod w ASCII\r\n");
        }
        else{
            UCA0TXBUF = UCA0RXBUF;      // echo - wypisanie do terminala odebranego znaku
            P1OUT ^= GRN;               // zmiana stanu na przeciwny diody zielonej
            P1OUT ^= RED;               // zmiana stanu na przeciwny diody czerwonej
            print("\r\n");
        }
}

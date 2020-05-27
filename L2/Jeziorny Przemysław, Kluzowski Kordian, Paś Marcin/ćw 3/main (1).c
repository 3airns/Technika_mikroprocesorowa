#include "msp430g2553.h"
int a;                                  // Zdefiniowanie zmiennej a
#define SW      BIT3                    // Zdefiniowanie przycisku
#define GREEN   BIT6                    // Zdefiniowanie zielonej diody
#define RED     BIT0                    // Zdefiniowanie czerwonej diody
void main(void){
    WDTCTL = WDTPW + WDTHOLD;           // Stop watchdog timer
    P1DIR |= RED;                       // Wybór kierunku rejestru(wyjscie)
    P1DIR &= ~SW;                       // Wybór kierunku rejestru(wyjscie)
    P1DIR |= GREEN;                     // Wybór kierunku rejestru(wyjscie)
    P1OUT &= ~RED;                      // Stan wyjsciowy diody
    P1OUT &= ~GREEN;                    // Stan wyjsciowy diody
    P1OUT |= SW;                        // Stan wyjsciowy przycisku
    P1REN |= SW;                        // Enable Resistor for SW pin
    TA0CCTL0 |= CCIE;                   // Przerwanie timer0
    TA0CTL |= TASSEL_1  + TACLR ;       // Ustawienie timer0
    TA1CCTL0 |= CCIE;                   // Przerwanie timer1
    TA1CTL |= TASSEL_1  + TACLR ;       // Przerwanie timer1
    TA0CCR0 = 10000;                    // Ustawienie timera0 na 2 sekundy
    TA1CCR0 = 25000;                    // Ustawienie timera1 na 5 sekund
    P1IES &= ~SW;                       // Ustawienie zbocza
    P1IE |= SW;                         // Przerwanie przycisku
    __bis_SR_register(LPM0_bits + GIE);}// Enter LPM4 and Enable CPU Interrupt
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void){          // Przerwanie przyciskiem
    a=0;                                // Wyzerowanie a
    TACTL |= MC_1;                      // Przerwanie timer0
    P1IFG &= ~SW;}                      // Flaga przerwania
#pragma vector = TIMER0_A0_VECTOR       // Przerwanie po odliczeniu timer0
__interrupt void CCR0_ISR(void){
    if (a==0){P1OUT |= RED;}            // Za³¹czenie diody
    else if (a<4){P1OUT ^= GREEN;       // Zmiana stanu diody
        P1OUT ^= RED;}                  // Zmiana stanu diody
    else{a=0;                           // Wyzerowanie a
        TA0CTL &= ~MC_3;                // Przerwanie timer0
        TA1CTL |= MC_1;                 // W³¹czenie timer1
        P1OUT &= ~GREEN;                // Wy³¹czenie diody
        P1OUT &= ~RED;}                 // Wy³¹czenie diody
    a++;}
#pragma vector = TIMER1_A0_VECTOR       // Przerwanie po odliczeniu timer1
__interrupt void CCR1_ISR(void){
    while(a<=4){
        P1OUT ^= GREEN;                 // Zmiana stanu diody
        __delay_cycles(200000);
       a++;}
    TA1CTL &= ~MC_3;}                   // Wy³¹czenie timer1


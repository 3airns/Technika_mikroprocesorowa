
#include "msp430g2553.h"

#define RED_LED BIT0
#define GREEN_LED BIT6
#define TXD BIT2
#define RXD BIT1

unsigned int i; 

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;
     
  //Konfiguracja zegara
        
  if (CALBC1_1MHZ==0xFF)   // Jeżeli stała kalibracji zostanie skasowana
     {    
        while(1);          
     } 

   DCOCTL  = 0;             // Wybranie najniższych ustawień
   BCSCTL1 = CALBC1_1MHZ;   // Ustawianie zakresu
   DCOCTL  = CALDCO_1MHZ;   // Ustawianie kroku i modulacji DCO 
      
  //Konfiguracja LED
      
   P1DIR  |=  RED_LED + GREEN_LED;  // P1.0 and P1.6 output
   P1OUT  &= ~RED_LED + GREEN_LED;  // P1.0 and P1.6 = 0
      
  //Konfiguracja pinów P1.1 (odbiornika) i P1.2 (nadajnika)//
      
   P1SEL  |=  RXD + TXD;  
   P1SEL2 |=  RXD + TXD;  
      
  //Konfiguracja UART
      
   UCA0CTL1 |=  UCSSEL_2 + UCSWRST;  // USCI Clock = SMCLK,USCI_A0 disabled
   UCA0BR0   =  104;                 
   UCA0BR1   =  0;                   
   UCA0MCTL  =  UCBRS_1;             // Modulation value = 1 from datasheet
   UCA0STAT |=  UCLISTEN;            // włączanie trybu pętli zwrotnej
   UCA0CTL1 &= ~UCSWRST;             // Inicjowanie automatu stanów
      
  //Konfiguracja przerwań
      
   IE2 |= UCA0TXIE;                  
   IE2 |= UCA0RXIE;                 

      
   UCA0TXBUF = i;                  
      
   __bis_SR_register(CPUOFF + GIE);
}


  // Przerwania nadajnika i odbiornika


  #pragma vector = USCIAB0TX_VECTOR
  __interrupt void TransmitInterrupt(void)
  {
    P1OUT  ^= RED_LED;
	i++;
  }

  #pragma vector = USCIAB0RX_VECTOR
  __interrupt void ReceiveInterrupt(void)
  {
	P1OUT  ^= GREEN_LED;//light up P1.0 Led on Tx 

    IFG2 &= ~UCA0RXIFG; // Clear RX flag 
  }
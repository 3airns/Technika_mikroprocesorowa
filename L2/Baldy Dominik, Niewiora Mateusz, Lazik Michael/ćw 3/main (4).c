#include <msp430.h>

#define LEDG	BIT6						// Green LED -> P1.6
#define LEDR	BIT0						// RED LED -> P1.0
#define SW	BIT3					// Switch -> P1.3
int licznik;

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;			// Stop watchdog timer

    P1DIR |= LEDG+LEDR;						// Set LED pin -> Output
	P1OUT &=~LEDG;						// Turn OFF GREEN
	P1OUT &= LEDR;						// Turn ON RED
	P1DIR &= ~SW;					// Set SW pin -> Input
    P1REN |= SW;					// Enable Resistor for SW pin
    P1OUT |= SW;					// Select Pull Up for SW pin

    TACCR0 = 3000;						// Set Timer Timeout Value
    TACCTL0 |= CCIE;					// Enable Overflow Interrupt

    TACTL |= MC_1 + TASSEL_1 + TACLR ;	// Set Mode -> Up Count, Clock -> ACLK, Clear Timer

   	__bis_SR_register(LPM3_bits + GIE);	// Goto LPM3 (Only ACLK active), Enable CPU Interrupt
}
if(!(P1IN & SW))
	{
	while(1)
	{
#pragma vector = TIMER0_A0_VECTOR		// CCR0 Interrupt Vector
__interrupt void CCR0_ISR(void)
{
	
	
	P1OUT ^= LEDG+LEDR;						// Toggle LED
	
	}
	licznik++;
	if{licznik==10}
	break;
	}
	P1OUT ^= LEDR
	__delay_cycles(250);
	P1OUT ^= LEDR
	__delay_cycles(250);
	P1OUT ^= LEDR
	__delay_cycles(250);
	P1OUT ^= LEDR
	__delay_cycles(250);
	licznik=0;
	break;
	}
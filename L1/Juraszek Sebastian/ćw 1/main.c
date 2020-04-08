#include <msp430.h>

#define SW  BIT3                // Przycisk S1 -> P1.3
#define RED BIT0                // Czerwona LED -> P1.0
#define GREEN   BIT6            // Zielona LED -> P1.0
#define Time  500
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    P1DIR |= RED+GREEN;         // Set LED pin -> wyjscie
    P1DIR &= ~SW;               // Set SW pin -> wejscie
    P1REN |= SW;                // Enable Resistor for SW pin
    P1OUT |= SW;
    P1OUT &= ~GREEN;
    P1OUT &= ~RED;              // Select Pull Up for SW pin
    unsigned int flag = 0;
    while(1)
    {
        if(!(P1IN & SW))            // If SW is Pressed
        {

      flag ++;
      __delay_cycles(22000);
        }
        switch (flag)
        case 1:
        {
                   {
                       P1OUT &=RED;
                       P1OUT &=~GREEN;
                       __delay_cycles(Time);
                       break;
                   };
          
            case 2:
                       {
                           P1OUT &=~RED;
                           P1OUT &=GREEN;
                           __delay_cycles(Time);
                           break;
                       };
            case 3:
                       {
                           P1OUT &=RED;
                           P1OUT &=GREEN;
                           __delay_cycles(Time);
                           break;
                       };
            case 4:
                       {
                           P1OUT &=~RED;
                           P1OUT &=~GREEN;
                           __delay_cycles(Time);
                           break;
                       };
            case 5:
                       {
                           P1OUT &=RED;
                           P1OUT &=GREEN;
                           __delay_cycles(Time);
                           break;
                       };
            case 6:
                       {
                           P1OUT &=~RED;
                           P1OUT &=~GREEN;
                           __delay_cycles(Time);
                           break;
                       };          
            default :
            {
                flag=0;
                __delay_cycles(Time);
            }
        }
    }
}

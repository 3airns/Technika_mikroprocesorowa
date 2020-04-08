#include <msp430.h>
#define SW BIT3
                                                                                                    
void main(void) {
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
 
    P1DIR |= (BIT0+BIT6);
 
       P1DIR &= ~SW; // Set SW pin -> Input
       P1REN |= SW; // Enable Resistor for SW pin
       P1OUT |= SW; // Select Pull Up for SW pin
       volatile unsigned int flag = 0;
       while(1)
    {
    if(!(P1IN & SW)) // If SW is Pressed
        {
        __delay_cycles(20000); // Wait 20ms
        if(!(P1IN & SW)) // Check if SW is still pressed
        { // Ignores presses shorter than 20ms
        if(!(P1IN & SW))
        {
        flag = flag+1;// Change flag value
        if(flag==3){
 
        flag=0;
        }
        while(!(P1IN & SW));
        }
        }
        }
    if(flag==0) // If SW is NOT pressed
    {
    volatile unsigned long i;
    P1OUT &= ~BIT6;
 
 
        P1OUT |= BIT0; //Red LED -> ON
 
        for(i = 0; i<10000; i++)
        {
        if(!(P1IN & SW))
        break; //delay
        }
 
        P1OUT &= ~BIT0; //Red LED -> OFF
 
 
        for(i = 0; i<10000; i++)
        {
                if(!(P1IN & SW))
                break; //delay
                } //delay
    }
        if(flag==1)
        {
 
    volatile unsigned long i;
 
    P1OUT &= ~BIT6; //Green LED -> OFF
 
 
    for(i = 0; i<10000; i++)
    {
            if(!(P1IN & SW))
            break; //delay
            } //delay
 
 
    P1OUT |= BIT6; //Green LED -> ON
 
    for(i = 0; i<10000; i++)
    {
            if(!(P1IN & SW))
            break; //delay
            }//delay
        }
        if(flag==2)
                {
 
            volatile unsigned long i;
 
            P1OUT &= ~BIT6; //Green LED -> OFF
            P1OUT |= BIT0;
 
            for(i = 0; i<10000; i++)
            {
                    if(!(P1IN & SW))
                    break; //delay
                    } //delay
 
            P1OUT &= ~BIT0;
            P1OUT |= BIT6; //Green LED -> ON
 
            for(i = 0; i<10000; i++)
            {
                    if(!(P1IN & SW))
                    break; //delay
                    }//delay
                }
    }
}
    #include <msp430.h>

    #define SW  BIT3
    #define RED BIT0
    #define GREEN   BIT6
    #define Time  1000
    unsigned int flag = 0;

    int case_flag();
    int check();


    void main(void)
    {
        WDTCTL = WDTPW | WDTHOLD;

        P1DIR |= RED+GREEN;
        P1DIR &= ~SW;
        P1REN |= SW;
        P1OUT |= SW;
        P1OUT &= ~GREEN;
        P1OUT &= ~RED;




        while(1)
        {
            case_flag(check());
        }
    }

    int check ()
       {
           if(!(P1IN & SW))
           {
                 flag ++;
                 __delay_cycles(320000);
           }
           return flag;
       }

    int case_flag()
    {
         switch (flag)
            {
                 case 1:
                {
                    P1OUT |=RED;
                    P1OUT &=~GREEN;
                    __delay_cycles(Time);
                    break;
                };
                case 2:
                {
                    P1OUT &=~RED;
                    P1OUT |=GREEN;
                    __delay_cycles(Time);
                    break;
                };
                case 3:
                {
                    P1OUT |=RED;
                    P1OUT |=GREEN;
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
                default :
                {
                    flag=0;
                    __delay_cycles(Time);
                }
            }

    }

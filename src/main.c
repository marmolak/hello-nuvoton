#include "N76E003.h"

/*
    Basic blink program which can be used as a hello world for nuvoton N76E003 dev board.
    Some header files has been ported to sdcc from KEIL or similar strange C compiler.
    Die KEIL! Die!
*/

#define DELAY_LED_MS 2000

static void setup()
{
    P15_PushPull_Mode;
}

void main(void)
{
    setup();

    for (;;)
    {
        set_P15;
        Timer0_Delay1ms(DELAY_LED_MS);
        clr_P15;
        Timer0_Delay1ms(DELAY_LED_MS);   
    }
}
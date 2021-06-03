#include "N76E003.h"

#include "SPIhw/SPIhw.h"

/*
    Basic blink program which can be used as a hello world for nuvoton N76E003 dev board.
    Some header files has been ported to sdcc from KEIL or similar strange C compiler.
    Die KEIL! Die!
*/

#define DELAY_LED_MS 2000

// Disable POR - Power on Reset
unsigned char _sdcc_external_startup(void)  
{
    __asm  
    mov	0xC7, #0xAA  
    mov	0xC7, #0x55  
    mov	0xFD, #0x5A  
    mov	0xC7, #0xAA  
    mov	0xC7, #0x55  
    mov	0xFD, #0xA5  
    __endasm;

    return 0;  
}


static void setup()
{
    // for logic analyzer
    P12_PushPull_Mode;

    InitialUART0_Timer1(9600);

    spihw_setup();

}

void main(void)
{
    setup();

    static const UINT8 *const hello = "Hello world!\n";
    for (const UINT8 *i = &hello[0]; *i != '\0'; ++i)
    {
        Send_Data_To_UART0(*i);
    }

    for (;;)
    {
        spihw_demo();

        set_P12;
        Timer0_Delay1ms(DELAY_LED_MS);
        clr_P12;
        Timer0_Delay1ms(DELAY_LED_MS);

    }
}
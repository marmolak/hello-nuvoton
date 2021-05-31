#include "SPIhw.h"

#include "N76E003.h"
#include "LCD/LCD_def.h"

// Move to external RAM - digits and chars
__xdata static const unsigned char char_table[] = {
    0x7e, 0x30, 0x6d, 0x79, 0x33, 0x5b, 0x5f, 0x70,
    0x7f, 0x7b, 0x77, 0x1f, 0x0d, 0x3d, 0x4f, 0x47,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x80, 0x00,
    0x7e, 0x30, 0x6d, 0x79, 0x33, 0x5b, 0x5f, 0x70,
    0x7f, 0x7b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x77, 0x1f, 0x0d, 0x3d, 0x4f, 0x47, 0x00,
    0x37, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00,
    0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08,
    0x00, 0x77, 0x1f, 0x0d, 0x3d, 0x4f, 0x47, 0x00,
    0x37, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x15, 0x1d,
    0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

void spihw_setup(void)
{
    // SS/CS 
    P15_Quasi_Mode;
    // release P15 - manually driving
    set_DISMODF;
    clr_SSOE;
    set_P15;

    // MOSI
    P00_Quasi_Mode;

    // SPCLK
    P10_Quasi_Mode;

    set_MSTR;

    // 1MB/s
    set_SPR1;
    set_SPR0;

    clr_LSBFE;

    clr_CPOL;
    clr_CPHA;

    // Enable SPI
    set_SPIEN;

    // Seems like kind of workaround, otherwise some strange things happen with display.
    // Strange things: only last digit is initialized with high brightness.
    // 100 ms seems to work without any issue.
    Timer0_Delay1ms(100);

    spi_transfer(OP_DISPLAYTEST, 0);

    spi_transfer(OP_SCANLIMIT, 7);

    spi_transfer(OP_DECODEMODE, 0);

    spi_transfer(OP_INTENSITY, 3);

    // cleanup
    for (unsigned char p = 1; p < 9; ++p)
    {
        spi_transfer(p, 0x00);
    }

    // start in shutdown mode
    spi_transfer(OP_SHUTDOWN, 0);
}

void spihw_demo(void)
{
    spi_transfer(OP_SHUTDOWN, 1);

    for (unsigned char i = 0; i < 10; ++i)
    {
        const unsigned char c = char_table[i];

        for (unsigned char p = 1; p < 9; ++p)
        {
            spi_transfer(p, c);
        }

        // spi_transfer(OP_SHUTDOWN, 1);
        Timer0_Delay1ms(1000);
    }

    Timer0_Delay1ms(1000);

    for (unsigned char p = 8; p > 0; --p)
    {
        const unsigned char c = char_table[p+1];
        spi_transfer(p, c);

        Timer0_Delay1ms(1000);
    }
}

static inline void _spi_send_byte(const volatile unsigned char byte)
{
    clr_SPIF;
    SPDR = byte;
    while((SPSR & SET_BIT7) != SET_BIT7);
    clr_SPIF;
}

static void spi_transfer(const volatile unsigned char opcode, const volatile unsigned char data)
{
    // 16-bit packet so we drive CS manually
    clr_P15;

    _spi_send_byte(opcode);
    _spi_send_byte(data);

    set_P15;
}
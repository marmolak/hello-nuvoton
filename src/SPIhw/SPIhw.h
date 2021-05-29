#ifndef SPIHW_H
#define SPIHW_H

// public
void spihw_setup(void);
void spihw_demo(void);

// private
static void spi_transfer(const unsigned char opcode, const unsigned char data);

#endif
// See LICENSE for license details.

#ifndef _UART_H
#define _UART_H

#include <stdint.h>

void uart_putc(uint8_t ch);
int uart_getc(void);
void uart_init(void);

#endif /* _UART_H */

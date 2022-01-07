// See LICENSE for license details.

#ifndef _UART_H
#define _UART_H

#include <stdint.h>

int uart_putc(char ch);
void uart_puts(char *s);
int uart_getc(void);
void uart_init(void);

#endif /* _UART_H */

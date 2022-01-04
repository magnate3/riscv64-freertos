// See LICENSE for license details.

#ifndef _PLATFORM_H
#define _PLATFORM_H

#include "const.h"
#include "clint.h"
#include "uart.h"

// Memory map
#if defined(__QEMU__)
# define CLINT_CTRL_ADDR _AC(0x02000000,UL)
# define UART0_CTRL_ADDR _AC(0x10000000,UL)
#elif defined(__NEMU__)
# define CLINT_CTRL_ADDR _AC(0xa2000000,UL)
# define UART0_CTRL_ADDR _AC(0xa10003f8,UL)
#elif defined(__NUTSHELL__)
# define CLINT_CTRL_ADDR _AC(0x40700000,UL)
# define UART0_CTRL_ADDR _AC(0x40600000,UL)
#endif

unsigned long get_cpu_freq(void);
unsigned long get_timer_freq(void);
uint64_t get_timer_value(void);

#endif /* _PLATFORM_H */

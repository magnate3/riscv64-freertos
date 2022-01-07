#ifndef __OS_H__
#define __OS_H__

//#include "types.h"
#include "riscv.h"
#include "platform_xv6.h"
#include "uart.h"

#include <stddef.h>
#include <stdarg.h>

/* printf */
extern int  printf(const char* s, ...);
extern void panic(char *s);

/* plic */
extern int plic_claim(void);
extern void plic_complete(int irq);


#endif /* __OS_H__ */

[origin project](https://github.com/OSCPU/FreeRTOS/tree/afbd3dca6a385498424a34a7e26493d97e58f8eb)


**add specs**

specs
![image](https://github.com/magnate3/riscv64-freertos/blob/main/pic/specs.png)

after add 
![image](https://github.com/magnate3/riscv64-freertos/blob/main/pic/spcs2.png)

**compile problem1**
```
/usr/lib/gcc/riscv64-unknown-elf/9.3.0/libc_nano.a(lib_a-errno.o): can't link double-float modules with soft-float modules

ifdef USE_RV64
#ARCH_FLAGS = -march=rv64imac -mabi=lp64 -mcmodel=medany
ARCH_FLAGS = -march=rv64imafdc -mabi=lp64d -mcmodel=medany
else
ARCH_FLAGS = -march=rv32imac -mabi=ilp32 -mcmodel=medany
endif
```
ARCH_FLAGS = -march=rv64imafdc -mabi=lp64d -mcmodel=medany

**compile problem2**
```
./riscv64/start.S: Assembler messages:
./riscv64/start.S:54: Error: illegal operands `li t0,MSTATUS_FS'
```

include  "encoding.h" header which include macro MSTATUS_FS

#include "encoding.h"


```
qemu-system-riscv64 -M virt -kernel build/FreeRTOS-simple.elf.bin -bios none -serial stdio -display none
recv msg : Blink
recv msg : Blink
recv msg : Blink
recv msg : Blink
recv msg : Blink
recv msg : Blink
recv msg : Blink
recv msg : Blink
recv msg : Blink
recv msg : Blink
```

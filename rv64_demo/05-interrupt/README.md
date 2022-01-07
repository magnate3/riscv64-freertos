[original project](https://github.com/OSCPU/FreeRTOS/tree/afbd3dca6a385498424a34a7e26493d97e58f8eb)


================================debug


qemu-system-riscv64 -M virt -kernel build/FreeRTOS-simple.elf -bios none -serial stdio -s -S

/opt/riscv/bin/riscv64-linux-gnu-gdb -x  debug.txt 


**test_if_mtimer**
```
(gdb) b test_if_mtimer
Breakpoint 2 at 0x8000355c: file ../..//Source/portable/GCC/RISC-V/portASM.S, line 175.
(gdb) c
Continuing.

Breakpoint 2, test_if_mtimer () at ../..//Source/portable/GCC/RISC-V/portASM.S:175
175                     addi t0, x0, 1
(gdb) bt
#0  test_if_mtimer () at ../..//Source/portable/GCC/RISC-V/portASM.S:175
#1  0x00000000800020f6 in prvIdleTask (pvParameters=0x0) at ../..//Source/tasks.c:3428
#2  0x0000000000000000 in ?? ()
Backtrace stopped: frame did not save the PC
(gdb) list
170
171     #if( portasmHAS_MTIME != 0 )
172
173             test_if_mtimer:                                         /* If there is a CLINT then the mtimer is used to generate the tick interrupt. */
174
175                     addi t0, x0, 1
176
177                     slli t0, t0, __riscv_xlen - 1   /* LSB is already set, shift into MSB.  Shift 31 on 32-bit or 63 on 64-bit cores. */
178                     addi t1, t0, 7                                  /* 0x8000[]0007 == machine timer interrupt. */
179                     bne a0, t1, test_if_external_interrupt
(gdb) n
177                     slli t0, t0, __riscv_xlen - 1   /* LSB is already set, shift into MSB.  Shift 31 on 32-bit or 63 on 64-bit cores. */
(gdb) n
178                     addi t1, t0, 7                                  /* 0x8000[]0007 == machine timer interrupt. */
(gdb) n
179                     bne a0, t1, test_if_external_interrupt
```


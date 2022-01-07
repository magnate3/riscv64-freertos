[original project](https://github.com/OSCPU/FreeRTOS/tree/afbd3dca6a385498424a34a7e26493d97e58f8eb)

================================run
```
qemu-system-riscv64 -M virt -kernel build/FreeRTOS-simple.elf.bin -bios none -serial stdio -display none
Hello, FreeRTOS!
timers Callback 0
recv msg : Blink
timers Callback 1
recv msg : Blink
external interruption!

timers Callback 2
recv msg : Blink
external interruption!

external interruption!

timers Callback 3
recv msg : Blink
external interruption!

external interruption!

timers Callback 4
recv msg : Blink
timers Callback 5
```

================================debug


qemu-system-riscv64 -M virt -kernel build/FreeRTOS-simple.elf -bios none -serial stdio -s -S

/opt/riscv/bin/riscv64-linux-gnu-gdb -x  debug.txt 


**handle_trap**

```
(gdb) bt
#0  handle_trap (cause=9223372036854775819, epc=2147492086) at ./riscv64/trap.c:35
#1  0x00000000800035b8 in test_if_external_interrupt () at ../..//Source/portable/GCC/RISC-V/portASM.S:226
```

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


================================assemble
(1)x0寄存器

搞过Linux的都知道Linux有两个特殊的设备：/dev/zero和/dev/null。从/dev/zero可以源源不断地读到0，往/dev/null写的任何内容都被丢弃。如果要创建一个需要填0的文件，就从/dev/zero拷贝，如果要丢弃一些输出，就把输出重定向到/dev/null。

RISC-V的x0寄存器就相当于是硬件版的/dev/zero和/dev/null的组合体。从x0读出来的总是0，往x0写进去的总是被丢弃。所以这x0提供了两种功能，一是提供常量0，在软件编程中0可以说是最常用的常量了；二是提供一个可以丢弃结果的场所。有了这个x0，很多本来需要单独指令的操作只要更普通的指令加上x0就可以实现了。

(2)SLLI
```
SLLI  x13，x12，3

将x12寄存器中的值左移3位，并将结果写入x13寄存器中 
```

(3) ADDI
```
ADDI rd, rs1, imm 
rd = rs1 + imm


_start:
        # i = 0
        # while (i < 5) i++;

        li x5, 0
        li x6, 5
loop:
        addi x5, x5, 1
        bne x5, x6, loop
```


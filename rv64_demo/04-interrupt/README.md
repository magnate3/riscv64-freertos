[original projec](https://github.com/OSCPU/FreeRTOS/tree/afbd3dca6a385498424a34a7e26493d97e58f8eb)

====================================problem1
```
/usr/lib/riscv64-unknown-elf/bin/ld: section .stack VMA [0000000080017c00,0000000080017fff] overlaps section .bss VMA [00000000800100c8,0000000080017d1f]
collect2: error: ld returned 1 exit status
Makefile:185: recipe for target 'build/FreeRTOS-simple.elf' failed
make: *** [build/FreeRTOS-simple.elf] Error 1

```

**by expanding flash and ram：**
```
MEMORY
{
  flash (rxai!w) : ORIGIN = 0x80000000, LENGTH = 128K
  ram (wxa!ri) : ORIGIN = 0x80020000, LENGTH = 64K
}
```
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

timers Callback 3
recv msg : Blink
external interruption!

timers Callback 4
recv msg : Blink
external interruption!

external interruption!

timers Callback 5
recv msg : Blink
timers Callback 6
recv msg : Blink
```

================================debug
[FreeRTOS 中 RISC-V-Qemu-virt_GCC运行原理](https://blog.csdn.net/u011011827/article/details/120336068)

qemu-system-riscv64 -M virt -kernel build/FreeRTOS-simple.elf -bios none -serial stdio -s -S

/opt/riscv/bin/riscv64-linux-gnu-gdb -x  debug.txt 


**vTaskSwitchContext**
```
(gdb) bt
#0  vTaskSwitchContext () at ../..//Source/tasks.c:2993
#1  0x00000000800035d0 in test_if_environment_call () at ../..//Source/portable/GCC/RISC-V/portASM.S:237
Backtrace stopped: frame did not save the PC
(gdb) 
```

**prvProcessTimerOrBlockTask**
```
(gdb) bt
#0  prvProcessTimerOrBlockTask (xNextExpireTime=1000, xListWasEmpty=0) at ../..//Source/timers.c:610
#1  0x0000000080002962 in prvTimerTask (pvParameters=0x0) at ../..//Source/timers.c:596
#2  0x000000008000293a in prvProcessExpiredTimer (xNextExpireTime=0, xTimeNow=11936128518282651045)
    at ../..//Source/timers.c:565
```

**freertos_risc_v_trap_handler**
```
(gdb) bt
#0  freertos_risc_v_trap_handler () at ../..//Source/portable/GCC/RISC-V/portASM.S:123
#1  0x000000008000297c in prvProcessTimerOrBlockTask (xNextExpireTime=1000, xListWasEmpty=0)
    at ../..//Source/timers.c:610
#2  0x0000000080002962 in prvTimerTask (pvParameters=0x0) at ../..//Source/timers.c:596
#3  0x000000008000293a in prvProcessExpiredTimer (xNextExpireTime=0, xTimeNow=11936128518282651045)
    at ../..//Source/timers.c:565
```

**prvTimerTask**
```
(gdb) bt
#0  prvTimerTask (pvParameters=0x0) at ../..//Source/timers.c:592
#1  0x000000008000293a in prvProcessExpiredTimer (xNextExpireTime=0, xTimeNow=11936128518282651045)
    at ../..//Source/timers.c:565
```

 

================================bbs

[FreeRTOS 10.4.3在RISCV(T-HEAD C906)平台上移植过程](https://bbs.aw-ol.com/topic/231/freertos-10-4-3%E5%9C%A8riscv-t-head-c906-%E5%B9%B3%E5%8F%B0%E4%B8%8A%E7%A7%BB%E6%A4%8D%E8%BF%87%E7%A8%8B?lang=zh-CN)
```
 D1_FreeRTOS/Debug  make
```

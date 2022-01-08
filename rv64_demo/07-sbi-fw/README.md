=============================== problem1

```
 section .stack VMA [000000008002f800,000000008002ffff] overlaps section .bss VMA [0000000080021170,000000008003554f]
```
ram (wxa) : ORIGIN = 0x80100000, LENGTH = 64K
ram is too small
ram (wxa) : ORIGIN = 0x80100000, LENGTH = 6M
```

=============================== problem1

```
 /* witch start of fw_jump.elf test code section start */
  . = 0x80200000;
  flash (rxai!w) : ORIGIN = 0x80200000, LENGTH = 1M
  not 
  flash (rxai!w) : ORIGIN = 0x80000000, LENGTH = 1M
```
# Firmware load address configuration. This is mandatory.
FW_TEXT_START=0x80000000
FW_JUMP_ADDR=0x80200000

=============================== run
```
make PLATFORM=generic FW_PAYLOAD_PATH= build/trusted_fw.elf  CROSS_COMPILE=riscv64-unknown-elf-
qemu-system-riscv64 -M virt -m 256M -nographic -bios build/platform/generic/firmware/fw_payload.elf

OpenSBI v0.9-158-g78c2b19
   ____                    _____ ____ _____
  / __ \                  / ____|  _ \_   _|
 | |  | |_ __   ___ _ __ | (___ | |_) || |
 | |  | | '_ \ / _ \ '_ \ \___ \|  _ < | |
 | |__| | |_) |  __/ | | |____) | |_) || |_
  \____/| .__/ \___|_| |_|_____/|____/_____|
        | |
        |_|

Platform Name             : riscv-virtio,qemu
Platform Features         : medeleg
Platform HART Count       : 1
Platform IPI Device       : aclint-mswi
Platform Timer Device     : aclint-mtimer @ 10000000Hz
Platform Console Device   : uart8250
Platform HSM Device       : ---
Platform Reboot Device    : sifive_test
Platform Shutdown Device  : sifive_test
Firmware Base             : 0x80000000
Firmware Size             : 236 KB
Runtime SBI Version       : 0.3

Domain0 Name              : root
Domain0 Boot HART         : 0
Domain0 HARTs             : 0*
Domain0 Region00          : 0x0000000002000000-0x000000000200ffff (I)
Domain0 Region01          : 0x0000000080000000-0x000000008003ffff ()
Domain0 Region02          : 0x0000000000000000-0xffffffffffffffff (R,W,X)
Domain0 Next Address      : 0x0000000080200000
Domain0 Next Arg1         : 0x0000000082200000
Domain0 Next Mode         : S-mode
Domain0 SysReset          : yes

Boot HART ID              : 0
Boot HART Domain          : root
Boot HART ISA             : rv64imafdcsu
Boot HART Features        : scounteren,mcounteren,time
Boot HART PMP Count       : 16
Boot HART PMP Granularity : 4
Boot HART PMP Address Bits: 54
Boot HART MHPM Count      : 0
Boot HART MIDELEG         : 0x0000000000000222
Boot HART MEDELEG         : 0x000000000000b109
sbi_trap_error: hart0: trap handler failed (error -2)
sbi_trap_error: hart0: mcause=0x0000000000000007 mtval=0x0000000002010008
sbi_trap_error: hart0: mepc=0x000000008000970a mstatus=0x8000000000007800
sbi_trap_error: hart0: ra=0x0000000080000836 sp=0x0000000080039ee0
sbi_trap_error: hart0: gp=0x0000000000000000 tp=0x000000008003a000
sbi_trap_error: hart0: s0=0x0000000080039f30 s1=0x000000008003a000
sbi_trap_error: hart0: a0=0x0000000000000000 a1=0x0000000082200000
sbi_trap_error: hart0: a2=0x0000000080200000 a3=0x000000000000001f
sbi_trap_error: hart0: a4=0x0000000000000000 a5=0x0000000002010000
sbi_trap_error: hart0: a6=0x0000000000000002 a7=0x0000000000000061
sbi_trap_error: hart0: s2=0x0000000000000001 s3=0x0000000000000000
sbi_trap_error: hart0: s4=0x0000000080016000 s5=0x0000000000000001
sbi_trap_error: hart0: s6=0x0000000080016020 s7=0x000000000000007f
sbi_trap_error: hart0: s8=0x0000000080015030 s9=0x0000000080034ca0
sbi_trap_error: hart0: s10=0x0000000000000000 s11=0x0000000000000000
sbi_trap_error: hart0: t0=0x0000000080039ef0 t1=0x0000000000000000
sbi_trap_error: hart0: t2=0x0000000000001000 t3=0x0000000000000027
sbi_trap_error: hart0: t4=0x0000000000000000 t5=0x0000000000000010
sbi_trap_error: hart0: t6=0x0000000000000000
QEMU: Terminated
```

=============================== run case2
opensbi-0.4-rv32-bin/platform/qemu/virt/firmware/fw_jump.elf
qemu-system-riscv64 -M virt -kernel build/trusted_fw.elf  -bios none -serial stdio -display none
qemu-system-riscv64 -nographic -machine virt  -bios fw_jump.elf -kernel build/trusted_fw.elf   -display none
```
qemu-system-riscv64 -nographic -machine virt  -bios fw_jump.elf -kernel build/trusted_fw.elf   -display none

OpenSBI v0.5-52-g30cdf00
   ____                    _____ ____ _____
  / __ \                  / ____|  _ \_   _|
 | |  | |_ __   ___ _ __ | (___ | |_) || |
 | |  | | '_ \ / _ \ '_ \ \___ \|  _ < | |
 | |__| | |_) |  __/ | | |____) | |_) || |_
  \____/| .__/ \___|_| |_|_____/|____/_____|
        | |
        |_|

Platform Name          : QEMU Virt Machine
Platform HART Features : RV64ACDFIMSU
Platform Max HARTs     : 8
Current Hart           : 0
Firmware Base          : 0x80000000
Firmware Size          : 120 KB
Runtime SBI Version    : 0.2

PMP0: 0x0000000080000000-0x000000008001ffff (A)
PMP1: 0x0000000000000000-0xffffffffffffffff (A,R,W,X)
Hello FreeRTOS!
Hello main_blinky!
start schedule Unhandled Exception 5:Load Access Fault
scause:0x5,stval:0x6211f448,sepc:0x80200b4a
--------------Dump Registers-----------------
Function Registers:
Unhandled Exception 5:Load Access Fault
scause:0x5,stval:0x8,sepc:0x80201f66
--------------Dump Registers-----------------
Function Registers:
        ra(x1) = 0x2d2d2d2d     user_sp = 0x3fe
Temporary Registers:
        t0(x5) = 0x2d2d000a     t1(x6) = 0xa2d
        t2(x7) = 0x0
        t3(x28) = 0x0   t4(x29) = 0x0
        t5(x30) = 0x0   t6(x31) = 0x0
Saved Registers:
        s0/fp(x8) = 0x0 s1(x9) = 0x0
        s2(x18) = 0x0   s3(x19) = 0x0
        s4(x20) = 0x0   s5(x21) = 0x0
        s6(x22) = 0x0   s7(x23) = 0x0
        s8(x24) = 0x0   s9(x25) = 0x0
        s10(x26) = 0x0  s11(x27) = 0x0
Function Arguments Registers:
        a0(x10) = 0x0   a1(x11) = 0x0
        a2(x12) = 0x0   a3(x13) = 0x0
        a4(x14) = 0x0   a5(x15) = 0x0
        a6(x16) = 0x0   a7(x17) = 0x0
sstatus = 0x0
        Supervisor Interrupt Disabled
        Last Time Supervisor Interrupt Disabled
        Last Privilege is User Mode
        Not Permit to Read Executable-only Page
-----------------Dump OK---------------------
```


=============================== run case3
```
qemu-system-riscv64 -nographic -machine virt  -bios fw_jump.elf -kernel build/trusted_fw.elf   -display none

OpenSBI v0.5-52-g30cdf00
   ____                    _____ ____ _____
  / __ \                  / ____|  _ \_   _|
 | |  | |_ __   ___ _ __ | (___ | |_) || |
 | |  | | '_ \ / _ \ '_ \ \___ \|  _ < | |
 | |__| | |_) |  __/ | | |____) | |_) || |_
  \____/| .__/ \___|_| |_|_____/|____/_____|
        | |
        |_|

Platform Name          : QEMU Virt Machine
Platform HART Features : RV64ACDFIMSU
Platform Max HARTs     : 8
Current Hart           : 0
Firmware Base          : 0x80000000
Firmware Size          : 120 KB
Runtime SBI Version    : 0.2

PMP0: 0x0000000080000000-0x000000008001ffff (A)
PMP1: 0x0000000000000000-0xffffffffffffffff (A,R,W,X)
Hello FreeRTOS!
vTaskCreate
task2 0x0
task1 0x0
task2 0x1
task1 0x1
task2 0x2
task1 0x2
QEMU: Terminated
```

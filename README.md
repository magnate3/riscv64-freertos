[riscv64-toolchain]https://github.com/sifive/freedom-tools/releases

************************************build
(1)  # include_next <stdint.h>
I'll bet you forgot to pass -ffreestanding to your compiler. 
```
CPPFLAGS = \
        -D__riscv_float_abi_soft -ffreestanding \
```



riscv64-unknown-elf-gcc -E   -march=rv32i      -mabi=ilp32 -dM -nostdlib - < /dev/null |grep STDC_HOSTED
#define __STDC_HOSTED__ 1

dpkg --contents gcc-riscv64-unknown-elf_9.3.0-0ubuntu1_arm64.deb  | grep stdint
-rw-r--r-- root/root      9457 2020-04-02 17:41 ./usr/lib/gcc/riscv64-unknown-elf/9.3.0/include/stdint-gcc.h
-rw-r--r-- root/root       328 2020-04-02 17:41 ./usr/lib/gcc/riscv64-unknown-elf/9.3.0/include/stdint.h
-rw-r--r-- root/root      3960 2020-04-02 17:41 ./usr/lib/gcc/riscv64-unknown-elf/9.3.0/plugin/include/config/newlib-stdint.h

************************************link
(1) problem1
```
/usr/lib/riscv64-unknown-elf/bin/ld: cannot find -lc
/usr/lib/riscv64-unknown-elf/bin/ld: cannot find -lgloss
collect2: error: ld returned 1 exit status
Makefile:58: recipe for target 'build/RTOSDemo.axf' failed


/usr/lib/gcc/riscv64-unknown-elf/9.3.0
```

(2) problem2
```
/usr/lib/riscv64-unknown-elf/bin/ld: /usr/lib/gcc/riscv64-unknown-elf/9.3.0/rv64imafdc/lp64d/libgcc.a(trunctfdf2.o): can't link double-float modules with soft-float modules
/usr/lib/riscv64-unknown-elf/bin/ld: failed to merge target specific data of file /usr/lib/gcc/riscv64-unknown-elf/9.3.0/rv64imafdc/lp64d/libgcc.a(trunctfdf2.o)
/usr/lib/riscv64-unknown-elf/bin/ld: /usr/lib/gcc/riscv64-unknown-elf/9.3.0/rv64imafdc/lp64d/libgcc.a(_clzsi2.o): can't link double-float modules with soft-float modules
/usr/lib/riscv64-unknown-elf/bin/ld: failed to merge target specific data of file /usr/lib/gcc/riscv64-unknown-elf/9.3.0/rv64imafdc/lp64d/libgcc.a(_clzsi2.o)
/usr/lib/riscv64-unknown-elf/bin/ld: /usr/lib/gcc/riscv64-unknown-elf/9.3.0/rv64imafdc/lp64d/libgcc.a(_clz.o): can't link double-float modules with soft-float modules
/usr/lib/riscv64-unknown-elf/bin/ld: failed to merge target specific data of file /usr/lib/gcc/riscv64-unknown-elf/9.3.0/rv64imafdc/lp64d/libgcc.a(_clz.o)
/usr/lib/riscv64-unknown-elf/bin/ld: build/RTOSDemo.axf(.text.prvQueueSendTask): relocation "xQueueGenericSend+0x0 (type R_RISCV_CALL)" goes out of range
/usr/lib/riscv64-unknown-elf/bin/ld: build/main_blinky.o: file class ELFCLASS32 incompatible with ELFCLASS64
/usr/lib/riscv64-unknown-elf/bin/ld: final link failed: file in wrong format
```

```
ls /usr/lib/gcc/riscv64-unknown-elf/9.3.0/
cc1         include        libgloss.a              libm_nano.a    rv32ea    rv32iafd    rv32imfd  rv64im
cc1plus     include-fixed  libgloss_nano.a         libnosys.a     rv32eac   rv32if      rv64i     rv64imac
collect2    install-tools  libg_nano.a             libsemihost.a  rv32em    rv32ifd     rv64ia    rv64imafc
crt0.o      libc.a         liblto_plugin.la        libsim.a       rv32emac  rv32im      rv64iac   rv64imafdc
crtbegin.o  libc_nano.a    liblto_plugin.so        lto1           rv32i     rv32imac    rv64iaf   rv64imf
crtend.o    libg.a         liblto_plugin.so.0      lto-wrapper    rv32ia    rv32imafc   rv64iafd
crti.o      libgcc.a       liblto_plugin.so.0.0.0  plugin         rv32iac   rv32imafdc  rv64if
crtn.o      libgcov.a      libm.a                  rv32e          rv32iaf   rv32imf     rv64ifd
```

************************************run

```
qemu-system-riscv64 -M virt -kernel build/RTOSDemo.axf  -bios none -serial stdio -display none
Hello FreeRTOS!
0: Tx: Transfer1
Unexpected value received

0: Tx: Transfer2
Unexpected value received

0: Tx: Transfer1
Unexpected value received

0: Tx: Transfer2
Unexpected value received

0: Tx: Transfer1
Unexpected value received

0: Tx: Transfer2
Unexpected value received

0: Tx: Transfer1
Unexpected value received

0: Tx: Transfer2
Unexpected value received

0: Tx: Transfer1
Unexpected value received

0: Tx: Transfer2
Unexpected value received

0: Tx: Transfer1
Unexpected value received

0: Tx: Transfer2
Unexpected value received

0: Tx: Transfer1
```


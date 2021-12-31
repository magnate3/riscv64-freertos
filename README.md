[riscv64-toolchain download](https://github.com/sifive/freedom-tools/releases)
all develop tools
![image](https://github.com/magnate3/riscv64-freertos/blob/main/pic/all.png)

************************************env
```
ubuntu@ubuntu:~$ uname -a
Linux ubuntu 4.15.0-76-generic #86-Ubuntu SMP Fri Jan 17 17:25:58 UTC 2020 aarch64 aarch64 aarch64 GNU/Linux 
ubuntu@ubuntu:~$ qemu-riscv64 -verion
qemu: unknown option 'verion'
ubuntu@ubuntu:~$ qemu-riscv64 -version
qemu-riscv64 version 6.1.0
Copyright (c) 2003-2021 Fabrice Bellard and the QEMU Project developers
```
install riscv64-unknown-elf
```
ls binutils-riscv64-unknown-elf_2.34-0ubuntu1_arm64.deb 
binutils-riscv64-unknown-elf_2.34-0ubuntu1_arm64.deb
ls gcc-riscv64-unknown-elf_9.3.0-0ubuntu1_arm64.deb 
gcc-riscv64-unknown-elf_9.3.0-0ubuntu1_arm64.deb
```

************************************build
(1)  # include_next <stdint.h>
to pass -ffreestanding to your compiler. 
```
CPPFLAGS = \
        -D__riscv_float_abi_soft -ffreestanding \
```


```
riscv64-unknown-elf-gcc -E   -march=rv32i      -mabi=ilp32 -dM -nostdlib - < /dev/null |grep STDC_HOSTED
#define __STDC_HOSTED__ 1

dpkg --contents gcc-riscv64-unknown-elf_9.3.0-0ubuntu1_arm64.deb  | grep stdint
-rw-r--r-- root/root      9457 2020-04-02 17:41 ./usr/lib/gcc/riscv64-unknown-elf/9.3.0/include/stdint-gcc.h
-rw-r--r-- root/root       328 2020-04-02 17:41 ./usr/lib/gcc/riscv64-unknown-elf/9.3.0/include/stdint.h
-rw-r--r-- root/root      3960 2020-04-02 17:41 ./usr/lib/gcc/riscv64-unknown-elf/9.3.0/plugin/include/config/newlib-stdint.h
```
add header
![image](https://github.com/magnate3/riscv64-freertos/blob/main/pic/inc.png)

************************************link
(1) problem1
```
/usr/lib/riscv64-unknown-elf/bin/ld: cannot find -lc
/usr/lib/riscv64-unknown-elf/bin/ld: cannot find -lgloss
collect2: error: ld returned 1 exit status
Makefile:58: recipe for target 'build/RTOSDemo.axf' failed


/usr/lib/gcc/riscv64-unknown-elf/9.3.0
```
add link package
![image](https://github.com/magnate3/riscv64-freertos/blob/main/pic/so.png)

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

first, look riscv64-linux-gnu-gcc -v
```
riscv64-linux-gnu-gcc -v
Using built-in specs.
COLLECT_GCC=riscv64-linux-gnu-gcc
COLLECT_LTO_WRAPPER=/usr/lib/gcc-cross/riscv64-linux-gnu/8/lto-wrapper
Target: riscv64-linux-gnu
Configured with: ../src/configure -v --with-pkgversion='Ubuntu 8.4.0-1ubuntu1~18.04' --with-bugurl=file:///usr/share/doc/gcc-8/README.Bugs --enable-languages=c,c++,go,d,fortran,objc,obj-c++ --prefix=/usr --with-gcc-major-version-only --program-suffix=-8 --enable-shared --enable-linker-build-id --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --libdir=/usr/lib --enable-nls --with-sysroot=/ --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --with-default-libstdcxx-abi=new --enable-gnu-unique-object --disable-libitm --disable-libsanitizer --disable-libquadmath --disable-libquadmath-support --enable-plugin --enable-default-pie --with-system-zlib --disable-libphobos --enable-multiarch --disable-werror --disable-multilib --with-arch=rv64imafdc --with-abi=lp64d --enable-checking=release --build=aarch64-linux-gnu --host=aarch64-linux-gnu --target=riscv64-linux-gnu --program-prefix=riscv64-linux-gnu- --includedir=/usr/riscv64-linux-gnu/include
Thread model: posix
gcc version 8.4.0 (Ubuntu 8.4.0-1ubuntu1~18.04) 
```
--with-arch=rv64imafdc --with-abi=lp64d
```
change to 64
CFLAGS  = -march=rv64imafdc -mabi=lp64d -mcmodel=medany
ASFLAGS = -march=rv64imafdc -mabi=lp64d -mcmodel=medany
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


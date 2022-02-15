#ifndef __TYPES_H__
#define __TYPES_H__
#include<stdint-gcc.h>
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int  uint32_t;
//typedef unsigned long long uint64_t;

/*
 * RISCV32: register is 32bits width
 */ 
//typedef uint32_t reg_t;
typedef uint64_t reg_t;

#endif /* __TYPES_H__ */

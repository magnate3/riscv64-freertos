/*
 * FreeRTOS V202112.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://www.github.com/FreeRTOS
 *
 */

/* FreeRTOS kernel includes. */
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

#include <stdio.h>
#include <stdlib.h>
#include "lib.h"

static uint8_t *mbegin = (uint8_t*) 0x08000000;
static uint8_t *mend   = (uint8_t*) 0x10000000;
/* Define the start address and size of the three RAM regions. */ 
#define RAM1_START_ADDRESS    ( ( uint8_t * ) 0x08000000 ) 
#define RAM1_SIZE             ( 1024 * 1024 ) 
/* Define the start address and size of the three RAM regions. */ 
//#define RAM1_START_ADDRESS    ( ( uint8_t * ) 0x00010000 ) 
//#define RAM1_SIZE             ( 65 * 1024 ) 

#define RAM2_START_ADDRESS    ( ( uint8_t * ) 0x00020000 ) 
#define RAM2_SIZE             ( 32 * 1024 ) 

#define RAM3_START_ADDRESS    ( ( uint8_t * ) 0x00030000 ) 
#define RAM3_SIZE             ( 32 * 1024 ) 

/* Create an array of HeapRegion_t definitions, with an index for each of the three 
RAM regions, and terminating the array with a NULL address.  The HeapRegion_t 
structures must appear in start address order, with the structure that contains the 
lowest start address appearing first. */ 
const HeapRegion_t xHeapRegions[] = 
{ 
    { RAM1_START_ADDRESS, RAM1_SIZE }, 
    { NULL,               0         }  /* Marks the end of the array. */ 
}; 
//const HeapRegion_t xHeapRegions[] = 
//{ 
//    { RAM1_START_ADDRESS, RAM1_SIZE }, 
//    { RAM2_START_ADDRESS, RAM2_SIZE }, 
//    { RAM3_START_ADDRESS, RAM3_SIZE }, 
//    { NULL,               0         }  /* Marks the end of the array. */ 
//}; 
/*-----------------------------------------------------------*/

int main_blinky( void )
{
	lib_printf( "Hello FreeRTOS! \n" );
        /* Initialize heap_5. */ 
        vPortDefineHeapRegions( xHeapRegions ); 
        char *m =NULL;
        char *os_m =NULL;
	lib_printf("%d, heap size : %d \n",__LINE__,  xPortGetFreeHeapSize());
	m = (char *)malloc(1024);
	lib_printf("%d, %d \n",__LINE__,  xPortGetFreeHeapSize());
	os_m = (char *)pvPortMalloc(1024);
	lib_printf("%d, %d \n",__LINE__,  xPortGetFreeHeapSize());
	free(m);
	lib_printf("%d, %d \n",__LINE__,  xPortGetFreeHeapSize());
	vPortFree(m);
	lib_printf("%d, %d \n",__LINE__,  xPortGetFreeHeapSize());

	return 0;
}

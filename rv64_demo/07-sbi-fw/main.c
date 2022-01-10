#include <FreeRTOS.h>
#include <task.h>
#include "debug_log.h"
#include "sbi.h"
#include "encoding.h"
#include "riscv2.h"
/* Prototypes for the standard FreeRTOS callback/hook functions implemented
within this file.  See https://www.freertos.org/a00016.html */
void vApplicationMallocFailedHook( void );
void vApplicationIdleHook( void );
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName );
void vApplicationTickHook( void );
#define mainCREATE_SIMPLE_BLINKY_DEMO_ONLY	1
extern void main_blinky( void );
static void task1(void *p_arg)
{ 
    int time = 0;
    for(;;)
    {
        debug_log("task1 0x%x\n",time++);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void task2(void *p_arg)
{ 
    int time = 0;
    for(;;)
    {
        debug_log("task2 0x%x\n",time++);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void vTaskCreate(void *p_arg)
{ 
    debug_log("vTaskCreate\n");
    xTaskCreate(task1,"task1",2048,NULL,4,NULL);
    xTaskCreate(task2,"task2",2048,NULL,4,NULL);

    //vTaskDelete(NULL);
}
void setup_pmp(void)
{
  // Set up a PMP to permit access to all of memory.
  // Ignore the illegal-instruction trap if PMPs aren't supported.
  uintptr_t pmpc = PMP_NAPOT | PMP_R | PMP_W | PMP_X;
  asm volatile ("la t0, 1f\n\t"
                "csrrw t0, mtvec, t0\n\t"
                "csrw pmpaddr0, %1\n\t"
                "csrw pmpcfg0, %0\n\t"
                ".align 2\n\t"
                "1: csrw mtvec, t0"
                : : "r" (pmpc), "r" (-1UL) : "t0");
}
#define TEST_CASE 2
int main(void)
{
    debug_log("Hello FreeRTOS!\n");
    //debug_log_init();
#if (TEST_CASE == 1) 
   //setup_pmp();
    // set M Previous Privilege mode to Supervisor, for mret.
    unsigned long x = r_mstatus();
    x &= ~MSTATUS_MPP_MASK;
    x |= MSTATUS_MPP_S;
    w_mstatus(x);

    // set M Exception Program Counter to main, for mret.
    // requires gcc -mcmodel=medany
    w_mepc((uint64_t) main_blinky);

    // disable paging for now.
    w_satp(0);

    // delegate all interrupts and exceptions to supervisor mode.
    w_medeleg(0xffff);
    w_mideleg(0xffff);

    // setup trap_entry
    //w_mtvec((uint64_t)trap_entry);

    // keep each CPU's hartid in its tp register, for cpuid().
    int id = r_mhartid();
    w_tp(id);
    // switch to supervisor mode and jump to main().
    asm volatile("mret");
    __builtin_unreachable();
#elif (TEST_CASE == 2)
   main_blinky();
#elif (TEST_CASE == 3)
   vTaskCreate(NULL);
   vTaskStartScheduler();
   while(1){};
#endif
    return 0;
}


void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If the application makes use of the
	vTaskDelete() API function (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	function, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
	/* The tests in the full demo expect some interaction with interrupts. */
	#if( mainCREATE_SIMPLE_BLINKY_DEMO_ONLY != 1 )
	{
		extern void vFullDemoTickHook( void );
		vFullDemoTickHook();
	}
	#endif
}
/*-----------------------------------------------------------*/

void vAssertCalled( void )
{
volatile uint32_t ulSetTo1ToExitFunction = 0;

	taskDISABLE_INTERRUPTS();
	while( ulSetTo1ToExitFunction != 1 )
	{
		__asm volatile( "NOP" );
	}
}

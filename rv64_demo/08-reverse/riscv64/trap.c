#include "os.h"
extern void trap_entry(void);
extern void uart_isr(void);
extern void timer_handler(void);

/* ISR */
uint64_t ulExampleInterruptHandler( void )
{
	BaseType_t xHigherPriorityTaskWoken;
	/* xHigherPriorityTaskWoken 指向的变量在使用前需要初始化为pdFALSE */
	xHigherPriorityTaskWoken = pdFALSE;
	/* 发送一个信号量 */
	xSemaphoreGiveFromISR( xBinarySemaphore, &xHigherPriorityTaskWoken );
	/* 根据xHigherPriorityTaskWoken 判断是否需要执行调度 */
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void trap_init()
{
	/*
	 * set the trap-vector base-address for machine-mode
	 */
	w_mtvec((reg_t)trap_entry);
}

void external_interrupt_handler()
{
	int irq = plic_claim();

	if (irq == UART0_IRQ){
      		uart_isr();
                ulExampleInterruptHandler( );
	} else if (irq) {
		printf("unexpected interrupt irq = %d\n", irq);
	}
	
	if (irq) {
		plic_complete(irq);
	}
}
/*************
csrr a0, mcause
csrr a1, mepc
*********************/
reg_t handle_trap(reg_t cause, reg_t epc)
{
	reg_t return_pc = epc;
	reg_t cause_code = cause & 0xfff;
	
	if (cause >> 63) {
		/* Asynchronous trap - interrupt */
		switch (cause << 1 >> 1) {
		case 3:
			uart_puts("software interruption!\n");
			/*
			 * acknowledge the software interrupt by clearing
    			 * the MSIP bit in mip.
			 */
			int id = r_mhartid();
    			*(uint32_t*)CLINT_MSIP(id) = 0;


			break;
		case 7:
			uart_puts("timer interruption!\n");
			timer_handler();
			break;
		case 11:
			uart_puts("external interruption!\n");
			external_interrupt_handler();
			break;
		default:
			uart_puts("unknown async exception!\n");
			break;
		}
	} else {
		/* Synchronous trap - exception */
		printf("Sync exceptions!, code = %d\n", cause_code);
		panic("OOPS! What can I do!");
		//return_pc += 4;
	}

	return return_pc;
}

void trap_test()
{
	/*
	 * Synchronous exception code = 7
	 * Store/AMO access fault
	 */
	*(int *)0x00000000 = 100;

	/*
	 * Synchronous exception code = 5
	 * Load access fault
	 */
	//int a = *(int *)0x00000000;

	uart_puts("Yeah! I'm return back from trap!\n");
}


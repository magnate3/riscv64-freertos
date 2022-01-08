[original project](https://github.com/OSCPU/FreeRTOS/tree/afbd3dca6a385498424a34a7e26493d97e58f8eb)

================================run
```
qemu-system-riscv64 -M virt -kernel build/FreeRTOS-simple.elf.bin -bios none -serial stdio -display none
Hello, FreeRTOS!
external interruption!

Handler soft interrupt task - Processing event.
external interruption!

Handler soft interrupt task - Processing event.
external interruption!

Handler soft interrupt task - Processing event.
Periodic task - waiting an interrupt.
external interruption!

Handler soft interrupt task - Processing event.
external interruption!

Handler soft interrupt task - Processing event.
external interruption!

Handler soft interrupt task - Processing event.
external interruption!

Handler soft interrupt task - Processing event.
external interruption!

Handler soft interrupt task - Processing event.
Periodic task - waiting an interrupt.
external interruption!

Handler soft interrupt task - Processing event.
```



**ulExampleInterruptHandler**

```
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
```

 


 


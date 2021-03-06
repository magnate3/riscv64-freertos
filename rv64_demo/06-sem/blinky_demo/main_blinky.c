//===========================================================
// mmRISC Project
//-----------------------------------------------------------
// File Name   : main_blinky.c
// Description : Main Blinky  Routine
//-----------------------------------------------------------
// History :
// Rev.01 2021.05.08 M.Maruyama First Release
//-----------------------------------------------------------
// Copyright (C) 2020-2021 M.Maruyama
//===========================================================
// This program is based on following program.
/*
 * FreeRTOS V202104.00
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
 * 1 tab == 4 spaces!
 */

// FreeRTOS kernel includes
#include <stdio.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

// mmRISC includes
#include "os.h"

// Priorities used by the tasks.
#define PRIORITY_TASK_IRQ                   ( tskIDLE_PRIORITY + 4 )
#define PRIORITY_TASK_QUEUE_RX     ( tskIDLE_PRIORITY + 3 )
#define PRIORITY_TASK_QUEUE_TX        ( tskIDLE_PRIORITY + 2 )

// The rate at which data is sent to the queue.
// The 200ms value is converted to ticks using the pdMS_TO_TICKS() macro.
#define mainQUEUE_SEND_FREQUENCY_MS     pdMS_TO_TICKS( 1000 )

// The maximum number items the queue can hold.  The priority of the receiving
// task is above the priority of the sending task, so the receiving task will
// preempt the sending task and remove the queue items each time the sending task
// writes to the queue.  Therefore the queue will never have more than one item in
// it at any time, and even with a queue length of 1, the sending task will never
// find the queue full.
#define mainQUEUE_LENGTH   ( 1 )

// The queue used among tasks.
static QueueHandle_t xQueue = NULL;

// Mutex Handle
static SemaphoreHandle_t xMutex = NULL;

// Binary Semaphore among ISR
static SemaphoreHandle_t xSemaphore = NULL;

//----------------------------------------------------
// Task Queue Sender
//----------------------------------------------------
static void Task_Queue_Tx( void *pvParameters )
{
    TickType_t xNextWakeTime;
    const unsigned long ulValueToSend = 100UL;
    const char * const pcMessage1 = "Transfer1";
    const char * const pcMessage2 = "Transfer2";
    int f = 1;
    BaseType_t xStatus;
    const TickType_t xTicksToWait = 1000UL;

    // Remove compiler warning about unused parameter.
    ( void ) pvParameters;

    // Initialise xNextWakeTime - this only needs to be done once.
    xNextWakeTime = xTaskGetTickCount();

    for( ;; )
    {
        // Take Mutex
        xStatus = xSemaphoreTake(xMutex, xTicksToWait);
        // if OK, use I/O
        if (xStatus == pdTRUE)
        {
            printf(" %s: %s\n", 
                    pcTaskGetName( xTaskGetCurrentTaskHandle() ),
                    ( f ) ? pcMessage1 : pcMessage2 ); // mmRISC
        }
        // Release Mutex
        xSemaphoreGive(xMutex);
        f = !f;

        // Place this task in the blocked state until it is time to run again.
        vTaskDelayUntil( &xNextWakeTime, mainQUEUE_SEND_FREQUENCY_MS );

        // Send to the queue - causing the queue receive task to unblock and
        // toggle the LED.  0 is used as the block time so the sending operation
        // will not block - it shouldn't need to block as the queue should always
        // be empty at this point in the code.
        xQueueSend( xQueue, &ulValueToSend, 0U );
    }
}

//----------------------------------------------------
// Task Queue Receiver
//----------------------------------------------------
static void Task_Queue_Rx( void *pvParameters )
{
    unsigned long ulReceivedValue;
    const unsigned long ulExpectedValue = 100UL;
    const char * const pcMessage1 = "Blink1";
    const char * const pcMessage2 = "Blink2";
    const char * const pcFailMessage = "Unexpected value received\r\n";
    int f = 1;
    BaseType_t xStatus;
    const TickType_t xTicksToWait = 1000UL;

    // Remove compiler warning about unused parameter.
    ( void ) pvParameters;

    for( ;; )
    {
        // Wait until something arrives in the queue - this task will block
        // indefinitely provided INCLUDE_vTaskSuspend is set to 1 in
        // FreeRTOSConfig.h.
        xQueueReceive( xQueue, &ulReceivedValue, portMAX_DELAY );

        //  To get here something must have been received from the queue, but
        // is it the expected value?  If it is, toggle the LED.
        if( ulReceivedValue == ulExpectedValue )
        {
            // Take Mutex
            xStatus = xSemaphoreTake(xMutex, xTicksToWait);
            // if OK, use I/O
            if (xStatus == pdTRUE)
            {
                    printf(" %s: %s\n", 
                        pcTaskGetName( xTaskGetCurrentTaskHandle() ),
                        ( f ) ? pcMessage1 : pcMessage2 ); // mmRISC
            }
            // Release Mutex
            xSemaphoreGive(xMutex);
            f = !f;
            //
            ulReceivedValue = 0U;
        }
        else
        {
            printf( pcFailMessage ); // mmRISC
        }
    }
}

//----------------------------------------------------
// Task IRQ
//----------------------------------------------------
static void Task_IRQ( void *pvParameters )
{
    BaseType_t xStatus;
    const TickType_t xTicksToWait = 1000UL;

    for ( ;; )
    {
        // Wait for IRQ from UART
        xSemaphoreTake(xSemaphore, portMAX_DELAY );
        // Take Mutex
        xStatus = xSemaphoreTake(xMutex, xTicksToWait);
        // if OK, use I/O
        if (xStatus == pdTRUE)
        {
	    printf("uart handle \n");
            //INT_UART_Handler();
        }
        // Release Mutex
        xSemaphoreGive(xMutex);
    }
}

//--------------------------------------------------
// Main Blinky
//--------------------------------------------------
int main_blinky( void )
{
    printf( "Hello FreeRTOS!\n" );

    // Create the queue.
    xQueue = xQueueCreate( mainQUEUE_LENGTH, sizeof( uint32_t ) );

    // Create Mutex
    xMutex = xSemaphoreCreateMutex();

    // Create Binary Semaphore
    xSemaphore = xSemaphoreCreateBinary();

    // Create Tasks
    if ((xQueue != NULL ) && (xMutex != NULL) && (xSemaphore != NULL))
    {
        xTaskCreate( Task_Queue_Rx, "Rx", configMINIMAL_STACK_SIZE * 2U, NULL,
                    PRIORITY_TASK_QUEUE_RX, NULL );
        xTaskCreate( Task_Queue_Tx, "Tx", configMINIMAL_STACK_SIZE * 2U, NULL,
                    PRIORITY_TASK_QUEUE_TX, NULL );
        xTaskCreate( Task_IRQ, "IRQ", configMINIMAL_STACK_SIZE * 2U, NULL,
                     PRIORITY_TASK_IRQ, NULL );
    }

    // Start Tasks
    vTaskStartScheduler();

    return 0;
}

//-------------------------------------
// External Interrupt Handler
//-------------------------------------
void external_interrupt_handler_test(void)
{
    BaseType_t xHigherPriorityTaskWoken;
    xHigherPriorityTaskWoken = pdFALSE;
    //
    // Check the interrupt is from UART?
    //if (INT_UART_Check())
    {
        // Give Semaphore to ISR
        xSemaphoreGiveFromISR(xSemaphore, &xHigherPriorityTaskWoken);
    }
    //
    // Request Context Switch
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

//===========================================================
// End of Program
//===========================================================


#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define BAUD 9600
#define UBRR_VALUE (((F_CPU / 16UL) / BAUD) - 1)
#define TX_QUEUE_SIZE 64

// Queue handles for UART communication
QueueHandle_t queueUART0toUART1, queueUART1toUART0;
QueueHandle_t txQueueUART0, txQueueUART1;

// Initialize UART communication
void uart_init(uint8_t uart, unsigned int ubrr)
{
    if (uart == 0)
    {
        UBRR0H = (UBRR_VALUE >> 8);
        UBRR0L = UBRR_VALUE;
        UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0) | (1 << UDRIE0);
        UCSR0C = (3 << UCSZ00);
    }
    else if (uart == 1)
    {
        UBRR1H = (UBRR_VALUE >> 8);
        UBRR1L = UBRR_VALUE;
        UCSR1B = (1 << RXEN1) | (1 << TXEN1) | (1 << RXCIE1) | (1 << UDRIE1);
        UCSR1C = (3 << UCSZ10);
    }
}

// ISR for UART0 RX complete
ISR(USART0_RX_vect)
{
    char data = UDR0;
    xQueueSendFromISR(queueUART0toUART1, &data, NULL);
}

// ISR for UART1 RX complete
ISR(USART1_RX_vect)
{
    char data = UDR1;
    xQueueSendFromISR(queueUART1toUART0, &data, NULL);
}

// ISR for UART0 Data Register Empty (UDRE)
ISR(USART0_UDRE_vect)
{
    char data;
    if (xQueueReceiveFromISR(txQueueUART0, &data, NULL) == pdPASS)
    {
        UDR0 = data;
    }
    else
    {
        UCSR0B &= ~(1 << UDRIE0);
    }
}

// ISR for UART1 Data Register Empty (UDRE)
ISR(USART1_UDRE_vect)
{
    char data;
    if (xQueueReceiveFromISR(txQueueUART1, &data, NULL) == pdPASS)
    {
        UDR1 = data;
    }
    else
    {
        UCSR1B &= ~(1 << UDRIE1);
    }
}

// Task for handling data from UART0 to UART1
void TaskUART0toUART1(void *pvParameters)
{
    char data;
    for (;;)
    {
        if (xQueueReceive(queueUART0toUART1, &data, portMAX_DELAY))
        {
            xQueueSend(txQueueUART1, &data, portMAX_DELAY);
            UCSR1B |= (1 << UDRIE1); // Ensure UDRE interrupt is enabled
        }
    }
}

// Task for handling data from UART1 to UART0
void TaskUART1toUART0(void *pvParameters)
{
    char data;
    for (;;)
    {
        if (xQueueReceive(queueUART1toUART0, &data, portMAX_DELAY))
        {
            xQueueSend(txQueueUART0, &data, portMAX_DELAY);
            UCSR0B |= (1 << UDRIE0); // Ensure UDRE interrupt is enabled
        }
    }
}

int main(void)
{
    uart_init(0, UBRR_VALUE); // Initialize UART0
    uart_init(1, UBRR_VALUE); // Initialize UART1
    sei(); // Enable global interrupts

    queueUART0toUART1 = xQueueCreate(TX_QUEUE_SIZE, sizeof(char));
    queueUART1toUART0 = xQueueCreate(TX_QUEUE_SIZE, sizeof(char));
    txQueueUART0 = xQueueCreate(TX_QUEUE_SIZE, sizeof(char));
    txQueueUART1 = xQueueCreate(TX_QUEUE_SIZE, sizeof(char));

    xTaskCreate(TaskUART0toUART1, "TX0toRX1", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(TaskUART1toUART0, "RX1toTX0", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    vTaskStartScheduler();

    while (1) {}
}

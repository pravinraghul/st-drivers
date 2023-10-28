#include "gpio.h"
#include "uart.h"
#include "SEGGER_RTT.h"

gpio_handle_t gpio_handle;
uart_handle_t uart_handle;

uint8_t buffer[2];

static void receive_complete_callback(void)
{
    SEGGER_RTT_printf(0, "Received: %x \r\n", buffer[0]);
    uart_transmit(&uart_handle, buffer, 1);
    SEGGER_RTT_printf(0, "Transmit: %x \r\n", buffer[0]);
}

void setup()
{
    GPIOC_CLK_ENABLE();
    GPIOD_CLK_ENABLE();

    gpio_handle.config.mode = GPIO_MODE_ALTFUNC;
    gpio_handle.config.pull = GPIO_PULL_NO;
    gpio_handle.config.speed = GPIO_SPEED_VERY_HIGH;
    gpio_handle.config.altfunc = 0x08;

    gpio_handle.port = GPIOC;
    gpio_handle.pin = 12;
    gpio_init(&gpio_handle);

    gpio_handle.port = GPIOD;
    gpio_handle.pin = 2;
    gpio_init(&gpio_handle);
    SEGGER_RTT_printf(0, "GPIO configured \r\n");

    UART5_CLK_ENABLE();
    uart_handle.uart = UART5;
    uart_handle.config.mode = UART_MODE_TX_RX;
    uart_handle.config.wordlength = UART_WORDLENGTH_8BIT;
    uart_handle.config.baudrate = UART_BAUDRATE_115200;
    uart_handle.config.stopbits = UART_STOPBITS_1BIT;
    uart_handle.config.parity = UART_PARITY_NONE;
    uart_handle.config.oversampling = UART_OVERSAMPLING_16;
    uart_handle.state = UART_STATE_READY;
    uart_handle.rx_cmpl_cb = receive_complete_callback;

    uart_init(&uart_handle);
    uart_enable_interrupt(UART5_IRQn);
    SEGGER_RTT_printf(0, "UART5 configured \r\n");
}

void UART5_IRQHandler(void)
{
    uart_interrupt_handler(&uart_handle);
}

int main(void)
{
    setup();

    while(1) {
        uart_receive(&uart_handle, buffer, 1);
    }
}

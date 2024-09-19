#include "gpio.h"
#include "uart.h"

#define LED_PORT GPIOG
#define LED_PIN 13

#define LED_ON 0x01
#define LED_OFF 0x00

gpio_handle_t led;
gpio_handle_t gpio_handle;
uart_handle_t uart_handle;

uint8_t buffer[2];

static void receive_complete_callback(void)
{
    // echo back the received data
    uart_transmit(&uart_handle, buffer, 1);

    switch (buffer[0])
    {
    case LED_ON:
        gpio_write(&led, 1);
        break;
    case LED_OFF:
        gpio_write(&led, 0);
        break;
    }
}

void setup()
{
    // LED configuration and initialization
    led.port = LED_PORT;
    led.pin = LED_PIN;
    led.config.mode = GPIO_MODE_OUTPUT;
    led.config.optype = GPIO_OPTYPE_PUSH_PULL;
    led.config.pull = GPIO_PULL_NO;
    led.config.speed = GPIO_SPEED_MEDIUM;

    GPIOG_CLK_ENABLE();
    gpio_init(&led);

    // UART configuration and initialization
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
}

void uart5_irq_handler(void)
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

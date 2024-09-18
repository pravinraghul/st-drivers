
#include "gpio.h"
#include "SEGGER_RTT.h"

#define BUTTON_PORT GPIOA
#define BUTTON_PIN 0

#define LED_PORT GPIOG
#define LED_PIN 13

gpio_handle_t led;
gpio_handle_t button;

void EXTI0_IRQHandler(void) {
    gpio_clear_interrupt(&button);

    if (gpio_read(&button)) {
        SEGGER_RTT_printf(0, "Button pressed \r\n");
        gpio_write(&led, 1);
    } else {
        SEGGER_RTT_printf(0, "Button released \r\n");
        gpio_write(&led, 0);
    }
}

int main() {

    // LED configuration and initialization
    led.port = LED_PORT;
    led.pin = LED_PIN;
    led.config.mode = GPIO_MODE_OUTPUT;
    led.config.optype = GPIO_OPTYPE_PUSH_PULL;
    led.config.pull = GPIO_PULL_NO;
    led.config.speed = GPIO_SPEED_MEDIUM;

    GPIOG_CLK_ENABLE();
    gpio_init(&led);

    // Button configuration and initialization
    button.port = BUTTON_PORT;
    button.pin = BUTTON_PIN;

    GPIOA_CLK_ENABLE();
    gpio_config_interrupt(&button, GPIO_EDGE_RISING_AND_FALLING);
    gpio_enable_interrupt(&button, EXTI0_IRQn);

    SEGGER_RTT_printf(0, "GPIO LED & button configured \r\n");

    while(1);

    return 0;
}

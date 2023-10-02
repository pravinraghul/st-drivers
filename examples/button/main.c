#include "gpio.h"

#define BUTTON_PORT GPIOA
#define BUTTON_PIN 0

#define LED_PORT GPIOG
#define LED_PIN 13

void EXTI0_IRQHandler(void) {
    gpio_clear_interrupt(BUTTON_PIN);

    if (gpio_read(BUTTON_PORT, BUTTON_PIN))
        gpio_write(LED_PORT, LED_PIN, 1);
    else
        gpio_write(LED_PORT, LED_PIN, 0);
}

int main() {
    GPIO_Config led_config;
    led_config.mode = GPIO_MODE_OUTPUT;
    led_config.optype = GPIO_OPTYPE_PUSH_PULL;
    led_config.pull = GPIO_PULL_NO;
    led_config.speed = GPIO_SPEED_MEDIUM;

    GPIOA_CLK_ENABLE();
    GPIOG_CLK_ENABLE();

    gpio_init(LED_PORT, LED_PIN, &led_config);
    gpio_config_interrupt(BUTTON_PIN, GPIO_EDGE_RISING_AND_FALLING);
    gpio_enable_interrupt(BUTTON_PIN, EXTI0_IRQn);

    while(1);

    return 0;
}

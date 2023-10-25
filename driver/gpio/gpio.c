#include "gpio.h"

void gpio_init(gpio_handle_t *gpio)
{
    gpio->port->MODER |= (gpio->config.mode << (2 * gpio->pin));
    gpio->port->OTYPER |= (gpio->config.optype << gpio->pin);
    gpio->port->OSPEEDR |= (gpio->config.speed << (2 * gpio->pin));
    gpio->port->PUPDR |= (gpio->config.pull << (2 * gpio->pin));

    if (gpio->pin < 8) {
        gpio->port->AFR[0] |= (gpio->config.altfunc << (gpio->pin * 4));
    } else {
        gpio->port->AFR[1] |= (gpio->config.altfunc << ((gpio->pin % 8) * 4));
    }
}

uint8_t gpio_read(gpio_handle_t *gpio)
{
    return (gpio->port->IDR >> gpio->pin) & 1;
}

void gpio_write(gpio_handle_t *gpio, uint8_t value)
{
    if (value)
        gpio->port->ODR |= (1 << gpio->pin);
    else 
        gpio->port->ODR &= ~(1 << gpio->pin);
}

void gpio_config_interrupt(gpio_handle_t *gpio, uint8_t edge)
{
    if (edge == GPIO_EDGE_RISING) {
        EXTI->RTSR |= (1 << gpio->pin);
    } else if (edge == GPIO_EDGE_FALLING) {
        EXTI->FTSR |= (1 << gpio->pin);
    } else if (edge == GPIO_EDGE_RISING_AND_FALLING){
        EXTI->RTSR |= (1 << gpio->pin);
        EXTI->FTSR |= (1 << gpio->pin);
    } else {
        // do nothing
    }
}

void gpio_enable_interrupt(gpio_handle_t *gpio, IRQn_Type irq_no)
{
    EXTI->IMR |= (1 << gpio->pin);
    NVIC_EnableIRQ(irq_no);
}

void gpio_clear_interrupt(gpio_handle_t *gpio)
{
    if (EXTI->PR & (1 << gpio->pin)) {
        EXTI->PR |= (1 << gpio->pin);
    }
}

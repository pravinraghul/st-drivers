#include "gpio.h"

void gpio_init(GPIO_TypeDef *portx, uint16_t pin, GPIO_Config *config)
{
    portx->MODER |= (config->mode << (2 * pin));
    portx->OTYPER |= (config->optype << pin);
    portx->OSPEEDR |= (config->speed << (2 * pin));
    portx->PUPDR |= (config->pull << (2 * pin));

    if (pin < 8) {
        portx->AFR[0] |= (config->altfunc << (pin * 4));
    } else {
        portx->AFR[1] |= (config->altfunc << ((pin % 8) * 4));
    }
}

uint8_t gpio_read(GPIO_TypeDef *portx, uint16_t pin)
{
    return (portx->IDR >> pin) & 1U;
}

void gpio_write(GPIO_TypeDef *portx, uint16_t pin, uint8_t value) 
{
    if (value)
        portx->ODR |= (1 << pin);
    else 
        portx->ODR &= ~(1 << pin);
}

void gpio_config_interrupt(uint16_t pin, uint8_t edge)
{
    if (edge == GPIO_EDGE_RISING) {
        EXTI->RTSR |= 1 << pin;
    } else if (edge == GPIO_EDGE_FALLING) {
        EXTI->FTSR |= 1 << pin;
    } else if (edge == GPIO_EDGE_RISING_AND_FALLING){
        EXTI->RTSR |= 1 << pin;
        EXTI->FTSR |= 1 << pin;
    } else {
        // do nothing
    }
}

void gpio_enable_interrupt(uint16_t pin, IRQn_Type irq_no)
{
    EXTI->IMR |= 1 << pin;
    NVIC_EnableIRQ(irq_no);
}

void gpio_clear_interrupt(uint16_t pin)
{
    if (EXTI->PR & (1 << pin)) {
        EXTI->PR |= (1 << pin);
    }
}

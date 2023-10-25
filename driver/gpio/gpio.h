#ifndef GPIO_H_
#define GPIO_H_

#include <stm32f4xx.h>

/* Mode configuration */
#define GPIO_MODE_INPUT 0
#define GPIO_MODE_OUTPUT 1
#define GPIO_MODE_ALTFUNC 2
#define GPIO_MODE_ANALOG 3

/* Output type configuration */
#define GPIO_OPTYPE_PUSH_PULL 0
#define GPIO_OPTYPE_OPEN_DRAIN  1

/* Speed configuration */
#define GPIO_SPEED_LOW 0
#define GPIO_SPEED_MEDIUM 1
#define GPIO_SPEED_HIGH 2
#define GPIO_SPEED_VERY_HIGH 3

/* Pull configuration */
#define GPIO_PULL_NO 0
#define GPIO_PULL_UP 1
#define GPIO_PULL_DOWN 2

/* Edge Trigger */
#define GPIO_EDGE_RISING 0
#define GPIO_EDGE_FALLING 1
#define GPIO_EDGE_RISING_AND_FALLING 2

/* Clock configuration */
#define GPIOA_CLK_ENABLE() (RCC->AHB1ENR |= (1 << 0))
#define GPIOB_CLK_ENABLE() (RCC->AHB1ENR |= (1 << 1))
#define GPIOC_CLK_ENABLE() (RCC->AHB1ENR |= (1 << 2))
#define GPIOD_CLK_ENABLE() (RCC->AHB1ENR |= (1 << 3))
#define GPIOE_CLK_ENABLE() (RCC->AHB1ENR |= (1 << 4))
#define GPIOF_CLK_ENABLE() (RCC->AHB1ENR |= (1 << 5))
#define GPIOG_CLK_ENABLE() (RCC->AHB1ENR |= (1 << 6))
#define GPIOH_CLK_ENABLE() (RCC->AHB1ENR |= (1 << 7))

typedef struct
{
    uint32_t mode;
    uint32_t optype;
    uint32_t pull;
    uint32_t speed;
    uint32_t altfunc;
} GPIO_Config;

void gpio_init(GPIO_TypeDef *gpiox, uint16_t pin, GPIO_Config *config);
uint8_t gpio_read(GPIO_TypeDef *gpiox, uint16_t pin);
void gpio_write(GPIO_TypeDef *gpiox, uint16_t pin, uint8_t value);

void gpio_config_interrupt(uint16_t pin, uint8_t edge);
void gpio_enable_interrupt(uint16_t pin, IRQn_Type irq_no);
void gpio_clear_interrupt(uint16_t pin);

#endif /* GPIO_H_ */

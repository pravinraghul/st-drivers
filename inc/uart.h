#ifndef UART_H_
#define UART_H_

#include <stm32f4xx.h>

#define UART_BAUDRATE_9600 0x683
#define UART_BAUDRATE_115200 0x8a

#define UART_WORDLENGTH_8BIT 0
#define UART_WORDLENGTH_9BIT (USART_CR1_M)

#define UART_STOPBITS_1BIT 0
#define UART_STOPBITS_2BIT (2 << USART_CR2_STOP)

#define UART_PARITY_NONE 0
#define UART_PARITY_EVEN (USART_CR1_PCE)
#define UART_PARITY_ODD (USART_CR1_PCE | USART_CR1_PS)

#define UART_MODE_TX_ONLY (USART_CR1_TE)
#define UART_MODE_RX_ONLY (USART_CR1_RE)
#define UART_MODE_TX_RX (USART_CR1_TE | USART_CR1_RE)

#define UART_OVERSAMPLING_16 0
#define UART_OVERSAMPLING_8 (USART_CR1_0VER8)

#define UART1_CLK_ENABLE() (RCC->APB2ENR |= RCC_APB2ENR_USART1EN)
#define UART2_CLK_ENABLE() (RCC->APB1ENR |= RCC_APB1ENR_USART2EN)
#define UART3_CLK_ENABLE() (RCC->APB1ENR |= RCC_APB1ENR_USART3EN)
#define UART4_CLK_ENABLE() (RCC->APB1ENR |= RCC_APB1ENR_UART4EN)
#define UART5_CLK_ENABLE() (RCC->APB1ENR |= RCC_APB1ENR_UART5EN)
#define UART6_CLK_ENABLE() (RCC->APB2ENR |= RCC_APB2ENR_USART6EN)

typedef void (*transmit_complete_callback_t) (void);
typedef void (*receive_complete_callback_t) (void);

typedef enum {
UART_STATE_RESET,
UART_STATE_READY,
UART_STATE_TXBUSY,
UART_STATE_RXBUSY
} uart_state_t;

typedef struct {
    uint32_t baudrate;
    uint32_t wordlength;
    uint32_t stopbits;
    uint32_t parity;
    uint32_t mode;
    uint32_t oversampling;
} uart_config_t;

typedef struct {
    USART_TypeDef *uart;
    uart_config_t config;
    uint8_t *txbuf;
    uint16_t txlen;
    uint8_t *rxbuf;
    uint16_t rxlen;
    uart_state_t state;
    transmit_complete_callback_t tx_cmpl_cb;
    receive_complete_callback_t rx_cmpl_cb;
} uart_handle_t;

void uart_init(uart_handle_t *handle);
void uart_transmit(uart_handle_t *handle, uint8_t *tbuf, uint16_t len);
void uart_receive(uart_handle_t *handle, uint8_t *rbuf, uint16_t len);

void uart_enable_interrupt(IRQn_Type irq_no);
void uart_interrupt_handler(uart_handle_t *handle);

#endif // UART_H_

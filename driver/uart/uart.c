#include "uart.h"

void uart_init(uart_handle_t *handle)
{
    handle->uart->BRR |= handle->config.baudrate;

    handle->uart->CR2 |= handle->config.stopbits;

    handle->uart->CR1 |= handle->config.mode | handle->config.wordlength |
        handle->config.parity | handle->config.oversampling ;

    handle->uart->CR1 |= USART_CR1_UE;
}

void uart_transmit(uart_handle_t *handle, uint8_t *txbuf, uint16_t len)
{
    handle->txbuf = txbuf;
    handle->txlen = len;
    handle->state = UART_STATE_TXBUSY;

    // enable transmit interrupt
    handle->uart->CR1 |= USART_CR1_TXEIE;
}

void uart_receive(uart_handle_t *handle, uint8_t *rxbuf, uint16_t len)
{
    handle->rxbuf = rxbuf;
    handle->rxlen = len;
    handle->state = UART_STATE_RXBUSY;

    // enable receive interrupt
    handle->uart->CR1 |= USART_CR1_RXNEIE;
}

void uart_enable_interrupt(IRQn_Type irq_no)
{
    NVIC_EnableIRQ(irq_no);
}

static void uart_handle_transmit_interrupt(uart_handle_t *handle)
{
    if (handle->state == UART_STATE_TXBUSY) {
        handle->uart->DR = (uint8_t) *handle->txbuf;
        handle->txbuf++;
        handle->txlen--;

        if (handle->txlen == 0) {
            // clear transmit interrupt
            handle->uart->CR1 &= ~USART_CR1_TXEIE;
            // set transmit complete interrupt
            handle->uart->CR1 |= USART_CR1_TCIE;
        }
    }
}

static void uart_handle_transmit_complete_interrupt(uart_handle_t *handle)
{
    // clear transmit complete interrupt
    handle->uart->CR1 &= ~USART_CR1_TCIE;
    handle->state = UART_STATE_READY;

    if (handle->tx_cmpl_cb)
        handle->tx_cmpl_cb();
}

static void uart_handle_receive_interrupt(uart_handle_t *handle)
{
    if (handle->state == UART_STATE_RXBUSY) {

        if (handle->config.parity == UART_PARITY_NONE)
            *handle->rxbuf = (uint8_t) handle->uart->DR;
        else
            *handle->rxbuf = (uint8_t) (handle->uart->DR & 0x007F);

        handle->rxlen--;
        if (handle->rxlen == 0) {
            // clear receive interrupt
            handle->uart->CR1 &= ~USART_CR1_RXNEIE;
            handle->state = UART_STATE_READY;

            if (handle->rx_cmpl_cb)
                handle->rx_cmpl_cb();
        }
    }
}

void uart_interrupt_handler(uart_handle_t *handle)
{
    // transmit interrupt
    if ((handle->uart->SR & USART_SR_TXE) &&
        (handle->uart->CR1 & USART_CR1_TXEIE)) {
        uart_handle_transmit_interrupt(handle);
    }

    // transmit complete interrupt
    if ((handle->uart->SR & USART_SR_TC) &&
        (handle->uart->CR1 & USART_CR1_TCIE)) {
        uart_handle_transmit_complete_interrupt(handle);
    }

    // receive interrupt
    if ((handle->uart->SR & USART_SR_RXNE) &&
        (handle->uart->CR1 & USART_CR1_RXNEIE)) {
        uart_handle_receive_interrupt(handle);
    }
}

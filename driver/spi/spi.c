#include "spi.h"
#include "SEGGER_RTT.h"

void spi_init(spi_handle_t *handle)
{
    handle->spi->CR1 |= handle->config.mode | handle->config.direction | \
        handle->config.datasize | handle->config.clkpolarity |          \
        handle->config.clkphase | handle->config.baudrate |             \
        handle->config.firstbit | handle->config.nss;
}

void spi_transmit_data(spi_handle_t *handle, uint8_t *buf, uint16_t len)
{
    handle->txbuf = buf;
    handle->txlen = len;
    handle->state = SPI_STATE_TXBUSY;

    if (!(handle->spi->CR1 & SPI_CR1_SPE))
        handle->spi->CR1 |= SPI_CR1_SPE;

    handle->spi->CR2 |= SPI_CR2_TXEIE;
}

void spi_receive_data(spi_handle_t *handle, uint8_t *buf, uint16_t len)
{
    handle->txbuf = buf;
    handle->txlen = len;
    handle->state = SPI_STATE_RXBUSY;

    if (!(handle->spi->CR1 & SPI_CR1_SPE))
        handle->spi->CR1 |= SPI_CR1_SPE;

    handle->spi->CR2 |= SPI_CR2_RXNEIE;
}

void spi_transmit_receive_data(spi_handle_t *handle, uint8_t *txbuf, uint8_t *rxbuf, uint16_t len)
{
    handle->txbuf = txbuf;
    handle->txlen = len;
    handle->rxbuf = rxbuf;
    handle->rxlen = len;
    handle->state = SPI_STATE_RXBUSY;

    if ((handle->spi->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
        handle->spi->CR1 |= SPI_CR1_SPE;

    handle->spi->CR2 |= (SPI_CR2_RXNEIE | SPI_CR2_TXEIE);
}

void spi_enable_interrupt(IRQn_Type irq_no)
{
    NVIC_EnableIRQ(irq_no);
}

static void spi_handle_tx_interrupt(spi_handle_t *handle)
{
    if (handle->config.datasize == SPI_DATASIZE_8BIT) {
        // 8 bit
        *((__IO uint8_t *)&handle->spi->DR) = (*(uint8_t *)handle->txbuf);
        handle->txbuf++;
        handle->txlen--;
    } else {
        // 16 bit
        *((__IO uint16_t *)&handle->spi->DR) = (*(uint16_t *)handle->txbuf);
        handle->txbuf += 2;
        handle->txlen -= 2;
    }

    if (handle->txlen == 0) {
        handle->spi->CR2 &= ~SPI_CR2_TXEIE;
        if (handle->state != SPI_STATE_RXBUSY)
            handle->state = SPI_STATE_READY;
    }
}

static void spi_handle_rx_interrupt(spi_handle_t *handle)
{
    if (handle->config.datasize == SPI_DATASIZE_8BIT) {
        // 8 bit
        (*(uint8_t *)handle->rxbuf) = *((__IO uint8_t *)&handle->spi->DR);
        handle->rxbuf++;
        handle->rxlen--;
    } else {
        // 16 bit
        (*(uint16_t *)handle->rxbuf) = *((__IO uint16_t *)&handle->spi->DR);
        handle->rxbuf += 2;
        handle->rxlen -= 2;
    }

    if (handle->rxlen == 0) {
        handle->spi->CR2 &= ~SPI_CR2_RXNEIE;
        handle->state = SPI_STATE_READY;
    }

}

void spi_interrupt_handler(spi_handle_t *handle)
{
    if (handle->spi->SR & SPI_SR_TXE && handle->spi->CR2 & SPI_CR2_TXEIE) {
        spi_handle_tx_interrupt(handle);
    }

    // rx interrupt
    if (handle->spi->SR & SPI_SR_RXNE && handle->spi->CR2 & SPI_CR2_RXNEIE) {
        spi_handle_rx_interrupt(handle);
    }
}

#ifndef SPI_H_
#define SPI_H_

#include <stm32f4xx.h>
#include <stddef.h>

/** SPI Configurations Macros **/

#define SPI_MODE_SLAVE 0
#define SPI_MODE_MASTER (SPI_CR1_MSTR | SPI_CR1_SSI)

#define SPI_DIRECTION_2LINE 0
#define SPI_DIRECTION_1LINE (SPI_CR1_BIDIOE)

#define SPI_DATASIZE_8BIT 0
#define SPI_DATASIZE_16BIT (SPI_CR1_DFF)

#define SPI_POLARITY_LOW 0
#define SPI_POLARITY_HIGH (SPI_CR1_CPOL)

#define SPI_PHASE_1EDGE 0
#define SPI_PHASE_2EDGE (SPI_CR1_CPHA)

#define SPI_SSM_DISABLE 0
#define SPI_SSM_ENABLE (SPI_CR1_SSM)

#define SPI_BAUDRATE_PCLK_DIV_2 0
#define SPI_BAUDRATE_PCLK_DIV_4 (SPI_CR1_BR_0)
#define SPI_BAUDRATE_PCLK_DIV_8 (SPI_CR1_BR_1)
#define SPI_BAUDRATE_PCLK_DIV_16 (SPI_CR1_BR_1 | SPI_CR1_BR_0)
#define SPI_BAUDRATE_PCLK_DIV_32 (SPI_CR1_BR_2)
#define SPI_BAUDRATE_PCLK_DIV_64 (SPI_CR1_BR_2 | SPI_CR1_BR_0)
#define SPI_BAUDRATE_PCLK_DIV_128 (SPI_CR1_BR_2 | SPI_CR1_BR_1)
#define SPI_BAUDRATE_PCLK_DIV_256 (SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0)

#define SPI_FIRSTBIT_MSB 0
#define SPI_FIRSTBIT_LSB (SPI_CR1_LSBFIRST)

#define SPI1_CLK_ENABLE() (RCC->APB2ENR |= RCC_APB2ENR_SPI1EN)
#define SPI2_CLK_ENABLE() (RCC->APB1ENR |= RCC_APB1ENR_SPI2EN)
#define SPI3_CLK_ENABLE() (RCC->APB1ENR |= RCC_APB1ENR_SPI3EN)
#define SPI4_CLK_ENABLE() (RCC->APB2ENR |= RCC_APB2ENR_SPI4EN)
#define SPI5_CLK_ENABLE() (RCC->APB2ENR |= RCC_APB2ENR_SPI5EN)
#define SPI6_CLK_ENABLE() (RCC->APB2ENR |= RCC_APB2ENR_SPI6EN)

typedef void (*transmit_complete_callback_t) (void);
typedef void (*receive_complete_callback_t) (void);

/** SPI states **/
typedef enum {
SPI_STATE_RESET,
SPI_STATE_READY,
SPI_STATE_TXBUSY,
SPI_STATE_RXBUSY
} spi_state_t ;

/** SPI configurations **/
typedef struct {
    uint32_t mode;
    uint32_t direction;
    uint32_t datasize;
    uint32_t clkpolarity;
    uint32_t clkphase;
    uint32_t nss;
    uint32_t baudrate;
    uint32_t firstbit;
} spi_config_t;

/** SPI handle **/
typedef struct {
    SPI_TypeDef *spi;
    spi_config_t config;
    spi_state_t state;
    uint8_t *txbuf;
    uint16_t txlen;
    uint8_t *rxbuf;
    uint16_t rxlen;
    transmit_complete_callback_t tx_cmpl_cb;
    receive_complete_callback_t rx_cmpl_cb;
} spi_handle_t;

void spi_init(spi_handle_t *handle);
void spi_transmit(spi_handle_t *handle, uint8_t *buf, uint16_t len);
void spi_receive(spi_handle_t *handle, uint8_t *buf, uint16_t len);
void spi_transmit_receive(spi_handle_t *handle, uint8_t *tbuf, uint8_t *rbuf, uint16_t len);

void spi_enable_interrupt(IRQn_Type irq_no);
void spi_interrupt_handler(spi_handle_t *handle);

#endif /* SPI_H_ */

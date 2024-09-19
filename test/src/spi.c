
#include "gpio.h"
#include "spi.h"

#include "SEGGER_RTT.h"

gpio_handle_t gpio_handle;
spi_handle_t spi_handle;

uint8_t tbuffer[4] = {0x12, 0x23, 0x34, 0x45};
uint8_t rbuffer[4] = {0};
uint8_t value = 1;

void receive_callback(void)
{
    // check the message
    for (int i = 0; i < 4; i++) {
        SEGGER_RTT_printf(0, "tbuffer[%d] = %d, rbuffer[%d] = %d\r\n", i, tbuffer[i], i, rbuffer[i]);
        if (tbuffer[i] != rbuffer[i]) {
            value = 0;
            break;
        }
    }

    if (value) {
        SEGGER_RTT_printf(0, "SPI loopback passed...! \r\n");
    } else {
        SEGGER_RTT_printf(0, "SPI loopback failed...! \r\n");
    }
    SEGGER_RTT_printf(0, "\r\n \r\n \r\n");
}

void setup()
{
    // GPIO configurations
    GPIOE_CLK_ENABLE();

    gpio_handle.port = GPIOE;
    gpio_handle.config.mode = GPIO_MODE_ALTFUNC;
    gpio_handle.config.pull = GPIO_PULL_UP;
    gpio_handle.config.speed = GPIO_SPEED_VERY_HIGH;
    gpio_handle.config.altfunc = 0x05;

    gpio_handle.pin = 2;
    gpio_init(&gpio_handle);

    gpio_handle.pin = 5;
    gpio_init(&gpio_handle);

    gpio_handle.pin = 6;
    gpio_init(&gpio_handle);

    // SPI configurations
    SPI4_CLK_ENABLE();

    spi_handle.spi = SPI4;
    spi_handle.config.baudrate = SPI_BAUDRATE_PCLK_DIV_2;
    spi_handle.config.direction = SPI_DIRECTION_2LINE;
    spi_handle.config.clkphase = SPI_PHASE_1EDGE;
    spi_handle.config.clkpolarity = SPI_POLARITY_LOW;
    spi_handle.config.datasize = SPI_DATASIZE_8BIT;
    spi_handle.config.firstbit = SPI_FIRSTBIT_MSB;
    spi_handle.config.nss = SPI_SSM_ENABLE;
    spi_handle.config.mode = SPI_MODE_MASTER;
    spi_handle.state = SPI_STATE_READY;
    spi_handle.rx_cmpl_cb = receive_callback;

    spi_init(&spi_handle);
    spi_enable_interrupt(SPI4_IRQn);
}

// SPI4 IRQHandler
void spi4_irq_handler(void)
{
    spi_interrupt_handler(&spi_handle);
}

int main()
{
    setup();
    SEGGER_RTT_printf(0, "SPI setup done..! \r\n");

    // some delay
    for (uint32_t i = 0; i < 1000 * 1000; i++);
    // some delay
    for (uint32_t i = 0; i < 1000 * 1000; i++);
    // some delay
    for (uint32_t i = 0; i < 1000 * 1000; i++);

    spi_transmit_receive(&spi_handle, tbuffer, rbuffer, 4);

    /* while (1) { */
    /*     spi_transmit_receive(&spi_handle, tbuffer, rbuffer, 4); */
    /*     while(spi_handle.state != SPI_STATE_READY); */

    /*     // check the message */
    /*     for (int i = 0; i < 4; i++) { */
    /*         SEGGER_RTT_printf(0, "tbuffer[%d] = %d, rbuffer[%d] = %d\r\n", i, tbuffer[i], i, rbuffer[i]); */
    /*         if (tbuffer[i] != rbuffer[i]) { */
    /*             value = 0; */
    /*             break; */
    /*         } */
    /*     } */

    /* } */
    return 0;
}

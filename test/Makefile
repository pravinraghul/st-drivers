CC=arm-none-eabi-gcc
CP=arm-none-eabi-objcopy
MARCH=cortex-m4
CFLAGS= -mcpu=$(MARCH) -mthumb -std=gnu11 -Wall -o0
CFLAGS += -DSTM32F429xx
CFLAGS += 					\
-I../inc 					\
-ICMSIS/Include 			\
-IDevice/STM32F4xx/Include
LDFLAGS = -nostdlib -T cortex-m-startup/linker.ld -Wl,-Map=build/test.map

all:

build:
	mkdir build

test-gpio: build build/gpio.hex

test-spi: build build/spi.hex

test-uart: build build/uart.hex

build/%_src.o: ../src/%.c
	$(CC) -c $(CFLAGS) -o $@ $<

build/%_test.o: src/%.c
	$(CC) -c $(CFLAGS) -o $@ $<

build/startup.o: cortex-m-startup/startup.c
	$(CC) -c $(CFLAGS) -o $@ $<

build/spi.elf: build/startup.o build/spi_src.o build/spi_test.o build/gpio_src.o
	$(CC) $(LDFLAGS) -o $@ $^

build/uart.elf: build/startup.o build/gpio_src.o build/uart_src.o build/uart_test.o
	$(CC) $(LDFLAGS) -o $@ $^

build/gpio.elf: build/startup.o build/gpio_src.o build/gpio_test.o
	$(CC) $(LDFLAGS) -o $@ $^

build/%.hex: build/%.elf
	$(CP) -O ihex $^ $@

clean:
	rm -rf build

.PHONY: test-gpio test-spi test-uart clean

# keep the build files
.PRECIOUS: build/%_src.o build/%_test.o build/%.elf
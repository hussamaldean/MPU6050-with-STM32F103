#ifndef __uart__h
#define __uart__h
#include "stm32f10x.h"                  // Device header
#include "stdbool.h"
#include "rcc.h"
#include "string.h"
void uart1_gpio_init(void);
void uart1_config();
bool uart1_transmite(uint8_t *data, uint16_t len, uint16_t timeout);
bool uart1_receive(uint8_t *data, uint16_t len, uint16_t timeout);

void UART1_print(unsigned char *p);


void uart_UART1_DMA_config(void);

/**
 * @brief UART1 DMA Transmit
 */
void uart_UART1_DMA_transmit(uint8_t *data, uint8_t len);

/**
 * @brief UART1 DMA Receive
 */
void uart_UART1_DMA_receive(uint8_t *data, uint16_t len);

#endif

#ifndef __rcc__h
#define __rcc__h
#include "stm32f10x.h"                  // Device header
/**
*@brief HSE confoguration
*/
void rcc_HSE_config(void);

/**
*@brief Systick configuration
*/
void rcc_systick_config(uint32_t arr);

uint64_t millis(void);
void delay(uint64_t ms);

#endif

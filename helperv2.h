#ifndef __helperv2__h
#define __helperv2__h
#include "stm32f10x.h"                  // Device header
#include "UART.h"
#include "stdio.h"


void log_error(char *p);
void log_info(char *p);
void log_debug(char *p);
void log_debug_array(char const * const label, void const *array, uint16_t const len);

#endif


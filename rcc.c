#include "rcc.h"
volatile uint64_t ms=0;
volatile uint64_t msr;


void rcc_HSE_config(void)
{
  /*
   * Configuration paramters
   *
   * HSE = 8MHz
   * PLL_M = 9
   * USB prescalar = 1.5
   * AHB prescalar = 1
   * Cortex precalar = 1
   * --> 72MHz System Clock
   *
   * APB1 prescalar = 2  --> 36, 72 MHz
   * APB2 prescalar = 1  --> 36, 36 MHz
   * ADC prescalr = 6    --> 12MHz
   */

  //PLL Configuration
  //PLL_MUL = 9
  RCC->CFGR &= ~RCC_CFGR_PLLMULL; //Clear
  RCC->CFGR |= (0x7UL << RCC_CFGR_PLLMULL);
  //USB Prescaler = 1.5
  RCC->CFGR &= ~(0x1UL << 22);

  //HSE Enable and Set as PLL source
  //Enable HSE Clock
  RCC->CR |= RCC_CR_HSEON;
  //Wait for HSE to be stable and ready to use
  while((RCC->CR & RCC_CR_HSERDY) == 0);
  //HSE as PLL source
  RCC->CFGR |= RCC_CFGR_PLLSRC;
  //Enable the PLL
  RCC->CR |= RCC_CR_PLLON;
  //Wait untill PLL is ready
  while((RCC->CR & RCC_CR_PLLRDY) == 0);

  //Flash pre-fetch enable and wait-state=2
  //0WS: 0-24MHz
  //1WS: 24-48MHz
  //2WS: 48-72MHz
  FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY_1;
  //Select PLL as main System Clock source
  RCC->CFGR &= ~RCC_CFGR_SW;
  RCC->CFGR |= RCC_CFGR_SW_1;
  //Wait until PLL system source is active
  while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_1);
  //AHB Prescaler = 1
  RCC->CFGR &= ~RCC_CFGR_HPRE;
  //APB1 divider=2
  RCC->CFGR &= ~RCC_CFGR_PPRE1;
  RCC->CFGR |= RCC_CFGR_PPRE1_2;
  //APB2 divider=1
  RCC->CFGR &= ~RCC_CFGR_PPRE2;
  //ADC prescalar = 12
  RCC->CFGR &= ~RCC_CFGR_ADCPRE;
  RCC->CFGR |= RCC_CFGR_ADCPRE_1;
}



void rcc_systick_config(uint32_t arr)
{
__disable_irq();
	SysTick->LOAD=arr-1;
	SysTick->VAL=0;
	SysTick->CTRL=7; //0b00000111;
	NVIC_SetPriority(SysTick_IRQn,7);
	__enable_irq();
}
void SysTick_Handler(void){
ms++;
}

uint64_t millis(void)
{
__disable_irq();
msr=ms;
__enable_irq();
return msr;
}

void delay(uint64_t ms)
{

uint64_t start=millis();
while(millis()-start!=ms){;}	
	


//	SysTick->LOAD=72000-1;
//	SysTick->VAL=0;
//	SysTick->CTRL=0x5;
//		for (int i=0;i<ms;i++)
//		{
//			while(!(SysTick->CTRL &0x10000)){}
//		}
//	SysTick->CTRL=0;
}
	
	



void rcc_hse_config(void)
{
	
		RCC->CFGR&=~(RCC_CFGR_PLLMULL);
		RCC->CFGR|=(7UL<<18);
		RCC->CFGR&=~(RCC_CFGR_USBPRE);
		RCC->CR|=RCC_CR_HSEON;
		while((RCC->CR&RCC_CR_HSERDY)==0){;}
		RCC->CFGR|=RCC_CFGR_PLLSRC;
		RCC->CR|=RCC_CR_PLLON;
		while((RCC->CR&RCC_CR_PLLRDY)==0){;}
		FLASH->ACR&=~(FLASH_ACR_LATENCY);
		FLASH->ACR|=FLASH_ACR_LATENCY_2;
		FLASH->ACR|=FLASH_ACR_PRFTBE;
		RCC->CFGR&=~(RCC_CFGR_SW);
		RCC->CFGR|=RCC_CFGR_SW_1;
		while((RCC->CFGR&(1<<3))==0){;}
		RCC->CFGR&=~RCC_CFGR_PPRE1;
		RCC->CFGR|=RCC_CFGR_PPRE1_2;
		RCC->CFGR&=~RCC_CFGR_PPRE2;
		RCC->CFGR &=~(RCC_CFGR_ADCPRE);
		RCC->CFGR |=RCC_CFGR_ADCPRE_DIV8;
		SystemCoreClockUpdate();
		
}
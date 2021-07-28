#include "UART.h"

void uart1_gpio_init(void)
	{
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_IOPAEN; 
		GPIOA->CRH |= GPIO_CRH_MODE9 | GPIO_CRH_CNF9_1;
		GPIOA->CRH &= ~(GPIO_CRH_CNF9_0);

	}
	
	
void uart1_config()
	{
		RCC->APB2ENR	|=	RCC_APB2ENR_USART1EN;
		USART1->BRR		 =  0x271;
		USART1->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
	}
	
	
bool uart1_transmite(uint8_t *data, uint16_t len, uint16_t timeout)

{
uint8_t dataIdx=0;
uint64_t startTick=millis();

while(dataIdx<len)
{

	if(USART1->SR&USART_SR_TXE) // TX buffer is empty
			{
			USART1->DR=data[dataIdx];
				dataIdx++;
			}
		else 
				{
				if((millis()-startTick)>=timeout){return false;}
				}
}

while((USART1->SR)&USART_SR_TC)
		{
		if((millis()-startTick)>=timeout){return false;}
		}
return true; 
}	

int uart1_write(char f)
	{
	while(!(USART1->SR&0x0080)){}
USART1->DR=(f&0xFF);
return f;		
	}

	void UART1_print(unsigned char *p)
{
while(*p!='\0')
{
uart1_write(*p);
p++;
}
}

bool uart1_receive(uint8_t *data, uint16_t len, uint16_t timeout)
	{
	
	  uint8_t dataRemain = len;
  uint32_t startTick = millis();
  while(dataRemain > 0)
  {
    if(USART1->SR & USART_SR_RXNE)
    {
      *data++ = (uint8_t)(USART1->DR & 0xFF);
      dataRemain--;
    }
    else //Manage timeout
    {
      if((millis() - startTick)>= timeout) return false;
    }
  }
  return true;
	}
void uart_UART1_DMA_config(void)
{
  //UART DMA Tx/Rx
  USART1->CR3 |= (1UL << 7);
  USART1->CR3 |= (1UL << 6);
  /** DMA Configuration **/
  /* DMA1-Channel4 - Tx */
  //DMA1 Clock Enable
  RCC->AHBENR |= (1UL << 0);
  //DMA1_Channel4 clear flags
  DMA1->IFCR = 0xF << 4;
  //Peripheral address
  DMA1_Channel4->CPAR = (uint32_t)(&(USART1->DR));
  //Normal mode
  DMA1_Channel4->CCR &=~ (1UL << 5);
  //Enable memory increment
  DMA1_Channel4->CCR |= (1UL << 7);
  //Disable Peripheral increment
  DMA1_Channel4->CCR &= ~(1UL << 6);
  //Set peripheral data size to 8
  DMA1_Channel4->CCR &= ~(3UL << 8);
  //Set Memory data size to 8
  DMA1_Channel4->CCR &= ~(3UL << 10);
  //Direction = Mem to Periph
  DMA1_Channel4->CCR |= (1UL << 4);
  //Disable DMA Channel
  DMA1_Channel4->CCR &= ~(1UL << 0);
  //Enable Transfer Complete Interrupt
  DMA1_Channel4->CCR |= (1UL << 1);

  /* DMA1-Channel5 - Rx */
  //DMA1_Channel5 clear flags
  DMA1->IFCR = 0xF << 5;
  //Peripheral address
  DMA1_Channel5->CPAR = (uint32_t)(&(USART1->DR));
  //Normal mode
  DMA1_Channel5->CCR |= (1UL << 5);
  //Enable memory increment
  DMA1_Channel5->CCR |= (1UL << 7);
  //Disable Peripheral increment
  DMA1_Channel5->CCR &= ~(1UL << 6);
  //Set peripheral data size to 8
  DMA1_Channel5->CCR &= ~(3UL << 8);
  //Set Memory data size to 8
  DMA1_Channel5->CCR &= ~(3UL << 10);
  //Direction = Periph to Mem
  DMA1_Channel5->CCR &= ~(1UL << 4);
  //Disable DMA Channel
  DMA1_Channel5->CCR &= ~(1UL << 0);
  //Enable Transfer Complete Interrupt
  DMA1_Channel5->CCR |= (1UL << 1);

  //Enable NVIC Interrupt
  NVIC_EnableIRQ(DMA1_Channel4_IRQn);
  //NVIC_SetPriority(DMA1_Channel4_IRQn, 6);
  NVIC_EnableIRQ(DMA1_Channel5_IRQn);
  //NVIC_SetPriority(DMA1_Channel5_IRQn, 6);
}

/**
 * @brief UART1 DMA Transmit
 */
void uart_UART1_DMA_transmit(uint8_t *data, uint8_t len)
{
	DMA1_Channel4->CCR &= ~(1UL << 0);
  DMA1->IFCR = 0xFUL << 4;
  //Memory address
  DMA1_Channel4->CMAR = (uint32_t)data;
  //no. of transfers
  DMA1_Channel4->CNDTR = len;
  //Enable DMA Channel
  DMA1_Channel4->CCR |= (1UL << 0);
}

/**
 * @brief UART1 DMA Receive
 */
void uart_UART1_DMA_receive(uint8_t *data, uint16_t len)
{
  //Clear overrun error, if any
	
  __IO uint32_t readTmp;
  readTmp = USART1->SR;
  readTmp = USART1->DR;
  (void)readTmp;
	
	//DMA1_Channel5->CCR &=~(1<<0);
  //Clear DMA flags
  DMA1->IFCR = 0xFUL << 5;
  //Memory address
  DMA1_Channel5->CMAR = (uint32_t)data;
  //no. of transfers
  DMA1_Channel5->CNDTR = len;
  //Enable DMA Channel
  DMA1_Channel5->CCR |= (1UL << 0);
}

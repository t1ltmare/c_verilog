/*********************************************************
STM32F411CEU6
GPIOC Pin13 --> LED
USART1 TX -->GPIOA Pin9, RX -->GPIOA Pin10
HSI=16 MHz,APB2ENR = 16MHz
Baud rate = 115200 
Stop bit=1
no flow control
data =8 bit 		
**********************************************************/

#include "stm32f4xx.h"

void USART1_Init(void);
void USART1_Tx_Char(uint8_t c);
void USART1_Tx_String(char *s);
void delay (void);

int main (void) {
	USART1_Init();

	while (1) {
		USART1_Tx_String("Hello World! \r\n");
		delay();
	}
	return 0;
}

void USART1_Init(void){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	
	GPIOA->MODER &=~(GPIO_MODER_MODE9|GPIO_MODER_MODE10);
	GPIOA->MODER |=GPIO_MODER_MODE9_1|GPIO_MODER_MODE10_1;

	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED9 | GPIO_OSPEEDR_OSPEED10;
	
	GPIOA->AFR[1] &= ~(GPIO_AFRH_AFSEL9|GPIO_AFRH_AFSEL10);
	GPIOA->AFR[1]|=	GPIO_AFRH_AFSEL9_0|
					GPIO_AFRH_AFSEL9_1|
					GPIO_AFRH_AFSEL9_2|		
					GPIO_AFRH_AFSEL10_0|
					GPIO_AFRH_AFSEL10_1|
					GPIO_AFRH_AFSEL10_2;	 
	
	USART1->CR1|= USART_CR1_OVER8|USART_CR1_TE|USART_CR1_RE;
		
	USART1->BRR &=~USART_BRR_DIV_Mantissa;
	USART1->BRR|=(17 << USART_BRR_DIV_Mantissa_Pos);
	USART1->BRR &=~USART_BRR_DIV_Fraction;
	USART1->BRR|=(3 << USART_BRR_DIV_Fraction_Pos);
	
	USART1->CR1|= USART_CR1_UE;				
}


void USART1_Tx_Char(uint8_t c){
	while (!(USART1->SR & USART_SR_TC));
	USART1->DR=(c);
	}

void USART1_Tx_String(char *s)	{
	while(*s!='\0')
	{
	USART1_Tx_Char(*s);
	s++;
		}
	}

void delay (void){
	for (uint32_t i=0; i < 4000000; i++)
		__asm("nop");
}

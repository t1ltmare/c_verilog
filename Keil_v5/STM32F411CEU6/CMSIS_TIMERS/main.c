#include "stm32f4xx.h"                  // Device header

volatile uint32_t tcnt = 0;

void SysTick_Handler(void)
{
	if(tcnt) tcnt--;
}

void delay(uint32_t ms)
{
	tcnt = ms;
	while(tcnt != 0)
	{
		
	}
}

int main(void)
{
	SystemCoreClockUpdate();
	
//	SysTick -> CTRL = SysTick_CTRL_CLKSOURCE_Msk | 
//										SysTick_CTRL_TICKINT_Msk | 
//										SysTick_CTRL_ENABLE_Msk;
	
	SysTick -> CTRL = (1 << 2) | 
										(1 << 1) | 
										(1 << 0);
	
	SysTick -> LOAD = SystemCoreClock/1000 - 1;
	SysTick -> VAL = SystemCoreClock/1000 - 1;
	
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	
	GPIOC -> MODER &= ~((0x03UL) << (13 * 2));
	GPIOC -> MODER |= ((0x01UL) << (13 * 2));
	
	while(1)
	{
		GPIOC -> BSRR = (1 << 13);
		delay(500);
		//for (volatile uint32_t i = 0; i < 500000; i++);
		
		GPIOC -> BSRR = (1 << (13 + 16));
		delay(500);
		//for (volatile uint32_t i = 0; i < 500000; i++);
	}
}

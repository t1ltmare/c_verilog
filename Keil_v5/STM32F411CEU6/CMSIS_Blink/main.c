#include "stm32f4xx.h"                  // Device header
//#include "stm32f411xe.h"                // Device header

// PORTC_13 LED

int main(void)
{
	
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	
	GPIOC->MODER &= ~((0x03UL) << (13 * 2));
	GPIOC->MODER |= ((0x01UL) << (13 * 2));
	
	while(1)
	{
		GPIOC->BSRR = (1 << 13);
		
		for (volatile uint32_t i = 0; i < 500000; i++);
		
		GPIOC->BSRR = (1 << (13 + 16));

		for (volatile uint32_t i = 0; i < 500000; i++);
	}
}

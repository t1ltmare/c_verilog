#include «stm32f4xx.h»

 

//GPIO_InitTypeDef GPIO_InitStructure;

 

int main(void)

{

uint32_t i;

 

RCC->AHB1ENR|=RCC_AHB1ENR_GPIODEN;

 

GPIOD->MODER = 0x55000000;

GPIOD->OTYPER = 0;

GPIOD->OSPEEDR = 0;

 

while(1)

{

GPIOD->ODR = 0x8000;

for(i=0;i<500000;i++){}

GPIOD->ODR = 0x4000;

for(i=0;i<500000;i++){}

GPIOD->ODR = 0x2000;

for(i=0;i<500000;i++){}

GPIOD->ODR = 0x1000;

for(i=0;i<500000;i++){}

}

}
#include "stm32f4xx.h"                
#include "stdint.h"

#define LEDPIN 13

volatile uint32_t tcnt = 0;
//volatile uint32_t tim3_counter = 0;

void led_init(void)
{
    RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	
	GPIOC -> MODER &= ~(0x03UL << LEDPIN * 2);
	GPIOC -> MODER |= (0x01UL << LEDPIN * 2);
    
    GPIOC -> ODR &= ~(0x01UL << LEDPIN * 2);
}

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

void systick_init(void)
{
    SysTick -> CTRL =   SysTick_CTRL_CLKSOURCE_Msk | 
                        SysTick_CTRL_TICKINT_Msk | 
                        SysTick_CTRL_ENABLE_Msk;
    SysTick -> LOAD =   SystemCoreClock/1000 - 1;
	SysTick -> VAL =    SystemCoreClock/1000 - 1;
}
/*
void tim2_init(void)
{
    RCC -> APB1ENR |= RCC_APB1ENR_TIM2EN;   // Включить тактирование TIM2
    TIM2 -> PSC = 99999;                    // Настроить предделитель
    TIM2 -> ARR = 249;                      // Настроить значение автоперезагрузки
    TIM2 -> DIER |= TIM_DIER_UIE;           // Включить прерывание по переполнению
    NVIC_EnableIRQ(TIM2_IRQn);              // Включить TIM2 в NVIC
    NVIC_SetPriority(TIM2_IRQn, 0);
    TIM2->CR1 |= TIM_CR1_CEN;               // Запустить таймер
}

void TIM2_IRQHandler(void) 
{
    if (TIM2->SR & TIM_SR_UIF)              // Если произошло прерывание по переполнению
    {
        TIM2->SR &= ~TIM_SR_UIF;            // Сбросить флаг прерывания
        GPIOC->ODR ^= (1 << LEDPIN);        // Переключить светодиод
    }
}
*/

/*
void tim3_init(void)
{
    RCC -> APB1ENR |= RCC_APB1ENR_TIM3EN;   // Включить тактирование TIM2
    TIM3 -> PSC = 99999;                    // Настроить предделитель
    TIM3 -> ARR = 0;                        // Настроить значение автоперезагрузки
    TIM3 -> DIER |= TIM_DIER_UIE;           // Включить прерывание по переполнению
    NVIC_EnableIRQ(TIM3_IRQn);              // Включить TIM2 в NVIC
    NVIC_SetPriority(TIM3_IRQn, 0);
    TIM3->CR1 &= ~TIM_CR1_CEN;              // Таймер пока не запускаем
}

void delay_ms(uint32_t ms)
{
    tim3_counter = 0;
    TIM3 -> ARR = ms/2 -1;                  // Установить новое значение ARR
    TIM3 -> CNT = 0;                        // Сбросить счетчик
    TIM3 -> SR &= ~TIM_SR_UIF;              // Сбросить флаг переполнения
    TIM3 -> CR1 |= TIM_CR1_CEN;             // Запустить таймер
    while (tim3_counter < 1);               // Ждать прерывания
    TIM3 -> CR1 &= ~TIM_CR1_CEN;            // Остановить таймер
}

void TIM3_IRQHandler(void)
{
    if (TIM3->SR & TIM_SR_UIF)              // Если произошло прерывание по переполнению
    {
        TIM3->SR &= ~TIM_SR_UIF;            // Сбросить флаг прерывания
        tim3_counter++;
    }
}
*/
int main(void)
{
	SystemCoreClockUpdate();
    
	led_init();
    //tim3_init();
	systick_init();
	//tim2_init();
	while(1)
	{
        GPIOC -> ODR ^= (1 << LEDPIN);
		delay(500);
	}
}

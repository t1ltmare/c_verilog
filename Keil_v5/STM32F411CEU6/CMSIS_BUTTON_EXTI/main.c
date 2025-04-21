#include "stm32f4xx.h"                  // Device header
#include "stdint.h"

#define BUTPIN 0 // Пин кнопки
#define LEDPIN 13 // Пин светодиода
#define DELAY 5 // Фильтр дребезга (мс)

void button_init(void)
{
    RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Тактирование порта кнопки
    
    GPIOA -> MODER &= ~(0x03UL << (0 * 2)); // Сброс битов режима для пина
    GPIOA -> MODER |= (0x00UL << (0 * 2)); // Установка пина на вход
    
    GPIOA -> PUPDR &= ~(0x03UL << (0 * 2)); // Сброс PU PD
    GPIOA -> PUPDR |= (0x01UL << (0 * 2)); // Включить pull-up
    
    RCC -> APB2ENR |= RCC_APB2ENR_SYSCFGEN; // Включить тактирование SYSCFG
    
    SYSCFG -> EXTICR[BUTPIN / 4] &= ~(0x0FUL << ((BUTPIN % 4) * 4)); // Сброс SYSCFG
    SYSCFG -> EXTICR[BUTPIN / 4] |= (0x00UL << ((BUTPIN % 4) * 4)); // Выбор порта GPIOA
    
    EXTI -> IMR |= (0x01UL << BUTPIN); // Разрешаем генерацию прерывания EXTI0
    EXTI -> FTSR |= (0x01UL << BUTPIN); // Прерывание по спаду
    
    NVIC_EnableIRQ(EXTI0_IRQn); // Разрешаем прерывание в NVIC
    NVIC_SetPriority(EXTI0_IRQn, 0); // Устанавливаем высокий приоритет прерывания
}

void led_init(void)
{
    RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // Тактирование порта GPIOC
    
    GPIOC -> MODER &= ~(0x03UL << (LEDPIN * 2)); // Сброс битов режима
	GPIOC -> MODER |= (0x01UL << (LEDPIN * 2)); // Установка режима выход
    GPIOC -> ODR &= ~(0x01UL << LEDPIN); // Сброс состояния светодиода
}

void delay_ms(uint32_t ms) // Фильтр дребезга
{
    uint32_t i;
    for (i = 0; i < (ms * 100000); i++)
    {
        __asm volatile("nop");
    }
}

void EXTI0_IRQHandler(void)
{
    if (EXTI -> PR & (0x01UL << BUTPIN)) // Если замечено прерывание
    {
        EXTI -> PR |= (0x01UL << BUTPIN); // Сбросить флаг прерывания
        
        delay_ms(DELAY); 
        
        GPIOC -> ODR ^= (0x01UL << LEDPIN); // Переключить светодиод
    }
}

int main(void)
{
	led_init();
    button_init();
    
    while(1) 
    {
        
    }
}

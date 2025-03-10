#include "stm32f411xe.h"                  // Device header

// PORTA_5 LED
// Тактирование порта
// Инициализируем порт и пин
// Меняем состояние нашего пина в бесконечном цикле
// Делаем задержку путем пустого счета цикла, например, цикла for

int main(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // включить тактирование GPIOA (эквивалентно SET_BIT(RCC -> AHB1ENR, RCC_AHB1ENR_GPIOAEN);
    
    GPIOA->MODER &= ~((0x03UL) << (5 * 2)); // сбросить биты MODER для PA5

    GPIOA->MODER |= ((0x01UL) << (5 * 2)); // установить PA5 как выход (01)
    
    while (1)
   {
        // Установить PA5 в высокий уровень
        GPIOA->BSRR = (1 << 5);  // Установить бит 5 в ODR (GPIOA->ODR = GPIO_ODR_OD5;)
       
        // Задержка
        for (volatile uint32_t i = 0; i < 1000000; i++);

        // Сбросить PA5 в низкий уровень
        GPIOA->BSRR = (1 << (5 + 16)); // Сбросить бит 5 в ODR  (GPIOA->ODR &= ~GPIO_ODR_OD5;)

        // Задержка
        for (volatile uint32_t i = 0; i < 1000000; i++);
   }   
   
}
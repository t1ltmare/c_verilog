#include "stm32f4xx.h"
#include "stdint.h"

#define LEDPIN 13
#define BUTPIN 0 

volatile uint32_t duty_cycle_index = 0;
const uint32_t duty_cycles[] = {0, 20, 40, 60, 80, 100};
const uint32_t num_duty_cycles = sizeof(duty_cycles) / sizeof(duty_cycles[0]);

void led_init(void) 
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    
    GPIOC->MODER &= ~(0x3UL << (LEDPIN * 2)); 
    GPIOC->MODER |= (0x1UL << (LEDPIN * 2));
    
    GPIOC->AFR[0] &= ~(0xF << (LEDPIN * 4));  // Сбросить биты AFR для пина
    GPIOC->AFR[0] |= (0x1 << (LEDPIN * 4));  // Выбрать AF1 (TIM2_CH1)
}

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

void pwm_init(void) 
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    
    TIM2->PSC = 999;  
    TIM2->ARR = 99;

    TIM2->CCMR1 &= ~TIM_CCMR1_CC1S;  
    TIM2->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1); 
    TIM2->CCMR1 |= TIM_CCMR1_OC1PE;  
    TIM2->CCER |= TIM_CCER_CC1E;    

    TIM2->CCR1 = 0;

    TIM2->CR1 |= TIM_CR1_CEN;
    TIM2->EGR |= TIM_EGR_UG;
}

void EXTI0_IRQHandler(void) 
{
    if (EXTI->PR & (0x1UL << BUTPIN)) {
        EXTI->PR |= (0x1UL << BUTPIN);
        duty_cycle_index = (duty_cycle_index + 1) % num_duty_cycles;
        TIM2->CCR1 = (duty_cycles[duty_cycle_index] * TIM2->ARR) / 100; 
    }
}

int main(void) {
    led_init(); 
    pwm_init();
    button_init();

    while (1) 
    {
        
    }
}
#include "stm32f4xx.h"
#include "stdint.h"

#define PWM_PORT GPIOB
#define PWM_PIN 6
#define PWM_AF 2 // AF2 для TIM4_CH1 на PB6

volatile uint8_t dutyCycleIndex = 0;
const uint8_t dutyCycles[] = {0, 20, 40, 60, 80, 100};
const uint8_t numDutyCycles = sizeof(dutyCycles) / sizeof(dutyCycles[0]);                
volatile int8_t direction = 1; // 1 = рост, -1 = уменьшение

void GPIO_Config(void) {
  // 1. Включаем тактирование портов GPIOA и GPIOB
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

  // 2. Настройка пина ШИМ (GPIOB Pin 6) как альтернативную функцию
  GPIOB->MODER &= ~(0x03 << (PWM_PIN * 2)); // Сбрасываем биты MODER
  GPIOB->MODER |= (0x02 << (PWM_PIN * 2));  // Альтернативная функция
  GPIOB->OTYPER &= ~(1 << PWM_PIN);          // Push-pull выход
  GPIOB->OSPEEDR |= (0x03 << (PWM_PIN * 2)); // Высокая скорость
  GPIOB->PUPDR &= ~(0x03 << (PWM_PIN * 2));  // Без подтягивающих резисторов

  // 3. Выбор альтернативной функции
  if (PWM_PIN <= 7) {
    GPIOB->AFR[0] &= ~(0x0F << (PWM_PIN * 4));
    GPIOB->AFR[0] |= (PWM_AF << (PWM_PIN * 4));
  } else {
    GPIOB->AFR[1] &= ~(0x0F << ((PWM_PIN - 8) * 4));
    GPIOB->AFR[1] |= (PWM_AF << ((PWM_PIN - 8) * 4));
  }
}

// Функция для инициализации ШИМ (Таймер 4)
void TIM_Config(void) {
  RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
  TIM4->PSC = 99;
  TIM4->ARR = 999;

  TIM4->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE); 
  TIM4->CCER |= TIM_CCER_CC1E;                                           

  TIM4->CR1 |= TIM_CR1_ARPE | TIM_CR1_CEN;
}

// Функция для установки коэффициента заполнения ШИМ
void setDutyCycle(uint8_t dutyCyclePercentage) {
  uint16_t pulseWidth = (uint16_t)((dutyCyclePercentage * (TIM4->ARR + 1)) / 100);
  TIM4->CCR1 = pulseWidth;
}

volatile uint32_t sysTickCounter = 0;

void SysTick_Handler(void) { 
  sysTickCounter++;
}

uint32_t HAL_GetTick(void) {
  return sysTickCounter;
}

void tim2_init(void)
{
    RCC -> APB1ENR |= RCC_APB1ENR_TIM2EN;   // Включить тактирование TIM2
    TIM2 -> PSC = 99999;                    // Настроить предделитель
    TIM2 -> ARR = 24;                      // Настроить значение автоперезагрузки
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
        dutyCycleIndex += direction; 
				if (dutyCycleIndex >= numDutyCycles - 1) 
				{
					direction = -1; // Начать уменьшение
				} else if (dutyCycleIndex <= 0) 
				{
					direction = 1;  // Начать увеличение
				}

				// Устанавливаем новый коэффициент заполнения
				setDutyCycle(dutyCycles[dutyCycleIndex]);    
		}
}

int main(void) {
  // 1. Инициализация периферии
  GPIO_Config();
  TIM_Config();

  // 2. Начальная установка ШИМ
  setDutyCycle(dutyCycles[dutyCycleIndex]);

  // Настройка SysTick для HAL_GetTick() (нужно для устранения дребезга)
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock / 1000); // 1ms tick
	
	tim2_init();
	
  while (1) 
	{
    
  }
}


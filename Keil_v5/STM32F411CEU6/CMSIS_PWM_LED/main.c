#include "stm32f4xx.h"
#include "stdint.h"

#define BUTTON_PORT GPIOA
#define BUTTON_PIN 0
#define PWM_PORT GPIOB
#define PWM_PIN 6
#define PWM_AF 2 // AF2 для TIM4_CH1 на PB6

volatile uint8_t dutyCycleIndex = 0;
const uint8_t dutyCycles[] = {0, 20, 40, 60, 80, 100};
const uint8_t numDutyCycles = sizeof(dutyCycles) / sizeof(dutyCycles[0]);
volatile uint32_t lastButtonPress = 0;                 

void GPIO_Config(void) {
  // 1. Включаем тактирование портов GPIOA и GPIOB
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN;

  // 2. Настройка пина кнопки (GPIOA Pin 0)
  GPIOA->MODER &= ~(0x03 << (BUTTON_PIN * 2)); // Сбрасываем биты MODER
  GPIOA->PUPDR |= (0x01 << (BUTTON_PIN * 2));  // Подтягивающий резистор

  // 3. Настройка пина ШИМ (GPIOB Pin 6) как альтернативную функцию
  GPIOB->MODER &= ~(0x03 << (PWM_PIN * 2)); // Сбрасываем биты MODER
  GPIOB->MODER |= (0x02 << (PWM_PIN * 2));  // Альтернативная функция
  GPIOB->OTYPER &= ~(1 << PWM_PIN);          // Push-pull выход
  GPIOB->OSPEEDR |= (0x03 << (PWM_PIN * 2)); // Высокая скорость
  GPIOB->PUPDR &= ~(0x03 << (PWM_PIN * 2));  // Без подтягивающих резисторов

  // 4. Выбор альтернативной функции
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
  // Преобразуем процент в значение CCR (Capture/Compare Register)
  uint16_t pulseWidth = (uint16_t)((dutyCyclePercentage * (TIM4->ARR + 1)) / 100);

  // Устанавливаем значение CCR для канала 1
  TIM4->CCR1 = pulseWidth;
}

volatile uint32_t sysTickCounter = 0; // volatile ОЧЕНЬ ВАЖНО!

void SysTick_Handler(void) { // Имя функции SysTick_Handler ОЧЕНЬ ВАЖНО!
  sysTickCounter++;
}

uint32_t HAL_GetTick(void) { // Тип возвращаемого значения uint32_t ОЧЕНЬ ВАЖНО!
  return sysTickCounter;
}

int main(void) {
  // 1. Инициализация периферии
  GPIO_Config();
  TIM_Config();

  // 2. Начальная установка ШИМ
  setDutyCycle(dutyCycles[dutyCycleIndex]);

  // Настройка SysTick для HAL_GetTick() (нужно для устранения дребезга)
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock / 300); // 1ms tick

  while (1) {
    // 3. Чтение состояния кнопки
    if (!(GPIOA->IDR & (1 << BUTTON_PIN))) { // Кнопка нажата (предполагается pull-up)
      // 4. Устранение дребезга
      if ((HAL_GetTick() - lastButtonPress) > 50) {
        // 5. Увеличение индекса коэффициента заполнения
        dutyCycleIndex++;
        if (dutyCycleIndex >= numDutyCycles) {
          dutyCycleIndex = 0; // Вернуться к началу, если достигнут конец массива
        }

        // 6. Установка нового коэффициента заполнения
        setDutyCycle(dutyCycles[dutyCycleIndex]);

        // 7. Запоминание времени последнего нажатия кнопки
        lastButtonPress = HAL_GetTick();
      }
    }
  }
}


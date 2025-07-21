#include "stm32f4xx.h"

volatile uint32_t systick_count = 0;

void SysTick_Handler(void) {
  systick_count++;
}

void Delay_ms(uint32_t ms) {
  uint32_t start_tick = systick_count;
  while ((systick_count - start_tick) < ms);
}

int main(void) {
  // Настройка SysTick для генерации прерывания каждые 1 мс
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock / 1000);

  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

  GPIOC->MODER &= ~((0x03UL) << (13 * 2));
  GPIOC->MODER |= ((0x01UL) << (13 * 2));

  while (1) {
    GPIOC->BSRR = (1 << 13); // Включить LED
    Delay_ms(1000);           

    GPIOC->BSRR = (1 << (13 + 16)); // Выключить LED
    Delay_ms(1000);            
  }
}
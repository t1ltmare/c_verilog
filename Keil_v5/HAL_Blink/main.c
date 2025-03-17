#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "stm32f4xx_hal_conf.h"         // Keil::Device:STM32Cube Framework:Classic

int main(void) 
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    GPIO_InitTypeDef GPIOA_Use;
    
    GPIOA_Use.Mode = GPIO_MODE_OUTPUT_PP;
    GPIOA_Use.Pin = GPIO_PIN_5;
    GPIOA_Use.Speed = GPIO_SPEED_FREQ_MEDIUM;
    GPIOA_Use.Pull = GPIO_NOPULL;
    
    HAL_GPIO_Init(GPIOA, &GPIOA_Use);
  
    while(1) 
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        for(int i = 0; i < 50000; i++);
    }
}

#include "stm32f4xx.h"

#define I2C_SLAVE_ADDRESS 0x50  // Адрес Slave-устройства (7-битный)

void I2C1_Init(void);
void I2C1_Write(uint8_t data);
void Delay_ms(uint32_t ms);

int main(void) {
    // Инициализация I2C1
    I2C1_Init();
    
    // Массив данных для отправки
    uint8_t data_to_send[] = {0x41, 0x39, 0xFF, 0xAA};
    
    while (1) {
        // Отправка всех 4 байтов
        for (int i = 0; i < 4; i++) {
            I2C1_Write(data_to_send[i]);
            Delay_ms(100);  // Задержка между посылками
        }
        
        Delay_ms(1000);  // Пауза перед повторной отправкой
    }
}

// Инициализация I2C1 (PB6=SCL, PB7=SDA)
void I2C1_Init(void) {
    // 1. Включаем тактирование GPIOB и I2C1
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    
    // 2. Настройка PB6 (SCL) и PB7 (SDA) в режим AF4 (I2C1)
    GPIOB->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7);
    GPIOB->MODER |= (GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1);  // AF mode
    GPIOB->OTYPER |= (GPIO_OTYPER_OT6 | GPIO_OTYPER_OT7);         // Open-drain
    GPIOB->PUPDR |= (GPIO_PUPDR_PUPDR6_0 | GPIO_PUPDR_PUPDR7_0); // Pull-up
    GPIOB->AFR[0] |= (4 << (6*4)) | (4 << (7*4));                // AF4
    
    // 3. Настройка I2C1 (Standard mode, 100 kHz)
    I2C1->CR1 &= ~I2C_CR1_PE;  // Отключаем I2C перед настройкой
    I2C1->CR2 = (16 << 0);     // APB1 clock freq (16 MHz для HSI)
    I2C1->CCR = 80;            // CCR = (APB1 freq)/(2*I2C freq) = 16M/(2*100k) = 80
    I2C1->TRISE = 17;          // TRISE = (APB1 freq/MHz) + 1 = 16 + 1 = 17
    I2C1->CR1 |= I2C_CR1_PE;   // Включаем I2C
}

// Функция отправки одного байта по I2C
void I2C1_Write(uint8_t data) {
    // 1. Ждем, пока I2C не освободится
    while (I2C1->SR2 & I2C_SR2_BUSY);
    
    // 2. Генерируем START условие
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB));  // Ждем флаг START
    
    // 3. Отправляем адрес Slave (в режиме записи)
    I2C1->DR = (I2C_SLAVE_ADDRESS << 1) | 0;  // 0 = Write
    while (!(I2C1->SR1 & I2C_SR1_ADDR));      // Ждем подтверждения адреса
    (void)I2C1->SR2;                          // Читаем SR2 для сброса флага
    
    // 4. Отправляем данные
    while (!(I2C1->SR1 & I2C_SR1_TXE));       // Ждем готовности передатчика
    I2C1->DR = data;                          // Записываем данные
    while (!(I2C1->SR1 & I2C_SR1_BTF));       // Ждем завершения передачи
    
    // 5. Генерируем STOP условие
    I2C1->CR1 |= I2C_CR1_STOP;
}

// Простая функция задержки
void Delay_ms(uint32_t ms) {
    for (volatile uint32_t i = 0; i < ms * 2000; i++);
}
#include "stm32f4xx.h"
#include <stdint.h>

// Определения для светодиода (например, PA5)
#define LED_PORT GPIOA
#define LED_PIN 5  // GPIO Pin 5

void SPI_Master_Init(void) {
    // 1. Включить тактирование SPI1 и порта GPIOA
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // 2. Настроить пины GPIOA для SPI1 (Alternate Function)
    // PA5 (SCK), PA6 (MISO), PA7 (MOSI)
    GPIOA->MODER &= ~(0x03 << (5 * 2));  // PA5
    GPIOA->MODER |=  (0x02 << (5 * 2));
    GPIOA->AFR[0] |= (5 << (5 * 4));

    GPIOA->MODER &= ~(0x03 << (6 * 2));  // PA6
    GPIOA->MODER |=  (0x02 << (6 * 2));
    GPIOA->AFR[0] |= (5 << (6 * 4));

    GPIOA->MODER &= ~(0x03 << (7 * 2));  // PA7
    GPIOA->MODER |=  (0x02 << (7 * 2));
    GPIOA->AFR[0] |= (5 << (7 * 4));

    // 3. Настройка SPI1
    SPI1->CR1 |= SPI_CR1_MSTR;    // Master mode
    SPI1->CR1 |= SPI_CR1_BR_1 | SPI_CR1_BR_0;  // Baud rate = fPCLK/8 (Adjust as needed!) Подберите подходящий делитель!
    SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;  // Software Slave Management (SSM) + Internal Slave Select

    //8-bit data size, 0x07
    SPI1->CR2 = (SPI1->CR2 & ~(0x0F << 8)) | (0x07 << 8);  //Явное значение для DS[3:0]

    SPI1->CR1 &= ~SPI_CR1_CPHA; // CPHA = 0 (подстройте под K1986BE92FI)
    SPI1->CR1 &= ~SPI_CR1_CPOL; // CPOL = 0 (подстройте под K1986BE92FI)
    SPI1->CR1 |= SPI_CR1_SPE;    // Enable SPI1

    // 4. Настройка пина для светодиода (Output) - CMSIS
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Включаем тактирование порта GPIOA (если еще не включено)

    // Настраиваем MODER (Mode Register) - режим работы пина
    GPIOA->MODER &= ~(0x03 << (LED_PIN * 2)); // Очищаем биты для данного пина
    GPIOA->MODER |= (0x01 << (LED_PIN * 2));  // Устанавливаем пин в режим Output (01)

    // Настраиваем OTYPER (Output Type Register) - тип выхода (push-pull или open-drain)
    GPIOA->OTYPER &= ~(1 << LED_PIN); // Push-pull (0)

    // Настраиваем OSPEEDR (Output Speed Register) - скорость переключения
    GPIOA->OSPEEDR &= ~(0x03 << (LED_PIN * 2)); // Low speed (00) - можно выбрать другую скорость
    GPIOA->OSPEEDR |= (0x01 << (LED_PIN * 2)); // Medium speed (01) - пример

    // Настраиваем PUPDR (Pull-up/Pull-down Register) - подтягивающие резисторы
    GPIOA->PUPDR &= ~(0x03 << (LED_PIN * 2)); // No pull-up, no pull-down (00)
}

// Функция передачи данных по SPI (Master)
uint8_t SPI_Master_Transmit(uint8_t data) {
    while (!(SPI1->SR & SPI_SR_TXE));  // Wait for TX buffer empty
    SPI1->DR = data;                 // Send data
    while (!(SPI1->SR & SPI_SR_RXNE)); // Wait for RX buffer not empty
    return SPI1->DR;                 // Read received data (optional)
}

// Функция для мигания светодиодом - CMSIS
void BlinkLED(int numBlinks) {
    for (int i = 0; i < numBlinks; i++) {
        GPIOA->BSRR = (1 << LED_PIN);   // Включить светодиод (установить бит в BSRR)
        for (volatile int j = 0; j < 500000; j++);   // Короткая задержка
        GPIOA->BSRR = (uint32_t)(1 << (LED_PIN + 16));  // Выключить светодиод (установить бит в BRR)
        for (volatile int j = 0; j < 500000; j++);   // Короткая задержка
    }
}

int main(void) {
    SPI_Master_Init();

    while (1) {
        uint8_t dataToSend = 0x5A; // Пример данных для отправки (замените на нужное вам число)
        uint8_t receivedData = SPI_Master_Transmit(dataToSend); // Отправить и получить

        // Проверка на равенство отправленного и полученного
        if (dataToSend == receivedData) {
            BlinkLED(3); // Мигнуть светодиодом 3 раза, если числа совпадают
        }

        // Задержка (необязательно, но полезно для отладки)
        for (volatile int i = 0; i < 100000; i++);
    }
}

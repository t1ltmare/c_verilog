#include "stm32f4xx.h"

#define SPI2_IRQn 36

volatile uint8_t received_data = 0;

void SPI2_Init(void);
void SPI2_Tx(uint8_t data);
void LED_Init(void);
void Blink_LED(void);
void SPI2_IRQHandler(void);

int main(void) {
    uint8_t received_data = 0b00000000;
    
    LED_Init();
    SPI2_Init();
    
    NVIC_EnableIRQ(SPI2_IRQn);
    
    while (1) {
        /*
        // Ждем получения данных
        while (!(SPI2->SR & SPI_SR_RXNE));
        received_data = SPI2->DR;
        
        // Отправляем данные обратно
        SPI2_Tx(received_data);
        
        // Мигаем светодиодом на Slave
        //Blink_LED();
        */
    }
}

void SPI2_Init(void) {
    // Включаем тактирование GPIOB и SPI2
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
    
    // Настраиваем PB13(SCK), PB14(MISO), PB15(MOSI) в режим альтернативной функции
    GPIOB->MODER &= ~(GPIO_MODER_MODE13 | GPIO_MODER_MODE14 | GPIO_MODER_MODE15);
    GPIOB->MODER |= (GPIO_MODER_MODE13_1 | GPIO_MODER_MODE14_1 | GPIO_MODER_MODE15_1);
    
    // AF5 для SPI2
    GPIOB->AFR[1] |=    GPIO_AFRH_AFSEL13_0|GPIO_AFRH_AFSEL13_2| // b0101
                        GPIO_AFRH_AFSEL14_0|GPIO_AFRH_AFSEL14_2|
                        GPIO_AFRH_AFSEL15_0|GPIO_AFRH_AFSEL15_2;
    
    SPI2->CR1 &= ~SPI_CR1_DFF;                  // 8 bits
    SPI2->CR1 |= SPI_CR1_CPOL | SPI_CR1_CPHA;   // Same as master
    SPI2->CR1 &= ~SPI_CR1_LSBFIRST;             // MSB first
    SPI2->CR1 |= SPI_CR1_SSM;                   // Software NSS
    SPI2->CR1 &= ~SPI_CR1_SSI;                  // Always selected    
    SPI2->CR1 &= ~SPI_CR1_MSTR;                 // Slave mode
    SPI2->CR1 |= SPI_CR1_SPE;                   // SPI enable
   
    SPI2->CR2 |= SPI_CR2_RXNEIE | SPI_CR2_ERRIE;// RX interrupt enable, error interrupt enable
}

void SPI2_Tx(uint8_t data) {
    // Ждем готовности передатчика
    while (!(SPI2->SR & SPI_SR_TXE));
    
    // Отправляем данные
    SPI2->DR = data;
    
    while(!(SPI2->SR & SPI_SR_BSY));
}

void LED_Init(void) {
    // Включаем тактирование GPIOC
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    
    // Настраиваем PC13 как выход
    GPIOC->MODER &= ~(GPIO_MODER_MODE13);
    GPIOC->MODER |= GPIO_MODER_MODE13_0;
}

void Blink_LED(void) {
    GPIOC->BSRR = (1 << 13);          
    for (volatile uint32_t i = 0; i < 50000; i++);
    GPIOC->BSRR = (1 << (13 + 16));   
    for (volatile uint32_t i = 0; i < 50000; i++);
}

void SPI2_IRQHandler(void) {
    if (SPI2->SR & SPI_SR_RXNE) {
        received_data = SPI2->DR;
        SPI2_Tx(received_data);
    }
    
    if (SPI2->SR & (SPI_SR_FRE | SPI_SR_OVR | SPI_SR_MODF)) {
        volatile uint32_t dummyRead = SPI2->DR;
        dummyRead = SPI2->SR;
        (void)dummyRead;
        
        while(1) {
            Blink_LED();
       }
    }
}

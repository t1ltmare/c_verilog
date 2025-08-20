#include "stm32f4xx.h"

#define SPI2_IRQn 36
#define DMA1_STREAM3_IRQn   14  // Для SPI2_RX
#define DMA1_STREAM4_IRQn   15  // Для SPI2_TX
//#define DMA_LISR_TCIF3      27
//#define DMA_LIFCR_CTCIF3    27
//#define DMA_HISR_TCIF4      5
//#define DMA_HIFCR_CTCIF4    5

volatile uint8_t rx_data = 0;
volatile uint8_t tx_data = 0;
volatile uint8_t transfer_complete = 0;

void SPI2_Init(void);
void LED_Init(void);
void Blink_LED(void);
void DMA_Init(void);
void SPI_DMA_Enable(void);

int main(void) {
    LED_Init();
    SPI2_Init();
    DMA_Init();
    SPI_DMA_Enable();
    
    NVIC_EnableIRQ(SPI2_IRQn);
    NVIC_EnableIRQ(DMA1_STREAM3_IRQn);
    NVIC_EnableIRQ(DMA1_STREAM4_IRQn);
    
    while (1) {
        if (transfer_complete) {
            transfer_complete = 0;
            tx_data = rx_data; // Эхо-ответ
        }
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
    
    SPI2->CR1 &= ~SPI_CR1_DFF;                      // 8 bits
    SPI2->CR1 |= SPI_CR1_CPOL | SPI_CR1_CPHA;       // Same as master
    SPI2->CR1 &= ~SPI_CR1_LSBFIRST;                 // MSB first
    SPI2->CR1 |= SPI_CR1_SSM;                       // Software NSS
    SPI2->CR1 &= ~SPI_CR1_SSI;                      // Always selected    
    SPI2->CR1 &= ~SPI_CR1_MSTR;                     // Slave mode
    
    SPI2->CR2 |= SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN; // DMA rx & tx enable
    
    SPI2->CR1 |= SPI_CR1_SPE;                       // SPI enable
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

void DMA_Init(void) {
    // Включаем тактирование DMA1
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
    
    // Очистка флагов прерываний
    DMA1->HIFCR = 0xFFFFFFFF;
    DMA1->LIFCR = 0xFFFFFFFF;
    
    // Настройка DMA для приёма (SPI2_RX -> DMA1 Stream3, Channel 0)
    DMA1_Stream3->CR &= ~DMA_SxCR_EN;
    DMA1_Stream3->CR = DMA_SxCR_CHSEL_0 |    // Channel 0
                      DMA_SxCR_MINC |        // Increment memory
                      DMA_SxCR_TCIE |        // Transfer complete interrupt
                      DMA_SxCR_CIRC;         // Circular mode
    DMA1_Stream3->PAR = (uint32_t)&SPI2->DR;
    DMA1_Stream3->M0AR = (uint32_t)&rx_data;
    DMA1_Stream3->NDTR = 1;                  // 1 byte
    
    // Настройка DMA для передачи (SPI2_TX -> DMA1 Stream4, Channel 0)
    DMA1_Stream4->CR &= ~DMA_SxCR_EN;
    DMA1_Stream4->CR = DMA_SxCR_CHSEL_0 |    // Channel 0
                      DMA_SxCR_DIR_0 |       // Memory to peripheral
                      DMA_SxCR_MINC |        // Increment memory
                      DMA_SxCR_TCIE |        // Transfer complete interrupt
                      DMA_SxCR_CIRC;         // Circular mode
    DMA1_Stream4->PAR = (uint32_t)&SPI2->DR;
    DMA1_Stream4->M0AR = (uint32_t)&tx_data;
    DMA1_Stream4->NDTR = 1;                  // 1 byte
}

void SPI_DMA_Enable(void) {
    // Включаем DMA потоки
    DMA1_Stream3->CR |= DMA_SxCR_EN;
    DMA1_Stream4->CR |= DMA_SxCR_EN;
}

void SPI2_IRQHandler(void) {
    // Обработка ошибок SPI
    if (SPI2->SR & (SPI_SR_OVR | SPI_SR_MODF | SPI_SR_FRE)) {
        volatile uint32_t dummy = SPI2->DR;
        dummy = SPI2->SR;
        (void)dummy;
        while(1) Blink_LED();
    }
}

void DMA1_Stream3_IRQHandler(void) {
    if (DMA1->LISR & DMA_LISR_TCIF3) {
        DMA1->LIFCR |= DMA_LIFCR_CTCIF3;
        transfer_complete = 1;
    }
}

void DMA1_Stream4_IRQHandler(void) {
    if (DMA1->HISR & DMA_HISR_TCIF4) {
        DMA1->HIFCR |= DMA_HIFCR_CTCIF4;
    }
}

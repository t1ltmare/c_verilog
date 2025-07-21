#include "stm32f4xx.h"

/********************
* GPIOB Pin13 -> SCK
* GPIOB Pin14 -> MISO
* GPIOB Pin15 -> MOSI
********************/

void SPI2_Init(void);
void SPI2_Tx(uint8_t data);
void LED_Init(void);
void Blink_LED(void);
void Delay(void);

int main(void) {
    uint8_t received_data;
    
    LED_Init();
    SPI2_Init();
    
    while (1) {
        // Отправляем данные и получаем ответ
        //received_data = 
        SPI2_Tx('A');			//0x41	0b01000001
        //Delay();
	    SPI2_Tx('9');			//0x39	0b00111001 
        //Delay();
	    SPI2_Tx(255);			//0xFF	0b11111111	  
        //Delay();
	    SPI2_Tx(0b10101010);    //0xAA	0b10101010
        //Delay();
        SPI2_Tx(0b00000000);    //0
        Blink_LED();
    }
    return 0;
}

void SPI2_Init(void) {
    // Включаем тактирование GPIOB и SPI2
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
    
    // Настраиваем PB13(SCK), PB14(MISO), PB15(MOSI) в режим альтернативной функции
    GPIOB->MODER &= ~(GPIO_MODER_MODE13 | GPIO_MODER_MODE14 | GPIO_MODER_MODE15);
    GPIOB->MODER |= GPIO_MODER_MODE13_1 | GPIO_MODER_MODE14_1 | GPIO_MODER_MODE15_1;
    
    // AF5 для SPI2
    GPIOB->AFR[1] |=    GPIO_AFRH_AFSEL13_0|GPIO_AFRH_AFSEL13_2| // b0101
                        GPIO_AFRH_AFSEL14_0|GPIO_AFRH_AFSEL14_2|
                        GPIO_AFRH_AFSEL15_0|GPIO_AFRH_AFSEL15_2;
    
    // Конфигурация SPI2 как Master
    SPI2->CR1 |= SPI_CR1_SSM|SPI_CR1_SSI;
    SPI2->CR1 |= SPI_CR1_MSTR;
    SPI2->CR1 |= SPI_CR1_SPE;
    SPI2->CR1 |= SPI_CR1_BR_2;
    SPI2->CR1 |= SPI_CR1_CPOL | SPI_CR1_CPHA;
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

void Delay(void) {
    for (volatile uint32_t i = 0; i < 250000; i++);
}
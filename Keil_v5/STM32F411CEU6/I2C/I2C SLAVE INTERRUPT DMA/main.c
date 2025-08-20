#include <stdint.h>

/* ====================== Hardware Register Definitions ====================== */
#define RCC_BASE       0x40023800
#define GPIOB_BASE     0x40020400
#define I2C1_BASE      0x40005400
#define DMA1_BASE      0x40026000  

/* Добавляем определения для NVIC */
#define NVIC_ISER0     (*((volatile uint32_t*)0xE000E100))
#define I2C1_EV_IRQn        31
#define I2C1_ER_IRQn        32
#define DMA1_STREAM6_IRQn   17

typedef struct {
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t RESERVED1[2];
    volatile uint32_t APB1RSTR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t RESERVED2[2];
    volatile uint32_t AHB1ENR;
    volatile uint32_t AHB2ENR;
    volatile uint32_t RESERVED3[2];
    volatile uint32_t APB1ENR;
    volatile uint32_t APB2ENR;
} RCC_TypeDef;

typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];
} GPIO_TypeDef;

typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t OAR1;
    volatile uint32_t OAR2;
    volatile uint32_t DR;
    volatile uint32_t SR1;
    volatile uint32_t SR2;
    volatile uint32_t CCR;
    volatile uint32_t TRISE;
} I2C_TypeDef;

/* Структура регистров DMA */
typedef struct {
    volatile uint32_t CR;
    volatile uint32_t NDTR;
    volatile uint32_t PAR;
    volatile uint32_t M0AR;
    volatile uint32_t M1AR;
    volatile uint32_t FCR;
} DMA_Stream_TypeDef;

typedef struct {
    volatile uint32_t LISR;
    volatile uint32_t HISR;
    volatile uint32_t LIFCR;
    volatile uint32_t HIFCR;
    DMA_Stream_TypeDef S[8];
} DMA_TypeDef;

#define RCC         ((RCC_TypeDef*) RCC_BASE)
#define GPIOB       ((GPIO_TypeDef*) GPIOB_BASE)
#define I2C1        ((I2C_TypeDef*) I2C1_BASE)
#define DMA1        ((DMA_TypeDef*) DMA1_BASE)

/* =========================== Clock Enable Macros =========================== */
#define GPIOB_CLK_ENABLE()    (RCC->AHB1ENR |= (1 << 1))    // GPIOB - бит 1
#define I2C1_CLK_ENABLE()     (RCC->APB1ENR |= (1 << 21))   // I2C1 - бит 21
#define DMA1_CLK_ENABLE()     (RCC->AHB1ENR |= (1 << 21))   // DMA1 - бит 21

/* ========================== GPIO Configuration ============================ */
#define GPIO_MODE_INPUT       0
#define GPIO_MODE_GPIO        1
#define GPIO_MODE_AFIO        2
#define GPIO_MODE_ANALOG      3

#define GPIO_MODE_PUSH_PULL   0
#define GPIO_MODE_OPEN_DRAIN  1

#define GPIO_LOW_SPEED        0
#define GPIO_MEDIUM_SPEED     1
#define GPIO_FAST_SPEED       2
#define GPIO_HIGH_SPEED       3

#define AF4                   4

/* ============================ I2C Definitions ============================= */
#define I2C_STANDARD_MODE     0
#define I2C_FAST_MODE         1

#define I2C_CR1_PE            0
#define I2C_CR1_SWRST         15
#define I2C_CR1_START         8
#define I2C_CR1_STOP          9
#define I2C_CR1_ACK	          10
#define I2C_CR1_NO_ACK	      11

#define I2C_CR2_ITEVTEN       9
#define I2C_CR2_ITBUFEN       10
#define I2C_CR2_ITERREN       8
#define I2C_CR2_DMAEN         11        // Разрешение DMA
#define I2C_CR2_LAST          12        // Флаг LAST для генерации NACK

#define I2C_SR1_SB            0
#define I2C_SR1_ADDR          1
#define I2C_SR1_TXE           7
#define I2C_SR1_BTF           2
#define I2C_SR1_RXNE          6
#define I2C_SR1_STOPF         4
#define I2C_SR1_AF            10

#define I2C_SR2_BUSY          1
#define I2C_SR2_TRA           2
#define I2C_SR2_MSL           0

#define I2C1_GPIOB6_SCL       6
#define I2C1_GPIOB7_SDA       7

/* ========================= DMA Configuration ============================== */
#define DMA_CHANNEL_1         (1 << 25) // CHSEL = chan 1
#define DMA_DIR_MEM_TO_PERIP  (1 << 6)  // DIR = mem to periph
#define DMA_PINC_DISABLE      (0 << 9)  // PINC = periph add pointer is fixed
#define DMA_MINC_ENABLE       (1 << 10) // MINC = mem add pointer is incremented after each data transfer
#define DMA_PSIZE_8BIT        (0 << 11) // PSIZE = byte (8-bit)
#define DMA_MSIZE_8BIT        (0 << 13) // MSIZE = byte (8-bit)
#define DMA_PL_HIGH           (2 << 16) // PL = high priority
#define DMA_TCIE              (1 << 4)  // TCIE = transfer complete interrupt enabled
#define DMA_ENABLE            (1 << 0)  // DMA enabled

/* ========================== Function Prototypes =========================== */
void GPIO_SetMode(GPIO_TypeDef* GPIOx, uint8_t pin, uint8_t speed, uint8_t type, uint8_t mode);
void I2C1_USE_GPIOB6_7(void);
void I2C_Init(I2C_TypeDef* I2Cx, void(*remap_func)(void), uint8_t mode, uint8_t slave_addr);
uint8_t I2C_ReadByte(I2C_TypeDef* I2Cx);
void I2C_Write(I2C_TypeDef* I2Cx, uint8_t data);
void I2C_Slave_Listen(I2C_TypeDef* I2Cx);
void I2C_ClearADDR(I2C_TypeDef* I2Cx);
void I2C_ClearSTOPF(I2C_TypeDef* I2Cx);
void I2C_EnableInterrupts(I2C_TypeDef* I2Cx);
void I2C_DisableInterrupts(I2C_TypeDef* I2Cx);
void DMA_Init(void);
void DMA_ConfigTX(uint8_t* buffer, uint32_t size);

/* ============================ Main Application ============================ */
#define SLAVE_ADDR 0x50
#define BUFFER_SIZE 1  // Размер буфера для DMA

volatile uint8_t received_data;
volatile uint8_t transfer_complete = 0;
volatile uint8_t dma_active = 0;
volatile uint32_t rx_buffer[BUFFER_SIZE];
volatile uint32_t tx_buffer[BUFFER_SIZE];
volatile uint32_t rx_index = 0;

int main(void) {
    GPIOB_CLK_ENABLE();
    I2C1_CLK_ENABLE();
    // Инициализация DMA
    DMA_Init();
    
    // Инициализация I2C
    I2C_Init(I2C1, I2C1_USE_GPIOB6_7, I2C_STANDARD_MODE, SLAVE_ADDR);
    
    // Включаем прерывания I2C
    I2C_EnableInterrupts(I2C1);
    
    // Настраиваем NVIC
    NVIC_ISER0 |= (1 << (I2C1_EV_IRQn & 0x1F));
    NVIC_ISER0 |= (1 << (I2C1_ER_IRQn & 0x1F));
    NVIC_ISER0 |= (1 << (DMA1_STREAM6_IRQn & 0x1F)); // Для Stream6

    while(1) {
        // Основной цикл может выполнять другие задачи
        // Вся работа I2C теперь обрабатывается в прерываниях
    }
}

/* ======================= GPIO Configuration Functions ====================== */
void GPIO_SetMode(GPIO_TypeDef* GPIOx, uint8_t pin, uint8_t speed, uint8_t type, uint8_t mode) {
    GPIOx->MODER   &= ~(3 << (pin * 2));
    GPIOx->OSPEEDR &= ~(3 << (pin * 2));
    GPIOx->OTYPER  &= ~(1 << pin);

    if(mode > GPIO_MODE_INPUT) {
        GPIOx->MODER |= (mode << (pin * 2));
    }

    if(speed > GPIO_LOW_SPEED) {
        GPIOx->OSPEEDR |= (speed << (pin * 2));
    }

    if(type > GPIO_MODE_PUSH_PULL) {
        GPIOx->OTYPER  |= (1 << pin);
    }
}

/* ====================== I2C Hardware Configuration ======================= */
void I2C1_USE_GPIOB6_7(void) {
    GPIOB_CLK_ENABLE();
    I2C1_CLK_ENABLE();

    GPIOB->AFR[0] &= ~(0xF << (6*4));
    GPIOB->AFR[0] &= ~(0xF << (7*4));
    GPIOB->AFR[0] |=  (AF4 << (6*4)) | (AF4 << (7*4));

    GPIO_SetMode(GPIOB, I2C1_GPIOB6_SCL, GPIO_HIGH_SPEED, GPIO_MODE_OPEN_DRAIN, GPIO_MODE_AFIO);
    GPIO_SetMode(GPIOB, I2C1_GPIOB7_SDA, GPIO_HIGH_SPEED, GPIO_MODE_OPEN_DRAIN, GPIO_MODE_AFIO);
}

/* ======================== I2C Core Functions ============================= */
void I2C_Init(I2C_TypeDef* I2Cx, void(*remap_func)(void), uint8_t mode, uint8_t slave_addr) {
    remap_func();

    // Программный сброс
    I2Cx->CR1 |=  (1 << I2C_CR1_SWRST);
    I2Cx->CR1 &= ~(1 << I2C_CR1_SWRST);

    // Настройка таймингов
    I2Cx->CR2 = (16 << 0);
    I2Cx->CCR = (80 << 0);
    I2Cx->TRISE = 17;

    // Настройка адреса слейва
    I2Cx->OAR1 = (slave_addr << 1);
    I2Cx->OAR1 |= (1 << 14);
    I2Cx->OAR1 &= ~(1 << 15);
    
    // Включение I2C
    I2Cx->CR1 |= (1 << I2C_CR1_PE);
    I2Cx->CR1 |= (1 << I2C_CR1_ACK);
    
    // Включение DMA для передачи
    I2Cx->CR2 |= (1 << I2C_CR2_DMAEN);
}

void I2C_EnableInterrupts(I2C_TypeDef* I2Cx) {
    I2Cx->CR2 |= (1 << I2C_CR2_ITEVTEN) |   // Разрешаем прерывания по событиям
                 (1 << I2C_CR2_ITERREN);    // Разрешаем прерывания ошибок
}

void I2C_DisableInterrupts(I2C_TypeDef* I2Cx) {
    I2Cx->CR2 &= ~((1 << I2C_CR2_ITEVTEN) |
                   (1 << I2C_CR2_ITERREN));
}

void I2C_ClearADDR(I2C_TypeDef* I2Cx) {
    volatile uint32_t dummy;
    dummy = I2Cx->SR1;
    dummy = I2Cx->SR2;
    (void)dummy;
}

void I2C_ClearSTOPF(I2C_TypeDef* I2Cx) {
    volatile uint32_t tmp;
    tmp = I2Cx->SR1;                        // Шаг 1: Чтение SR1
    I2Cx->CR1 |= (1 << I2C_CR1_ACK);        // Шаг 2: Запись в CR1
    (void)tmp;
}

/* ======================== DMA Configuration ============================== */
void DMA_Init(void) {
    DMA1_CLK_ENABLE();
    
    // Очистка всех флагов для Stream6
    DMA1->HIFCR = 0xFFFFFFFF;
}

void DMA_ConfigTX(uint8_t* buffer, uint32_t size) {
    // Проверка размера
    if(size > BUFFER_SIZE) size = BUFFER_SIZE;
    
    // Используем Stream6 для I2C1_TX
    DMA_Stream_TypeDef* stream = &DMA1->S[6];   // Stream6
    
    // Отключить DMA перед настройкой
    stream->CR &= ~DMA_ENABLE;
    while(stream->CR & DMA_ENABLE);             // Ждем отключения
    
    // Настройка параметров DMA
    stream->CR = DMA_CHANNEL_1 |                // Канал 1
                 DMA_DIR_MEM_TO_PERIP |
                 DMA_PINC_DISABLE |
                 DMA_MINC_ENABLE |
                 DMA_PSIZE_8BIT |
                 DMA_MSIZE_8BIT |
                 DMA_PL_HIGH |
                 DMA_TCIE;
    
    stream->NDTR = size;
    stream->PAR = (uint32_t)&I2C1->DR;          // Периферийный адрес
    stream->M0AR = (uint32_t)buffer;            // Адрес памяти
    
    // Установка флага LAST для генерации NACK
    I2C1->CR2 |= (1 << I2C_CR2_LAST);
    
    // Включаем DMA
    stream->CR |= DMA_ENABLE;
    dma_active = 1;
}

/* ======================== I2C Interrupt Handlers ========================= */
void I2C1_EV_IRQHandler(void) {
    if (I2C1->SR1 & (1 << I2C_SR1_ADDR)) {
        I2C_ClearADDR(I2C1);
        
        if (I2C1->SR2 & (1 << I2C_SR2_TRA)) {
            DMA_ConfigTX((uint8_t*)rx_buffer, BUFFER_SIZE);
        } else {
            rx_index = 0;
        }
    }
    
    if (I2C1->SR1 & (1 << I2C_SR1_RXNE)) {
        if (rx_index < BUFFER_SIZE) {
            rx_buffer[rx_index++] = I2C1->DR;
        } else {
            volatile uint8_t dummy = I2C1->DR;
            (void)dummy;
        }
    }
    
    if (I2C1->SR1 & (1 << I2C_SR1_STOPF)) {
        I2C_ClearSTOPF(I2C1);
        transfer_complete = 1;
        dma_active = 0;
        I2C1->CR2 &= ~(1 << I2C_CR2_LAST);  // Сброс LAST
    }
}

void I2C1_ER_IRQHandler(void) {
    if (I2C1->SR1 & (1 << I2C_SR1_AF)) {
        I2C1->SR1 &= ~(1 << I2C_SR1_AF);
        I2C1->CR1 &= ~(1 << I2C_CR1_PE);
        I2C1->CR1 |= (1 << I2C_CR1_PE);
        I2C1->CR1 |= (1 << I2C_CR1_ACK);
        DMA1->S[6].CR &= ~DMA_ENABLE;       // Остановка Stream6
        dma_active = 0;
        I2C1->CR2 &= ~(1 << I2C_CR2_LAST);
    }
}

/* ======================== DMA Interrupt Handler =========================== */
void DMA1_Stream6_IRQHandler(void) {
    // Проверяем флаг завершения передачи для потока 6 (HISR)
    if (DMA1->HISR & (1 << 21)) {           // TCIF6 флаг (бит 21 в HISR)
        DMA1->HIFCR = (1 << 21);            // Очищаем флаг (CTCIF6 в HIFCR)
        
        transfer_complete = 1;
        dma_active = 0;
        DMA1->S[6].CR &= ~DMA_ENABLE;
        I2C1->CR2 &= ~(1 << I2C_CR2_LAST);
    }
    
    // Проверяем флаг ошибки передачи
    if (DMA1->HISR & (1 << 19)) {           // TEIF6 флаг (бит 19 в HISR)
        DMA1->HIFCR = (1 << 19);            // CTEIF6
        dma_active = 0;
        DMA1->S[6].CR &= ~DMA_ENABLE;
        I2C1->CR2 &= ~(1 << I2C_CR2_LAST);
    }
}
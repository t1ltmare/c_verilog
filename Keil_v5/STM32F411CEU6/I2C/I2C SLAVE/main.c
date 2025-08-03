#include <stdint.h>

/* ====================== Hardware Register Definitions ====================== */
#define RCC_BASE       0x40023800
#define GPIOB_BASE     0x40020400
#define GPIOC_BASE     0x40020800  // Добавлено для GPIOC
#define I2C1_BASE      0x40005400

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

#define RCC     ((RCC_TypeDef*) RCC_BASE)
#define GPIOB   ((GPIO_TypeDef*) GPIOB_BASE)
#define GPIOC   ((GPIO_TypeDef*) GPIOC_BASE)  // Добавлено
#define I2C1    ((I2C_TypeDef*) I2C1_BASE)

/* =========================== Clock Enable Macros =========================== */
#define GPIOB_CLK_ENABLE()    (RCC->AHB1ENR |= (1 << 1))
#define GPIOC_CLK_ENABLE()    (RCC->AHB1ENR |= (1 << 2))  // Добавлено
#define I2C1_CLK_ENABLE()     (RCC->APB1ENR |= (1 << 21))

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

// I2C Control Register 1
#define I2C_CR1_PE            0
#define I2C_CR1_ACK           10
#define I2C_CR1_SWRST         15
#define I2C_CR1_START         8
#define I2C_CR1_STOP          9

// I2C Status Register 1
#define I2C_SR1_SB            0
#define I2C_SR1_ADDR          1
#define I2C_SR1_BTF           2
#define I2C_SR1_STOPF         4
#define I2C_SR1_RXNE          6
#define I2C_SR1_TXE           7

// I2C Status Register 2
#define I2C_SR2_TRA           2

#define I2C1_GPIOB6_SCL       6
#define I2C1_GPIOB7_SDA       7

/* ========================== Function Prototypes =========================== */
void GPIO_SetMode(GPIO_TypeDef* GPIOx, uint8_t pin, uint8_t speed, uint8_t type, uint8_t mode);
void I2C1_USE_GPIOB6_7(void);
void I2C_Init_Slave(void);

/* ============================ Main Application ============================ */
#define SLAVE_ADDR 0x50

int main(void) {
    // Инициализация периферии
    I2C1_USE_GPIOB6_7();
    I2C_Init_Slave();
    
    // Настройка светодиода для отладки (PC13)
    GPIOC_CLK_ENABLE();  // Включаем тактирование GPIOC
    GPIOC->MODER |= (1 << 26); // PC13 output (бит 26 = 1, бит 27 = 0)
    GPIOC->OTYPER &= ~(1 << 13); // Push-pull
    GPIOC->OSPEEDR |= (3 << 26); // High speed
    GPIOC->ODR &= ~(1 << 13);    // Выключить светодиод
    
    while(1) {
        // 1. Ожидание совпадения адреса
        if (I2C1->SR1 & (1 << I2C_SR1_ADDR)) {
            // Мигаем светодиодом при получении адреса
            GPIOC->ODR ^= (1 << 13);
            
            // 2. Сброс ADDR флага (чтение SR1, затем SR2)
            volatile uint32_t tmp = I2C1->SR1;
            tmp = I2C1->SR2;
            (void)tmp;
            
            // 3. Определение направления передачи
            if (I2C1->SR2 & (1 << I2C_SR2_TRA)) {
                // Режим передачи (мастер читает данные)
                I2C1->DR = 0x50; // Отправляем ответ
            } else {
                // Режим приема (мастер отправляет данные)
                // Читаем и игнорируем данные
                while (!(I2C1->SR1 & (1 << I2C_SR1_RXNE))) {}
                volatile uint8_t data = I2C1->DR;
                (void)data;
            }
            
            // 4. Обработка STOP условия
            while (!(I2C1->SR1 & (1 << I2C_SR1_STOPF))) {}
            I2C1->CR1 |= (1 << I2C_CR1_PE); // Сброс STOP флага
        }
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

/* ======================== I2C Slave Initialization ======================== */
void I2C_Init_Slave(void) {
    // Сброс и настройка I2C
    I2C1->CR1 |=  (1 << I2C_CR1_SWRST);
    I2C1->CR1 &= ~(1 << I2C_CR1_SWRST);

    // Установка slave адреса (0x50)
    I2C1->OAR1 = (SLAVE_ADDR << 1);
    
    // Включение ACK
    I2C1->CR1 |= (1 << I2C_CR1_ACK);
    
    // Включение I2C
    I2C1->CR1 |= (1 << I2C_CR1_PE);
    
    I2Cx->CR2 |= (16 << 0);      // FREQ = 16 MHz
    I2Cx->CCR |= (80 << 0);       // CCR for 100 kHz
    I2Cx->TRISE = 17;             // TRISE for 100 kHz
}
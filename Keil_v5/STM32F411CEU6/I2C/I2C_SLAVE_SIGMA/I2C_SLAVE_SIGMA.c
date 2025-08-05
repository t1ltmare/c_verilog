#include <stdint.h>

/* ====================== Hardware Register Definitions ====================== */
#define RCC_BASE       0x40023800
#define GPIOB_BASE     0x40020400
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
#define I2C1    ((I2C_TypeDef*) I2C1_BASE)

/* =========================== Clock Enable Macros =========================== */
#define GPIOB_CLK_ENABLE()    (RCC->AHB1ENR |= (1 << 1))
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
#define I2C_CR1_PE            0
#define I2C_CR1_ACK           10

#define I2C_SR1_ADDR          1

#define I2C1_GPIOB6_SCL       6
#define I2C1_GPIOB7_SDA       7

/* ========================== Function Prototypes =========================== */
void GPIO_SetMode(GPIO_TypeDef* GPIOx, uint8_t pin, uint8_t speed, uint8_t type, uint8_t mode);
void I2C1_USE_GPIOB6_7(void);
void I2C_Slave_Init(I2C_TypeDef* I2Cx, void(*remap_func)(void), uint8_t own_address);

/* ============================ Main Application ============================ */
#define SLAVE_ADDR 0x50

int main(void) {
    I2C_Slave_Init(I2C1, I2C1_USE_GPIOB6_7, SLAVE_ADDR);
    
    while(1) {
        // Wait for address match
        if (I2C1->SR1 & (1 << I2C_SR1_ADDR)) {
            volatile uint32_t _dummy = I2C1->SR1 | I2C1->SR2;  // Clear ADDR flag
            (void)_dummy;
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
void I2C_Slave_Init(I2C_TypeDef* I2Cx, void(*remap_func)(void), uint8_t own_address) {
    remap_func();

    I2Cx->CR1 &= ~(1 << I2C_CR1_PE); // Disable I2C
    I2Cx->OAR1 = (own_address << 1);
    I2Cx->CR1 |= (1 << I2C_CR1_ACK); // Enable ACK
    I2Cx->CR1 |= (1 << I2C_CR1_PE);  // Enable I2C
}
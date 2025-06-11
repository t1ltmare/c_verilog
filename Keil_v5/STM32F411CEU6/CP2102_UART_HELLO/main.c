/*********************************************************
STM32F411CEU6
GPIOC Pin13 --> LED (PC13)
USART1 TX --> GPIOA Pin9, RX --> GPIOA Pin10
HSI=16 MHz, APB2ENR = 16MHz
Baud rate = 115200
Stop bit=1
no flow control
data =8 bit
**********************************************************/

#include "stm32f4xx.h"

// Function Prototypes
void USART1_Init(void);
void USART1_Tx_Char(uint8_t c);
void USART1_Tx_String(char *s);
void delay (void);
void LED_Init(void);  // Initialize LED
void LED_Toggle(void); // Toggle LED state

// Global Variables
volatile uint8_t receivedData = 0; // Store received data

// Greeting and Farewell Messages
const char greetingMessage[] = "Hello from STM32! Command received.\r\n";
const char farewellMessage[] = "Goodbye from STM32! Command received.\r\n";

// Interrupt Handler for USART1
void USART1_IRQHandler(void) {
    // Check if the RXNE (Read Data Register Not Empty) flag is set
    if (USART1->SR & USART_SR_RXNE) {
        // Read the received data from the Data Register
        receivedData = USART1->DR;

        // Clear the RXNE flag (by reading SR then DR - sequence is important)
        volatile uint32_t temp = USART1->SR;
        temp = USART1->DR;
        (void)temp; // Prevent compiler warnings about unused variable

        // Process the received data (command)
        if (receivedData == '1') {  // If command is '1'
            USART1_Tx_String(greetingMessage); // Send greeting
			LED_Toggle(); // Toggle LED
            delay(); // Short delay for LED visibility
			LED_Toggle();

        } else if (receivedData == '0') { // If command is '0'
            USART1_Tx_String(farewellMessage); // Send farewell
			LED_Toggle(); // Toggle LED
            delay(); // Short delay for LED visibility
			LED_Toggle();
        } else {
          USART1_Tx_String("Invalid Command\r\n");
		  LED_Toggle(); // Toggle LED
          delay(); // Short delay for LED visibility
		  LED_Toggle();
        }
    }
}

int main(void) {
    USART1_Init();  // Initialize USART1
    LED_Init();     // Initialize LED

    // Enable USART1 interrupt in NVIC
    NVIC_EnableIRQ(USART1_IRQn);
	NVIC_SetPriority(USART1_IRQn, 0);   // Set highest priority
	
	USART1->CR1 |= USART_CR1_RXNEIE;   // Enable RXNE interrupt

    while (1) {
        // Main loop (do nothing, everything happens in interrupt handler)
    }

    return 0;
}

void USART1_Init(void) {
    // Enable clock for GPIOA
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // Enable clock for USART1
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    // Configure GPIO pins for USART1 (PA9: TX, PA10: RX)
    GPIOA->MODER &= ~(GPIO_MODER_MODE9 | GPIO_MODER_MODE10);
    GPIOA->MODER |= GPIO_MODER_MODE9_1 | GPIO_MODER_MODE10_1; // Alternate Function Mode

    // Configure Alternate Function Register for USART1 (AF7)
    GPIOA->AFR[1] &= ~((0x0F << (4 * (9 - 8))) | (0x0F << (4 * (10 - 8))));
    GPIOA->AFR[1] |= ((7 << (4 * (9 - 8))) | (7 << (4 * (10 - 8))));

    // Configure USART1 parameters
    USART1->CR1 = 0; // Reset USART1 control register 1
    USART1->CR1 |= USART_CR1_TE | USART_CR1_RE; // Enable Transmitter and Receiver
	USART1->CR1 |= USART_CR1_OVER8;   // Enable oversampling by 8

	// Set baud rate (115200)
	USART1->BRR = (17<<4) | 2;

	//Enable UART
    USART1->CR1 |= USART_CR1_UE;
}

// Transmit a single character
void USART1_Tx_Char(uint8_t c) {
    // Wait until transmit data register is empty
    while (!(USART1->SR & USART_SR_TXE));

    // Write data to data register
    USART1->DR = c;

    // Wait until transmission is complete
    while (!(USART1->SR & USART_SR_TC));
}

// Transmit a string
void USART1_Tx_String(char *s) {
    while (*s != '\0') {
        USART1_Tx_Char(*s);
        s++;
    }
}

// Simple delay function
void delay(void) {
    for (volatile uint32_t i = 0; i < 400000; i++);  // Adjusted delay value
}

void LED_Init(void) {
    // Enable clock for GPIOC
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    // Configure PC13 as output
    GPIOC->MODER &= ~(GPIO_MODER_MODE13);
    GPIOC->MODER |= GPIO_MODER_MODE13_0;   // Output mode
}

void LED_Toggle(void) {
    // Toggle PC13 (LED)
    GPIOC->ODR ^= GPIO_ODR_ODR_13;
}
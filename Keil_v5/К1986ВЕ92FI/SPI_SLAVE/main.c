// K1986BE92FI - Slave (ПРИМЕРНЫЙ КОД, ТРЕБУЕТ АДАПТАЦИИ ПО ДАТАШИТУ!)
#include <stdint.h> //  Вам нужно будет подключить правильный заголовочный файл для вашего компилятора и K1986BE92FI!
#include "K1986VE9xI.h"                 // Device header
#include "system_K1986VE9xI.h"          // Milandr::Device:Startup


// --- ОПРЕДЕЛЕНИЯ (ВАМ НУЖНО БУДЕТ ИХ ОПРЕДЕЛИТЬ САМОСТОЯТЕЛЬНО, ИЗУЧИВ ДАТАШИТ!) ---
// Адреса регистров:
#define SPI_CTRL (*((volatile uint32_t *)0x40000000))  // Пример адреса регистра управления SPI
#define SPI_STATUS (*((volatile uint32_t *)0x40000004))// Пример адреса регистра статуса SPI
#define SPI_DATA (*((volatile uint32_t *)0x40000008))  // Пример адреса регистра данных SPI
#define PORT_CTRL (*((volatile uint32_t *)0x40010000)) // Пример адреса регистра управления портами
// Биты в регистрах (примеры):
#define SPI_ENABLE (1 << 0)    // Бит включения SPI
#define SPI_MASTER_MODE (1 << 1) // Бит выбора режима Master (0 - Slave)
#define SPI_RX_READY (1 << 2)    // Бит готовности данных для приема
#define LED_PIN_NUM 2           // Номер пина, к которому подключен светодиод (порт C, пин 2)

#define PORT_OE (*((volatile uint32_t *)0x40010004)) //Укажите регистр управления направлением порта
#define PORT_SET (*((volatile uint32_t *)0x40010008))  // Регистр установки пина в 1
#define PORT_RESET (*((volatile uint32_t *)0x4001000C)) // Регистр сброса пина в 0
#define PORTC_EN_BIT (1 << 0) //Бит для включения тактирования порта C (смотреть даташит!)

//Функция для включения тактирования
void enable_peripheral_clock(void){
    // Предположим, что это тактирование для PORTC (проверьте даташит!)
    // Пример (должен быть адаптирован к конкретному МК):
    RCC_CTRL |= RCC_PORTC_EN;
}

// --- ПЕРЕМЕННЫЕ ---
volatile uint8_t receivedData = 0;

// --- ФУНКЦИИ ---

// Функция инициализации SPI (Slave)
void SPI_Slave_Init(void) {
    // 1. Включить тактирование SPI и портов GPIO (ВАЖНО! Смотрите даташит K1986BE92FI!)
    enable_peripheral_clock();  //Как именно на K1986BE92FI включается тактирование?
    // 2. Настроить пины GPIO для SPI (Alternate Function) - УКАЖИТЕ ПРАВИЛЬНЫЕ ПИНЫ
    //   Примерно так, но смотрите даташит:
    PORT_CTRL |= (1 << 0); // Настройка SCK на alternate function
    PORT_CTRL |= (1 << 1); // Настройка MISO на alternate function
    PORT_CTRL |= (1 << 2); // Настройка MOSI на alternate function

    // 3. Настройка SPI (смотрите даташит K1986BE92FI!)
    SPI_CTRL &= ~SPI_MASTER_MODE; // Режим Slave
    // Укажите другие параметры SPI (например, CPHA, CPOL, разрядность данных)
    SPI_CTRL |= SPI_ENABLE;      // Включить SPI
}

// Функция приема данных по SPI (Slave)
uint8_t SPI_Slave_Receive(void) {
    // Ждем, пока не придут данные
    while (!(SPI_STATUS & SPI_RX_READY));

    // Читаем полученные данные
    receivedData = SPI_DATA;

    // Возвращаем полученные данные
    return receivedData;
}

// --- Инициализация GPIO для светодиода (АДАПТИРУЙТЕ!) ---
void GPIO_Init(void){
    // 1. Включение тактирования порта C
    enable_peripheral_clock();

    // 2. Настроить пины GPIO для LED
    PORT_CTRL &= ~(1 << 2); // настройка пина LED_PIN_NUM (Pin2) на OUTPUT
    PORT_OE |= (1 << 2); // Настройка пина на вывод
}

// --- Включение светодиода (АДАПТИРУЙТЕ!) ---
void LED_On(void){
    PORT_SET |= (1 << 2); //Установите бит в 1.  Настройка LED_PIN_NUM
}

// --- Выключение светодиода (АДАПТИРУЙТЕ!) ---
void LED_Off(void){
    PORT_RESET |= (1 << 2);  //Сбросьте бит в 0.  Настройка LED_PIN_NUM
}

// Функция для мигания светодиодом (как именно управлять GPIO на K1986BE92FI?)
void BlinkLED(int numBlinks) {
    for (int i = 0; i < numBlinks; i++) {
        LED_On();  // Включить светодиод
        for (volatile int j = 0; j < 500000; j++);  // Короткая задержка
        LED_Off();  // Выключить светодиод
        for (volatile int j = 0; j < 500000; j++);   // Короткая задержка
    }
}

int main(void) {
    SPI_Slave_Init();
    GPIO_Init();

    while (1) {
        uint8_t received = SPI_Slave_Receive();

        // Отправляем полученное число обратно
        SPI_DATA = received; //ЭТО ОЧЕНЬ ВАЖНО! ВЕДУЩИЙ ЖДЕТ, ЧТО ВЕДОМЫЙ ЧТО-ТО ОТПРАВИТ.

        //Моргаем, если полученное число равно 0x5A
        BlinkLED(received == 0x5A ? 3 : 0);
    }
}

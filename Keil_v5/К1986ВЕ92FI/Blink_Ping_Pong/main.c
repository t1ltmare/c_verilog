//Содержимое файла main.c
#include <MDR32FxQI_port.h>
#include <MDR32FxQI_rst_clk.h>

// Определения для светодиодов
#define LED1_PORT MDR_PORTC
#define LED1_PIN  PORT_Pin_2
#define LED2_PORT MDR_PORTA
#define LED2_PIN  PORT_Pin_3
#define LED3_PORT MDR_PORTA  
#define LED3_PIN  PORT_Pin_5  
#define LED4_PORT MDR_PORTF
#define LED4_PIN  PORT_Pin_0

// Простейшая функция задержки, позднее мы заменим ее на реализацию через таймер
void Delay(int waitTicks) {
    volatile int i; // volatile чтобы компилятор не выкинул цикл
    for (i = 0; i < waitTicks; i++) {
        __NOP();
    }
}

// Функция инициализации портов GPIO
void InitGPIO(void);

// Точка входа, отсюда начинается исполнение программы
int main() {
    // Включаем тактирование нужных портов
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTC | RST_CLK_PCLK_PORTA | RST_CLK_PCLK_PORTF, ENABLE);

    // Инициализация GPIO портов
    InitGPIO();

    // Массив светодиодов
    MDR_PORT_TypeDef* ledsPort[] = {LED1_PORT, LED2_PORT, LED3_PORT, LED4_PORT};
    uint32_t ledsPin[] = {LED1_PIN, LED2_PIN, LED3_PIN, LED4_PIN};
    uint8_t numLeds = sizeof(ledsPort) / sizeof(ledsPort[0]); // Количество светодиодов
    uint8_t direction = 1;      // 1 - вперед, 0 - назад
    uint8_t currentLed = 0;    // Индекс текущего светодиода

    // Запускаем бесконечный цикл обработки - Основной цикл
    while (1) {
        // Выключить все светодиоды
        for (uint8_t i = 0; i < numLeds; i++) {
            PORT_ResetBits(ledsPort[i], ledsPin[i]);
        }

        // Включить текущий светодиод
        PORT_SetBits(ledsPort[currentLed], ledsPin[currentLed]);

        // Задержка
        Delay(250000); // Сокращаем задержку

        // Переход к следующему светодиоду
        if (direction == 1) {
            currentLed++;
            if (currentLed >= numLeds) {
                currentLed = numLeds - 2;
                direction = 0;
            }
        } else {
            currentLed--;
            if (currentLed == 255) {  // Underflow
                currentLed = 1;
                direction = 1;
            }
        }
    }
}

// Инициализация портов GPIO
void InitGPIO(void) {
    PORT_InitTypeDef GPIOInitStruct;

    // Включаем тактирование портов
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTC, ENABLE);
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTA, ENABLE);
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTF, ENABLE);

    // Инициализируем структуру конфигурации вывода(-ов) порта значениями по умолчанию
    PORT_StructInit(&GPIOInitStruct);

    // Общие настройки
    GPIOInitStruct.PORT_OE = PORT_OE_OUT;
    GPIOInitStruct.PORT_SPEED = PORT_SPEED_SLOW;
    GPIOInitStruct.PORT_MODE = PORT_MODE_DIGITAL;

    // Настройка PORTC Pin 2
    GPIOInitStruct.PORT_Pin = LED1_PIN;
    PORT_Init(LED1_PORT, &GPIOInitStruct);

    // Настройка PORTA Pin 3
    GPIOInitStruct.PORT_Pin = LED2_PIN;
    PORT_Init(LED2_PORT, &GPIOInitStruct);

    // Настройка PORTA Pin 5
    GPIOInitStruct.PORT_Pin = LED3_PIN;
    PORT_Init(LED3_PORT, &GPIOInitStruct);

    // Настройка PORTF Pin 0
    GPIOInitStruct.PORT_Pin = LED4_PIN;
    PORT_Init(LED4_PORT, &GPIOInitStruct);
}

#include "hw_init.h"              // Device config

void SysTickInit(void)
{
    SysTick -> LOAD |= (8000)-1;
    SysTick -> CTRL |= SysTick_CTRL_CLKSOURCE_Pos;
    SysTick -> CTRL |= SysTick_CTRL_COUNTFLAG_Pos;
    SysTick -> CTRL |= SysTick_CTRL_ENABLE_Pos;
}

void PORTC_Init(void)
{
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTC, ENABLE);

    PORT_InitTypeDef PortInit;

    PortInit.PORT_OE = PORT_OE_OUT;
    PortInit.PORT_FUNC = PORT_FUNC_PORT;
    PortInit.PORT_MODE = PORT_MODE_DIGITAL;
    PortInit.PORT_SPEED = PORT_SPEED_SLOW;
    PortInit.PORT_Pin = PORT_Pin_0;
    PORT_Init(MDR_PORTC, &PortInit);
}

void UART_InitFunction(void)
{
    RST_CLK_PCLKcmd(RST_CLK_PCLK_UART2, ENABLE);  // Включаем тактирование для UART2
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTA, ENABLE);  // Включаем тактирование для PORTA, так как UART2 обычно использует пины PORTA

    PORT_InitTypeDef PortInit;
    UART_InitTypeDef UART_InitStructure;

    PortInit.PORT_PULL_UP = PORT_PULL_UP_OFF;
    PortInit.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
    PortInit.PORT_PD_SHM = PORT_PD_SHM_OFF;
    PortInit.PORT_PD = PORT_PD_DRIVER;
    PortInit.PORT_GFEN = PORT_GFEN_OFF;
    PortInit.PORT_FUNC = PORT_FUNC_ALTER;
    PortInit.PORT_SPEED = PORT_SPEED_MAXFAST;
    PortInit.PORT_MODE = PORT_MODE_DIGITAL;

    // НАСТРОЙКА ПОРТА PA0 НА ВЫВОД = UART2_TXD (Проверьте в документации, что это правильный пин!)
    PortInit.PORT_OE = PORT_OE_OUT;
    PortInit.PORT_Pin = PORT_Pin_0;  // PA0
    PORT_Init(MDR_PORTA, &PortInit);

    // НАСТРОЙКА ПОРТА PA1 НА ВВОД = UART2_RXD (Проверьте в документации, что это правильный пин!)
    PortInit.PORT_OE = PORT_OE_IN;
    PortInit.PORT_Pin = PORT_Pin_1; // PA1
    PORT_Init(MDR_PORTA, &PortInit);

    // УСТАНОВКА ПРЕДДЕЛИТЕЛЯ ЧАСТОТЫ UART2
    UART_BRGInit(MDR_UART2, UART_HCLKdiv2);

    // НАСТРОЙКА ПАРАМЕТРОВ UART2: 9600, 8БИТ, 1 СТОП БИТ, БЕЗ ПАРИТЕТА, ОТКЛ. БУФЕРА FIFO, КОНТРОЛЬ ПОТОКА RX/TX
    UART_InitStructure.UART_BaudRate = 9600;
    UART_InitStructure.UART_WordLength = UART_WordLength8b;
    UART_InitStructure.UART_StopBits = UART_StopBits1;
    UART_InitStructure.UART_Parity = UART_Parity_No;
    UART_InitStructure.UART_FIFOMode = UART_FIFO_OFF;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_RXE | UART_HardwareFlowControl_TXE;

    // КОНФИГУРИРОВАНИЕ ПАРАМЕТРОВ UART2
    while(UART_Init(MDR_UART2, &UART_InitStructure) != SUCCESS);
    UART_ITConfig(MDR_UART2, UART_IT_RX, ENABLE);

    // КОМАНДА ВКЛЮЧЕНИЯ UART2
    UART_Cmd(MDR_UART2, ENABLE);

    NVIC_EnableIRQ(UART2_IRQn);        // Разрешаем прерывания по UART2
    NVIC_SetPriority(UART2_IRQn, 1);    // Устанавливаем первый приоритет прерываниям UART2

}
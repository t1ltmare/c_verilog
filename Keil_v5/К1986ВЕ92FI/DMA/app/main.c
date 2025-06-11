#include "hw_init.h"              // Device config

volatile uint32_t delay_dec = 0;										// Переменная счетчика delay_dec
uint16_t dataReceive = 0;														// Переменная с полученными данными

void SysTick_Handler(void)													// Функция обратного отсчета
{
    if (delay_dec != 0) delay_dec--;
}

void delay_ms(uint32_t delay_ms)										// Функция задержки
{
    delay_dec = delay_ms;
    while(delay_dec){};
}

void UART2_IRQHandler(void) 
{
<<<<<<< HEAD
    if (UART_GetITStatus(MDR_UART2, UART_IT_RX) != RESET) 
    {
        UART_ClearITPendingBit(MDR_UART2, UART_IT_RX); 
        dataReceive = UART_ReceiveData(MDR_UART2); 
        while(UART_GetFlagStatus(MDR_UART2, UART_FLAG_BUSY)); 

        PORT_SetBits(MDR_PORTC, PORT_Pin_0);

        // Проверка команды и передача данных
        if (dataReceive == '1') {
            char greeting[] = "Hello from Milandr!\r\n";
            for (int i = 0; greeting[i] != '\0'; i++) {
                UART_SendData(MDR_UART2, greeting[i]);
                while(UART_GetFlagStatus(MDR_UART2, UART_FLAG_BUSY)); 
            }
        } else if (dataReceive == '0') {
            char farewell[] = "Goodbye from Milandr!\r\n";
            for (int i = 0; farewell[i] != '\0'; i++) {
                UART_SendData(MDR_UART2, farewell[i]); 
                while(UART_GetFlagStatus(MDR_UART2, UART_FLAG_BUSY)); 
            }
        } else {
            char invalid[] = "Invalid command!\r\n";
            for (int i = 0; invalid[i] != '\0'; i++) {
                UART_SendData(MDR_UART2, invalid[i]); 
                 while(UART_GetFlagStatus(MDR_UART2, UART_FLAG_BUSY)); 
            }
        }
        PORT_ResetBits(MDR_PORTC, PORT_Pin_0);
=======
    if (UART_GetITStatus(MDR_UART1, UART_IT_RX) != RESET)				// Если не ресет, тогда...
    {
        UART_ClearITPendingBit(MDR_UART1, UART_IT_RX);					// ??? Очистить полученные биты ???
        dataReceive = UART_ReceiveData(MDR_UART1);							// dataReceive присваиваем полученные с UART данные
        while(UART_GetFlagStatus(MDR_UART1, UART_FLAG_BUSY));		// Ждем флага конца операции
        PORT_SetBits(MDR_PORTC, PORT_Pin_0);										// ??? Зачем 1 в C0 ???
        UART_SendData(MDR_UART1, dataReceive);									// Отправляем полученные данные обратно
        while(UART_GetFlagStatus(MDR_UART1, UART_FLAG_BUSY));		// Ждем флаг
        PORT_ResetBits(MDR_PORTC, PORT_Pin_0);									// ??? Зачем ресетим C0 ???
>>>>>>> 608900108b7cabb4efc7e5591f4123c6433d9219
    }
}

int main()
{
<<<<<<< HEAD
    RST_CLK_HSIcmd(ENABLE);
    while(!RST_CLK_HSIstatus());
    SysTickInit();
    PORTC_Init();
    UART_InitFunction();
    PORT_ResetBits(MDR_PORTC, PORT_Pin_0);
=======
    RST_CLK_HSIcmd(ENABLE);													// Включаем клок и ресет?
    while(!RST_CLK_HSIstatus());										// Если статус не успешный, тогда ждем успешного статуса
    SysTickInit();																	// Включает систик для задержек
    PORTC_Init();																		// ??? Зачем инициализация порта C ???
    UART_InitFunction();														// Инициализация UART
    UART_ClearITPendingBit(MDR_UART1, UART_IT_RX);	// ??? Что это ???
    PORT_ResetBits(MDR_PORTC, PORT_Pin_0);					// Ресет бита C0
>>>>>>> 608900108b7cabb4efc7e5591f4123c6433d9219
    while(1)
    {
      /*  while(UART_GetFlagStatus(MDR_UART1, UART_FLAG_TXFE) == RESET);
        {

        }
        UART_SendData(MDR_UART1, 00);*/
    }
}
#include "hw_init.h"              // Device config

volatile uint32_t delay_dec = 0;
uint16_t dataReceive = 0;

void SysTick_Handler(void)
{
    if (delay_dec != 0) delay_dec--;
}

void delay_ms(uint32_t delay_ms)
{
    delay_dec = delay_ms;
    while(delay_dec){};
}

void UART1_IRQHandler(void)
{
    if (UART_GetITStatus(MDR_UART1, UART_IT_RX) != RESET)
    {
        UART_ClearITPendingBit(MDR_UART1, UART_IT_RX);
        dataReceive = UART_ReceiveData(MDR_UART1);
        while(UART_GetFlagStatus(MDR_UART1, UART_FLAG_BUSY));
        PORT_SetBits(MDR_PORTC, PORT_Pin_0);
        UART_SendData(MDR_UART1, dataReceive);
        while(UART_GetFlagStatus(MDR_UART1, UART_FLAG_BUSY));
        PORT_ResetBits(MDR_PORTC, PORT_Pin_0);
    }
}

int main()
{
    RST_CLK_HSIcmd(ENABLE);
    while(!RST_CLK_HSIstatus());
    SysTickInit();
    PORTC_Init();
    UART_InitFunction();
    UART_ClearITPendingBit(MDR_UART1, UART_IT_RX);
    PORT_ResetBits(MDR_PORTC, PORT_Pin_0);
    while(1)
    {
      /*  while(UART_GetFlagStatus(MDR_UART1, UART_FLAG_TXFE) == RESET);
        {
            
        }
        UART_SendData(MDR_UART1, 00);*/
    }
}

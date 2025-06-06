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
    RST_CLK_PCLKcmd(RST_CLK_PCLK_UART1, ENABLE);
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTB, ENABLE);
    
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
    
    // мюярпнийю онпрю PB5 мю бшбнд = UART1_TXD
    PortInit.PORT_OE = PORT_OE_OUT;
    PortInit.PORT_Pin = PORT_Pin_5;
    PORT_Init(MDR_PORTB, &PortInit);
    
    // мюярпнийю онпрю PB6 мю ббнд = UART1_RXD
    PortInit.PORT_OE = PORT_OE_OUT;
    PortInit.PORT_Pin = PORT_Pin_6;
    PORT_Init(MDR_PORTB, &PortInit);
    
    // сярюмнбйю опеддекхрекъ вюярнрш UART1 HCLKdiv = 1
    UART_BRGInit(MDR_UART1, UART_HCLKdiv2);
    
    // мюярпнийю оюпюлерпнб UART2: 115200, 8ахр, 1 ярно ахр, аег оюпхрерю, нрйк. астепю FIFO, йнмрпнкэ онрнйю RX/TX
    UART_InitStructure.UART_BaudRate = 9600;
    UART_InitStructure.UART_WordLength = UART_WordLength8b;
    UART_InitStructure.UART_StopBits = UART_StopBits1;
    UART_InitStructure.UART_Parity = UART_Parity_No;
    UART_InitStructure.UART_FIFOMode = UART_FIFO_OFF;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_RXE | UART_HardwareFlowControl_TXE;
    
    // йнмтхцспхпнбюмхе оюпюлерпнб UART1
    while(UART_Init(MDR_UART1, &UART_InitStructure) != SUCCESS);
    UART_ITConfig(MDR_UART1, UART_IT_RX, ENABLE);
    
    // йнлюмдю бйкчвемхъ UART1
    UART_Cmd(MDR_UART1, ENABLE);
    
    NVIC_EnableIRQ(UART1_IRQn);        // пЮГПЕЬЮЕЛ ОПЕПШБЮМХЪ ОН UART
    NVIC_SetPriority(UART1_IRQn, 1);    // сЯРЮМЮБКХБЮЕЛ ОЕПБШИ ОПХНПХРЕР ОПЕПШБЮМХЪЛ UART
    
}

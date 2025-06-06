#ifndef HW_INIT_H
#define HW_INIT_H
#include "MDR32FxQI_port.h"             // Milandr::Drivers:PORT
#include "MDR32FxQI_rst_clk.h"          // Milandr::Drivers:RST_CLK
#include "MDR32FxQI_uart.h"             // Milandr::Drivers:UART
#include "K1986VE9xI.h"                 // Device header

void SysTickInit(void);
void PORTC_Init(void);
void UART_InitFunction(void);

#endif

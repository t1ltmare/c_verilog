#include "stm32f411xe.h"                  // Device header

// PORTA_5 LED
// ������������ �����
// �������������� ���� � ���
// ������ ��������� ������ ���� � ����������� �����
// ������ �������� ����� ������� ����� �����, ��������, ����� for

int main(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // �������� ������������ GPIOA (������������ SET_BIT(RCC -> AHB1ENR, RCC_AHB1ENR_GPIOAEN);
    
    GPIOA->MODER &= ~((0x03UL) << (5 * 2)); // �������� ���� MODER ��� PA5

    GPIOA->MODER |= ((0x01UL) << (5 * 2)); // ���������� PA5 ��� ����� (01)
    
    while (1)
   {
        // ���������� PA5 � ������� �������
        GPIOA->BSRR = (1 << 5);  // ���������� ��� 5 � ODR (GPIOA->ODR = GPIO_ODR_OD5;)
       
        // ��������
        for (volatile uint32_t i = 0; i < 1000000; i++);

        // �������� PA5 � ������ �������
        GPIOA->BSRR = (1 << (5 + 16)); // �������� ��� 5 � ODR  (GPIOA->ODR &= ~GPIO_ODR_OD5;)

        // ��������
        for (volatile uint32_t i = 0; i < 1000000; i++);
   }   
   
}
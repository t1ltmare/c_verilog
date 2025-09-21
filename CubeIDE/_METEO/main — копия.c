/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32f4xx_hal.h"
#include "SSD1680.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DISP_CS_Pin       	GPIO_PIN_15
#define DISP_CS_GPIO_Port 	GPIOA
#define EDP_D_C_Pin       	GPIO_PIN_1
#define EDP_D_C_GPIO_Port 	GPIOA
#define DISP_RST_Pin      	GPIO_PIN_2
#define DISP_RST_GPIO_Port 	GPIOA
#define DISP_BUSY_Pin     	GPIO_PIN_0
#define DISP_BUSY_GPIO_Port GPIOA
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */
SSD1680_HandleTypeDef hepd;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */
static void MX_SSD1680_Init(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  HAL_Delay(100);

  MX_SSD1680_Init();

  // Очистка экрана
  SSD1680_Clear(&hepd, ColorWhite);
  SSD1680_StartAddress(hepd, 0, 0);

  // Вывод текста
  SSD1680_Text(&hepd, 0, 10, "Hello World!", &cp866_8x8);
  SSD1680_Text(&hepd, 0, 30, "SSD1680 E-Paper", &cp866_8x8);
  SSD1680_Text(&hepd, 0, 50, "2.13\" 250x122", &cp866_8x8);

  // Красная линия (правильный расчет размера буфера)
  uint8_t red_line[16 * 2] = {0}; // 122 пикселей = 16 байт на строку
  for(int i = 0; i < sizeof(red_line); i++) {
      red_line[i] = 0xFF;
  }
  SSD1680_SetRegion(&hepd, 0, 70, 122, 2, NULL, red_line);

  // Обновление дисплея
  SSD1680_Refresh(&hepd, FullRefresh);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  HAL_Delay(1000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, EDP_D_C_Pin|EPD_RESET_Pin|EPD_CS_Pin|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin : EDP_BUSY_Pin */
  GPIO_InitStruct.Pin = EDP_BUSY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(EDP_BUSY_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : EDP_D_C_Pin EPD_RESET_Pin EPD_CS_Pin PA15 */
  GPIO_InitStruct.Pin = EDP_D_C_Pin|EPD_RESET_Pin|EPD_CS_Pin|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
static void MX_SSD1680_Init(void) {
    hepd.SPI_Handle = &hspi1;
    hepd.SPI_Timeout = 1000;
    hepd.CS_Port = EPD_CS_GPIO_Port;
    hepd.CS_Pin = EPD_CS_Pin;
    hepd.DC_Port = EDP_D_C_GPIO_Port;
    hepd.DC_Pin = EDP_D_C_Pin;
    hepd.RESET_Port = EPD_RESET_GPIO_Port;
    hepd.RESET_Pin = EPD_RESET_Pin;
    hepd.BUSY_Port = EDP_BUSY_GPIO_Port;
    hepd.BUSY_Pin = EDP_BUSY_Pin;
    hepd.Color_Depth = 1;
    hepd.Scan_Mode = NarrowScan;
    hepd.Resolution_X = 122;
    hepd.Resolution_Y = 250;

    // 1. Аппаратный сброс
    SSD1680_Reset(&hepd);

    // 2. Программный сброс (команда 0x12)
    SSD1680_Send(&hepd, SSD1680_SW_RESET, NULL, 0);
    HAL_Delay(10);
    SSD1680_Wait(&hepd);

    // 3. Установка режима данных (команда 0x11)
    uint8_t data_mode = 0x03; // RightThenDown
    SSD1680_Send(&hepd, SSD1680_DATA_ENTRY_MODE, &data_mode, DownThenLeft);

    // 4. Установка размеров RAM (команды 0x44, 0x45)
    uint8_t ram_x_range[] = {0, 0, (250-1) & 0xFF, (250-1) >> 8}; // 250 строк
    SSD1680_Send(&hepd, SSD1680_RAM_X_RANGE, ram_x_range, sizeof(ram_x_range));

    uint8_t ram_y_range[] = {0, (122-1)/8}; // 122 пикселей = 15.25 байт → 16 байт
    SSD1680_Send(&hepd, SSD1680_RAM_Y_RANGE, ram_y_range, sizeof(ram_y_range));

    // 5. Настройка напряжений
    uint8_t gate_voltage = 0x17; // VGH=20V, VGL=-20V
    SSD1680_Send(&hepd, SSD1680_GATE_VOLTAGE, &gate_voltage, 1);

    uint8_t source_voltage[] = {0x41, 0x00, 0x32}; // VSH=15V, VSL=-15V
    SSD1680_Send(&hepd, SSD1680_SOURCE_VOLTAGE, source_voltage, sizeof(source_voltage));

    uint8_t vcom_voltage = 0x36; // VCOM= -1.5V
    SSD1680_Send(&hepd, SSD1680_VCOM_VOLTAGE, &vcom_voltage, 1);

    // 6. Мягкий старт (команда 0x0C)
    uint8_t softstart[] = {0x80, 0x90, 0x90, 0x00};
    SSD1680_Send(&hepd, SSD1680_BOOSTER_SOFT_START, softstart, sizeof(softstart));

    // 7. Граница (команда 0x3C)
    uint8_t border = 0x01; // Черная граница
    SSD1680_Send(&hepd, SSD1680_BORDER, &border, 1);

    // 8. Загрузка LUT
    uint8_t temp_sensor = 0x80; // Внутренний датчик температуры
    SSD1680_Send(&hepd, SSD1680_SELECT_TEMP_SENSOR, &temp_sensor, 1);

    uint8_t temp_value = 0x64; // Значение температуры
    SSD1680_Send(&hepd, SSD1680_WRITE_TEMP, &temp_value, 1);

    uint8_t lut_load = 0x91; // Загрузка LUT
    SSD1680_Send(&hepd, SSD1680_UPDATE_CONTROL_2, &lut_load, 1);
    SSD1680_Send(&hepd, SSD1680_MASTER_ACTIVATION, NULL, 0);
    SSD1680_Wait(&hepd);

}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

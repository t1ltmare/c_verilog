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
#define DISP_DC_Pin       	GPIO_PIN_1
#define DISP_DC_GPIO_Port 	GPIOA
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
  HAL_Delay(10);
  MX_SSD1680_Init();

  // Шаг 2: Set initial configuration
  //SSD1680_Reset(&hepd);
  //SSD1680_Send(&hepd, 0x12, NULL, 0); // SW reset
  HAL_Delay(10);

  // Шаг 3: Send initialization code
  SSD1680_GateScanRange(&hepd, 0, hepd.Resolution_Y); // Set gate driver output
  SSD1680_Border(&hepd, ColorWhite);
  SSD1680_Clear(&hepd, ColorWhite);

  // Шаг 4: Load waveform LUT
  uint8_t temp_sensor = 0x80; // Внутренний датчик температуры
  SSD1680_Send(&hepd, 0x18, &temp_sensor, 1);
  uint8_t lut_load = 0x91; // Загрузка LUT
  SSD1680_Send(&hepd, 0x22, &lut_load, 1);
  SSD1680_Send(&hepd, 0x20, NULL, 0); // Активация
  SSD1680_Wait(&hepd);
  uint8_t scale[122 / 8 * 4 ] = {
      0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
      0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
      0x7F, 0xFF, 0x7F, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF,
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    };
    SSD1680_SetRegion(&hepd, 0, 0, 122, 4, scale, NULL);

    SSD1680_Text(&hepd, 0, 16, "Font 8x8", &cp866_8x8);
    SSD1680_Text(&hepd, 0, 24, "Font 8x14", &cp866_8x14);
    SSD1680_Text(&hepd, 0, 38, "Font 8x16", &cp866_8x16);
  // Шаг 5: Write image and drive display panel
  // Вывод текста
  //SSD1680_Wait();
  //SSD1680_Text(&hepd, 0, 0, "Hello, World!", &cp866_8x8);
  //HAL_Delay(1000);
  // Настройка мягкого старта
  uint8_t softstart[] = {0x80, 0x90, 0x90, 0x00};
  SSD1680_Send(&hepd, 0x0C, softstart, sizeof(softstart));
  // Обновление дисплея
  uint8_t refresh_mode = 0xF7; // Полное обновление
  //SSD1680_UpdateControl();
  SSD1680_Send(&hepd, 0x22, &refresh_mode, 1);
  SSD1680_Send(&hepd, 0x20, NULL, 0);
  SSD1680_Wait(&hepd);

  // Шаг 6: Power off
  //SSD1680_DeepSleep(&hepd);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  hepd.CS_Port = DISP_CS_GPIO_Port;
  hepd.CS_Pin = DISP_CS_Pin;
  hepd.DC_Port = DISP_DC_GPIO_Port;
  hepd.DC_Pin = DISP_DC_Pin;
  hepd.RESET_Port = DISP_RST_GPIO_Port;
  hepd.RESET_Pin = DISP_RST_Pin;
  hepd.BUSY_Port = DISP_BUSY_GPIO_Port;
  hepd.BUSY_Pin = DISP_BUSY_Pin;
  hepd.Color_Depth = 1;
  hepd.Scan_Mode = 1;
  hepd.Resolution_X = 122;
  hepd.Resolution_Y = 250;
  SSD1680_Init(&hepd);
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

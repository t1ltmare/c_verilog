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
#define EPD_CS_Pin       	GPIO_PIN_4
#define EPD_CS_GPIO_Port 	GPIOA
#define EPD_DC_Pin       	GPIO_PIN_1
#define EPD_DC_GPIO_Port 	GPIOA
#define EPD_RST_Pin      	GPIO_PIN_2
#define EPD_RST_GPIO_Port 	GPIOA
#define EPD_BUSY_Pin     	GPIO_PIN_0
#define EPD_BUSY_GPIO_Port 	GPIOA

#define TEMP 75

// LeftThenUp = 0,   /**< X decrements then Y decrements. Starts from bottom-right corner and goes left. */
// RightThenUp,      /**< X increments then Y decrements. Starts from bottom-left corner and goes right. */
// LeftThenDown,     /**< X decrements then Y increments. Starts from top-right corner and goes left. */
// RightThenDown,    /**< X increments then Y increments. Starts from top-left corner and goes right. */
// UpThenLeft,       /**< Y decrements then X decrements. Starts from bottom-right corner and goes up. */
// UpThenRight,      /**< Y decrements then X increments. Starts from bottom-left corner and goes up. */
// DownThenLeft,     /**< Y increments then X decrements. Starts from top-right corner and goes down. */
// DownThenRight     /**< Y increments then X increments. Starts from top-left corner and goes gown. */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */
SSD1680_HandleTypeDef hepd;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_I2C1_Init(void);
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
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  // Заполнение параметров дисплея
	hepd.SPI_Handle = &hspi1;  		// Ваш SPI-хендл
	hepd.SPI_Timeout = 1000;
	hepd.CS_Port = EPD_CS_GPIO_Port;
	hepd.CS_Pin = EPD_CS_Pin;
	hepd.DC_Port = EPD_DC_GPIO_Port;
	hepd.DC_Pin = EPD_DC_Pin;
	hepd.RESET_Port = EPD_RST_GPIO_Port;
	hepd.RESET_Pin = EPD_RST_Pin;
	hepd.BUSY_Port = EPD_BUSY_GPIO_Port;
	hepd.BUSY_Pin = EPD_BUSY_Pin;
	hepd.Color_Depth = 2;      		// Два цвета (черный + красный)
	hepd.Scan_Mode = NarrowScan; 	// Режим сканирования
	hepd.Resolution_X = 128;   		// Ширина дисплея
	hepd.Resolution_Y = 256;   		// Высота дисплея

	// �?нициализация дисплея
	SSD1680_Init(&hepd);
	SSD1680_Border(&hepd, ColorWhite);
	SSD1680_Clear(&hepd, ColorWhite);

	Bar_16(1, 20, 100, 8, 148);
	Bar_16(0, 40, 100, 32, 148);
	Bar_8(1, 60, 100, 56, 148);
	Bar_8(0, 80, 100, 80, 148);

    SSD1680_VerticalText(&hepd, 80, 100, "TEMP", &cp866_8x8_r);
    SSD1680_VerticalText(&hepd, 56, 100, "HUMID", &cp866_8x8_r);
    SSD1680_VerticalText(&hepd, 32, 100, "PRESS", &cp866_8x16_r);
    SSD1680_VerticalText(&hepd, 8, 100, "CO2", &cp866_8x16_r);

    //SSD1680_SetRegion(&hepd, 32, 148, 16, 100, red_line, NULL);

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
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

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
  __HAL_RCC_GPIOB_CLK_ENABLE();

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
void Bar_16(uint8_t color, uint8_t num, uint8_t length, uint8_t x, uint8_t y) {
    uint8_t elements = length * 2;
    uint8_t line[elements];
    uint8_t numx = num*2;

    // Определяем паттерны для разных цветов
    const uint8_t patterns[2][3] = {
        {0x00, 0xFE, 0x7F}, // Black: fill, odd, even
        {0xFF, 0x01, 0x80}  // Red:   fill, odd, even
    };

    uint8_t fill = patterns[color][0];
    uint8_t odd_pattern = patterns[color][1];
    uint8_t even_pattern = patterns[color][2];

    uint8_t end_index = (numx % 2) ? elements - 2 : elements;
    uint8_t condition = (numx % 2) ? 1 : 0;

    for(int i = 2; i < end_index; i++) {
        if(condition ? (i <= numx) : (i < numx)) {
            line[i] = fill;
        } else {
            line[i] = (i % 2) ? odd_pattern : even_pattern;
        }
    }

    line[0] = line[1] = fill;
    line[elements-2] = line[elements-1] = fill;

    // Передаем данные в драйвер
    if(color) {
        SSD1680_SetRegion(&hepd, x, y, 16, length, NULL, line);
    } else {
        SSD1680_SetRegion(&hepd, x, y, 16, length, line, NULL);
    }
}

void Bar_8(uint8_t color, uint8_t num, uint8_t length, uint8_t x, uint8_t y) {
    uint8_t line[length];

    // Определяем паттерны для разных цветов
    const uint8_t patterns[2][2] = {
        {0x00, 0x7E}, // Black: fill, pattern
        {0xFF, 0x81}  // Red:   fill, pattern
    };

    uint8_t fill = patterns[color][0];
    uint8_t pattern = patterns[color][1];

    for(int i = 1; i < length; i++) {
        if(i <= num) {
            line[i] = fill;
        } else {
            line[i] = pattern;
        }
    }

    line[0] = fill;
    line[length-1] = fill;

    // Передаем данные в драйвер
    if(color) {
        SSD1680_SetRegion(&hepd, x, y, 8, length, NULL, line);
    } else {
        SSD1680_SetRegion(&hepd, x, y, 8, length, line, NULL);
    }
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

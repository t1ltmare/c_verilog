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
#include "bme280.h"
#include "bme68x.h"
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

// Адрес BME688 (SDO подключен к VCC для адреса 0x77)
#define BME688_ADDR    BME68X_I2C_ADDR_HIGH  // 0x77
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
SPI_HandleTypeDef hspi1;
/* USER CODE BEGIN PV */
SSD1680_HandleTypeDef hepd;

uint8_t scd41_addr = 0x62 << 1; //7 bit address shifted left
uint16_t co2 = 0; //co2 in ppm
float tempscd; // temp in F
float rh; //rh in %

// BME688 variables
struct bme68x_dev bme688_sensor;
uint8_t bme688_addr = BME688_ADDR;
uint8_t bme688_sensorI2cAddress = BME688_ADDR; 

// BME688 configuration
struct bme68x_conf config0;
struct bme68x_heatr_conf heaterConfig0;
struct bme68x_data data;
uint16_t sample_count = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_I2C1_Init(void);

/* USER CODE BEGIN PFP */

void scd41_start(void);
uint8_t scd41_read(uint16_t *co2, float *temp, float *rh);

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

  bme280_init(&hi2c1, BME280_OVERSAMPLE_X1, BME280_OVERSAMPLE_X1, BME280_OVERSAMPLE_X1, BME280_NORMAL_MODE, BME280_STANDBY_0_5MS, BME280_FILTER_COEFF_16);

  // Initialize BME688 sensors
  memset(&bme688_sensor, 0, sizeof(bme688_sensor));
  bme688_sensor.intf = BME68X_I2C_INTF;
  bme688_sensor.read = bme68x_i2c_read;
  bme688_sensor.write = bme68x_i2c_write;
  bme688_sensor.delay_us = bme68x_delay_us;
  bme688_sensor.intf_ptr = &bme688_sensorI2cAddress;
  bme688_sensor.amb_temp = 25;

  // Initialize BME688 sensors
  int8_t initResult = bme68x_interface_init(&bme688_sensor, BME68X_I2C_INTF);
  bme68x_check_rslt("bme68x_interface_init", initResult);

  if (initResult == BME68X_OK) {
      initResult = bme68x_init(&bme688_sensor);
      bme68x_check_rslt("bme68x_init", initResult);
  }

  // Configure BME688
  if (initResult == BME68X_OK) {
      config0.filter = BME68X_FILTER_OFF;
      config0.odr = BME68X_ODR_NONE;
      config0.os_hum = BME68X_OS_16X;
      config0.os_pres = BME68X_OS_1X;
      config0.os_temp = BME68X_OS_2X;

      initResult = bme68x_set_conf(&config0, &bme688_sensor); // Исправлено
      bme68x_check_rslt("bme68x_set_conf", initResult);
  }

  if (initResult == BME68X_OK) {
      heaterConfig0.enable = BME68X_ENABLE;
      heaterConfig0.heatr_temp = 300;
      heaterConfig0.heatr_dur = 100;

      initResult = bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heaterConfig0, &bme688_sensor); // Исправлено
      bme68x_check_rslt("bme68x_set_heatr_conf", initResult);
  }

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

	// Инициализация дисплея
	SSD1680_Init(&hepd);
	SSD1680_Border(&hepd, ColorWhite);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	scd41_start();
	HAL_Delay(5000);  // warm-up delay

	while (1)
	{
		float temp = bme280_get_temperature();
		float pressure = bme280_get_pressure() / 133.32;
		float h = bme280_get_humidity();

		char temp_str[20];
		char pressure_str[20];
		char h_str[20];
		char temp_scd41[20];
		char h_scd41[20];
		char co2_scd41[20];
		char bme688_temp[20];
		char bme688_hum[20];
		char bme688_pres[20];
		char bme688_gas[20];

		scd41_read(&co2, &tempscd, &rh);

		// Read BME688 data
    int8_t setModeResult = bme68x_set_op_mode(BME68X_FORCED_MODE, &bme688_sensor);

    if (setModeResult == BME68X_OK) {
      uint32_t del_period = bme68x_get_meas_dur(BME68X_FORCED_MODE, &config0, &bme688_sensor) + (heaterConfig0.heatr_dur * 1000);
      bme688_sensor.delay_us(del_period, bme688_sensor.intf_ptr);

      uint8_t n_fields;
      int8_t getDataResult = bme68x_get_data(BME68X_FORCED_MODE, &data, &n_fields, &bme688_sensor);
      
      if (getDataResult == BME68X_OK) {
        snprintf(temp_str, sizeof(temp_str), "TEMP %.1f\xF8" "C", temp);
        snprintf(h_str, sizeof(h_str), "HUMID %.1f%%", h);
        snprintf(pressure_str, sizeof(pressure_str), "PRESS %.1f", pressure);
        snprintf(temp_scd41, sizeof(temp_scd41), "TEMP41 %.1f\xF8" "C", tempscd);
        snprintf(h_scd41, sizeof(h_scd41), "HUM41 %.1f%%", rh);
        snprintf(co2_scd41, sizeof(co2_scd41), "CO2 %.1d", co2);
      } else {
        bme68x_check_rslt("bme68x_get_data", getDataResult);
      }
    } else {
        bme68x_check_rslt("bme68x_set_op_mode", setModeResult);
    }

		if (n_fields) {
		  snprintf(bme688_temp, sizeof(bme688_temp), "B0T: %d.%02dC",
				   (int)data.temperature, (int)((data.temperature - (int)data.temperature) * 100));
		  snprintf(bme688_hum, sizeof(bme688_hum), "B0H: %d.%01d%%",
				   (int)data.humidity, (int)((data.humidity - (int)data.humidity) * 10));
		  snprintf(bme688_pres, sizeof(bme688_pres), "B0P: %dhPa",
				   (int)(data.pressure / 100.0F));
		  snprintf(bme688_gas, sizeof(bme688_gas), "B0G: %lu",
				   (long unsigned int)data.gas_resistance);
		} else {
		  strcpy(bme688_temp, "B0T: ---");
		  strcpy(bme688_hum, "B0H: ---");
		  strcpy(bme688_pres, "B0P: ---");
		  strcpy(bme688_gas, "B0G: ---");
		}

		SSD1680_Clear(&hepd, ColorWhite);

		Bar_8(1, 20, 100, 8, 148);
		Bar_8(0, 40, 100, 32, 148);
		Bar_8(1, 60, 100, 56, 148);
		Bar_8(0, 80, 100, 80, 148);

		SSD1680_VerticalText(&hepd, 88, 148, temp_str, &cp866_8x8_r);
		SSD1680_VerticalText(&hepd, 64, 148, h_str, &cp866_8x8_r);
		SSD1680_VerticalText(&hepd, 40, 148, pressure_str, &cp866_8x8_r);
		SSD1680_VerticalText(&hepd, 16, 148, co2_scd41, &cp866_8x8_r);
		SSD1680_VerticalText(&hepd, 88, 10, temp_scd41, &cp866_8x8_r);
		SSD1680_VerticalText(&hepd, 64, 10, h_scd41, &cp866_8x8_r);

    // Display BME688 data
    SSD1680_VerticalText(&hepd, 48, 10, bme688_temp, &cp866_8x8_r);
    SSD1680_VerticalText(&hepd, 40, 10, bme688_hum, &cp866_8x8_r);
    SSD1680_VerticalText(&hepd, 32, 10, bme688_pres, &cp866_8x8_r);
    SSD1680_VerticalText(&hepd, 24, 10, bme688_gas, &cp866_8x8_r);

		SSD1680_Refresh(&hepd, FullRefresh);
		SSD1680_Wait(&hepd);

		sample_count++;
		HAL_Delay(60000);
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

//start periodic measurement (command 0x21B1)
void scd41_start(void){
  uint8_t cmd[2] = {0x21, 0xB1}; //split command into 2 bytes
  HAL_I2C_Master_Transmit(&hi2c1, scd41_addr, cmd, 2, HAL_MAX_DELAY);
}

//read measurement (command 0xEC05)
uint8_t scd41_read(uint16_t *co2, float *temp, float *rh){
  uint8_t cmd[2] = {0xEC, 0x05};
  uint8_t rx[9];

  HAL_I2C_Master_Transmit(&hi2c1, scd41_addr, cmd, 2, HAL_MAX_DELAY);
  HAL_Delay(1);

  if (HAL_I2C_Master_Receive(&hi2c1, scd41_addr, rx, 9, HAL_MAX_DELAY) != HAL_OK)
    return 0;

  *co2 = (rx[0] << 8) | rx[1];
  uint16_t temp_raw = (rx[3] << 8) | rx[4];
  float temp_c = -45.0f + 175.0f * ((float)temp_raw / 65535.0f);
  *temp = temp_c;// * 9.0f / 5.0f + 32.0f;

  uint16_t rh_raw = (rx[6] << 8) | rx[7];
  *rh = 100.0f * ((float)rh_raw / 65535.0f);

  return 1;
}

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

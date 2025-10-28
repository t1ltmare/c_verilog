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
#include "stdio.h"
#include "ssd1306.h"
#include "ssd1306_tests.h"
#include "ssd1306_fonts.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

IWDG_HandleTypeDef hiwdg;

RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim9;
TIM_HandleTypeDef htim11;

/* USER CODE BEGIN PV */
char timeData[15];
char dateData[15];
char feedTime[15];

typedef enum {
    STATE_IDLE,
    STATE_FEEDING,
    STATE_PAUSE
} feeding_state_t;

feeding_state_t feeding_state = STATE_IDLE;
uint32_t feeding_start_time = 0;
uint8_t feeding_step = 0;
uint8_t current_servo_id = 0;

uint16_t encval = 0;
int16_t enc_total = 0;
int16_t enc_last = 0;
int16_t diff;
int16_t curr_counter;
int16_t prevCounter = 0;
uint8_t hours, minutes;
uint8_t frame = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM11_Init(void);
static void MX_I2C1_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM2_Init(void);
static void MX_IWDG_Init(void);
static void MX_TIM9_Init(void);
/* USER CODE BEGIN PFP */
uint8_t get_current_servo_id(void);
void set_servo_angle(uint8_t servo_id, uint16_t angle);
void set_time (uint8_t hr, uint8_t min, uint8_t sec);
void set_date (uint8_t year, uint8_t month, uint8_t date, uint8_t day);
void set_alarm (uint8_t hr, uint8_t min, uint8_t sec, uint8_t date);
void get_time_date(char *time, char *date);
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc);
void get_enc(char *feedTime, uint8_t hours, uint8_t minutes);
void get_hours(char *lastNum, uint8_t *currNum);
void get_minutes(char *lastNum, uint8_t *currNum);
void ssd1306_TestDrawBitmap(uint8_t frame);
//void encoder_to_time(uint16_t encval, uint16_t hours, uint16_t minutes);
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
  MX_TIM11_Init();
  MX_I2C1_Init();
  MX_RTC_Init();
  MX_TIM2_Init();
  MX_IWDG_Init();
  MX_TIM9_Init();
  /* USER CODE BEGIN 2 */
  // �?нициализация дисплея
  ssd1306_Init();
  uint8_t y = 0;
  ssd1306_Fill(Black);

  // �?нициализация Ш�?М, энкодера
  HAL_TIM_PWM_Start_IT(&htim11, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start_IT(&htim9, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start_IT(&htim9, TIM_CHANNEL_2);

  __HAL_TIM_SET_COUNTER(&htim2, 0);
  HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);

  // �?нициализация времени и даты
  get_time_date(timeData, dateData);

  // Настройка времени
  RTC_TimeTypeDef gTime;
  HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
  uint8_t currNum;
  char hoursT[15];
  char minutesT[15];
  uint8_t hoursTime;
  uint8_t minutesTime;

  uint32_t timeout_start = HAL_GetTick();
  ssd1306_Fill(Black);
  ssd1306_UpdateScreen();

  while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == GPIO_PIN_SET) {
	  HAL_IWDG_Refresh(&hiwdg);

	  if (HAL_GetTick() - timeout_start > 30000) {
		  hoursTime = 0;  // Автоматически выставляем 0 часов
		  break;
	  }

	  get_hours(hoursT, &currNum);

	  ssd1306_Fill(Black);
	  ssd1306_SetCursor(2, 0);
	  ssd1306_WriteString("Set hours:", Font_7x10, White);
	  ssd1306_SetCursor(2, 15);
	  ssd1306_WriteString(hoursT, Font_16x26, White);
	  ssd1306_UpdateScreen();

	  hoursTime = currNum;
	  HAL_Delay(10);
  }

  HAL_Delay(500);
  enc_total = 0;
  enc_last = __HAL_TIM_GET_COUNTER(&htim2);
  timeout_start = HAL_GetTick();

  while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == GPIO_PIN_SET) {
	  HAL_IWDG_Refresh(&hiwdg);

	  if (HAL_GetTick() - timeout_start > 30000) {
		  hoursTime = 0;  // Автоматически выставляем 0 часов
		  break;
	  }

	  get_minutes(minutesT, &currNum);

	  ssd1306_Fill(Black);
	  ssd1306_SetCursor(2, 0);
	  ssd1306_WriteString("Set minutes:", Font_7x10, White);
	  ssd1306_SetCursor(2, 15);
	  ssd1306_WriteString(minutesT, Font_16x26, White);
	  ssd1306_UpdateScreen();

	  minutesTime = currNum;
	  HAL_Delay(10);
  }
/*
  while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_SET) {
 	  get_hours(lastNum, currNum);
 	  ssd1306_SetCursor(2, 0);
 	  ssd1306_WriteString(lastNum, Font_16x26, White);
 	  ssd1306_UpdateScreen();
   }
*/
  ssd1306_SetCursor(2, y);
  ssd1306_WriteString("Font 16x26", Font_16x26, White);
  y += 26;

  ssd1306_SetCursor(2, y);
  ssd1306_WriteString("Font 11x18", Font_11x18, White);
  y += 18;

  ssd1306_SetCursor(2, y);
  ssd1306_WriteString("Font 7x10", Font_7x10, White);
  y += 10;

  ssd1306_SetCursor(2, y);
  ssd1306_WriteString("Font 6x8", Font_6x8, White);

  ssd1306_UpdateScreen();
  HAL_Delay(1000);

  set_time(hoursTime, minutesTime, 0);

  enc_total = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  HAL_IWDG_Refresh(&hiwdg);

	  get_time_date(timeData, dateData);
	  get_enc(feedTime, hours, minutes);

	  if (feeding_state != STATE_PAUSE) {
		  ssd1306_Fill(Black);
		  ssd1306_SetCursor(2, 0);
		  ssd1306_WriteString("Feeding time", Font_7x10, White);
		  ssd1306_SetCursor(2, 10);
		  ssd1306_WriteString(feedTime, Font_11x18, White);
		  ssd1306_SetCursor(2, 32);
		  ssd1306_WriteString("Current time", Font_7x10, White);
		  ssd1306_SetCursor(2, 42);
		  ssd1306_WriteString(timeData, Font_11x18, White);
		  ssd1306_UpdateScreen();
		  HAL_Delay(10);
	  } else {
		  ssd1306_TestDrawBitmap(frame);
		  frame++;
		  frame = (frame > 5) ? 0 : frame;
	  }

	  switch (feeding_state) {
	          case STATE_IDLE:
	              if (!strcmp(feedTime, timeData)) {
	                  feeding_state = STATE_FEEDING;
	                  feeding_step = 0;
	                  feeding_start_time = HAL_GetTick();
	                  current_servo_id = get_current_servo_id();
	                  set_servo_angle(current_servo_id, 0);
	              }
	              break;

	          case STATE_FEEDING:
	              if (HAL_GetTick() - feeding_start_time >= 0) {
	            	  set_servo_angle(current_servo_id, 180);
	                  feeding_state = STATE_PAUSE;
	                  feeding_start_time = HAL_GetTick();
	                  feeding_step = 1;
	              }
	              break;

	          case STATE_PAUSE:

				  static const uint32_t pause_durations[] = {1000, 100, 100, 100, 100, 100, 5000};
				  static const uint16_t servo_angles[] = {165, 180, 165, 180, 165, 180, 0};

				  if (feeding_step >= 1 && feeding_step <= 7) {
					  if (HAL_GetTick() - feeding_start_time >= pause_durations[feeding_step - 1]) {
						  set_servo_angle(current_servo_id, servo_angles[feeding_step - 1]);
						  feeding_start_time = HAL_GetTick();
						  feeding_step++;

						  if (feeding_step > 7) {
							  feeding_state = STATE_IDLE;
							  set_servo_angle(current_servo_id, 0);  // Возврат в исходное положение
						  }
					  }
				  }
				  break;
	  	  }

	  	  HAL_Delay(10);  // Короткая задержка для стабильности

	  //ssd1306_TestAll();

	  /*
	  set_servo_angle(0);
	  HAL_Delay(50000);
	  set_servo_angle(180);
	  HAL_Delay(1000);
	  set_servo_angle(165);
	  HAL_Delay(100);
	  set_servo_angle(180);
	  HAL_Delay(100);
	  set_servo_angle(165);
	  HAL_Delay(100);
	  set_servo_angle(180);
	  HAL_Delay(100);
	  set_servo_angle(165);
	  HAL_Delay(100);
	  set_servo_angle(180);
	  HAL_Delay(5000);
	  */
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI
                              |RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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
  hi2c1.Init.ClockSpeed = 400000;
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
  * @brief IWDG Initialization Function
  * @param None
  * @retval None
  */
static void MX_IWDG_Init(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_32;
  hiwdg.Init.Reload = 4095;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI1;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM9 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM9_Init(void)
{

  /* USER CODE BEGIN TIM9_Init 0 */

  /* USER CODE END TIM9_Init 0 */

  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM9_Init 1 */

  /* USER CODE END TIM9_Init 1 */
  htim9.Instance = TIM9;
  htim9.Init.Prescaler = 71;
  htim9.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim9.Init.Period = 19999;
  htim9.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim9.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim9) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim9, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim9, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM9_Init 2 */

  /* USER CODE END TIM9_Init 2 */
  HAL_TIM_MspPostInit(&htim9);

}

/**
  * @brief TIM11 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM11_Init(void)
{

  /* USER CODE BEGIN TIM11_Init 0 */

  /* USER CODE END TIM11_Init 0 */

  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM11_Init 1 */

  /* USER CODE END TIM11_Init 1 */
  htim11.Instance = TIM11;
  htim11.Init.Prescaler = 71;
  htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim11.Init.Period = 19999;
  htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim11) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim11) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim11, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM11_Init 2 */

  /* USER CODE END TIM11_Init 2 */
  HAL_TIM_MspPostInit(&htim11);

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin : PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/*
void encoder_to_time(uint16_t encval, uint16_t hours, uint16_t minutes) {
    uint16_t total_minutes = encval * 5;
    hours = (total_minutes / 60) % 12;
    minutes = total_minutes % 60;
}
*/
void get_enc(char *feedTime, uint8_t hours, uint8_t minutes) {
    curr_counter = __HAL_TIM_GET_COUNTER(&htim2);

    // Вычисляем разницу с учетом переполнения
    diff = (int16_t)(curr_counter - enc_last);
    if (!(diff % 2)){
    	enc_total += diff/2;
      	if (enc_total > 95) {
      	    enc_total = 0;
      	} else if (enc_total < 0) {
      		enc_total = 95;
      	}

    	enc_last = curr_counter;
    }
    uint16_t total_minutes = enc_total * 15;
    hours = (total_minutes / 60) % 24;
    minutes = total_minutes % 60;
	sprintf((char*)feedTime, "%02d:%02d:00", hours, minutes);
}

void get_hours(char *lastNum, uint8_t *currNum) {
    curr_counter = __HAL_TIM_GET_COUNTER(&htim2);

    // Вычисляем разницу с учетом переполнения
    diff = (int16_t)(curr_counter - enc_last);
    if (!(diff % 2)){
    	enc_total += diff/2;
      	if (enc_total > 23) {
      	    enc_total = 0;
      	} else if (enc_total < 0) {
      		enc_total = 23;
      	}

    	enc_last = curr_counter;
    }
    *currNum = enc_total;
	sprintf((char*)lastNum, "%02d", *currNum);
}

void get_minutes(char *lastNum, uint8_t *currNum) {
    curr_counter = __HAL_TIM_GET_COUNTER(&htim2);

    // Вычисляем разницу с учетом переполнения
    diff = (int16_t)(curr_counter - enc_last);
    if (!(diff % 2)){
        enc_total += diff/2;
        if (enc_total > 59) {  // Ограничение 0-59 для минут
            enc_total = 0;
        } else if (enc_total < 0) {
            enc_total = 59;
        }

        enc_last = curr_counter;
    }

    *currNum = enc_total;  // Разыменовываем указатель
    sprintf((char*)lastNum, "%02d", *currNum);
}

void set_time (uint8_t hr, uint8_t min, uint8_t sec)
{
	RTC_TimeTypeDef sTime = {0};

	sTime.Hours = hr;
	sTime.Minutes = min;
	sTime.Seconds = sec;
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}
}

void set_date (uint8_t year, uint8_t month, uint8_t date, uint8_t day)  // monday = 1
{

	RTC_DateTypeDef sDate = {0};
	sDate.WeekDay = day;
	sDate.Month = month;
	sDate.Date = date;
	sDate.Year = year;
	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}


}

void set_alarm (uint8_t hr, uint8_t min, uint8_t sec, uint8_t date)
{
	RTC_AlarmTypeDef sAlarm = {0};
	sAlarm.AlarmTime.Hours = hr;
	sAlarm.AlarmTime.Minutes = min;
	sAlarm.AlarmTime.Seconds = sec;
	sAlarm.AlarmTime.SubSeconds = 0;
	sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
	sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
	sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
	sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
	sAlarm.AlarmDateWeekDay = date;
	sAlarm.Alarm = RTC_ALARM_A;
	if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}
}

void get_time_date(char *time, char *date)
{
  RTC_DateTypeDef gDate;
  RTC_TimeTypeDef gTime;

  /* Get the RTC current Time */
  HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);

  /* Display time Format: hh:mm:ss */
  sprintf((char*)time,"%02d:%02d:%02d",gTime.Hours, gTime.Minutes, gTime.Seconds);

  /* Display date Format: dd-mm-yyyy */
  sprintf((char*)date,"%02d-%02d-%2d",gDate.Date, gDate.Month, 2000 + gDate.Year);
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);  // turn the LED ON
}

uint8_t get_current_servo_id(void) {
    RTC_DateTypeDef gDate;
    HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);
    return gDate.Date % 3;  // 0, 1 или 2 в зависимости от даты
}

void set_servo_angle(uint8_t servo_id, uint16_t angle) {
    if(angle > 180) angle = 180;
    uint16_t pulse_width = 500 + (angle * 2000 / 180);

    switch(servo_id) {
		case 0:  // Первый сервопривод - TIM11 CH1
			__HAL_TIM_SET_COMPARE(&htim11, TIM_CHANNEL_1, pulse_width);
			break;
		case 1:  // Второй сервопривод - TIM9 CH1
			__HAL_TIM_SET_COMPARE(&htim9, TIM_CHANNEL_1, pulse_width);
			break;
		case 2:  // Третий сервопривод - TIM9 CH2
			__HAL_TIM_SET_COMPARE(&htim9, TIM_CHANNEL_2, pulse_width);
			break;
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

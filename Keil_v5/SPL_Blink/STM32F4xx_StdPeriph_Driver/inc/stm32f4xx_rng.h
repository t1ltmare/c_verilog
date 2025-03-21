/**
  ******************************************************************************
  * @file    stm32f4xx_rng.h
  * @author  MCD Application Team
  * @version V1.6.0
  * @date    10-July-2015
  * @brief   This file contains all the functions prototypes for the Random
  *          Number Generator(RNG) firmware library.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4xx_RNG_H
#define __STM32F4xx_RNG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/** @addtogroup STM32F4xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup RNG
  * @{
  */
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F410xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx)
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/** @defgroup RNG_Exported_Constants
  * @{
  */

/** @defgroup RNG_flags_definition
  * @{
  */
#define RNG_FLAG_DRDY               ((uint8_t)0x0001) /*!< Data ready */
#define RNG_FLAG_CECS               ((uint8_t)0x0002) /*!< Clock error current status */
#define RNG_FLAG_SECS               ((uint8_t)0x0004) /*!< Seed error current status */

#define IS_RNG_GET_FLAG(RNG_FLAG) (((RNG_FLAG) == RNG_FLAG_DRDY) || \
                                   ((RNG_FLAG) == RNG_FLAG_CECS) || \
                                   ((RNG_FLAG) == RNG_FLAG_SECS))
#define IS_RNG_CLEAR_FLAG(RNG_FLAG) (((RNG_FLAG) == RNG_FLAG_CECS) || \
                                    ((RNG_FLAG) == RNG_FLAG_SECS))
/**
  * @}
  */

/** @defgroup RNG_interrupts_definition
  * @{
  */
#define RNG_IT_CEI                  ((uint8_t)0x20) /*!< Clock error interrupt */
#define RNG_IT_SEI                  ((uint8_t)0x40) /*!< Seed error interrupt */

#define IS_RNG_IT(IT) ((((IT) & (uint8_t)0x9F) == 0x00) && ((IT) != 0x00))
#define IS_RNG_GET_IT(RNG_IT) (((RNG_IT) == RNG_IT_CEI) || ((RNG_IT) == RNG_IT_SEI))
/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/*  Function used to set the RNG configuration to the default reset state *****/
void RNG_DeInit(void);

/* Configuration function *****************************************************/
void RNG_Cmd(FunctionalState NewState);

/* Get 32 bit Random number function ******************************************/
uint32_t RNG_GetRandomNumber(void);

/* Interrupts and flags management functions **********************************/
void RNG_ITConfig(FunctionalState NewState);
FlagStatus RNG_GetFlagStatus(uint8_t RNG_FLAG);
void RNG_ClearFlag(uint8_t RNG_FLAG);
ITStatus RNG_GetITStatus(uint8_t RNG_IT);
void RNG_ClearITPendingBit(uint8_t RNG_IT);
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F410xx || STM32F429_439xx || STM32F469_479xx */

#ifdef __cplusplus
}
#endif

#endif /*__STM32F4xx_RNG_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

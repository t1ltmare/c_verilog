/**
  ******************************************************************************
  * @file    stm32f4xx_pwr.h
  * @author  MCD Application Team
  * @version V1.6.0
  * @date    10-July-2015
  * @brief   This file contains all the functions prototypes for the PWR firmware
  *          library.
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
#ifndef __STM32F4xx_PWR_H
#define __STM32F4xx_PWR_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/** @addtogroup STM32F4xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup PWR
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/** @defgroup PWR_Exported_Constants
  * @{
  */

/** @defgroup PWR_PVD_detection_level
  * @{
  */
#define PWR_PVDLevel_0                  PWR_CR_PLS_LEV0
#define PWR_PVDLevel_1                  PWR_CR_PLS_LEV1
#define PWR_PVDLevel_2                  PWR_CR_PLS_LEV2
#define PWR_PVDLevel_3                  PWR_CR_PLS_LEV3
#define PWR_PVDLevel_4                  PWR_CR_PLS_LEV4
#define PWR_PVDLevel_5                  PWR_CR_PLS_LEV5
#define PWR_PVDLevel_6                  PWR_CR_PLS_LEV6
#define PWR_PVDLevel_7                  PWR_CR_PLS_LEV7

#define IS_PWR_PVD_LEVEL(LEVEL) (((LEVEL) == PWR_PVDLevel_0) || ((LEVEL) == PWR_PVDLevel_1)|| \
                                 ((LEVEL) == PWR_PVDLevel_2) || ((LEVEL) == PWR_PVDLevel_3)|| \
                                 ((LEVEL) == PWR_PVDLevel_4) || ((LEVEL) == PWR_PVDLevel_5)|| \
                                 ((LEVEL) == PWR_PVDLevel_6) || ((LEVEL) == PWR_PVDLevel_7))
/**
  * @}
  */


/** @defgroup PWR_Regulator_state_in_STOP_mode
  * @{
  */
#define PWR_MainRegulator_ON                        ((uint32_t)0x00000000)
#define PWR_LowPowerRegulator_ON                    PWR_CR_LPDS

/* --- PWR_Legacy ---*/
#define PWR_Regulator_ON                            PWR_MainRegulator_ON
#define PWR_Regulator_LowPower                      PWR_LowPowerRegulator_ON

#define IS_PWR_REGULATOR(REGULATOR) (((REGULATOR) == PWR_MainRegulator_ON) || \
                                     ((REGULATOR) == PWR_LowPowerRegulator_ON))

/**
  * @}
  */

/** @defgroup PWR_Regulator_state_in_UnderDrive_mode
  * @{
  */
#define PWR_MainRegulator_UnderDrive_ON               PWR_CR_MRUDS
#define PWR_LowPowerRegulator_UnderDrive_ON           ((uint32_t)(PWR_CR_LPDS | PWR_CR_LPUDS))

#define IS_PWR_REGULATOR_UNDERDRIVE(REGULATOR) (((REGULATOR) == PWR_MainRegulator_UnderDrive_ON) || \
                                                ((REGULATOR) == PWR_LowPowerRegulator_UnderDrive_ON))

/**
  * @}
  */
#if defined(STM32F410xx) || defined(STM32F446xx)
/** @defgroup PWR_Wake_Up_Pin
  * @{
  */
#define PWR_WakeUp_Pin1           ((uint32_t)0x00)
#define PWR_WakeUp_Pin2           ((uint32_t)0x01)
#if defined(STM32F410xx)
#define PWR_WakeUp_Pin3           ((uint32_t)0x02)
#endif /* STM32F410xx */

#if defined(STM32F446xx)
#define IS_PWR_WAKEUP_PIN(PIN) (((PIN) == PWR_WakeUp_Pin1) || \
                                ((PIN) == PWR_WakeUp_Pin2))
#else /* STM32F410xx */
#define IS_PWR_WAKEUP_PIN(PIN) (((PIN) == PWR_WAKEUP_PIN1) || ((PIN) == PWR_WAKEUP_PIN2) || \
                                ((PIN) == PWR_WAKEUP_PIN3))
#endif /* STM32F446xx */
/**
  * @}
  */
#endif /* STM32F410xx || STM32F446xx */

/** @defgroup PWR_STOP_mode_entry
  * @{
  */
#define PWR_STOPEntry_WFI               ((uint8_t)0x01)
#define PWR_STOPEntry_WFE               ((uint8_t)0x02)
#define IS_PWR_STOP_ENTRY(ENTRY) (((ENTRY) == PWR_STOPEntry_WFI) || ((ENTRY) == PWR_STOPEntry_WFE))
/**
  * @}
  */

/** @defgroup PWR_Regulator_Voltage_Scale
  * @{
  */
#define PWR_Regulator_Voltage_Scale1    ((uint32_t)0x0000C000)
#define PWR_Regulator_Voltage_Scale2    ((uint32_t)0x00008000)
#define PWR_Regulator_Voltage_Scale3    ((uint32_t)0x00004000)
#define IS_PWR_REGULATOR_VOLTAGE(VOLTAGE) (((VOLTAGE) == PWR_Regulator_Voltage_Scale1) || \
                                           ((VOLTAGE) == PWR_Regulator_Voltage_Scale2) || \
                                           ((VOLTAGE) == PWR_Regulator_Voltage_Scale3))
/**
  * @}
  */

/** @defgroup PWR_Flag
  * @{
  */
#define PWR_FLAG_WU                     PWR_CSR_WUF
#define PWR_FLAG_SB                     PWR_CSR_SBF
#define PWR_FLAG_PVDO                   PWR_CSR_PVDO
#define PWR_FLAG_BRR                    PWR_CSR_BRR
#define PWR_FLAG_VOSRDY                 PWR_CSR_VOSRDY
#define PWR_FLAG_ODRDY                  PWR_CSR_ODRDY
#define PWR_FLAG_ODSWRDY                PWR_CSR_ODSWRDY
#define PWR_FLAG_UDRDY                  PWR_CSR_UDSWRDY

/* --- FLAG Legacy ---*/
#define PWR_FLAG_REGRDY                  PWR_FLAG_VOSRDY

#define IS_PWR_GET_FLAG(FLAG) (((FLAG) == PWR_FLAG_WU) || ((FLAG) == PWR_FLAG_SB) || \
                               ((FLAG) == PWR_FLAG_PVDO) || ((FLAG) == PWR_FLAG_BRR) || \
                               ((FLAG) == PWR_FLAG_VOSRDY) || ((FLAG) == PWR_FLAG_ODRDY) || \
                               ((FLAG) == PWR_FLAG_ODSWRDY) || ((FLAG) == PWR_FLAG_UDRDY))


#define IS_PWR_CLEAR_FLAG(FLAG) (((FLAG) == PWR_FLAG_WU) || ((FLAG) == PWR_FLAG_SB) || \
                                 ((FLAG) == PWR_FLAG_UDRDY))

/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* Function used to set the PWR configuration to the default reset state ******/
void PWR_DeInit(void);

/* Backup Domain Access function **********************************************/
void PWR_BackupAccessCmd(FunctionalState NewState);

/* PVD configuration functions ************************************************/
void PWR_PVDLevelConfig(uint32_t PWR_PVDLevel);
void PWR_PVDCmd(FunctionalState NewState);

/* WakeUp pins configuration functions ****************************************/
#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F401xx) || defined(STM32F411xE)
void PWR_WakeUpPinCmd(FunctionalState NewState);
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F401xx || STM32F411xE */
#if defined(STM32F410xx) || defined(STM32F446xx)
void PWR_WakeUpPinCmd(uint32_t PWR_WakeUpPinx, FunctionalState NewState);
#endif /* STM32F410xx || STM32F446xx */
/* Main and Backup Regulators configuration functions *************************/
void PWR_BackupRegulatorCmd(FunctionalState NewState);
void PWR_MainRegulatorModeConfig(uint32_t PWR_Regulator_Voltage);
void PWR_OverDriveCmd(FunctionalState NewState);
void PWR_OverDriveSWCmd(FunctionalState NewState);
void PWR_UnderDriveCmd(FunctionalState NewState);

#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F446xx)
void PWR_MainRegulatorUnderDriveCmd(FunctionalState NewState);
void PWR_LowRegulatorUnderDriveCmd(FunctionalState NewState);
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F446xx */

#if defined(STM32F401xx) || defined(STM32F410xx) || defined(STM32F411xE)
void PWR_MainRegulatorLowVoltageCmd(FunctionalState NewState);
void PWR_LowRegulatorLowVoltageCmd(FunctionalState NewState);
#endif /* STM32F401xx || STM32F410xx || STM32F411xE */

/* FLASH Power Down configuration functions ***********************************/
void PWR_FlashPowerDownCmd(FunctionalState NewState);

/* Low Power modes configuration functions ************************************/
void PWR_EnterSTOPMode(uint32_t PWR_Regulator, uint8_t PWR_STOPEntry);
void PWR_EnterUnderDriveSTOPMode(uint32_t PWR_Regulator, uint8_t PWR_STOPEntry);
void PWR_EnterSTANDBYMode(void);

/* Flags management functions *************************************************/
FlagStatus PWR_GetFlagStatus(uint32_t PWR_FLAG);
void PWR_ClearFlag(uint32_t PWR_FLAG);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_PWR_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

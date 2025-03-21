/**
  ******************************************************************************
  * @file    stm32f4xx_cryp.h
  * @author  MCD Application Team
  * @version V1.6.0
  * @date    10-July-2015
  * @brief   This file contains all the functions prototypes for the Cryptographic
  *          processor(CRYP) firmware library.
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
#ifndef __STM32F4xx_CRYP_H
#define __STM32F4xx_CRYP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/** @addtogroup STM32F4xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup CRYP
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/**
  * @brief   CRYP Init structure definition
  */
typedef struct
{
  uint32_t CRYP_AlgoDir;   /*!< Encrypt or Decrypt. This parameter can be a
                                value of @ref CRYP_Algorithm_Direction */
  uint32_t CRYP_AlgoMode;  /*!< TDES-ECB, TDES-CBC, DES-ECB, DES-CBC, AES-ECB,
                                AES-CBC, AES-CTR, AES-Key, AES-GCM and AES-CCM.
                                This parameter can be a value of @ref CRYP_Algorithm_Mode */
  uint32_t CRYP_DataType;  /*!< 32-bit data, 16-bit data, bit data or bit string.
                                This parameter can be a value of @ref CRYP_Data_Type */
  uint32_t CRYP_KeySize;   /*!< Used only in AES mode only : 128, 192 or 256 bit
                                key length. This parameter can be a value of
                                @ref CRYP_Key_Size_for_AES_only */
}CRYP_InitTypeDef;

/**
  * @brief   CRYP Key(s) structure definition
  */
typedef struct
{
  uint32_t CRYP_Key0Left;  /*!< Key 0 Left  */
  uint32_t CRYP_Key0Right; /*!< Key 0 Right */
  uint32_t CRYP_Key1Left;  /*!< Key 1 left  */
  uint32_t CRYP_Key1Right; /*!< Key 1 Right */
  uint32_t CRYP_Key2Left;  /*!< Key 2 left  */
  uint32_t CRYP_Key2Right; /*!< Key 2 Right */
  uint32_t CRYP_Key3Left;  /*!< Key 3 left  */
  uint32_t CRYP_Key3Right; /*!< Key 3 Right */
}CRYP_KeyInitTypeDef;
/**
  * @brief   CRYP Initialization Vectors (IV) structure definition
  */
typedef struct
{
  uint32_t CRYP_IV0Left;  /*!< Init Vector 0 Left  */
  uint32_t CRYP_IV0Right; /*!< Init Vector 0 Right */
  uint32_t CRYP_IV1Left;  /*!< Init Vector 1 left  */
  uint32_t CRYP_IV1Right; /*!< Init Vector 1 Right */
}CRYP_IVInitTypeDef;

/**
  * @brief  CRYP context swapping structure definition
  */
typedef struct
{
  /*!< Current Configuration */
  uint32_t CR_CurrentConfig;
  /*!< IV */
  uint32_t CRYP_IV0LR;
  uint32_t CRYP_IV0RR;
  uint32_t CRYP_IV1LR;
  uint32_t CRYP_IV1RR;
  /*!< KEY */
  uint32_t CRYP_K0LR;
  uint32_t CRYP_K0RR;
  uint32_t CRYP_K1LR;
  uint32_t CRYP_K1RR;
  uint32_t CRYP_K2LR;
  uint32_t CRYP_K2RR;
  uint32_t CRYP_K3LR;
  uint32_t CRYP_K3RR;
  uint32_t CRYP_CSGCMCCMR[8];
  uint32_t CRYP_CSGCMR[8];
}CRYP_Context;


/* Exported constants --------------------------------------------------------*/

/** @defgroup CRYP_Exported_Constants
  * @{
  */

/** @defgroup CRYP_Algorithm_Direction
  * @{
  */
#define CRYP_AlgoDir_Encrypt      ((uint16_t)0x0000)
#define CRYP_AlgoDir_Decrypt      ((uint16_t)0x0004)
#define IS_CRYP_ALGODIR(ALGODIR) (((ALGODIR) == CRYP_AlgoDir_Encrypt) || \
                                  ((ALGODIR) == CRYP_AlgoDir_Decrypt))

/**
  * @}
  */

/** @defgroup CRYP_Algorithm_Mode
  * @{
  */

/*!< TDES Modes */
#define CRYP_AlgoMode_TDES_ECB    ((uint32_t)0x00000000)
#define CRYP_AlgoMode_TDES_CBC    ((uint32_t)0x00000008)

/*!< DES Modes */
#define CRYP_AlgoMode_DES_ECB     ((uint32_t)0x00000010)
#define CRYP_AlgoMode_DES_CBC     ((uint32_t)0x00000018)

/*!< AES Modes */
#define CRYP_AlgoMode_AES_ECB     ((uint32_t)0x00000020)
#define CRYP_AlgoMode_AES_CBC     ((uint32_t)0x00000028)
#define CRYP_AlgoMode_AES_CTR     ((uint32_t)0x00000030)
#define CRYP_AlgoMode_AES_Key     ((uint32_t)0x00000038)
#define CRYP_AlgoMode_AES_GCM     ((uint32_t)0x00080000)
#define CRYP_AlgoMode_AES_CCM     ((uint32_t)0x00080008)

#define IS_CRYP_ALGOMODE(ALGOMODE) (((ALGOMODE) == CRYP_AlgoMode_TDES_ECB) || \
                                   ((ALGOMODE) == CRYP_AlgoMode_TDES_CBC)|| \
                                   ((ALGOMODE) == CRYP_AlgoMode_DES_ECB) || \
                                   ((ALGOMODE) == CRYP_AlgoMode_DES_CBC) || \
                                   ((ALGOMODE) == CRYP_AlgoMode_AES_ECB) || \
                                   ((ALGOMODE) == CRYP_AlgoMode_AES_CBC) || \
                                   ((ALGOMODE) == CRYP_AlgoMode_AES_CTR) || \
                                   ((ALGOMODE) == CRYP_AlgoMode_AES_Key) || \
                                   ((ALGOMODE) == CRYP_AlgoMode_AES_GCM) || \
                                   ((ALGOMODE) == CRYP_AlgoMode_AES_CCM))
/**
  * @}
  */

/** @defgroup CRYP_Phase
  * @{
  */

/*!< The phases are valid only for AES-GCM and AES-CCM modes */
#define CRYP_Phase_Init           ((uint32_t)0x00000000)
#define CRYP_Phase_Header         CRYP_CR_GCM_CCMPH_0
#define CRYP_Phase_Payload        CRYP_CR_GCM_CCMPH_1
#define CRYP_Phase_Final          CRYP_CR_GCM_CCMPH

#define IS_CRYP_PHASE(PHASE) (((PHASE) == CRYP_Phase_Init)    || \
                              ((PHASE) == CRYP_Phase_Header)  || \
                              ((PHASE) == CRYP_Phase_Payload) || \
                              ((PHASE) == CRYP_Phase_Final))

/**
  * @}
  */

/** @defgroup CRYP_Data_Type
  * @{
  */
#define CRYP_DataType_32b         ((uint16_t)0x0000)
#define CRYP_DataType_16b         ((uint16_t)0x0040)
#define CRYP_DataType_8b          ((uint16_t)0x0080)
#define CRYP_DataType_1b          ((uint16_t)0x00C0)
#define IS_CRYP_DATATYPE(DATATYPE) (((DATATYPE) == CRYP_DataType_32b) || \
                                    ((DATATYPE) == CRYP_DataType_16b)|| \
                                    ((DATATYPE) == CRYP_DataType_8b)|| \
                                    ((DATATYPE) == CRYP_DataType_1b))
/**
  * @}
  */

/** @defgroup CRYP_Key_Size_for_AES_only
  * @{
  */
#define CRYP_KeySize_128b         ((uint16_t)0x0000)
#define CRYP_KeySize_192b         ((uint16_t)0x0100)
#define CRYP_KeySize_256b         ((uint16_t)0x0200)
#define IS_CRYP_KEYSIZE(KEYSIZE) (((KEYSIZE) == CRYP_KeySize_128b)|| \
                                  ((KEYSIZE) == CRYP_KeySize_192b)|| \
                                  ((KEYSIZE) == CRYP_KeySize_256b))
/**
  * @}
  */

/** @defgroup CRYP_flags_definition
  * @{
  */
#define CRYP_FLAG_BUSY            ((uint8_t)0x10)  /*!< The CRYP core is currently
                                                        processing a block of data
                                                        or a key preparation (for
                                                        AES decryption). */
#define CRYP_FLAG_IFEM            ((uint8_t)0x01)  /*!< Input Fifo Empty */
#define CRYP_FLAG_IFNF            ((uint8_t)0x02)  /*!< Input Fifo is Not Full */
#define CRYP_FLAG_INRIS           ((uint8_t)0x22)  /*!< Raw interrupt pending */
#define CRYP_FLAG_OFNE            ((uint8_t)0x04)  /*!< Input Fifo service raw
                                                        interrupt status */
#define CRYP_FLAG_OFFU            ((uint8_t)0x08)  /*!< Output Fifo is Full */
#define CRYP_FLAG_OUTRIS          ((uint8_t)0x21)  /*!< Output Fifo service raw
                                                        interrupt status */

#define IS_CRYP_GET_FLAG(FLAG) (((FLAG) == CRYP_FLAG_IFEM)  || \
                                ((FLAG) == CRYP_FLAG_IFNF)  || \
                                ((FLAG) == CRYP_FLAG_OFNE)  || \
                                ((FLAG) == CRYP_FLAG_OFFU)  || \
                                ((FLAG) == CRYP_FLAG_BUSY)  || \
                                ((FLAG) == CRYP_FLAG_OUTRIS)|| \
                                ((FLAG) == CRYP_FLAG_INRIS))
/**
  * @}
  */

/** @defgroup CRYP_interrupts_definition
  * @{
  */
#define CRYP_IT_INI               ((uint8_t)0x01) /*!< IN Fifo Interrupt */
#define CRYP_IT_OUTI              ((uint8_t)0x02) /*!< OUT Fifo Interrupt */
#define IS_CRYP_CONFIG_IT(IT) ((((IT) & (uint8_t)0xFC) == 0x00) && ((IT) != 0x00))
#define IS_CRYP_GET_IT(IT) (((IT) == CRYP_IT_INI) || ((IT) == CRYP_IT_OUTI))

/**
  * @}
  */

/** @defgroup CRYP_Encryption_Decryption_modes_definition
  * @{
  */
#define MODE_ENCRYPT             ((uint8_t)0x01)
#define MODE_DECRYPT             ((uint8_t)0x00)

/**
  * @}
  */

/** @defgroup CRYP_DMA_transfer_requests
  * @{
  */
#define CRYP_DMAReq_DataIN             ((uint8_t)0x01)
#define CRYP_DMAReq_DataOUT            ((uint8_t)0x02)
#define IS_CRYP_DMAREQ(DMAREQ) ((((DMAREQ) & (uint8_t)0xFC) == 0x00) && ((DMAREQ) != 0x00))
/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/*  Function used to set the CRYP configuration to the default reset state ****/
void CRYP_DeInit(void);

/* CRYP Initialization and Configuration functions ****************************/
void CRYP_Init(CRYP_InitTypeDef* CRYP_InitStruct);
void CRYP_StructInit(CRYP_InitTypeDef* CRYP_InitStruct);
void CRYP_KeyInit(CRYP_KeyInitTypeDef* CRYP_KeyInitStruct);
void CRYP_KeyStructInit(CRYP_KeyInitTypeDef* CRYP_KeyInitStruct);
void CRYP_IVInit(CRYP_IVInitTypeDef* CRYP_IVInitStruct);
void CRYP_IVStructInit(CRYP_IVInitTypeDef* CRYP_IVInitStruct);
void CRYP_Cmd(FunctionalState NewState);
void CRYP_PhaseConfig(uint32_t CRYP_Phase);
void CRYP_FIFOFlush(void);
/* CRYP Data processing functions *********************************************/
void CRYP_DataIn(uint32_t Data);
uint32_t CRYP_DataOut(void);

/* CRYP Context swapping functions ********************************************/
ErrorStatus CRYP_SaveContext(CRYP_Context* CRYP_ContextSave,
                             CRYP_KeyInitTypeDef* CRYP_KeyInitStruct);
void CRYP_RestoreContext(CRYP_Context* CRYP_ContextRestore);

/* CRYP DMA interface function ************************************************/
void CRYP_DMACmd(uint8_t CRYP_DMAReq, FunctionalState NewState);

/* Interrupts and flags management functions **********************************/
void CRYP_ITConfig(uint8_t CRYP_IT, FunctionalState NewState);
ITStatus CRYP_GetITStatus(uint8_t CRYP_IT);
FunctionalState CRYP_GetCmdStatus(void);
FlagStatus CRYP_GetFlagStatus(uint8_t CRYP_FLAG);

/* High Level AES functions **************************************************/
ErrorStatus CRYP_AES_ECB(uint8_t Mode,
                         uint8_t *Key, uint16_t Keysize,
                         uint8_t *Input, uint32_t Ilength,
                         uint8_t *Output);

ErrorStatus CRYP_AES_CBC(uint8_t Mode,
                         uint8_t InitVectors[16],
                         uint8_t *Key, uint16_t Keysize,
                         uint8_t *Input, uint32_t Ilength,
                         uint8_t *Output);

ErrorStatus CRYP_AES_CTR(uint8_t Mode,
                         uint8_t InitVectors[16],
                         uint8_t *Key, uint16_t Keysize,
                         uint8_t *Input, uint32_t Ilength,
                         uint8_t *Output);

ErrorStatus CRYP_AES_GCM(uint8_t Mode, uint8_t InitVectors[16],
                         uint8_t *Key, uint16_t Keysize,
                         uint8_t *Input, uint32_t ILength,
                         uint8_t *Header, uint32_t HLength,
                         uint8_t *Output, uint8_t *AuthTAG);

ErrorStatus CRYP_AES_CCM(uint8_t Mode,
                         uint8_t* Nonce, uint32_t NonceSize,
                         uint8_t* Key, uint16_t Keysize,
                         uint8_t* Input, uint32_t ILength,
                         uint8_t* Header, uint32_t HLength, uint8_t *HBuffer,
                         uint8_t* Output,
                         uint8_t* AuthTAG, uint32_t TAGSize);

/* High Level TDES functions **************************************************/
ErrorStatus CRYP_TDES_ECB(uint8_t Mode,
                           uint8_t Key[24],
                           uint8_t *Input, uint32_t Ilength,
                           uint8_t *Output);

ErrorStatus CRYP_TDES_CBC(uint8_t Mode,
                          uint8_t Key[24],
                          uint8_t InitVectors[8],
                          uint8_t *Input, uint32_t Ilength,
                          uint8_t *Output);

/* High Level DES functions **************************************************/
ErrorStatus CRYP_DES_ECB(uint8_t Mode,
                         uint8_t Key[8],
                         uint8_t *Input, uint32_t Ilength,
                         uint8_t *Output);

ErrorStatus CRYP_DES_CBC(uint8_t Mode,
                         uint8_t Key[8],
                         uint8_t InitVectors[8],
                         uint8_t *Input,uint32_t Ilength,
                         uint8_t *Output);

#ifdef __cplusplus
}
#endif

#endif /*__STM32F4xx_CRYP_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

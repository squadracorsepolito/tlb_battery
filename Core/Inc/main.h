/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define FB_IMD_ERR_GPIO_IN_Pin GPIO_PIN_0
#define FB_IMD_ERR_GPIO_IN_GPIO_Port GPIOC
#define SD_FB_AMS_ERR_RLY_TO_IMD_ERR_RLY_GPIO_IN_Pin GPIO_PIN_1
#define SD_FB_AMS_ERR_RLY_TO_IMD_ERR_RLY_GPIO_IN_GPIO_Port GPIOC
#define SD_FB_SD_MID_IN_TO_AMS_ERR_RLY_GPIO_IN_Pin GPIO_PIN_3
#define SD_FB_SD_MID_IN_TO_AMS_ERR_RLY_GPIO_IN_GPIO_Port GPIOC
#define SD_FB_SD_DLY_CAPS_TO_SD_FIN_OUT_AIRS_ADC1_IN_Pin GPIO_PIN_0
#define SD_FB_SD_DLY_CAPS_TO_SD_FIN_OUT_AIRS_ADC1_IN_GPIO_Port GPIOA
#define SD_FB_SD_PRCH_RLY_TO_SD_MID_OUT_ADC1_IN_Pin GPIO_PIN_1
#define SD_FB_SD_PRCH_RLY_TO_SD_MID_OUT_ADC1_IN_GPIO_Port GPIOA
#define T_VCP_TX_USART_TX_Pin GPIO_PIN_2
#define T_VCP_TX_USART_TX_GPIO_Port GPIOA
#define T_VCP_RX_USART_RX_Pin GPIO_PIN_3
#define T_VCP_RX_USART_RX_GPIO_Port GPIOA
#define FB_AMS_ERR_GPIO_IN_Pin GPIO_PIN_4
#define FB_AMS_ERR_GPIO_IN_GPIO_Port GPIOA
#define FB_AIR_POS_INT_SD_REL_GPIO_IN_Pin GPIO_PIN_5
#define FB_AIR_POS_INT_SD_REL_GPIO_IN_GPIO_Port GPIOA
#define FB_AIR_NEG_INT_SD_REL_GPIO_IN_Pin GPIO_PIN_6
#define FB_AIR_NEG_INT_SD_REL_GPIO_IN_GPIO_Port GPIOA
#define FB_DCBUS_PRCH_RLY_INT_SD_REL_GPIO_IN_Pin GPIO_PIN_7
#define FB_DCBUS_PRCH_RLY_INT_SD_REL_GPIO_IN_GPIO_Port GPIOA
#define LED_RED_GPIO_OUT_Pin GPIO_PIN_4
#define LED_RED_GPIO_OUT_GPIO_Port GPIOC
#define LED_ORANGE_GPIO_OUT_Pin GPIO_PIN_5
#define LED_ORANGE_GPIO_OUT_GPIO_Port GPIOC
#define LED_BLUE_GPIO_OUT_Pin GPIO_PIN_0
#define LED_BLUE_GPIO_OUT_GPIO_Port GPIOB
#define LED_GREEN_GPIO_OUT_Pin GPIO_PIN_1
#define LED_GREEN_GPIO_OUT_GPIO_Port GPIOB
#define FB_SHRT2GND_AIR_NEG_GPIO_IN_Pin GPIO_PIN_2
#define FB_SHRT2GND_AIR_NEG_GPIO_IN_GPIO_Port GPIOB
#define FB_SHRT2GND_AIR_POS_GPIO_IN_Pin GPIO_PIN_10
#define FB_SHRT2GND_AIR_POS_GPIO_IN_GPIO_Port GPIOB
#define FB_nDCBUS_PRCH_RLY_AUX_GPIO_IN_Pin GPIO_PIN_12
#define FB_nDCBUS_PRCH_RLY_AUX_GPIO_IN_GPIO_Port GPIOB
#define FB_nSHRT2GND_AIRS_GPIO_IN_Pin GPIO_PIN_13
#define FB_nSHRT2GND_AIRS_GPIO_IN_GPIO_Port GPIOB
#define FB_TSAL_GREEN_GPIO_IN_Pin GPIO_PIN_14
#define FB_TSAL_GREEN_GPIO_IN_GPIO_Port GPIOB
#define FB_ANY_IMPL_ERR_LTCH_GPIO_IN_Pin GPIO_PIN_15
#define FB_ANY_IMPL_ERR_LTCH_GPIO_IN_GPIO_Port GPIOB
#define FB_nANY_IMPL_ERR_GPIO_IN_Pin GPIO_PIN_6
#define FB_nANY_IMPL_ERR_GPIO_IN_GPIO_Port GPIOC
#define FB_DCBUS_OVER_60V_IMPL_ERR_GPIO_IN_Pin GPIO_PIN_7
#define FB_DCBUS_OVER_60V_IMPL_ERR_GPIO_IN_GPIO_Port GPIOC
#define FB_AIR_NEG_IMPL_ERR_GPIO_IN_Pin GPIO_PIN_8
#define FB_AIR_NEG_IMPL_ERR_GPIO_IN_GPIO_Port GPIOC
#define FB_DCBUS_PRCH_RLY_IMPL_ERR_GPIO_IN_Pin GPIO_PIN_9
#define FB_DCBUS_PRCH_RLY_IMPL_ERR_GPIO_IN_GPIO_Port GPIOC
#define FB_AIR_POS_IMPL_ERR_GPIO_IN_Pin GPIO_PIN_8
#define FB_AIR_POS_IMPL_ERR_GPIO_IN_GPIO_Port GPIOA
#define FB_nAIR_NEG_AUX_GPIO_IN_Pin GPIO_PIN_9
#define FB_nAIR_NEG_AUX_GPIO_IN_GPIO_Port GPIOA
#define FB_nAIR_POS_AUX_GPIO_IN_Pin GPIO_PIN_10
#define FB_nAIR_POS_AUX_GPIO_IN_GPIO_Port GPIOA
#define FB_nDCBUS_OVER_60V_GPIO_IN_Pin GPIO_PIN_11
#define FB_nDCBUS_OVER_60V_GPIO_IN_GPIO_Port GPIOA
#define SD_FB_SD_FNL_IN_TO_SD_DLY_CAPS_GPIO_IN_Pin GPIO_PIN_10
#define SD_FB_SD_FNL_IN_TO_SD_DLY_CAPS_GPIO_IN_GPIO_Port GPIOC
#define FB_SD_PRCH_RLY_GPIO_IN_Pin GPIO_PIN_11
#define FB_SD_PRCH_RLY_GPIO_IN_GPIO_Port GPIOC
#define SD_FB_IMD_ERR_RLY_TO_SD_PRCH_RLY_GPIO_IN_Pin GPIO_PIN_12
#define SD_FB_IMD_ERR_RLY_TO_SD_PRCH_RLY_GPIO_IN_GPIO_Port GPIOC
#define SN65HVD23x_R_CAN1_RX_Pin GPIO_PIN_8
#define SN65HVD23x_R_CAN1_RX_GPIO_Port GPIOB
#define SN65HVD23x_D_CAN1_TX_Pin GPIO_PIN_9
#define SN65HVD23x_D_CAN1_TX_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

#define VCP_UART_Handle huart2 /*!< Virtual COMM Port UART handle */

/* ADC Analog VDD in mV */
#define ADC_VDDA_mV (3300.0)

/* Shutdown Feedbacks constants and mappings */
#define SD_FB_ADC_Handle                                      hadc1
#define SD_FB_SD_DLY_CAPS_TO_SD_FIN_OUT_AIRS_ADC1_IN_ADC_CHNL ADC_CHANNEL_0
#define SD_FB_SD_PRCH_RLY_TO_SD_MID_OUT_ADC1_IN_ADC_CHNL      ADC_CHANNEL_1
#define SD_FB_ADC_OFFSET                                      (0.0)//(367.0)
#define SD_FB_ADC_GAIN                                        (7.272727)

/* CAN mappings */
#define CAN_MCB_Handle hcan1
#define CAN_MCB_TxAttemptsMailboxesFull (3U)
#define CAN_MCB_AttemptRetryDelayMsMailboxesFull (1U)
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

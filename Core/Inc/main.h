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
#define M95256_nS_GPIO_OUT_Pin GPIO_PIN_0
#define M95256_nS_GPIO_OUT_GPIO_Port GPIOC
#define M95256_D_SPI_MOSI_Pin GPIO_PIN_1
#define M95256_D_SPI_MOSI_GPIO_Port GPIOC
#define STAT2_LED_GPIO_OUT_Pin GPIO_PIN_2
#define STAT2_LED_GPIO_OUT_GPIO_Port GPIOC
#define STAT1_LED_GPIO_OUT_Pin GPIO_PIN_3
#define STAT1_LED_GPIO_OUT_GPIO_Port GPIOC
#define WARN_LED_GPIO_OUT_Pin GPIO_PIN_0
#define WARN_LED_GPIO_OUT_GPIO_Port GPIOA
#define STAT3_LED_GPIO_OUT_Pin GPIO_PIN_1
#define STAT3_LED_GPIO_OUT_GPIO_Port GPIOA
#define T_VCP_TX_USART_TX_Pin GPIO_PIN_2
#define T_VCP_TX_USART_TX_GPIO_Port GPIOA
#define T_VCP_RX_USART_RX_Pin GPIO_PIN_3
#define T_VCP_RX_USART_RX_GPIO_Port GPIOA
#define ERR_LED_GPIO_OUT_Pin GPIO_PIN_4
#define ERR_LED_GPIO_OUT_GPIO_Port GPIOA
#define SDC_POST_AMS_IMD_RLY_GPIO_IN_Pin GPIO_PIN_5
#define SDC_POST_AMS_IMD_RLY_GPIO_IN_GPIO_Port GPIOA
#define SDC_TSAC_INIT_IN_ADC_IN_Pin GPIO_PIN_6
#define SDC_TSAC_INIT_IN_ADC_IN_GPIO_Port GPIOA
#define M95256_nW_GPIO_OUT_Pin GPIO_PIN_4
#define M95256_nW_GPIO_OUT_GPIO_Port GPIOC
#define SDC_PRECH_CMD__INT_STATE_OPEN_GPIO_IN_Pin GPIO_PIN_5
#define SDC_PRECH_CMD__INT_STATE_OPEN_GPIO_IN_GPIO_Port GPIOC
#define SDC_TSAC_FINAL_IN_ADC_IN_Pin GPIO_PIN_0
#define SDC_TSAC_FINAL_IN_ADC_IN_GPIO_Port GPIOB
#define TSAL_GREEN_GPIO_IN_Pin GPIO_PIN_1
#define TSAL_GREEN_GPIO_IN_GPIO_Port GPIOB
#define AIR_NEG__MECH_STATE_OPEN_3V3_Pin GPIO_PIN_2
#define AIR_NEG__MECH_STATE_OPEN_3V3_GPIO_Port GPIOB
#define AIR_POS__MECH_STATE_OPEN_GPIO_IN_Pin GPIO_PIN_10
#define AIR_POS__MECH_STATE_OPEN_GPIO_IN_GPIO_Port GPIOB
#define nSTG_AIR_POS_GPIO_IN_Pin GPIO_PIN_12
#define nSTG_AIR_POS_GPIO_IN_GPIO_Port GPIOB
#define nSTG_AIR_NEG_GPIO_IN_Pin GPIO_PIN_14
#define nSTG_AIR_NEG_GPIO_IN_GPIO_Port GPIOB
#define ANY_IMPL_ERR_LATCH_GPIO_IN_Pin GPIO_PIN_15
#define ANY_IMPL_ERR_LATCH_GPIO_IN_GPIO_Port GPIOB
#define IMD_ERR_LATCH_GPIO_IN_Pin GPIO_PIN_6
#define IMD_ERR_LATCH_GPIO_IN_GPIO_Port GPIOC
#define AMS_ERR_LATCH_GPIO_IN_Pin GPIO_PIN_7
#define AMS_ERR_LATCH_GPIO_IN_GPIO_Port GPIOC
#define DCBUS_RELAYS_IMPL_GPIO_IN_Pin GPIO_PIN_8
#define DCBUS_RELAYS_IMPL_GPIO_IN_GPIO_Port GPIOC
#define DCBUS_IMPL_GPIO_IN_Pin GPIO_PIN_9
#define DCBUS_IMPL_GPIO_IN_GPIO_Port GPIOC
#define AIR_POS__INT_STATE_CLOSED_GPIO_IN_Pin GPIO_PIN_8
#define AIR_POS__INT_STATE_CLOSED_GPIO_IN_GPIO_Port GPIOA
#define AIR_NEG__INT_STATE_CLOSED_GPIO_IN_Pin GPIO_PIN_9
#define AIR_NEG__INT_STATE_CLOSED_GPIO_IN_GPIO_Port GPIOA
#define DCBUS_PRECH__INT_STATE_CLOSED_GPIO_IN_Pin GPIO_PIN_10
#define DCBUS_PRECH__INT_STATE_CLOSED_GPIO_IN_GPIO_Port GPIOA
#define DCBUS_PRECH__MECH_STATE_OPEN_GPIO_IN_Pin GPIO_PIN_11
#define DCBUS_PRECH__MECH_STATE_OPEN_GPIO_IN_GPIO_Port GPIOA
#define nDCBUS_OVER_60V_GPIO_IN_Pin GPIO_PIN_12
#define nDCBUS_OVER_60V_GPIO_IN_GPIO_Port GPIOA
#define M95256_C_SPI_SCK_Pin GPIO_PIN_10
#define M95256_C_SPI_SCK_GPIO_Port GPIOC
#define M95256_Q_SPI_MISO_Pin GPIO_PIN_11
#define M95256_Q_SPI_MISO_GPIO_Port GPIOC
#define SN65HVD23x_R_CAN_RX_Pin GPIO_PIN_8
#define SN65HVD23x_R_CAN_RX_GPIO_Port GPIOB
#define SN65HVD23x_D_CAN_TX_Pin GPIO_PIN_9
#define SN65HVD23x_D_CAN_TX_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

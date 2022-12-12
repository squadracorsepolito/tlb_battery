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
#define TSAL_GREEN_STATUS_3_3V_Pin GPIO_PIN_0
#define TSAL_GREEN_STATUS_3_3V_GPIO_Port GPIOC
#define SCS_ERROR_N_3_3V_Pin GPIO_PIN_1
#define SCS_ERROR_N_3_3V_GPIO_Port GPIOC
#define SCS_ERROR_N_3_3VC2_Pin GPIO_PIN_2
#define SCS_ERROR_N_3_3VC2_GPIO_Port GPIOC
#define SD_reading_2_Pin GPIO_PIN_3
#define SD_reading_2_GPIO_Port GPIOC
#define SD_reading_4_ADC_Pin GPIO_PIN_0
#define SD_reading_4_ADC_GPIO_Port GPIOA
#define SD_reading_6_ADC_Pin GPIO_PIN_1
#define SD_reading_6_ADC_GPIO_Port GPIOA
#define Check_SCS_AIR_POS_3_3V_Pin GPIO_PIN_4
#define Check_SCS_AIR_POS_3_3V_GPIO_Port GPIOA
#define Check_SCS_AIR_NEG_3_3V_Pin GPIO_PIN_5
#define Check_SCS_AIR_NEG_3_3V_GPIO_Port GPIOA
#define AIR_POS_Closed_N_3_3V_Pin GPIO_PIN_6
#define AIR_POS_Closed_N_3_3V_GPIO_Port GPIOA
#define AIR_NEG_Closed_N_3_3V_Pin GPIO_PIN_7
#define AIR_NEG_Closed_N_3_3V_GPIO_Port GPIOA
#define PREC_Closed_N_3_3V_Pin GPIO_PIN_4
#define PREC_Closed_N_3_3V_GPIO_Port GPIOC
#define ctrl_relay_prec_SD_direct_3_3V_Pin GPIO_PIN_5
#define ctrl_relay_prec_SD_direct_3_3V_GPIO_Port GPIOC
#define AIR_POS_INTENTIONAL_STATE_3_3V_Pin GPIO_PIN_0
#define AIR_POS_INTENTIONAL_STATE_3_3V_GPIO_Port GPIOB
#define AIR_NEG_INTENTIONAL_STATE_3_3V_Pin GPIO_PIN_1
#define AIR_NEG_INTENTIONAL_STATE_3_3V_GPIO_Port GPIOB
#define PREC_TS_INTENTIONAL_STATE_3_3V_Pin GPIO_PIN_2
#define PREC_TS_INTENTIONAL_STATE_3_3V_GPIO_Port GPIOB
#define HV_signal_N_3_3V_Pin GPIO_PIN_10
#define HV_signal_N_3_3V_GPIO_Port GPIOB
#define LATCH_IMP_OUT_3_3V_Pin GPIO_PIN_12
#define LATCH_IMP_OUT_3_3V_GPIO_Port GPIOB
#define IMD_ERR_neg_3_3V_Pin GPIO_PIN_13
#define IMD_ERR_neg_3_3V_GPIO_Port GPIOB
#define AMS_ERR_neg_3_3V_Pin GPIO_PIN_14
#define AMS_ERR_neg_3_3V_GPIO_Port GPIOB
#define IMP_HV_3_3V_Pin GPIO_PIN_15
#define IMP_HV_3_3V_GPIO_Port GPIOB
#define IMP_PREC_HV_3_3V_Pin GPIO_PIN_6
#define IMP_PREC_HV_3_3V_GPIO_Port GPIOC
#define IMP_AIR_NEG_3_3V_Pin GPIO_PIN_7
#define IMP_AIR_NEG_3_3V_GPIO_Port GPIOC
#define IMP_AIR_POS_3_3V_Pin GPIO_PIN_8
#define IMP_AIR_POS_3_3V_GPIO_Port GPIOC
#define IMP_OUT_N_3_3V_Pin GPIO_PIN_9
#define IMP_OUT_N_3_3V_GPIO_Port GPIOC
#define gpio_RED_Pin GPIO_PIN_8
#define gpio_RED_GPIO_Port GPIOA
#define gpio_GREEN_Pin GPIO_PIN_9
#define gpio_GREEN_GPIO_Port GPIOA
#define gpio_BLUE_Pin GPIO_PIN_10
#define gpio_BLUE_GPIO_Port GPIOA
#define ORANGE_LED_Pin GPIO_PIN_11
#define ORANGE_LED_GPIO_Port GPIOA
#define SD_reading_3_Pin GPIO_PIN_11
#define SD_reading_3_GPIO_Port GPIOC
#define SD_reading_5_Pin GPIO_PIN_12
#define SD_reading_5_GPIO_Port GPIOC
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

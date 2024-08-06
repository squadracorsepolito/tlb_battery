/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
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
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include <limits.h>

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#define GPIO_IntEventCnt_t       uint8_t
#define GPIO_IntEventCnt_MAX_VAL (UINT8_MAX)
/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */

enum GPIO_IntEventsEnum {
    SD_FB_SD_FNL_IN_TO_SD_DLY_CAPS,
    AMS_ERR,
    IMD_ERR,
    FB_ANY_IMPL_ERR_LTCH,
    FB_DCBUS_OVER_60V_IMPL_ERR,
    // keep the below enum
    GPIO_INT_EVENTS_ENUM_NUM
};

extern GPIO_IntEventCnt_t GPIO_rise_edge_events[GPIO_INT_EVENTS_ENUM_NUM];
extern GPIO_IntEventCnt_t GPIO_fall_edge_events[GPIO_INT_EVENTS_ENUM_NUM];

void GPIO_IntRiseEdgeCnt_OverrunErr_callback(void);
void GPIO_IntFallEdgeCnt_OverrunErr_callback(void);

void GPIO_IntEventRoutine();

/**
* @brief Get a decimal representation of the state of a GPIO
* @param __GPIO_BASE_NAME__ The base name of a GPIO i.e. the one found in main.h but without _Pin or _GPIO_Port
* @retval decimal representation of the GPIO: 0U if OFF, MAX_UINT if ON
*/
#define GPIO_READ_PIN_DEC(__GPIO_BASE_NAME__) \
    (HAL_GPIO_ReadPin(__GPIO_BASE_NAME__##_GPIO_Port, __GPIO_BASE_NAME__##_Pin) == GPIO_PIN_SET ? (-1U) : (0U))

#define GPIO_READ_PIN(__GPIO_BASE_NAME__) HAL_GPIO_ReadPin(__GPIO_BASE_NAME__##_GPIO_Port, __GPIO_BASE_NAME__##_Pin)

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */


/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED_RED_GPIO_OUT_Pin|LED_ORANGE_GPIO_OUT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED_BLUE_GPIO_OUT_Pin|LED_GREEN_GPIO_OUT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PCPin PCPin PCPin PCPin
                           PCPin PCPin PCPin PCPin
                           PCPin PCPin */
  GPIO_InitStruct.Pin = FB_nIMD_ERR_GPIO_IN_Pin|SD_FB_AMS_ERR_RLY_TO_IMD_ERR_RLY_GPIO_IN_Pin|SD_FB_SD_MID_IN_TO_AMS_ERR_RLY_GPIO_IN_Pin|FB_nANY_IMPL_ERR_GPIO_IN_Pin
                          |FB_DCBUS_OVER_60V_IMPL_ERR_GPIO_IN_Pin|FB_AIR_NEG_IMPL_ERR_GPIO_IN_Pin|FB_DCBUS_PRCH_RLY_IMPL_ERR_GPIO_IN_Pin|SD_FB_SD_FNL_IN_TO_SD_DLY_CAPS_GPIO_IN_Pin
                          |FB_SD_PRCH_RLY_GPIO_IN_Pin|SD_FB_IMD_ERR_RLY_TO_SD_PRCH_RLY_GPIO_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin PAPin PAPin
                           PAPin PAPin PAPin PAPin */
  GPIO_InitStruct.Pin = FB_nAMS_ERR_GPIO_IN_Pin|FB_AIR_POS_INT_SD_REL_GPIO_IN_Pin|FB_AIR_NEG_INT_SD_REL_GPIO_IN_Pin|FB_DCBUS_PRCH_RLY_INT_SD_REL_GPIO_IN_Pin
                          |FB_AIR_POS_IMPL_ERR_GPIO_IN_Pin|FB_nAIR_NEG_AUX_GPIO_IN_Pin|FB_nAIR_POS_AUX_GPIO_IN_Pin|FB_nDCBUS_OVER_60V_GPIO_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PCPin PCPin */
  GPIO_InitStruct.Pin = LED_RED_GPIO_OUT_Pin|LED_ORANGE_GPIO_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin */
  GPIO_InitStruct.Pin = LED_BLUE_GPIO_OUT_Pin|LED_GREEN_GPIO_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin PBPin
                           PBPin PBPin */
  GPIO_InitStruct.Pin = FB_SHRT2GND_AIR_NEG_GPIO_IN_Pin|FB_SHRT2GND_AIR_POS_GPIO_IN_Pin|FB_nDCBUS_PRCH_RLY_AUX_GPIO_IN_Pin|FB_nSHRT2GND_AIRS_GPIO_IN_Pin
                          |FB_TSAL_GREEN_GPIO_IN_Pin|FB_ANY_IMPL_ERR_LTCH_GPIO_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */

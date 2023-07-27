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
#include "critical_section.h"

#include <stdio.h>

static volatile GPIO_IntEventCnt_t GPIO_INT_RISE_EDGE_CNT_ASYNC[GPIO_INT_EVENTS_ENUM_NUM];
static volatile GPIO_IntEventCnt_t GPIO_INT_FALL_EDGE_CNT_ASYNC[GPIO_INT_EVENTS_ENUM_NUM];

static volatile GPIO_IntEventCnt_t GPIO_INT_RISE_EDGE_CNT_SYNC[GPIO_INT_EVENTS_ENUM_NUM];
static volatile GPIO_IntEventCnt_t GPIO_INT_FALL_EDGE_CNT_SYNC[GPIO_INT_EVENTS_ENUM_NUM];

GPIO_IntEventCnt_t GPIO_rise_edge_events[GPIO_INT_EVENTS_ENUM_NUM];
GPIO_IntEventCnt_t GPIO_fall_edge_events[GPIO_INT_EVENTS_ENUM_NUM];

void _GPIO_IntRiseEdgeCnt_OverrunErr_callback(void);
void _GPIO_IntFallEdgeCnt_OverrunErr_callback(void);

void _GPIO_IntEdgeHandler(enum GPIO_IntEventsEnum gpio_int_event, GPIO_PinState edge);

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
void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, LED_RED_GPIO_OUT_Pin | LED_ORANGE_GPIO_OUT_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, LED_BLUE_GPIO_OUT_Pin | LED_GREEN_GPIO_OUT_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pins : PCPin PCPin */
    GPIO_InitStruct.Pin  = FB_IMD_ERR_GPIO_IN_Pin | SD_FB_SD_FNL_IN_TO_SD_DLY_CAPS_GPIO_IN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : PCPin PCPin PCPin PCPin
                           PCPin PCPin PCPin */
    GPIO_InitStruct.Pin = SD_FB_AMS_ERR_RLY_TO_IMD_ERR_RLY_GPIO_IN_Pin | SD_FB_SD_MID_IN_TO_AMS_ERR_RLY_GPIO_IN_Pin |
                          FB_nANY_IMPL_ERR_GPIO_IN_Pin | FB_DCBUS_OVER_60V_IMPL_ERR_GPIO_IN_Pin |
                          FB_AIR_NEG_IMPL_ERR_GPIO_IN_Pin | FB_SD_PRCH_RLY_GPIO_IN_Pin |
                          SD_FB_IMD_ERR_RLY_TO_SD_PRCH_RLY_GPIO_IN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin  = FB_AMS_ERR_GPIO_IN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(FB_AMS_ERR_GPIO_IN_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : PAPin PAPin PAPin PAPin
                           PAPin PAPin PAPin */
    GPIO_InitStruct.Pin = FB_AIR_POS_INT_SD_REL_GPIO_IN_Pin | FB_AIR_NEG_INT_SD_REL_GPIO_IN_Pin |
                          FB_DCBUS_PRCH_RLY_INT_SD_REL_GPIO_IN_Pin | FB_AIR_POS_IMPL_ERR_GPIO_IN_Pin |
                          FB_nAIR_NEG_AUX_GPIO_IN_Pin | FB_nAIR_POS_AUX_GPIO_IN_Pin | FB_nDCBUS_OVER_60V_GPIO_IN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : PCPin PCPin */
    GPIO_InitStruct.Pin   = LED_RED_GPIO_OUT_Pin | LED_ORANGE_GPIO_OUT_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : PBPin PBPin */
    GPIO_InitStruct.Pin   = LED_BLUE_GPIO_OUT_Pin | LED_GREEN_GPIO_OUT_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : PBPin PBPin PBPin PBPin
                           PBPin */
    GPIO_InitStruct.Pin = FB_SHRT2GND_AIR_NEG_GPIO_IN_Pin | FB_SHRT2GND_AIR_POS_GPIO_IN_Pin |
                          FB_nDCBUS_PRCH_RLY_AUX_GPIO_IN_Pin | FB_nSHRT2GND_AIRS_GPIO_IN_Pin |
                          FB_TSAL_GREEN_GPIO_IN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin  = FB_ANY_IMPL_ERR_LTCH_GPIO_IN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(FB_ANY_IMPL_ERR_LTCH_GPIO_IN_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin  = FB_DCBUS_PRCH_RLY_IMPL_ERR_GPIO_IN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(FB_DCBUS_PRCH_RLY_IMPL_ERR_GPIO_IN_GPIO_Port, &GPIO_InitStruct);

    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);

    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/* USER CODE BEGIN 2 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    GPIO_PinState edge_type;
    if (GPIO_Pin == SD_FB_SD_FNL_IN_TO_SD_DLY_CAPS_GPIO_IN_Pin) {
        edge_type = HAL_GPIO_ReadPin(SD_FB_SD_FNL_IN_TO_SD_DLY_CAPS_GPIO_IN_GPIO_Port,SD_FB_SD_FNL_IN_TO_SD_DLY_CAPS_GPIO_IN_Pin);
        _GPIO_IntEdgeHandler(SD_FB_SD_FNL_IN_TO_SD_DLY_CAPS, edge_type);
    } else if (GPIO_Pin == FB_AMS_ERR_GPIO_IN_Pin) {
        edge_type = HAL_GPIO_ReadPin(FB_AMS_ERR_GPIO_IN_GPIO_Port,FB_AMS_ERR_GPIO_IN_Pin);
        _GPIO_IntEdgeHandler(AMS_ERR, edge_type);
    } else if (GPIO_Pin == FB_IMD_ERR_GPIO_IN_Pin) {
        edge_type = HAL_GPIO_ReadPin(FB_IMD_ERR_GPIO_IN_GPIO_Port,FB_IMD_ERR_GPIO_IN_Pin );
        _GPIO_IntEdgeHandler(IMD_ERR, edge_type);
    } else if (GPIO_Pin == FB_DCBUS_OVER_60V_IMPL_ERR_GPIO_IN_Pin) {
        edge_type = HAL_GPIO_ReadPin(FB_DCBUS_OVER_60V_IMPL_ERR_GPIO_IN_GPIO_Port,FB_DCBUS_OVER_60V_IMPL_ERR_GPIO_IN_Pin);
        _GPIO_IntEdgeHandler(FB_DCBUS_OVER_60V_IMPL_ERR, edge_type);
    } else if (GPIO_Pin == FB_ANY_IMPL_ERR_LTCH_GPIO_IN_Pin) {
        edge_type = HAL_GPIO_ReadPin(FB_ANY_IMPL_ERR_LTCH_GPIO_IN_GPIO_Port,FB_ANY_IMPL_ERR_LTCH_GPIO_IN_Pin);
        _GPIO_IntEdgeHandler(FB_ANY_IMPL_ERR_LTCH, edge_type);
    } else {
        __NOP();
    }
}

void GPIO_IntEventRoutine() {
    for (uint8_t i = 0; i < GPIO_INT_EVENTS_ENUM_NUM; i++) {
        // if sync and async don't match, an event was triggered
        if (GPIO_INT_RISE_EDGE_CNT_ASYNC[i] != GPIO_INT_RISE_EDGE_CNT_SYNC[i]) {
            GPIO_rise_edge_events[i] =
                ((GPIO_IntEventCnt_MAX_VAL - GPIO_INT_RISE_EDGE_CNT_SYNC[i]) + GPIO_INT_RISE_EDGE_CNT_ASYNC[i]) %
                GPIO_IntEventCnt_MAX_VAL;
        }

        if (GPIO_INT_FALL_EDGE_CNT_ASYNC[i] != GPIO_INT_FALL_EDGE_CNT_SYNC[i]) {
            GPIO_fall_edge_events[i] =
                ((GPIO_IntEventCnt_MAX_VAL - GPIO_INT_FALL_EDGE_CNT_SYNC[i]) + GPIO_INT_FALL_EDGE_CNT_ASYNC[i]) %
                GPIO_IntEventCnt_MAX_VAL;
        }
    }
}

void _GPIO_IntRiseEdgeCnt_OverrunErr_callback(void) {
    __NOP();
}

void _GPIO_IntFallEdgeCnt_OverrunErr_callback(void) {
    __NOP();
}

void _GPIO_IntEdgeHandler(enum GPIO_IntEventsEnum gpio_int_event, GPIO_PinState edge) {
    assert_param(gpio_int_event < GPIO_INT_EVENTS_ENUM_NUM);
    uint8_t *edge_cnt_async_ptr;
    uint8_t *edge_cnt_sync_ptr;
    uint8_t edge_type;
    // rising edge
    if (edge == GPIO_PIN_SET) {
        edge_cnt_async_ptr = (uint8_t *)&(GPIO_INT_RISE_EDGE_CNT_ASYNC[gpio_int_event]);
        edge_cnt_sync_ptr  = (uint8_t *)&(GPIO_INT_RISE_EDGE_CNT_SYNC[gpio_int_event]);
        edge_type          = 1;
    } else {
        edge_cnt_async_ptr = (uint8_t *)&(GPIO_INT_FALL_EDGE_CNT_ASYNC[gpio_int_event]);
        edge_cnt_sync_ptr  = (uint8_t *)&(GPIO_INT_FALL_EDGE_CNT_SYNC[gpio_int_event]);
        edge_type          = 0;
    }
    // check for overrun
    // if async counter increments over sync counter then consumer of sync counter is lagging behind
    // do not increment and throw an error
    if (*edge_cnt_async_ptr + 1 != *edge_cnt_sync_ptr) {
        (*edge_cnt_async_ptr)++;  // if all ok increment edge counter
    } else {
        if (edge_type == 1)
            _GPIO_IntRiseEdgeCnt_OverrunErr_callback();
        else
            _GPIO_IntFallEdgeCnt_OverrunErr_callback();
    }
}
/* USER CODE END 2 */

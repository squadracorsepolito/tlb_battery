/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    can.c
  * @brief   This file provides code for the configuration
  *          of the CAN instances.
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
#include "can.h"

/* USER CODE BEGIN 0 */
#include <stdint.h>

//volatile struct CAN_tlb_batt_shtdwn_fb tlb_batt_shtdwn_fb_can_msg;
//volatile struct CAN_tlb_batt_sig_fb tlb_batt_sig_fb_can_msg;

union CAN_MsgUnion tlb_batt_shtdwn_fb_can_msg;
union CAN_MsgUnion tlb_batt_sig_fb_can_msg;

/* USER CODE END 0 */

CAN_HandleTypeDef hcan1;

/* CAN1 init function */
void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 3;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_12TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = ENABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* USER CODE END CAN1_Init 2 */

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**CAN1 GPIO Configuration
    PB8     ------> CAN1_RX
    PB9     ------> CAN1_TX
    */
    GPIO_InitStruct.Pin = SN65HVD23x_R_CAN1_RX_Pin|SN65HVD23x_D_CAN1_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();

    /**CAN1 GPIO Configuration
    PB8     ------> CAN1_RX
    PB9     ------> CAN1_TX
    */
    HAL_GPIO_DeInit(GPIOB, SN65HVD23x_R_CAN1_RX_Pin|SN65HVD23x_D_CAN1_TX_Pin);

  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
#define FLOAT_PNT_TO_FIXED_PNT(FLOAT_PNT_VAL, FSR_DOMAIN_FLOAT_PNT_VALUE, FSR_FIXED_PNT) \
    (FSR_FIXED_PNT / (double)FSR_DOMAIN_FLOAT_PNT_VALUE * FLOAT_PNT_VAL)

void CAN_serialize_tlb_batt_shtdwn_fb(struct CAN_tlb_batt_shtdwn_fb *tlb_batt_shtdwn_fb_can_msg, uint8_t data[8]) {
    uint64_t data64 = 0;
    data64 |= ((uint64_t)tlb_batt_shtdwn_fb_can_msg->sd_mid_in_to_ams_err_rly) ;
    data64 |= ((uint64_t)tlb_batt_shtdwn_fb_can_msg->ams_err_rly_to_imd_err_rly) << 1U;
    data64 |= ((uint64_t)tlb_batt_shtdwn_fb_can_msg->imd_err_rly_to_sd_prch_rly) << 2U;
    data64 |= ((uint64_t)tlb_batt_shtdwn_fb_can_msg->sd_fnl_in_to_sd_dly_caps) <<  3U;
    data64 |= ((uint64_t)tlb_batt_shtdwn_fb_can_msg->sd_prch_rly_to_sd_mid_out_V) << 8;
    //data64 |= ((uint64_t)tlb_batt_shtdwn_fb_can_msg->sd_fnl_in_to_sd_dly_caps) <<  11U;
    data64 |= ((uint64_t)tlb_batt_shtdwn_fb_can_msg->sd_dly_caps_to_sd_fin_out_airs_V) << 16U;
    for (int i = 0; i < 8; i++) {
        data[i] = (uint8_t)((data64 >> (i * 8)) & 0xFFU);
    }
}
void CAN_serialize_tlb_batt_sig_fb(struct CAN_tlb_batt_sig_fb *tlb_batt_sig_fb_can_msg, uint8_t data[8]) {
    uint64_t data64 = 0;
    data64 |= ((uint64_t)tlb_batt_sig_fb_can_msg->ams_err);
    data64 |= ((uint64_t)tlb_batt_sig_fb_can_msg->imd_err) << 1U;
    data64 |= ((uint64_t)tlb_batt_sig_fb_can_msg->sd_prch_rly) << 2U;
    data64 |= ((uint64_t)tlb_batt_sig_fb_can_msg->shrt2gnd_air_neg) << 3U;
    data64 |= ((uint64_t)tlb_batt_sig_fb_can_msg->shrt2gnd_air_pos) << 4U;
    data64 |= ((uint64_t)tlb_batt_sig_fb_can_msg->shrt2gnd_airs) << 5U;
    data64 |= ((uint64_t)tlb_batt_sig_fb_can_msg->dcbus_over_60v) << 6U;
    data64 |= ((uint64_t)tlb_batt_sig_fb_can_msg->air_neg_int_sd_rel) << 7U;
    data64 |= ((uint64_t)tlb_batt_sig_fb_can_msg->air_pos_int_sd_rel) << 8U;
    data64 |= ((uint64_t)tlb_batt_sig_fb_can_msg->dcbus_prch_rly_int_sd_rel) << 9U;
    data64 |= ((uint64_t)tlb_batt_sig_fb_can_msg->air_neg_aux) << 10U;
    data64 |= ((uint64_t)tlb_batt_sig_fb_can_msg->air_pos_aux) << 11U;
    data64 |= ((uint64_t)tlb_batt_sig_fb_can_msg->dcbus_prch_rly_aux) << 12U;
    data64 |= ((uint64_t)tlb_batt_sig_fb_can_msg->air_neg_impl_err) << 13U;
    data64 |= ((uint64_t)tlb_batt_sig_fb_can_msg->air_pos_impl_err) << 14U;
    data64 |= ((uint64_t)tlb_batt_sig_fb_can_msg->dcbus_prch_rly_impl_err) <<  15U;
    data64 |= ((uint64_t)tlb_batt_sig_fb_can_msg->dcbus_over_60v_impl_err) << 16U;
    data64 |= ((uint64_t)tlb_batt_sig_fb_can_msg->any_impl_err) <<  17U;
    data64 |= ((uint64_t)tlb_batt_sig_fb_can_msg->any_impl_err_ltch) << 18U;
    data64 |= ((uint64_t)tlb_batt_sig_fb_can_msg->tsal_green) <<  19U;
    for (int i = 0; i < 8; i++) {
        data[i] = (uint8_t)((data64 >> (i * 8)) & 0xFFU);
    }
}

void CAN_update_tlb_batt_shtdwn_fb(struct DB_SHTDWN_FB *db_shtdwn_fb,
                                   struct CAN_tlb_batt_shtdwn_fb *tlb_batt_shtdwn_fb_can_msg) {
    tlb_batt_shtdwn_fb_can_msg->sd_mid_in_to_ams_err_rly   = (db_shtdwn_fb->sd_mid_in_to_ams_err_rly & 0b1U);
    tlb_batt_shtdwn_fb_can_msg->ams_err_rly_to_imd_err_rly = (db_shtdwn_fb->ams_err_rly_to_imd_err_rly & 0b1U);
    tlb_batt_shtdwn_fb_can_msg->imd_err_rly_to_sd_prch_rly = (db_shtdwn_fb->imd_err_rly_to_sd_prch_rly & 0b1U);
    tlb_batt_shtdwn_fb_can_msg->sd_prch_rly_to_sd_mid_out_V =
        (uint8_t)FLOAT_PNT_TO_FIXED_PNT(db_shtdwn_fb->sd_prch_rly_to_sd_mid_out_V, 25.0, UINT8_MAX);
    tlb_batt_shtdwn_fb_can_msg->sd_fnl_in_to_sd_dly_caps = (db_shtdwn_fb->sd_fnl_in_to_sd_dly_caps & 0b1U);
    tlb_batt_shtdwn_fb_can_msg->sd_dly_caps_to_sd_fin_out_airs_V =
        (uint8_t)FLOAT_PNT_TO_FIXED_PNT(db_shtdwn_fb->sd_dly_caps_to_sd_fin_out_airs_V, 25.0, UINT8_MAX);
}

void CAN_update_tlb_batt_sig_fb(struct DB_TLB_SIG_FB *db_tlb_sig_fb,
                                struct CAN_tlb_batt_sig_fb *tlb_batt_sig_fb_can_msg) {
    tlb_batt_sig_fb_can_msg->ams_err                   = (db_tlb_sig_fb->ams_err & 0b1U);
    tlb_batt_sig_fb_can_msg->imd_err                   = (db_tlb_sig_fb->imd_err & 0b1U);
    tlb_batt_sig_fb_can_msg->sd_prch_rly               = (db_tlb_sig_fb->sd_prch_rly & 0b1U);
    tlb_batt_sig_fb_can_msg->shrt2gnd_air_neg          = (db_tlb_sig_fb->shrt2gnd_air_neg & 0b1U);
    tlb_batt_sig_fb_can_msg->shrt2gnd_air_pos          = (db_tlb_sig_fb->shrt2gnd_air_pos & 0b1U);
    tlb_batt_sig_fb_can_msg->shrt2gnd_airs             = (db_tlb_sig_fb->shrt2gnd_airs & 0b1U);
    tlb_batt_sig_fb_can_msg->dcbus_over_60v            = (db_tlb_sig_fb->dcbus_over_60v & 0b1U);
    tlb_batt_sig_fb_can_msg->air_neg_int_sd_rel        = (db_tlb_sig_fb->air_neg_int_sd_rel & 0b1U);
    tlb_batt_sig_fb_can_msg->air_pos_int_sd_rel        = (db_tlb_sig_fb->air_pos_int_sd_rel & 0b1U);
    tlb_batt_sig_fb_can_msg->dcbus_prch_rly_int_sd_rel = (db_tlb_sig_fb->dcbus_prch_rly_int_sd_rel & 0b1U);
    tlb_batt_sig_fb_can_msg->air_neg_aux               = (db_tlb_sig_fb->air_neg_aux & 0b1U);
    tlb_batt_sig_fb_can_msg->air_pos_aux               = (db_tlb_sig_fb->air_pos_aux & 0b1U);
    tlb_batt_sig_fb_can_msg->dcbus_prch_rly_aux        = (db_tlb_sig_fb->dcbus_prch_rly_aux & 0b1U);
    tlb_batt_sig_fb_can_msg->air_neg_impl_err          = (db_tlb_sig_fb->air_neg_impl_err & 0b1U);
    tlb_batt_sig_fb_can_msg->air_pos_impl_err          = (db_tlb_sig_fb->air_pos_impl_err & 0b1U);
    tlb_batt_sig_fb_can_msg->dcbus_prch_rly_impl_err   = (db_tlb_sig_fb->dcbus_prch_rly_impl_err & 0b1U);
    tlb_batt_sig_fb_can_msg->dcbus_over_60v_impl_err   = (db_tlb_sig_fb->dcbus_over_60v_impl_err & 0b1U);
    tlb_batt_sig_fb_can_msg->any_impl_err              = (db_tlb_sig_fb->any_impl_err & 0b1U);
    tlb_batt_sig_fb_can_msg->any_impl_err_ltch         = (db_tlb_sig_fb->any_impl_err_ltch & 0b1U);
    tlb_batt_sig_fb_can_msg->tsal_green                = (db_tlb_sig_fb->tsal_green & 0b1U);
}

/* USER CODE END 1 */

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
#include "main.h"
#include "mcb.h"
#include "usart.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define print_log(_STATIC_STR_, _NOT_USED) \
    HAL_UART_Transmit(&VCP_UART_Handle, (uint8_t *)_STATIC_STR_, strlen(_STATIC_STR_), VCP_TX_LOG_BUF_MAX_TIMEOUT_MS)

struct mcb_tlb_battery_tsal_status_t CAN_tlb_battery_tsal_status;
struct mcb_tlb_battery_shut_status_t CAN_tlb_battery_shut_status;

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

/**
 * @brief Print the error message in the serial console and activate
 *        the CAN error LED
 * */
static void _CAN_error_handler(CAN_HandleTypeDef *hcan) {
    //#define print_log(x,y) ((void)0U)

    uint32_t err_code = HAL_CAN_GetError(hcan);

    if ((err_code & HAL_CAN_ERROR_EWG) == HAL_CAN_ERROR_EWG)
        print_log("Protocol error warning", NO_HEADER);
    if ((err_code & HAL_CAN_ERROR_EPV) == HAL_CAN_ERROR_EPV)
        print_log("Error passive", NO_HEADER);
    if ((err_code & HAL_CAN_ERROR_BOF) == HAL_CAN_ERROR_BOF)
        print_log("Bus-off error", NO_HEADER);
    if ((err_code & HAL_CAN_ERROR_STF) == HAL_CAN_ERROR_STF)
        print_log("Stuff error", NO_HEADER);
    if ((err_code & HAL_CAN_ERROR_FOR) == HAL_CAN_ERROR_FOR)
        print_log("Form error", NO_HEADER);
    if ((err_code & HAL_CAN_ERROR_ACK) == HAL_CAN_ERROR_ACK)
        print_log("ACK error", NO_HEADER);
    if ((err_code & HAL_CAN_ERROR_BR) == HAL_CAN_ERROR_BR)
        print_log("Bit Recessive error", NO_HEADER);
    if ((err_code & HAL_CAN_ERROR_BD) == HAL_CAN_ERROR_BD)
        print_log("Bit Dominant error", NO_HEADER);
    if ((err_code & HAL_CAN_ERROR_CRC) == HAL_CAN_ERROR_CRC)
        print_log("CRC error", NO_HEADER);
    if ((err_code & HAL_CAN_ERROR_RX_FOV0) == HAL_CAN_ERROR_RX_FOV0)
        print_log("FIFO 0 overrun error", NO_HEADER);
    if ((err_code & HAL_CAN_ERROR_RX_FOV1) == HAL_CAN_ERROR_RX_FOV1)
        print_log("FIFO 1 overrun error", NO_HEADER);
    if ((err_code & HAL_CAN_ERROR_TX_ALST0) == HAL_CAN_ERROR_TX_ALST0)
        print_log("TX 0 arbitration lost error", NO_HEADER);
    if ((err_code & HAL_CAN_ERROR_TX_TERR0) == HAL_CAN_ERROR_TX_TERR0)
        print_log("TX 0 transmit error", NO_HEADER);
    if ((err_code & HAL_CAN_ERROR_TX_ALST1) == HAL_CAN_ERROR_TX_ALST1)
        print_log("TX 1 arbitration lost error", NO_HEADER);
    if ((err_code & HAL_CAN_ERROR_TX_TERR1) == HAL_CAN_ERROR_TX_TERR1)
        print_log("TX 1 transmit error", NO_HEADER);
    if ((err_code & HAL_CAN_ERROR_TX_ALST2) == HAL_CAN_ERROR_TX_ALST2)
        print_log("TX 2 arbitration lost error", NO_HEADER);
    if ((err_code & HAL_CAN_ERROR_TX_TERR2) == HAL_CAN_ERROR_TX_TERR2)
        print_log("TX 2 transmit error", NO_HEADER);
    if ((err_code & HAL_CAN_ERROR_TIMEOUT) == HAL_CAN_ERROR_TIMEOUT)
        print_log("Timeout error", NO_HEADER);
    if ((err_code & HAL_CAN_ERROR_NOT_INITIALIZED) == HAL_CAN_ERROR_NOT_INITIALIZED)
        print_log("CAN bus not initialized", NO_HEADER);
    if ((err_code & HAL_CAN_ERROR_NOT_READY) == HAL_CAN_ERROR_NOT_READY)
        print_log("CAN bus not ready", NO_HEADER);
    if ((err_code & HAL_CAN_ERROR_NOT_STARTED) == HAL_CAN_ERROR_NOT_STARTED)
        print_log("CAN bus not started", NO_HEADER);
    if ((err_code & HAL_CAN_ERROR_PARAM) == HAL_CAN_ERROR_PARAM)
        print_log("Parameter error", NO_HEADER);
    if ((err_code & HAL_CAN_ERROR_INTERNAL) == HAL_CAN_ERROR_INTERNAL)
        print_log("Internal error", NO_HEADER);

    char buf[50];

    uint16_t rec_val = (uint16_t)((hcan->Instance->ESR && CAN_ESR_REC_Msk) >> CAN_ESR_REC_Pos);
    if (rec_val > 0) {
        sprintf(buf, "REC (Receive Error Counter) %d", rec_val);
        print_log(buf, NO_HEADER);
    }

    uint16_t tec_val = (uint16_t)((hcan->Instance->ESR && CAN_ESR_TEC_Msk) >> CAN_ESR_TEC_Pos);
    if (tec_val > 0) {
        sprintf(buf, "TEC (Transmit Error Counter) %d", tec_val);
        print_log(buf, NO_HEADER);
    }
    print_log("ERROR\r\n", NO_HEADER);
}

void _CAN_tlb_battery_shut_status_msg_construct(struct DB_data_t *db_data,
                                                struct mcb_tlb_battery_shut_status_t *can_data_msg) {
    // clang-format off
    assert_param(mcb_tlb_battery_shut_status_is_shut_closed_pre_ams_imd_latch_is_in_range(db_data->sd_mid_in_to_ams_err_rly & 0b1U));
    assert_param(mcb_tlb_battery_shut_status_is_shut_closed_post_ams_latch_is_in_range(db_data->ams_err_rly_to_imd_err_rly & 0b1U));
    assert_param(mcb_tlb_battery_shut_status_is_shut_closed_post_imd_latch_is_in_range(db_data->imd_err_rly_to_sd_prch_rly & 0b1U));
    assert_param(mcb_tlb_battery_shut_status_is_shutdown_closed_pre_tlb_batt_final_is_in_range(db_data->sd_fnl_in_to_sd_dly_caps & 0b1U));
    assert_param(mcb_tlb_battery_shut_status_is_ams_error_latched_is_in_range(db_data->ams_err & 0b1U));
    assert_param(mcb_tlb_battery_shut_status_is_imd_error_latched_is_in_range(db_data->imd_err & 0b1U));
    assert_param(mcb_tlb_battery_shut_status_is_sd_prch_rly_closed_is_in_range(db_data->sd_prch_rly & 0b1U));
    assert_param(mcb_tlb_battery_shut_status_shutdown_adc_post_sd_precharge_relay_is_in_range(db_data->sd_prch_rly_to_sd_mid_out_V));
    assert_param(mcb_tlb_battery_shut_status_shutdown_adc_ai_rs_opening_delay_caps_is_in_range(db_data->sd_dly_caps_to_sd_fin_out_airs_V));

    can_data_msg->is_shut_closed_pre_ams_imd_latch      = mcb_tlb_battery_shut_status_is_shut_closed_pre_ams_imd_latch_encode(db_data->sd_mid_in_to_ams_err_rly & 0b1U);
    can_data_msg->is_shut_closed_post_ams_latch         = mcb_tlb_battery_shut_status_is_shut_closed_post_ams_latch_encode(db_data->ams_err_rly_to_imd_err_rly & 0b1U);
    can_data_msg->is_shut_closed_post_imd_latch         = mcb_tlb_battery_shut_status_is_shut_closed_post_imd_latch_encode(db_data->imd_err_rly_to_sd_prch_rly & 0b1U);
    can_data_msg->is_shutdown_closed_pre_tlb_batt_final = mcb_tlb_battery_shut_status_is_shutdown_closed_pre_tlb_batt_final_encode(db_data->sd_fnl_in_to_sd_dly_caps & 0b1U);
    can_data_msg->is_ams_error_latched                  = mcb_tlb_battery_shut_status_is_ams_error_latched_encode(db_data->ams_err & 0b1U);
    can_data_msg->is_imd_error_latched                  = mcb_tlb_battery_shut_status_is_imd_error_latched_encode(db_data->imd_err & 0b1U);
    can_data_msg->is_sd_prch_rly_closed                 = mcb_tlb_battery_shut_status_is_sd_prch_rly_closed_encode(db_data->sd_prch_rly & 0b1U);
    can_data_msg->shutdown_adc_post_sd_precharge_relay  = mcb_tlb_battery_shut_status_shutdown_adc_post_sd_precharge_relay_encode(db_data->sd_prch_rly_to_sd_mid_out_V);
    can_data_msg->shutdown_adc_ai_rs_opening_delay_caps = mcb_tlb_battery_shut_status_shutdown_adc_ai_rs_opening_delay_caps_encode(db_data->sd_dly_caps_to_sd_fin_out_airs_V);
    // clang-format on
}

void _CAN_tlb_battery_tsal_status_msg_construct(struct DB_data_t *db_data,
                                                struct mcb_tlb_battery_tsal_status_t *can_data_msg) {
    // clang-format off
    assert_param(mcb_tlb_battery_tsal_status_tsal_is_green_on_is_in_range(db_data->shrt2gnd_air_neg & 0b1U));
    assert_param(mcb_tlb_battery_tsal_status_scs_short2_gnd_air_pos_is_in_range(db_data->shrt2gnd_air_pos & 0b1U));
    assert_param(mcb_tlb_battery_tsal_status_scs_is_any_short2_gnd_present_is_in_range(db_data->shrt2gnd_airs & 0b1U));
    assert_param(mcb_tlb_battery_tsal_status_tsal_is_dc_bus_over60_v_is_in_range(db_data->dcbus_over_60v & 0b1U));
    assert_param(mcb_tlb_battery_tsal_status_intentional_state_air_neg_is_in_range(db_data->air_neg_int_sd_rel & 0b1U));
    assert_param(mcb_tlb_battery_tsal_status_intentional_state_air_pos_is_in_range(db_data->air_pos_int_sd_rel & 0b1U));
    assert_param(mcb_tlb_battery_tsal_status_intentional_state_relay_precharge_is_in_range(db_data->dcbus_prch_rly_int_sd_rel & 0b1U));
    assert_param(mcb_tlb_battery_tsal_status_tsal_is_air_neg_closed_is_in_range(db_data->air_neg_aux & 0b1U));
    assert_param(mcb_tlb_battery_tsal_status_tsal_is_air_pos_closed_is_in_range(db_data->air_pos_aux & 0b1U));
    assert_param(mcb_tlb_battery_tsal_status_tsal_is_relay_precharge_closed_is_in_range(db_data->dcbus_prch_rly_aux & 0b1U));
    assert_param(mcb_tlb_battery_tsal_status_imp_is_air_neg_imp_present_is_in_range(db_data->air_neg_impl_err & 0b1U));
    assert_param(mcb_tlb_battery_tsal_status_imp_is_air_pos_imp_present_is_in_range(db_data->air_pos_impl_err & 0b1U));
    assert_param(mcb_tlb_battery_tsal_status_imp_is_relay_precharge_imp_present_is_in_range(db_data->dcbus_prch_rly_impl_err & 0b1U));
    assert_param(mcb_tlb_battery_tsal_status_imp_is_dc_bus_voltage_imp_present_is_in_range(db_data->dcbus_over_60v_impl_err & 0b1U));
    assert_param(mcb_tlb_battery_tsal_status_imp_is_any_imp_present_is_in_range(db_data->any_impl_err & 0b1U));
    assert_param(mcb_tlb_battery_tsal_status_imp_is_any_imp_latched_is_in_range(db_data->any_impl_err_ltch & 0b1U));
    assert_param(mcb_tlb_battery_tsal_status_tsal_is_green_on_is_in_range(db_data->tsal_green & 0b1U));

    can_data_msg->scs_short2_gnd_air_neg             = mcb_tlb_battery_tsal_status_tsal_is_green_on_encode(db_data->shrt2gnd_air_neg & 0b1U);
    can_data_msg->scs_short2_gnd_air_pos             = mcb_tlb_battery_tsal_status_scs_short2_gnd_air_pos_encode(db_data->shrt2gnd_air_pos & 0b1U);
    can_data_msg->scs_is_any_short2_gnd_present      = mcb_tlb_battery_tsal_status_scs_is_any_short2_gnd_present_encode(db_data->shrt2gnd_airs & 0b1U);
    can_data_msg->tsal_is_dc_bus_over60_v            = mcb_tlb_battery_tsal_status_tsal_is_dc_bus_over60_v_encode(db_data->dcbus_over_60v & 0b1U);
    can_data_msg->intentional_state_air_neg          = mcb_tlb_battery_tsal_status_intentional_state_air_neg_encode(db_data->air_neg_int_sd_rel & 0b1U);
    can_data_msg->intentional_state_air_pos          = mcb_tlb_battery_tsal_status_intentional_state_air_pos_encode(db_data->air_pos_int_sd_rel & 0b1U);
    can_data_msg->intentional_state_relay_precharge  = mcb_tlb_battery_tsal_status_intentional_state_relay_precharge_encode(db_data->dcbus_prch_rly_int_sd_rel & 0b1U);
    can_data_msg->tsal_is_air_neg_closed             = mcb_tlb_battery_tsal_status_tsal_is_air_neg_closed_encode(db_data->air_neg_aux & 0b1U);
    can_data_msg->tsal_is_air_pos_closed             = mcb_tlb_battery_tsal_status_tsal_is_air_pos_closed_encode(db_data->air_pos_aux & 0b1U);
    can_data_msg->tsal_is_relay_precharge_closed     = mcb_tlb_battery_tsal_status_tsal_is_relay_precharge_closed_encode(db_data->dcbus_prch_rly_aux & 0b1U);
    can_data_msg->imp_is_air_neg_imp_present         = mcb_tlb_battery_tsal_status_imp_is_air_neg_imp_present_encode(db_data->air_neg_impl_err & 0b1U);
    can_data_msg->imp_is_air_pos_imp_present         = mcb_tlb_battery_tsal_status_imp_is_air_pos_imp_present_encode(db_data->air_pos_impl_err & 0b1U);
    can_data_msg->imp_is_relay_precharge_imp_present = mcb_tlb_battery_tsal_status_imp_is_relay_precharge_imp_present_encode(db_data->dcbus_prch_rly_impl_err & 0b1U);
    can_data_msg->imp_is_dc_bus_voltage_imp_present  = mcb_tlb_battery_tsal_status_imp_is_dc_bus_voltage_imp_present_encode(db_data->dcbus_over_60v_impl_err & 0b1U);
    can_data_msg->imp_is_any_imp_present             = mcb_tlb_battery_tsal_status_imp_is_any_imp_present_encode(db_data->any_impl_err & 0b1U);
    can_data_msg->imp_is_any_imp_latched             = mcb_tlb_battery_tsal_status_imp_is_any_imp_latched_encode(db_data->any_impl_err_ltch & 0b1U);
    can_data_msg->tsal_is_green_on                   = mcb_tlb_battery_tsal_status_tsal_is_green_on_encode(db_data->tsal_green & 0b1U);
    // clang-format on
}
/**
 * @brief Send a CAN message given it's id on the MAIN CAN BUS
 * @note This function will automatically gather all data for the specific message
 * @param can_msg_id The id of the message to send
 * @param timeout_ms If no mailbox is free until timeout_ms exit with error HAL_TIMEOUT
 * @return - HAL_OK if everything went ok
 *         - HAL_ERROR in case of errors either on the CAN peripheral or
 *         internally in the function
 *         - HAL_TIMEOUT in case a message can't be sent in case of full mailboxes
 */
HAL_StatusTypeDef _CAN_MCB_SendMsg(uint16_t can_msg_id, uint16_t timeout_ms) {
#define CAN_TX_PAYLOAD_ARRAY_SIZE (8U)
    static uint8_t can_tx_payload[CAN_TX_PAYLOAD_ARRAY_SIZE] = {0};
    static CAN_TxHeaderTypeDef can_tx_header = {.IDE = CAN_ID_STD, .RTR = CAN_RTR_DATA, .TransmitGlobalTime = DISABLE};

    if (can_msg_id == MCB_TLB_BATTERY_TSAL_STATUS_FRAME_ID) {
        can_tx_header.StdId = MCB_TLB_BATTERY_TSAL_STATUS_FRAME_ID;
        can_tx_header.DLC   = MCB_TLB_BATTERY_TSAL_STATUS_LENGTH;
        _CAN_tlb_battery_tsal_status_msg_construct(&DB_data, &CAN_tlb_battery_tsal_status);
        mcb_tlb_battery_tsal_status_pack(can_tx_payload,
                                         &CAN_tlb_battery_tsal_status,
                                         //(const struct mcb_tlb_battery_tsal_status_t *)&CAN_tlb_battery_tsal_status,
                                         CAN_TX_PAYLOAD_ARRAY_SIZE);
    } else if (can_msg_id == MCB_TLB_BATTERY_SHUT_STATUS_FRAME_ID) {
        can_tx_header.StdId = MCB_TLB_BATTERY_SHUT_STATUS_FRAME_ID;
        can_tx_header.DLC   = MCB_TLB_BATTERY_SHUT_STATUS_LENGTH;
        _CAN_tlb_battery_shut_status_msg_construct(&DB_data, &CAN_tlb_battery_shut_status);
        mcb_tlb_battery_shut_status_pack(can_tx_payload, &CAN_tlb_battery_shut_status, CAN_TX_PAYLOAD_ARRAY_SIZE);
    } else {
        // Message id not found error out
        // TODO: make assertions on can_msg_id so this event can't happen
        return HAL_ERROR;
    }

    // Try sending the CAN Message

#if 0
    uint16_t send_but_full_mailboxes_cnt = 0U;
    // While there are no free mailboxes, wait and retry or exit when maximum attempts are reached
    while(HAL_CAN_GetTxMailboxesFreeLevel(&CAN_MCB_Handle) == 0){
        if(send_but_full_mailboxes_cnt < CAN_MCB_TxAttemptsMailboxesFull){
            // Abort all transmission requests and retry requests
            HAL_CAN_AbortTxRequest (&CAN_MCB_Handle, CAN_TX_MAILBOX0 | CAN_TX_MAILBOX1 | CAN_TX_MAILBOX2);
            break;
        }
        HAL_Delay(CAN_MCB_TxAttemptsMailboxesFull);

        send_but_full_mailboxes_cnt++;
    }
    send_but_full_mailboxes_cnt = 0;
#else

    uint32_t tick = HAL_GetTick();
    // if no mailbox is available wait
    while (HAL_CAN_GetTxMailboxesFreeLevel(&CAN_MCB_Handle) == 0) {
        // if timeout is reached exit with timeout
        if (HAL_GetTick() - tick >= timeout_ms) {
            print_log("CAN - timeout error\r\n", NO_HEADER);
            return HAL_TIMEOUT;
        }
    }

#endif

    // Send message
    uint32_t usedTxMailbox;
    return HAL_CAN_AddTxMessage(&CAN_MCB_Handle, &can_tx_header, can_tx_payload, &usedTxMailbox);
}

void CAN_SendMsg(CAN_HandleTypeDef *hcan, uint16_t can_msg_id) {
    assert_param(hcan);

    HAL_StatusTypeDef ret_code = HAL_OK;

    // Check that message is sent on the correct CAN bus line
    if (hcan == &CAN_MCB_Handle) {
        ret_code = _CAN_MCB_SendMsg(can_msg_id, 1);
    }  // add below additional can modules

    can_error_detected = ret_code != HAL_OK;

    if (ret_code != HAL_OK) {
        _CAN_error_handler(hcan);
    }
}

void CAN_Send100msMessages(void) {
    CAN_SendMsg(&CAN_MCB_Handle, MCB_TLB_BATTERY_TSAL_STATUS_FRAME_ID);
    CAN_SendMsg(&CAN_MCB_Handle, MCB_TLB_BATTERY_SHUT_STATUS_FRAME_ID);
}

/* USER CODE END 1 */

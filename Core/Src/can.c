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
#include "bsp.h"
#include "gpio.h"
#include "main.h"
#include "mcb.h"
#include "usart.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

CAN_TxHeaderTypeDef CAN_CAN1_tx_header = {.ExtId = 0, .IDE=CAN_ID_STD, .RTR=CAN_RTR_DATA};
volatile uint8_t CAN_err = 0;

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
  hcan1.Init.AutoRetransmission = ENABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */
    
    CAN_FilterTypeDef filter;


    filter.FilterScale = CAN_FILTERSCALE_16BIT;
    filter.FilterMode  = CAN_FILTERMODE_IDLIST;
    /* HAL considers IdLow and IdHigh not as just the ID of the can message but
        as the combination of: 
        STDID + RTR + IDE + 3 most significant bits of EXTID (not used in 16 bit mode)
        Hence if you care only to filter by id shift by 5
    */
    filter.FilterIdLow  = MCB_DIAG_TOOL_XCP_TX_TLB_BAT_FRAME_ID << 5U;     // Filter for this ID (shift by 5 see filtering registers on RM)
    filter.FilterIdHigh = MCB_DIAG_TOOL_XCP_TX_TLB_BAT_FRAME_ID << 5U;      // repeat the filter to not leave it empty
    filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filter.FilterBank           = 0;
    filter.FilterActivation     = ENABLE;
    filter.SlaveStartFilterBank = 14;

    HAL_CAN_ConfigFilter(&hcan1, &filter);

    HAL_CAN_ActivateNotification(&hcan1,
                                 CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_ERROR_WARNING | CAN_IT_ERROR_PASSIVE |
                                     CAN_IT_BUSOFF | CAN_IT_LAST_ERROR_CODE | CAN_IT_ERROR);
    HAL_CAN_Start(&hcan1);

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
    GPIO_InitStruct.Pin = SN65HVD23x_R_CAN_RX_Pin|SN65HVD23x_D_CAN_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
    HAL_NVIC_SetPriority(CAN1_SCE_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);
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
    HAL_GPIO_DeInit(GPIOB, SN65HVD23x_R_CAN_RX_Pin|SN65HVD23x_D_CAN_TX_Pin);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_SCE_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    CAN_RxHeaderTypeDef rx_header;
    uint8_t buffer[8] = {0};
    if(hcan == &MCB_Handle){
        if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, buffer) == HAL_OK) {
            // Reset when preparing for flash via CAN bus
            if(rx_header.StdId == MCB_DIAG_TOOL_XCP_TX_TLB_BAT_FRAME_ID && buffer[0] == 0xff && buffer[1] == 0x00) {
                NVIC_SystemReset();
            }
        }
    }
}

/**
 * @brief Print the error message in the serial console and activate
 *        the CAN error LED
 * */
static void CAN_error_handler(CAN_HandleTypeDef *hcan) {
    return;
#define print_log(x, y) ((void)0U)

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


void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan) {
    if(hcan == &MCB_Handle){
        CAN_error_handler(hcan);
    }
}

static HAL_StatusTypeDef CAN_wait(CAN_HandleTypeDef *hcan, uint8_t timeout) {
    uint32_t tick = HAL_GetTick();
    while (HAL_CAN_GetTxMailboxesFreeLevel(hcan) == 0) {
        if(HAL_GetTick() - tick > timeout) return HAL_TIMEOUT;
    }
    return HAL_OK;
}

HAL_StatusTypeDef CAN_send(CAN_HandleTypeDef *hcan, uint8_t *buffer, CAN_TxHeaderTypeDef *header) {
    if(CAN_wait(hcan, 1) != HAL_OK) return HAL_TIMEOUT;
    uint32_t mailbox;

    volatile HAL_StatusTypeDef status = HAL_CAN_AddTxMessage(hcan, header, buffer, &mailbox);

    return status;
}

//void _CAN_SendInstantMessagesRoutine(void) {
//    // check instant ERROR messages
//
//    uint8_t send_shut_status_message = 0;
//    uint8_t send_tsal_status_message = 0;
//
//    if (GPIO_fall_edge_events[SD_FB_SD_FNL_IN_TO_SD_DLY_CAPS] != 0) {
//        send_shut_status_message         = 1;
//        DB_data.sd_fnl_in_to_sd_dly_caps = 1U;
//    }
//
//    if (GPIO_rise_edge_events[AMS_ERR] != 0) {
//        send_tsal_status_message = 1;
//        DB_data.ams_err          = 1U;
//    }
//    if (GPIO_rise_edge_events[IMD_ERR] != 0) {
//        send_tsal_status_message = 1;
//        DB_data.imd_err          = 1U;
//    }
//    if (GPIO_rise_edge_events[FB_ANY_IMPL_ERR_LTCH] != 0) {
//        send_tsal_status_message  = 1;
//        DB_data.any_impl_err_ltch = 1U;
//    }
//    if (GPIO_rise_edge_events[FB_DCBUS_OVER_60V_IMPL_ERR] != 0) {
//        send_tsal_status_message        = 1;
//        DB_data.dcbus_over_60v_impl_err = 1U;
//    }
//    // if an error toggled
//    // - if the error was not already active
//    // -- send message
//    // - else (the error was already active)
//    // -- do nothing
//    // else (the error is not active)
//    // -- do nothing
//    // AMS Error
//    if (send_shut_status_message) {
//        // re-sample
//        CAN_SendMsg(&MCB_Handle, MCB_TLB_BATTERY_SHUT_STATUS_FRAME_ID);
//    }
//    if (send_tsal_status_message) {
//        // re-sample
//        CAN_SendMsg(&MCB_Handle, MCB_TLB_BATTERY_TSAL_STATUS_FRAME_ID);
//    }
//}

//void CAN_SendMessagesRoutine(void) {
//    static volatile uint32_t cnt100ms  = 0U;
//    static volatile uint32_t cnt1000ms = 0U;
//
//    // Instant Messages
//    // _CAN_SendInstantMessagesRoutine(); //TODO
//
//    // 100 ms messages
//    if (HAL_GetTick() >= cnt100ms) {
//        cnt100ms = HAL_GetTick() + 100U;
//        CAN_Send100msMessages();
//    }
//
//    // 1s messages
//    if (HAL_GetTick() >= cnt1000ms) {
//        cnt1000ms = HAL_GetTick() + 1000U;
//        CAN_Send1secMessages();
//    }
//}

//void CAN_Send100msMessages(void) {
//    CAN_SendMsg(&MCB_Handle, MCB_TLB_BATTERY_TSAL_STATUS_FRAME_ID);
//    CAN_SendMsg(&MCB_Handle, MCB_TLB_BATTERY_SHUT_STATUS_FRAME_ID);
//}
//
//void CAN_Send1secMessages(void) {
//    __NOP();
//};
/* USER CODE END 1 */

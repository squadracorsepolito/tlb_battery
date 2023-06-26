/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    can.h
  * @brief   This file contains all the function prototypes for
  *          the can.c file
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
#ifndef __CAN_H__
#define __CAN_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "db.h"
/* USER CODE END Includes */

extern CAN_HandleTypeDef hcan1;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_CAN1_Init(void);

/* USER CODE BEGIN Prototypes */
struct CAN_tlb_batt_shtdwn_fb {
    uint8_t sd_mid_in_to_ams_err_rly : 1; /*!< shutdown line status from middle section input to AMS ERROR realy - OFF(0)/ON(!0) */
    uint8_t ams_err_rly_to_imd_err_rly : 1; /*!< shutdown line status from AMS ERROR relay to IMD ERROR relay - OFF(0)/ON(!0) */
    uint8_t imd_err_rly_to_sd_prch_rly : 1; /*!< shutdown line status from IMD ERROR relay to shutdown precharge relay - OFF(0)/ON(!0) */
    uint8_t sd_prch_rly_to_sd_mid_out_V : 8; /*!< shutdown line status from shutdown precharge realy to middle section otput - milliVolts*/
    uint8_t sd_fnl_in_to_sd_dly_caps : 1; /*!< shutdown line status from shutdown final section input to shutdown delay caps - OFF(0)/ON(!0) */
    uint8_t sd_dly_caps_to_sd_fin_out_airs_V: 8; /*!< shutdown line status from shutdown delay capacitors to shutdown final section output AIRs - milliVolts */
};

struct CAN_tlb_batt_sig_fb {
    uint8_t ams_err : 1;     /*!< TLB signal feedback: AMS error control signal - OFF(0)/ON(!0) - (ON == ERROR) */
    uint8_t imd_err : 1;     /*!< TLB signal feedback: IMD error control signal - OFF(0)/ON(!0) - (ON == ERROR) */
    uint8_t sd_prch_rly : 1; /*!< TLB signal feedback: shutdown precharge relay control signal - OFF(0)/ON(!0) */

    uint8_t shrt2gnd_air_neg : 1; /*!< TLB signal feedback: short to ground of AIR negative- OFF(0)/ON(!0) - (ON == ERROR) */
    uint8_t shrt2gnd_air_pos : 1; /*!< TLB signal feedback: short to ground of AIR positive - OFF(0)/ON(!0) - (ON == ERROR) */
    uint8_t shrt2gnd_airs : 1; /*!< TLB signal feedback: short to ground of an/any AIR - OFF(0)/ON(!0) - (ON == ERROR) */

    uint8_t dcbus_over_60v : 1; /*!< TLB signal feedback: high voltage DC bus (post AIR+) over 60V - OFF(0)/ON(!0) - (ON == ACTIVE) */

    uint8_t air_neg_int_sd_rel : 1; /*!< TLB signal feedback: air negative closed, intentional state signal, shutdown reliant - OFF(0)/ON(!0) - (ON == CLOSED) */
    uint8_t air_pos_int_sd_rel : 1; /*!< TLB signal feedback: air positive closed, intentional state signal, shutdown reliant - OFF(0)/ON(!0) - (ON == CLOSED) */
    uint8_t dcbus_prch_rly_int_sd_rel : 1; /*!< TLB signal feedback: high voltage DC bus precharge relay closed, intentional state signal, shutdown reliant - OFF(0)/ON(!0) - (ON == CLOSED) */

    uint8_t air_neg_aux : 1; /*!< TLB signal feedback: inverted (n == not), AIR negaitve closed, auxiliary output (actual state) - OFF(0)/ON(!0) - (ON == CLOSED) */
    uint8_t air_pos_aux : 1; /*!< TLB signal feedback: inverted (n == not), AIR positive closed, auxiliary output (actual state) - OFF(0)/ON(!0) - (ON == CLOSED) */
    uint8_t dcbus_prch_rly_aux : 1; /*!< TLB signal feedback: inverted (n == not), high voltage DC bus precharge relay closed, auxiliary output (actual state) - OFF(0)/ON(!0) - (ON == CLOSED) */

    uint8_t air_neg_impl_err : 1; /*!< TLB signal feedback: AIR negative implausibility error (AIR_NEX_AUX != AIR_NEG_INT) - OFF(0)/ON(!0) - (ON == ERROR) */
    uint8_t air_pos_impl_err : 1; /*!< TLB signal feedback: AIR positive implausibility error (AIR_POS_AUX != AIR_POS_INT) - OFF(0)/ON(!0) - (ON == ERROR) */
    uint8_t dcbus_prch_rly_impl_err : 1; /*!< TLB signal feedback: high voltage DC bus precharge reelay implausibility error (DCBUS_PRCH_RLY_AUX != DCBUS_PRCH_RLY_INT) - OFF(0)/ON(!0) - (ON == ERROR) */

    uint8_t dcbus_over_60v_impl_err : 1; /*!< TLB signal feedback: high voltage DC bus over 60V implausibility error - OFF(0)/ON(!0) - (ON == ERROR) */

    uint8_t any_impl_err : 1; /*!< TLB signal feedback: any type/general-case implausibility error in TLB - OFF(0)/ON(!0) - (ON == ERROR) */
    uint8_t any_impl_err_ltch : 1; /*!< TLB signal feedback: any type/general case implausibility error in TLB, latched value - OFF(0)/ON(!0) - (ON == ERROR) */

    uint8_t tsal_green : 1; /*!< TLB signal feedback: Tractive System Active Light green status - OFF(0)/ON(!0) - (ON == ACTIVE)*/
};

union CAN_MsgUnion {
    struct CAN_tlb_batt_shtdwn_fb tlb_batt_shtdwn_fb;
    struct CAN_tlb_batt_sig_fb tlb_batt_sig_fb;
    uint8_t data[8];
};

//extern volatile struct CAN_tlb_batt_shtdwn_fb tlb_batt_shtdwn_fb_can_msg;
//extern volatile struct CAN_tlb_batt_sig_fb tlb_batt_sig_fb_can_msg;
extern union CAN_MsgUnion tlb_batt_sig_fb_can_msg;
extern union CAN_MsgUnion tlb_batt_shtdwn_fb_can_msg;

void CAN_update_tlb_batt_shtdwn_fb(struct DB_SHTDWN_FB *db_shtdwn_fb,
                                      struct CAN_tlb_batt_shtdwn_fb *tlb_batt_shtdwn_fb_can_msg);
void CAN_update_tlb_batt_sig_fb(struct DB_TLB_SIG_FB *db_tlb_sig_fb,
                                   struct CAN_tlb_batt_sig_fb *tlb_batt_sig_fb_can_msg);

void CAN_serialize_tlb_batt_shtdwn_fb(struct CAN_tlb_batt_shtdwn_fb *tlb_batt_sig_fb_can_msg, uint8_t data[8]);
void CAN_serialize_tlb_batt_sig_fb(struct CAN_tlb_batt_sig_fb *tlb_batt_sig_fb_can_msg, uint8_t data[8]);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __CAN_H__ */


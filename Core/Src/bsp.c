/**
 * @file    bsp.c
 * @author  Simone Ruffini [simone.ruffini@squadracorsepolito.com, simone.ruffini.work@gmail.com]
 * @date    Thu Jul  4 03:00:17 PM CEST 2024
 * @updated Thu Jul  4 03:00:17 PM CEST 2024
 * @prefix  varius depending on module
 *
 * @brief   Implementation of the board support package functions
 * @details This code contains the implementation of functions acting on the 
 *          specific peripherals of this board
 *
 * @license Licensed under "THE BEER-WARE LICENSE", Revision 69 
 *          see LICENSE file in the root directory of this software component
 */

/*---------- Includes -------------------------------------------------------*/

#include "bsp.h"

#include "adc.h"
#include "can.h"
#include "conf.h"
#include "mcb.h"

/* # Shared #################################################################*/

/*---------- Private define -------------------------------------------------*/

/*---------- Private macro --------------------------------------------------*/

/*---------- Private variables ----------------------------------------------*/

/*---------- Private function prototypes ------------------------------------*/

/*---------- Exported Variables ---------------------------------------------*/
volatile uint32_t ERR_flags;

/*---------- Exported Functions ---------------------------------------------*/

/*---------- Private Functions ----------------------------------------------*/

/* # SDC Sensing ############################################################*/

/*---------- Private define -------------------------------------------------*/

#define SDC_ANAL_SENS_PHYSICAL_FULL_RANGE_V \
    (24.0) /*!< The maximum physical value of the SDC read by the ADC that corresponds to it's full dynamic range */
#define SDC_ANAL_SENS_READING_GAIN \
    (SDC_ANAL_SENS_PHYSICAL_FULL_RANGE_V / ADC_ADC1_VREF_V) /*!< Gain to get physical value from ADC output */
#define SDC_ANAL_SENS_READING_OFFSET_V (0.600) /*!< SDC offset to add after conversion due to error in reading */

/*---------- Private macro --------------------------------------------------*/

/*---------- Private variables ----------------------------------------------*/

static const struct GPIO_Tuple SDC_SENS_Probe_to_GPIO_Tuple_map[SDC_SENS_Probe_NUM] = {
    [SDC_SENS_TSAC_InitialIn] = {.GPIO_Port = SDC_TSAC_INIT_IN_ADC_IN_GPIO_Port,
                                      .GPIO_Pin  = SDC_TSAC_INIT_IN_ADC_IN_Pin},
    [SDC_SENS_Post_AMS_IMD_Rly]    = {.GPIO_Port = SDC_POST_AMS_IMD_RLY_GPIO_IN_GPIO_Port,
                                      .GPIO_Pin  = SDC_POST_AMS_IMD_RLY_GPIO_IN_Pin},
    [SDC_SENS_TSAC_FinalIn]   = {.GPIO_Port = SDC_TSAC_FINAL_IN_ADC_IN_GPIO_Port,
                                      .GPIO_Pin  = SDC_TSAC_FINAL_IN_ADC_IN_Pin}
};
static const enum ADC_Channel SDC_ANAL_SENS_Probe_to_ADC_Channel_map[SDC_ANAL_SENS_Probe_NUM] =
    {[SDC_ANAL_SENS_TSAC_InitialIn] = ADC_Channel6, [SDC_ANAL_SENS_TSAC_FinalIn] = ADC_Channel8};

/*---------- Private function prototypes ------------------------------------*/

/*---------- Exported Variables ---------------------------------------------*/

//assert_param(SDC_ANAL_SENS_Probe_NUM <=32U);
uint32_t SDC_SENS_probe_values;
float SDC_ANAL_SENS_probe_values[SDC_ANAL_SENS_Probe_NUM];

/*---------- Exported Functions ---------------------------------------------*/

uint8_t SDC_SENS_Probe_sampleStatus(enum SDC_SENS_Probe probe) {
    assert_param(probe != SDC_SENS_Probe_NUM);
    GPIO_TypeDef *port = SDC_SENS_Probe_to_GPIO_Tuple_map[probe].GPIO_Port;
    uint16_t pin       = SDC_SENS_Probe_to_GPIO_Tuple_map[probe].GPIO_Pin;
    return (HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_SET) ? 1 : 0;
}

uint8_t SDC_SENS_Probe_getStatus(enum SDC_SENS_Probe probe) {
    assert_param(probe != SDC_SENS_Probe_NUM);
    return (SDC_SENS_probe_values & ((1U) << probe)) ? 1 : 0;
}

void SDC_SENS_Routine(void) {
    static uint32_t routine_tim = SDC_SENS_SAMPLING_PERIOD_MS;
    if (HAL_GetTick() >= routine_tim) {
        routine_tim = HAL_GetTick() + SDC_SENS_SAMPLING_PERIOD_MS;
        for (int i = 0; i < SDC_SENS_Probe_NUM; i++) {
            // TODO: add persistency of errors
            SDC_SENS_probe_values &= ~((uint32_t)((1U) << i));                 // reset the value
            SDC_SENS_probe_values |= ((SDC_SENS_Probe_sampleStatus(i)) << i);  // set the value
        }
    }
}

float SDC_ANAL_SENS_Probe_sampleVoltage(enum SDC_ANAL_SENS_Probe probe) {
    assert_param(probe == SDC_ANAL_SENS_Probe_NUM);
    enum ADC_Channel chnl = SDC_ANAL_SENS_Probe_to_ADC_Channel_map[probe];
    //int16_t adcVal        = ADC_ADC1_getChannelRaw(chnl);
    int16_t adcVal = ADC_ADC1_getChannelRawFiltered(chnl);
    if (!(adcVal > 0)) {
        return (-1.0f);
    }
    float physicalVal = ADC_CONV_RAW_TO_V(adcVal, ADC_GET_RESOLUTION_BITS(&hadc1), ADC_ADC1_VREF_V);
    return (physicalVal * SDC_ANAL_SENS_READING_GAIN + SDC_ANAL_SENS_READING_OFFSET_V);
}

float SDC_ANAL_SENS_Probe_getVoltage(enum SDC_ANAL_SENS_Probe probe) {
    assert_param(probe != SDC_ANAL_SENS_Probe_NUM);
    return SDC_ANAL_SENS_probe_values[probe];
}

void SDC_ANAL_SENS_Routine(void) {
    static uint32_t routine_tim = SDC_ANAL_SENS_SAMPLING_PERIOD_MS;
    if (HAL_GetTick() >= routine_tim) {
        routine_tim = HAL_GetTick() + SDC_ANAL_SENS_SAMPLING_PERIOD_MS;
        for (int i = 0; i < SDC_ANAL_SENS_Probe_NUM; i++) {
            // TODO: add a filter
            SDC_ANAL_SENS_probe_values[i] = SDC_ANAL_SENS_Probe_sampleVoltage(i);
        }
    }
}
/*---------- Private Functions ----------------------------------------------*/

/* # Internal Signals Sensing  ##############################################*/

/*---------- Private define -------------------------------------------------*/

/*---------- Private macro --------------------------------------------------*/

/*---------- Private variables ----------------------------------------------*/

static const struct GPIO_Tuple SIG_SENS_Probe_to_GPIO_Tuple_map[SIG_SENS_Probe_NUM] = {
    [SIG_SENS_AMS_Err_Latched] = {.GPIO_Port = AMS_ERR_LATCH_GPIO_IN_GPIO_Port, .GPIO_Pin = AMS_ERR_LATCH_GPIO_IN_Pin},
    [SIG_SENS_IMD_Err_Latched] = {.GPIO_Port = IMD_ERR_LATCH_GPIO_IN_GPIO_Port, .GPIO_Pin = IMD_ERR_LATCH_GPIO_IN_Pin},
    [SIG_SENS_SDCPrechRlyCmd]  = {.GPIO_Port = SDC_PRECH_CMD__INT_STATE_OPEN_GPIO_IN_GPIO_Port,
                                  .GPIO_Pin  = SDC_PRECH_CMD__INT_STATE_OPEN_GPIO_IN_Pin},
    [SIG_SENS_DCBusOver60V] = {.GPIO_Port = nDCBUS_OVER_60V_GPIO_IN_GPIO_Port, .GPIO_Pin = nDCBUS_OVER_60V_GPIO_IN_Pin},
    [SIG_SENS_TSAL_Green]   = {.GPIO_Port = TSAL_GREEN_GPIO_IN_GPIO_Port, .GPIO_Pin = TSAL_GREEN_GPIO_IN_Pin},
    [SIG_SENS_STG_on_AIR_Pos_MechStateSig] = {.GPIO_Port = nSTG_AIR_POS_GPIO_IN_GPIO_Port,
                                              .GPIO_Pin  = nSTG_AIR_POS_GPIO_IN_Pin},
    [SIG_SENS_STG_on_AIR_Neg_MechStateSig] = {.GPIO_Port = nSTG_AIR_NEG_GPIO_IN_GPIO_Port,
                                              .GPIO_Pin  = nSTG_AIR_NEG_GPIO_IN_Pin},
    [SIG_SENS_AnyImpl_Latched]             = {.GPIO_Port = ANY_IMPL_ERR_LATCH_GPIO_IN_GPIO_Port,
                                              .GPIO_Pin  = ANY_IMPL_ERR_LATCH_GPIO_IN_Pin},
    [SIG_SENS_Impl_AIRsSignals]            = {.GPIO_Port = DCBUS_RELAYS_IMPL_GPIO_IN_GPIO_Port,
                                              .GPIO_Pin  = DCBUS_RELAYS_IMPL_GPIO_IN_Pin},
    [SIG_SENS_Impl_HVRlysSignals] = {.GPIO_Port = DCBUS_IMPL_GPIO_IN_GPIO_Port, .GPIO_Pin = DCBUS_IMPL_GPIO_IN_Pin}};

static uint8_t SIG_SENS_GPIO_invert_matrix[SIG_SENS_Probe_NUM] = {[SIG_SENS_AMS_Err_Latched]             = 0U,
                                                                  [SIG_SENS_IMD_Err_Latched]             = 0U,
                                                                  [SIG_SENS_SDCPrechRlyCmd]              = 0U,
                                                                  [SIG_SENS_DCBusOver60V]                = 1U,
                                                                  [SIG_SENS_TSAL_Green]                  = 0U,
                                                                  [SIG_SENS_STG_on_AIR_Pos_MechStateSig] = 1U,
                                                                  [SIG_SENS_STG_on_AIR_Neg_MechStateSig] = 1U,
                                                                  [SIG_SENS_AnyImpl_Latched]             = 0U,
                                                                  [SIG_SENS_Impl_AIRsSignals]            = 0U,
                                                                  [SIG_SENS_Impl_HVRlysSignals]          = 0U};

/*---------- Private function prototypes ------------------------------------*/

/*---------- Exported Variables ---------------------------------------------*/
//assert_param(SIG_SENS_Probe_NUM <= 32U);
uint32_t SIG_SENS_probe_values;

/*---------- Exported Functions ---------------------------------------------*/

uint8_t SIG_SENS_Probe_sampleStatus(enum SIG_SENS_Probe probe) {
    assert_param(probe != SIG_SENS_Probe_NUM);
    GPIO_TypeDef *port = SIG_SENS_Probe_to_GPIO_Tuple_map[probe].GPIO_Port;
    uint16_t pin       = SIG_SENS_Probe_to_GPIO_Tuple_map[probe].GPIO_Pin;

    // Invert reading if values on GPIO are inverted
    GPIO_PinState compareValue = SIG_SENS_GPIO_invert_matrix[probe] ? GPIO_PIN_RESET : GPIO_PIN_SET;
    return (HAL_GPIO_ReadPin(port, pin) == compareValue) ? 1 : 0;
}

uint8_t SIG_SENS_Probe_getStatus(enum SIG_SENS_Probe probe) {
    assert_param(probe != SIG_SENS_Probe_NUM);
    return (SIG_SENS_probe_values & ((1U) << probe)) ? 1 : 0;
}

void SIG_SENS_Routine(void) {
    static uint32_t routine_tim = SIG_SENS_SAMPLING_PERIOD_MS;
    if (HAL_GetTick() >= routine_tim) {
        routine_tim = HAL_GetTick() + SIG_SENS_SAMPLING_PERIOD_MS;
        for (int i = 0; i < SIG_SENS_Probe_NUM; i++) {
            // TODO: add persistency of errors
            SIG_SENS_probe_values &= ~((uint32_t)((1U) << i));                 // reset the value
            SIG_SENS_probe_values |= ((SIG_SENS_Probe_sampleStatus(i)) << i);  // set the value
        }
    }
}
/*---------- Private Functions ----------------------------------------------*/

/* # HV Relays Sensing ######################################################*/

/*---------- Private define -------------------------------------------------*/

/*---------- Private macro --------------------------------------------------*/

/*---------- Private variables ----------------------------------------------*/

uint32_t HVRLYS_SENS_probe_values;

static const struct GPIO_Tuple HVRLYS_SENS_Probe_to_GPIO_Tuple_map[HVRLYS_SENS_Probe_NUM] = {
    [HVRLYS_SENS_AIR_Pos_Cmd]                    = {.GPIO_Port = AIR_POS__INT_STATE_CLOSED_GPIO_IN_GPIO_Port,
                                                    .GPIO_Pin  = AIR_POS__INT_STATE_CLOSED_GPIO_IN_Pin},
    [HVRLYS_SENS_AIR_Pos_MechStateClosed]        = {.GPIO_Port = AIR_POS__MECH_STATE_OPEN_GPIO_IN_GPIO_Port,
                                                    .GPIO_Pin  = AIR_POS__MECH_STATE_OPEN_GPIO_IN_Pin},
    [HVRLYS_SENS_AIR_Neg_Cmd]                    = {.GPIO_Port = AIR_NEG__INT_STATE_CLOSED_GPIO_IN_GPIO_Port,
                                                    .GPIO_Pin  = AIR_NEG__INT_STATE_CLOSED_GPIO_IN_Pin},
    [HVRLYS_SENS_AIR_Neg_MechStateClosed]        = {.GPIO_Port = AIR_NEG__MECH_STATE_OPEN_3V3_GPIO_Port,
                                                    .GPIO_Pin  = AIR_NEG__MECH_STATE_OPEN_3V3_Pin},
    [HVRLYS_SENS_DCBus_PrechRly_Cmd]             = {.GPIO_Port = DCBUS_PRECH__INT_STATE_CLOSED_GPIO_IN_GPIO_Port,
                                                    .GPIO_Pin  = DCBUS_PRECH__INT_STATE_CLOSED_GPIO_IN_Pin},
    [HVRLYS_SENS_DCBus_PrechRly_MechStateClosed] = {.GPIO_Port = DCBUS_PRECH__MECH_STATE_OPEN_GPIO_IN_GPIO_Port,
                                                    .GPIO_Pin  = DCBUS_PRECH__MECH_STATE_OPEN_GPIO_IN_Pin},
};

static uint8_t HVRLYS_SENS_GPIO_invert_matrix[SIG_SENS_Probe_NUM] = {[HVRLYS_SENS_AIR_Pos_Cmd]                    = 0U,
                                                                     [HVRLYS_SENS_AIR_Pos_MechStateClosed]        = 1U,
                                                                     [HVRLYS_SENS_AIR_Neg_Cmd]                    = 0U,
                                                                     [HVRLYS_SENS_AIR_Neg_MechStateClosed]        = 1U,
                                                                     [HVRLYS_SENS_DCBus_PrechRly_Cmd]             = 0U,
                                                                     [HVRLYS_SENS_DCBus_PrechRly_MechStateClosed] = 1U};
/*---------- Private function prototypes ------------------------------------*/

/*---------- Exported Variables ---------------------------------------------*/

/*---------- Exported Functions ---------------------------------------------*/

uint8_t HVRLYS_SENS_Probe_sampleStatus(enum HVRLYS_SENS_Probe probe) {
    if (probe == HVRLYS_SENS_Probe_NUM)
        return -1;
    GPIO_TypeDef *port = HVRLYS_SENS_Probe_to_GPIO_Tuple_map[probe].GPIO_Port;
    uint16_t pin       = HVRLYS_SENS_Probe_to_GPIO_Tuple_map[probe].GPIO_Pin;

    // Invert reading if values on GPIO are inverted
    GPIO_PinState compareValue = HVRLYS_SENS_GPIO_invert_matrix[probe] ? GPIO_PIN_RESET : GPIO_PIN_SET;
    return HAL_GPIO_ReadPin(port, pin) == compareValue ? 1 : 0;
}
uint8_t HVRLYS_SENS_Probe_getStatus(enum HVRLYS_SENS_Probe probe) {
    assert_param(probe != HVRLYS_SENS_Probe_NUM);
    return (HVRLYS_SENS_probe_values & ((1U) << probe)) ? 1 : 0;
}

void HVRLYS_SENS_Routine(void) {
    static uint32_t routine_tim = HVRLYS_SENS_SAMPLING_PERIOD_MS;
    if (HAL_GetTick() >= routine_tim) {
        routine_tim = HAL_GetTick() + HVRLYS_SENS_SAMPLING_PERIOD_MS;
        for (int i = 0; i < HVRLYS_SENS_Probe_NUM; i++) {
            // TODO: add persistency of errors
            HVRLYS_SENS_probe_values &= ~((uint32_t)((1U) << i));                    // reset the value
            HVRLYS_SENS_probe_values |= ((HVRLYS_SENS_Probe_sampleStatus(i)) << i);  // set the value
        }
    }
}

/*---------- Private Functions ----------------------------------------------*/

/* # Logging ################################################################*/

/*---------- Private define -------------------------------------------------*/

/*---------- Private macro --------------------------------------------------*/

/*---------- Private variables ----------------------------------------------*/

/*---------- Private function prototypes ------------------------------------*/

/*---------- Exported Variables ---------------------------------------------*/

/*---------- Exported Functions ---------------------------------------------*/

/*---------- Private Functions ----------------------------------------------*/

/* # Main CAN Bus Communication #############################################*/

/*---------- Private define -------------------------------------------------*/

/*---------- Private macro --------------------------------------------------*/

/*---------- Private variables ----------------------------------------------*/

/*---------- Private function prototypes ------------------------------------*/

/*---------- Exported Variables ---------------------------------------------*/

/*---------- Exported Functions ---------------------------------------------*/
void MCB_send_msg(uint32_t id) {
    uint8_t buffer[8] = {0};

    union {
        struct mcb_tlb_battery_tsal_status_t tlb_battery_tsal_status;
        struct mcb_tlb_battery_shut_status_t tlb_battery_shut_status;
        //struct mcb_tlb_battery_helo_t tlb_battery_helo;
    } msg;

    CAN_CAN1_tx_header.StdId = id;

    switch (id) {
        case MCB_TLB_BATTERY_TSAL_STATUS_FRAME_ID:

            // clang-format off
            //assert_param(mcb_tlb_battery_tsal_status_tsal_is_green_on_is_in_range(SIG_SENS_Probe_getStatus(SIG_SENS_TSAL_Green)));
            //assert_param(mcb_tlb_battery_tsal_status_scs_short2_gnd_air_pos_is_in_range(SIG_SENS_Probe_getStatus(SIG_SENS_STG_on_AIR_Pos_MechStateSig)));
            //assert_param(mcb_tlb_battery_tsal_status_scs_is_any_short2_gnd_present_is_in_range(DB_data.shrt2gnd_airs & 0b1U));
            //assert_param(mcb_tlb_battery_tsal_status_tsal_is_dc_bus_over60_v_is_in_range(DB_data.dcbus_over_60v & 0b1U));
            //assert_param(mcb_tlb_battery_tsal_status_intentional_state_air_neg_is_in_range(DB_data.air_neg_int_sd_rel & 0b1U));
            //assert_param(mcb_tlb_battery_tsal_status_intentional_state_air_pos_is_in_range(DB_data.air_pos_int_sd_rel & 0b1U));
            //assert_param(mcb_tlb_battery_tsal_status_intentional_state_relay_precharge_is_in_range(DB_data.dcbus_prch_rly_int_sd_rel & 0b1U));
            //assert_param(mcb_tlb_battery_tsal_status_tsal_is_air_neg_closed_is_in_range(DB_data.air_neg_aux & 0b1U));
            //assert_param(mcb_tlb_battery_tsal_status_tsal_is_air_pos_closed_is_in_range(DB_data.air_pos_aux & 0b1U));
            //assert_param(mcb_tlb_battery_tsal_status_tsal_is_relay_precharge_closed_is_in_range(DB_data.dcbus_prch_rly_aux & 0b1U));
            //assert_param(mcb_tlb_battery_tsal_status_imp_is_air_neg_imp_present_is_in_range(DB_data.air_neg_impl_err & 0b1U));
            //assert_param(mcb_tlb_battery_tsal_status_imp_is_air_pos_imp_present_is_in_range(DB_data.air_pos_impl_err & 0b1U));
            //assert_param(mcb_tlb_battery_tsal_status_imp_is_relay_precharge_imp_present_is_in_range(DB_data.dcbus_prch_rly_impl_err & 0b1U));
            //assert_param(mcb_tlb_battery_tsal_status_imp_is_dc_bus_voltage_imp_present_is_in_range(DB_data.dcbus_over_60v_impl_err & 0b1U));
            //assert_param(mcb_tlb_battery_tsal_status_imp_is_any_imp_present_is_in_range(DB_data.any_impl_err & 0b1U));
            //assert_param(mcb_tlb_battery_tsal_status_imp_is_any_imp_latched_is_in_range(DB_data.any_impl_err_ltch & 0b1U));
            //assert_param(mcb_tlb_battery_tsal_status_tsal_is_green_on_is_in_range(DB_data.tsal_green & 0b1U));

            msg.tlb_battery_tsal_status.scs_short2_gnd_air_neg             = mcb_tlb_battery_tsal_status_scs_short2_gnd_air_neg_encode(SIG_SENS_Probe_getStatus(SIG_SENS_STG_on_AIR_Neg_MechStateSig) & 0b1U);
            msg.tlb_battery_tsal_status.scs_short2_gnd_air_pos             = mcb_tlb_battery_tsal_status_scs_short2_gnd_air_pos_encode(SIG_SENS_Probe_getStatus(SIG_SENS_STG_on_AIR_Pos_MechStateSig) & 0b1U);
            msg.tlb_battery_tsal_status.scs_is_any_short2_gnd_present      = mcb_tlb_battery_tsal_status_scs_is_any_short2_gnd_present_encode((SIG_SENS_Probe_getStatus(SIG_SENS_STG_on_AIR_Pos_MechStateSig) & 0b1U) | (SIG_SENS_Probe_getStatus(SIG_SENS_STG_on_AIR_Neg_MechStateSig) & 0b1U));
            msg.tlb_battery_tsal_status.tsal_is_dc_bus_over60_v            = mcb_tlb_battery_tsal_status_tsal_is_dc_bus_over60_v_encode(SIG_SENS_Probe_getStatus(SIG_SENS_DCBusOver60V) & 0b1U);
            msg.tlb_battery_tsal_status.intentional_state_air_neg          = mcb_tlb_battery_tsal_status_intentional_state_air_neg_encode(HVRLYS_SENS_Probe_getStatus(HVRLYS_SENS_AIR_Neg_Cmd) & 0b1U);
            msg.tlb_battery_tsal_status.intentional_state_air_pos          = mcb_tlb_battery_tsal_status_intentional_state_air_pos_encode(HVRLYS_SENS_Probe_getStatus(HVRLYS_SENS_AIR_Pos_Cmd) & 0b1U);
            msg.tlb_battery_tsal_status.intentional_state_relay_precharge  = mcb_tlb_battery_tsal_status_intentional_state_relay_precharge_encode(HVRLYS_SENS_Probe_getStatus(HVRLYS_SENS_DCBus_PrechRly_Cmd) & 0b1U);
            msg.tlb_battery_tsal_status.tsal_is_air_neg_closed             = mcb_tlb_battery_tsal_status_tsal_is_air_neg_closed_encode(HVRLYS_SENS_Probe_getStatus(HVRLYS_SENS_AIR_Neg_MechStateClosed) & 0b1U);
            msg.tlb_battery_tsal_status.tsal_is_air_pos_closed             = mcb_tlb_battery_tsal_status_tsal_is_air_pos_closed_encode(HVRLYS_SENS_Probe_getStatus(HVRLYS_SENS_AIR_Pos_MechStateClosed) & 0b1U);
            msg.tlb_battery_tsal_status.tsal_is_relay_precharge_closed     = mcb_tlb_battery_tsal_status_tsal_is_relay_precharge_closed_encode(HVRLYS_SENS_Probe_getStatus(HVRLYS_SENS_DCBus_PrechRly_MechStateClosed) & 0b1U);
            msg.tlb_battery_tsal_status.imp_is_air_neg_imp_present         = mcb_tlb_battery_tsal_status_imp_is_air_neg_imp_present_encode(SIG_SENS_Probe_getStatus(SIG_SENS_Impl_AIRsSignals) & 0b1U);
            msg.tlb_battery_tsal_status.imp_is_air_pos_imp_present         = mcb_tlb_battery_tsal_status_imp_is_air_pos_imp_present_encode(SIG_SENS_Probe_getStatus(SIG_SENS_Impl_AIRsSignals) & 0b1U);
            msg.tlb_battery_tsal_status.imp_is_relay_precharge_imp_present = mcb_tlb_battery_tsal_status_imp_is_relay_precharge_imp_present_encode(SIG_SENS_Probe_getStatus(SIG_SENS_Impl_HVRlysSignals) & 0b1U);
            msg.tlb_battery_tsal_status.imp_is_dc_bus_voltage_imp_present  = mcb_tlb_battery_tsal_status_imp_is_dc_bus_voltage_imp_present_encode(0 & 0b1U);
            msg.tlb_battery_tsal_status.imp_is_any_imp_present             = mcb_tlb_battery_tsal_status_imp_is_any_imp_present_encode(0 & 0b1U);
            msg.tlb_battery_tsal_status.imp_is_any_imp_latched             = mcb_tlb_battery_tsal_status_imp_is_any_imp_latched_encode(SIG_SENS_Probe_getStatus(SIG_SENS_AnyImpl_Latched) & 0b1U);
            msg.tlb_battery_tsal_status.tsal_is_green_on                   = mcb_tlb_battery_tsal_status_tsal_is_green_on_encode(SIG_SENS_Probe_getStatus(SIG_SENS_TSAL_Green) & 0b1U);
            // clang-format on

            CAN_CAN1_tx_header.DLC = mcb_tlb_battery_tsal_status_pack(buffer, &msg.tlb_battery_tsal_status, 8);
            break;
        case MCB_TLB_BATTERY_SHUT_STATUS_FRAME_ID:

            // clang-format off
            //assert_param(mcb_tlb_battery_shut_status_is_shut_closed_pre_ams_imd_latch_is_in_range(DB_data.sd_mid_in_to_ams_err_rly & 0b1U));
            //assert_param(mcb_tlb_battery_shut_status_is_shut_closed_post_ams_latch_is_in_range(DB_data.ams_err_rly_to_imd_err_rly & 0b1U));
            //assert_param(mcb_tlb_battery_shut_status_is_shut_closed_post_imd_latch_is_in_range(DB_data.imd_err_rly_to_sd_prch_rly & 0b1U));
            //assert_param(mcb_tlb_battery_shut_status_is_shutdown_closed_pre_tlb_batt_final_is_in_range(DB_data.sd_fnl_in_to_sd_dly_caps & 0b1U));
            //assert_param(mcb_tlb_battery_shut_status_is_ams_error_latched_is_in_range(DB_data.ams_err & 0b1U));
            //assert_param(mcb_tlb_battery_shut_status_is_imd_error_latched_is_in_range(DB_data.imd_err & 0b1U));
            //assert_param(mcb_tlb_battery_shut_status_is_sd_prch_rly_closed_is_in_range(DB_data.sd_prch_rly & 0b1U));
            //assert_param(mcb_tlb_battery_shut_status_shutdown_adc_post_sd_precharge_relay_is_in_range(DB_data.sd_prch_rly_to_sd_mid_out_V));
            //assert_param(mcb_tlb_battery_shut_status_shutdown_adc_ai_rs_opening_delay_caps_is_in_range(DB_data.sd_dly_caps_to_sd_fin_out_airs_V));

            msg.tlb_battery_shut_status.is_shut_closed_pre_ams_imd_latch      = mcb_tlb_battery_shut_status_is_shut_closed_pre_ams_imd_latch_encode(SDC_SENS_Probe_getStatus(SDC_SENS_TSAC_InitialIn) & 0b1U);
            msg.tlb_battery_shut_status.is_shut_closed_post_ams_latch         = mcb_tlb_battery_shut_status_is_shut_closed_post_ams_latch_encode(SDC_SENS_Probe_getStatus(SDC_SENS_Post_AMS_IMD_Rly) & 0b1U);
            msg.tlb_battery_shut_status.is_shut_closed_post_imd_latch         = mcb_tlb_battery_shut_status_is_shut_closed_post_imd_latch_encode(SDC_SENS_Probe_getStatus(SDC_SENS_Post_AMS_IMD_Rly) & 0b1U);
            msg.tlb_battery_shut_status.is_shutdown_closed_pre_tlb_batt_final = mcb_tlb_battery_shut_status_is_shutdown_closed_pre_tlb_batt_final_encode(SDC_SENS_Probe_getStatus(SDC_SENS_TSAC_FinalIn) & 0b1U);
            msg.tlb_battery_shut_status.is_ams_error_latched                  = mcb_tlb_battery_shut_status_is_ams_error_latched_encode(SIG_SENS_Probe_getStatus(SIG_SENS_AMS_Err_Latched) & 0b1U);
            msg.tlb_battery_shut_status.is_imd_error_latched                  = mcb_tlb_battery_shut_status_is_imd_error_latched_encode(SIG_SENS_Probe_getStatus(SIG_SENS_IMD_Err_Latched) & 0b1U);
            msg.tlb_battery_shut_status.is_sd_prch_rly_closed                 = mcb_tlb_battery_shut_status_is_sd_prch_rly_closed_encode(SIG_SENS_Probe_getStatus(SIG_SENS_SDCPrechRlyCmd) & 0b1U);
            msg.tlb_battery_shut_status.shutdown_adc_post_sd_precharge_relay  = mcb_tlb_battery_shut_status_shutdown_adc_post_sd_precharge_relay_encode(SDC_ANAL_SENS_Probe_getVoltage(SDC_ANAL_SENS_TSAC_InitialIn));
            msg.tlb_battery_shut_status.shutdown_adc_ai_rs_opening_delay_caps = mcb_tlb_battery_shut_status_shutdown_adc_ai_rs_opening_delay_caps_encode(SDC_ANAL_SENS_Probe_getVoltage(SDC_ANAL_SENS_TSAC_FinalIn));
            // clang-format on

            CAN_CAN1_tx_header.DLC = mcb_tlb_battery_shut_status_pack(buffer, &msg.tlb_battery_shut_status, 8);
            break;
        case MCB_TLB_BATTERY_HELO:

            buffer[0] = VERSION_MAJOR;
            buffer[1] = VERSION_MINOR;
            buffer[2] = VERSION_PATCH;

            CAN_CAN1_tx_header.DLC = 3;
            break;
        default:
            return;
    };
    // TODO: check for HAL_TIMEOUT
    CAN_send(&MCB_Handle, buffer, &CAN_CAN1_tx_header);
}

void MCB_SendMessagesRoutine(void) {
    static volatile uint32_t routine_1ms_tim   = 0U;
    static volatile uint32_t routine_10ms_tim  = 0U;
    static volatile uint32_t routine_100ms_tim = 0U;

    // 100 ms messages
    //if (HAL_GetTick() >= routine_100ms_tim) {
    //    routine_100ms_tim = HAL_GetTick() + 100U;
    //}

    // 10ms messages
    if (HAL_GetTick() >= routine_10ms_tim) {
        routine_10ms_tim = HAL_GetTick() + 10U;
        MCB_send_msg(MCB_TLB_BATTERY_TSAL_STATUS_FRAME_ID);
        MCB_send_msg(MCB_TLB_BATTERY_SHUT_STATUS_FRAME_ID);
    }

    //if (HAL_GetTick() >= routine_1ms_tim) {
    //    routine_1ms_tim = HAL_GetTick() + 1U;
    //}
}

/*---------- Private Functions ----------------------------------------------*/

/* # Status Leds  ###########################################################*/

/*---------- Private define -------------------------------------------------*/

/*---------- Private macro --------------------------------------------------*/

/*---------- Private variables ----------------------------------------------*/
static const struct GPIO_Tuple STAT_LED_Device_to_GPIO_Tuple_map[STAT_LED_Device_NUM] = {
    [STAT_LED_KEEP_ALIVE] = {.GPIO_Port = STAT1_LED_GPIO_OUT_GPIO_Port, .GPIO_Pin = STAT1_LED_GPIO_OUT_Pin},
    [STAT_LED_USER1]      = {.GPIO_Port = STAT2_LED_GPIO_OUT_GPIO_Port, .GPIO_Pin = STAT2_LED_GPIO_OUT_Pin},
    [STAT_LED_USER2]      = {.GPIO_Port = STAT3_LED_GPIO_OUT_GPIO_Port, .GPIO_Pin = STAT3_LED_GPIO_OUT_Pin},
    [STAT_LED_WARN]       = {.GPIO_Port = WARN_LED_GPIO_OUT_GPIO_Port, .GPIO_Pin = WARN_LED_GPIO_OUT_Pin},
    [STAT_LED_ERR]        = {.GPIO_Port = ERR_LED_GPIO_OUT_GPIO_Port, .GPIO_Pin = ERR_LED_GPIO_OUT_Pin}};

/*---------- Private function prototypes ------------------------------------*/

/*---------- Exported Variables ---------------------------------------------*/

/*---------- Exported Functions ---------------------------------------------*/

void STAT_LED_toggle(enum STAT_LED_Device device) {
    assert_param(device != STAT_LED_Device_NUM);
    GPIO_TypeDef *port = STAT_LED_Device_to_GPIO_Tuple_map[device].GPIO_Port;
    uint16_t pin       = STAT_LED_Device_to_GPIO_Tuple_map[device].GPIO_Pin;
    HAL_GPIO_TogglePin(port, pin);
}

void STAT_LED_enable(enum STAT_LED_Device device) {
    assert_param(device != STAT_LED_Device_NUM);
    GPIO_TypeDef *port = STAT_LED_Device_to_GPIO_Tuple_map[device].GPIO_Port;
    uint16_t pin       = STAT_LED_Device_to_GPIO_Tuple_map[device].GPIO_Pin;
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
}
void STAT_LED_disable(enum STAT_LED_Device device) {
    assert_param(device != STAT_LED_Device_NUM);
    GPIO_TypeDef *port = STAT_LED_Device_to_GPIO_Tuple_map[device].GPIO_Port;
    uint16_t pin       = STAT_LED_Device_to_GPIO_Tuple_map[device].GPIO_Pin;
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
}
void STAT_LED_Routine(void) {
    static uint32_t keep_alive_blink_tim      = STAT_LED_KEEP_ALIVE_BLINK_PERIOD_MS;
    static uint32_t show_error_with_delay_tim = 0;
    if (HAL_GetTick() >= keep_alive_blink_tim) {
        keep_alive_blink_tim = HAL_GetTick() + STAT_LED_KEEP_ALIVE_BLINK_PERIOD_MS;
        STAT_LED_toggle(STAT_LED_KEEP_ALIVE);
    }
    if (ERR_flags) {
        show_error_with_delay_tim = HAL_GetTick() + STAT_LED_ERR_PERSISTENCY_MS;
        STAT_LED_enable(STAT_LED_ERR);
    } else {
        if (HAL_GetTick() >= show_error_with_delay_tim) {
            STAT_LED_disable(STAT_LED_ERR);
        }
    }
}

/*---------- Private Functions ----------------------------------------------*/

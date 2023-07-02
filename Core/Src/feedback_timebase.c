/**
 * @file    feedback_timabase.c
 * @prefix  FBTMBS
 * @author  Simone Ruffini [simone.ruffini.work@gmail.com | simone.ruffini@squadracorse.com]
 * @date    Tue May 23 03:33:07 PM CEST 2023
 *
 * @brief Source file for feedback reading using timebase library of stmlibs
 *
 * @license Licensed under "THE BEER-WARE LICENSE", Revision 69 (see LICENSE)
 */

/* Includes ------------------------------------------------------------------*/
#include "feedback_timebase.h"

#include "adc.h"
#include "db.h"
#include "main.h"
#include "tim.h"

#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/*!< Base interval of the timebase: 1000us = 1kHz
 @note timer must generate a 1kHz pulse) */
#define __FBTMBS_BASE_INTERVAL_US (1000U)

/*!< Use basic timer7 as the generator of the pulse (must be initialized beforehand) */
#define __FBTMBS_HTIM htim6

/*!< Feedback signals sampling = 100'000us = 10Hz */
#define __FBTMBS_READ_FB_INTERVAL_US ((100U) * __FBTMBS_BASE_INTERVAL_US)

/* Private macro -------------------------------------------------------------*/
/**
* @brief Get a decimal representation of the state of a GPIO
* @param __GPIO_BASE_NAME__ The base name of a GPIO i.e. the one found in main.h but without _Pin or _GPIO_Port
* @retval decimal representation of the GPIO: 0U if OFF, MAX_UINT if ON
*/
#define __GPIO_READPIN_DECIMAL(__GPIO_BASE_NAME__) \
    (HAL_GPIO_ReadPin(__GPIO_BASE_NAME__##_GPIO_Port, __GPIO_BASE_NAME__##_Pin) == GPIO_PIN_SET ? (-1U) : (0U))

/* Private variables ---------------------------------------------------------*/
TIMEBASE_HandleTypeDef hfbtmbs = {0}; /*!< feedback timebase handle */

/* Private function prototypes -----------------------------------------------*/
STMLIBS_StatusTypeDef _sample_shutdown_fb(void);
STMLIBS_StatusTypeDef _sample_tlb_signals_fb(void);
STMLIBS_StatusTypeDef _sample_fb(void);

/* Exported functions --------------------------------------------------------*/
void FBTMBS_init(void) {
    uint8_t timebase_ref_idx;

    // link the handle to the timer and initialize the period correctly
    TIMEBASE_init(&hfbtmbs, &__FBTMBS_HTIM, __FBTMBS_BASE_INTERVAL_US);

    // Generate a timebase and use the returned referecne index to associate a callback
    TIMEBASE_add_interval(&hfbtmbs, __FBTMBS_READ_FB_INTERVAL_US, &timebase_ref_idx);
    TIMEBASE_register_callback(&hfbtmbs, timebase_ref_idx, &_sample_fb);
}

void FBTMBS_routines_runner(void) {
    TIMEBASE_routine(&hfbtmbs);
}

/* Private functions ---------------------------------------------------------*/

/**
* @brief  Sample shutdown feedback signals and store in the db
* @retval return status: - STMLIBS_OK if procedure completed without errors
*                        - STMLIBS_ERROR otherwise
*/
STMLIBS_StatusTypeDef _sample_shutdown_fb(void) {
    // clang-format off
    DB_data.sd_mid_in_to_ams_err_rly         = __GPIO_READPIN_DECIMAL(SD_FB_SD_MID_IN_TO_AMS_ERR_RLY_GPIO_IN);
    DB_data.ams_err_rly_to_imd_err_rly       = __GPIO_READPIN_DECIMAL(SD_FB_AMS_ERR_RLY_TO_IMD_ERR_RLY_GPIO_IN);
    DB_data.imd_err_rly_to_sd_prch_rly       = __GPIO_READPIN_DECIMAL(SD_FB_IMD_ERR_RLY_TO_SD_PRCH_RLY_GPIO_IN);
    DB_data.sd_prch_rly_to_sd_mid_out_V      = ADC_Get_Filtered_Real(&SD_FB_ADC_Handle, SD_FB_SD_PRCH_RLY_TO_SD_MID_OUT_ADC1_IN_ADC_CHNL) / 1000.0;
    DB_data.sd_fnl_in_to_sd_dly_caps         = __GPIO_READPIN_DECIMAL(SD_FB_SD_FNL_IN_TO_SD_DLY_CAPS_GPIO_IN);
    DB_data.sd_dly_caps_to_sd_fin_out_airs_V = ADC_Get_Filtered_Real(&SD_FB_ADC_Handle, SD_FB_SD_DLY_CAPS_TO_SD_FIN_OUT_AIRS_ADC1_IN_ADC_CHNL) / 1000.0;
    // clang-format on
    return STMLIBS_OK;
}
STMLIBS_StatusTypeDef _sample_tlb_signals_fb(void) {
    DB_data.ams_err     = __GPIO_READPIN_DECIMAL(FB_AMS_ERR_GPIO_IN);
    DB_data.imd_err     = __GPIO_READPIN_DECIMAL(FB_IMD_ERR_GPIO_IN);
    DB_data.sd_prch_rly = __GPIO_READPIN_DECIMAL(FB_SD_PRCH_RLY_GPIO_IN);

    DB_data.shrt2gnd_air_neg = __GPIO_READPIN_DECIMAL(FB_SHRT2GND_AIR_NEG_GPIO_IN);
    DB_data.shrt2gnd_air_pos = __GPIO_READPIN_DECIMAL(FB_SHRT2GND_AIR_POS_GPIO_IN);
    DB_data.shrt2gnd_airs    = ~__GPIO_READPIN_DECIMAL(FB_nSHRT2GND_AIRS_GPIO_IN);

    DB_data.dcbus_over_60v = ~__GPIO_READPIN_DECIMAL(FB_nDCBUS_OVER_60V_GPIO_IN);

    DB_data.air_neg_int_sd_rel        = __GPIO_READPIN_DECIMAL(FB_AIR_NEG_INT_SD_REL_GPIO_IN);
    DB_data.air_pos_int_sd_rel        = __GPIO_READPIN_DECIMAL(FB_AIR_POS_INT_SD_REL_GPIO_IN);
    DB_data.dcbus_prch_rly_int_sd_rel = __GPIO_READPIN_DECIMAL(FB_DCBUS_PRCH_RLY_INT_SD_REL_GPIO_IN);

    DB_data.air_neg_aux        = ~__GPIO_READPIN_DECIMAL(FB_nAIR_NEG_AUX_GPIO_IN);
    DB_data.air_pos_aux        = ~__GPIO_READPIN_DECIMAL(FB_nAIR_POS_AUX_GPIO_IN);
    DB_data.dcbus_prch_rly_aux = ~__GPIO_READPIN_DECIMAL(FB_nDCBUS_PRCH_RLY_AUX_GPIO_IN);

    DB_data.air_neg_impl_err        = __GPIO_READPIN_DECIMAL(FB_AIR_NEG_IMPL_ERR_GPIO_IN);
    DB_data.air_pos_impl_err        = __GPIO_READPIN_DECIMAL(FB_AIR_POS_IMPL_ERR_GPIO_IN);
    DB_data.dcbus_prch_rly_impl_err = __GPIO_READPIN_DECIMAL(FB_DCBUS_PRCH_RLY_IMPL_ERR_GPIO_IN);

    DB_data.dcbus_over_60v_impl_err = __GPIO_READPIN_DECIMAL(FB_DCBUS_OVER_60V_IMPL_ERR_GPIO_IN);

    DB_data.any_impl_err      = ~__GPIO_READPIN_DECIMAL(FB_nANY_IMPL_ERR_GPIO_IN);
    DB_data.any_impl_err_ltch = __GPIO_READPIN_DECIMAL(FB_ANY_IMPL_ERR_LTCH_GPIO_IN);

    DB_data.tsal_green = __GPIO_READPIN_DECIMAL(FB_TSAL_GREEN_GPIO_IN);

    return STMLIBS_OK;
}

/**
* @brief  Sample all feedback signals and store sampled values 
* @retval return status: - STMLIBS_OK if procedure completed without errors
*                        - STMLIBS_ERROR otherwise
*/
STMLIBS_StatusTypeDef _sample_fb(void) {
    STMLIBS_StatusTypeDef ret = STMLIBS_OK;
    ret                       = _sample_shutdown_fb();
    // TODO implement some sort of error checking and logging
    ret = _sample_tlb_signals_fb();
    return ret;
}

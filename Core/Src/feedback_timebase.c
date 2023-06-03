/**
 * @file feedback_timabase.c
 * @prefix FBTMBS
 * @author Simone Ruffini [simone.ruffini.work@gmail.com]
 * @date Tue May 23 03:33:07 PM CEST 2023
 * @brief Source file for feedback reading using timebase library of stmlibs
 *
 * @license Licensed under "THE BEER-WARE LICENSE", Revision 69 (see LICENSE)
 */

/* Includes ------------------------------------------------------------------*/
#include "feedback_timebase.h"

#include "db.h"
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
static STMLIBS_StatusTypeDef sample_shutdown_fb(void);
static STMLIBS_StatusTypeDef sample_fb(void);

/* Exported functions --------------------------------------------------------*/
void FBTMBS_init(void) {
    uint8_t timebase_ref_idx;

    // link the handle to the timer and initialize the period correctly
    TIMEBASE_init(&hfbtmbs, &__FBTMBS_HTIM, __FBTMBS_BASE_INTERVAL_US);

    // Generate a timebase and use the returned referecne index to associate a callback
    TIMEBASE_add_interval(&hfbtmbs, __FBTMBS_READ_FB_INTERVAL_US, &timebase_ref_idx);
    TIMEBASE_register_callback(&hfbtmbs, timebase_ref_idx, &sample_fb);
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
static STMLIBS_StatusTypeDef sample_shutdown_fb(void) {
    db_shtdwn_fb.sd_mid_in_to_ams_err_rly       = __GPIO_READPIN_DECIMAL(SD_FB_SD_MID_IN_TO_AMS_ERR_RLY_GPIO_IN);
    db_shtdwn_fb.ams_err_rly_to_imd_err_rly     = __GPIO_READPIN_DECIMAL(SD_FB_AMS_ERR_RLY_TO_IMD_ERR_RLY_GPIO_IN);
    db_shtdwn_fb.imd_err_rly_to_sd_prch_rly     = __GPIO_READPIN_DECIMAL(SD_FB_IMD_ERR_RLY_TO_SD_PRCH_RLY_GPIO_IN);
    db_shtdwn_fb.sd_prch_rly_to_sd_mid_out      = 0;  // TODO: add adc reading
    db_shtdwn_fb.sd_fnl_in_to_sd_dly_caps       = __GPIO_READPIN_DECIMAL(SD_FB_SD_FNL_IN_TO_SD_DLY_CAPS_GPIO_IN);
    db_shtdwn_fb.sd_dly_caps_to_sd_fin_out_airs = 0;  // TODO: add adc reading
    db_shtdwn_fb.sample_tick                    = HAL_GetTick();
    return STMLIBS_OK;
}
/**
* @brief  Sample all feedback signals and store sampled values 
* @retval return status: - STMLIBS_OK if procedure completed without errors
*                        - STMLIBS_ERROR otherwise
*/
static STMLIBS_StatusTypeDef sample_fb(void) {
    STMLIBS_StatusTypeDef ret = STMLIBS_OK;
    ret                       = sample_shutdown_fb();
    // TODO implement some sort of error checking and logging
    return ret;
}

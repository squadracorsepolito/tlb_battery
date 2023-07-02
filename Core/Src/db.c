/**
 * @file db.c
 * @prefix DB
 * @author Simone Ruffini [simone.ruffini.work@gmail.com]
 * @date Tue May 23 03:33:07 PM CEST 2023
 * 
 * @brief     
 * 
 * @license Licensed under "THE BEER-WARE LICENSE", Revision 69 (see LICENSE)
 */

/* Includes ------------------------------------------------------------------*/
#include "db.h"

#include "stdio.h"
#include "stm32f4xx_hal.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported variables --------------------------------------------------------*/

struct DB_data_t DB_data = {
    .shtdwn_status_fb_csv_head_str =
        "sd_mid_in_to_ams_err_rly,ams_err_rly_to_imd_err_rly,imd_err_rly_to_sd_prch_rly,"
        "sd_prch_rly_to_sd_mid_out_V,sd_fnl_in_to_sd_dly_caps,sd_dly_caps_to_sd_fin_out_airs_V,"
        "ams_err,imd_err,sd_prch_rly",
    .tlb_intrnl_sig_fb_csv_head_str = "shrt2gnd_air_neg,shrt2gnd_air_pos,shrt2gnd_airs,"
                                      "dcbus_over_60v,"
                                      "air_neg_int_sd_rel,air_pos_int_sd_rel,dcbus_prch_rly_int_sd_rel,"
                                      "air_neg_aux,air_pos_aux,dcbus_prch_rly_aux,"
                                      "air_neg_impl_err,air_pos_impl_err,dcbus_prch_rly_impl_err,"
                                      "dcbus_over_60v_impl_err,"
                                      "any_impl_err,any_impl_err_ltch,"
                                      "tsal_green"};

/* Exported functions --------------------------------------------------------*/
void DB_shtdwn_status_fb_ToStringCSV(struct DB_data_t* DB_data, char buf[static 256]){
    assert_param(DB_data);
    assert_param(buf);
    sprintf(buf,
            "%4u,%4u,%4u,%4.1f,%4u,%4.1f",
            DB_data->sd_mid_in_to_ams_err_rly,
            DB_data->ams_err_rly_to_imd_err_rly,
            DB_data->imd_err_rly_to_sd_prch_rly,
            DB_data->sd_prch_rly_to_sd_mid_out_V,
            DB_data->sd_fnl_in_to_sd_dly_caps,
            DB_data->sd_dly_caps_to_sd_fin_out_airs_V);
};

void DB_tlb_intrnl_sig_fb_ToStringCSV(struct DB_data_t* DB_data, char buf[static 256]){
    assert_param(DB_data);
    assert_param(buf);

    sprintf(buf,
            "%4u,%4u,%4u,"

            "%4u,%4u,%4u,"

            "%4u,"

            "%4u,%4u,%4u,"

            "%4u,%4u,%4u,"

            "%4u,%4u,%4u,"

            "%4u,"

            "%4u,%4u,"

            "%4u",

            DB_data->ams_err,
            DB_data->imd_err,
            DB_data->sd_prch_rly,

            DB_data->shrt2gnd_air_neg,
            DB_data->shrt2gnd_air_pos,
            DB_data->shrt2gnd_airs,

            DB_data->dcbus_over_60v,

            DB_data->air_neg_int_sd_rel,
            DB_data->air_pos_int_sd_rel,
            DB_data->dcbus_prch_rly_int_sd_rel,

            DB_data->air_neg_aux,
            DB_data->air_pos_aux,
            DB_data->dcbus_prch_rly_aux,

            DB_data->air_neg_impl_err,
            DB_data->air_pos_impl_err,
            DB_data->dcbus_prch_rly_impl_err,

            DB_data->dcbus_over_60v_impl_err,

            DB_data->any_impl_err,
            DB_data->any_impl_err_ltch,

            DB_data->tsal_green);
}
/* Private functions ---------------------------------------------------------*/

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

struct DB_SHTDWN_FB DB_shtdwn_fb = {
    .csv_header_string = "sd_mid_in_to_ams_err_rly,ams_err_rly_to_imd_err_rly,imd_err_rly_to_sd_prch_rly,"
                         "sd_prch_rly_to_sd_mid_out_mV,sd_fnl_in_to_sd_dly_caps,sd_dly_caps_to_sd_fin_out_airs_mV,"
                         "sample_tick"};

struct DB_TLB_SIG_FB DB_tlb_sig_fb = {.csv_header_string =
                                          "ams_err,imd_err,sd_prch_rly,"
                                          "shrt2gnd_air_neg,shrt2gnd_air_pos,shrt2gnd_airs,"
                                          "dcbus_over_60v,"
                                          "air_neg_int_sd_rel,air_pos_int_sd_rel,dcbus_prch_rly_int_sd_rel,"
                                          "air_neg_aux,air_pos_aux,dcbus_prch_rly_aux,"
                                          "air_neg_impl_err,air_pos_impl_err,dcbus_prch_rly_impl_err,"
                                          "dcbus_over_60v_impl_err,"
                                          "any_impl_err,any_impl_err_ltch,"
                                          "tsal_green,"
                                          "sample_tick"};

/* Exported functions --------------------------------------------------------*/
void DB_SHTDWN_FB_ToStringCSV(struct DB_SHTDWN_FB *DB_shtdwn_fb, char buf[static 256]) {
    assert_param(DB_shtdwn_fb);
    assert_param(buf);
    sprintf(buf,
            "%4u,%4u,%4u,%4.1f,%4u,%4.1f,%0lu",
            DB_shtdwn_fb->sd_mid_in_to_ams_err_rly,
            DB_shtdwn_fb->ams_err_rly_to_imd_err_rly,
            DB_shtdwn_fb->imd_err_rly_to_sd_prch_rly,
            DB_shtdwn_fb->sd_prch_rly_to_sd_mid_out_mV,
            DB_shtdwn_fb->sd_fnl_in_to_sd_dly_caps,
            DB_shtdwn_fb->sd_dly_caps_to_sd_fin_out_airs_mV,
            DB_shtdwn_fb->sample_tick);
};

void DB_TLB_SIG_FB_ToStringCSV(struct DB_TLB_SIG_FB *DB_tlb_sig_fb, char buf[static 256]) {
    assert_param(DB_tlb_sig_fb);
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

            "%4u,"

            "%0lu",

            DB_tlb_sig_fb->ams_err,
            DB_tlb_sig_fb->imd_err,
            DB_tlb_sig_fb->sd_prch_rly,

            DB_tlb_sig_fb->shrt2gnd_air_neg,
            DB_tlb_sig_fb->shrt2gnd_air_pos,
            DB_tlb_sig_fb->shrt2gnd_airs,

            DB_tlb_sig_fb->dcbus_over_60v,

            DB_tlb_sig_fb->air_neg_int_sd_rel,
            DB_tlb_sig_fb->air_pos_int_sd_rel,
            DB_tlb_sig_fb->dcbus_prch_rly_int_sd_rel,

            DB_tlb_sig_fb->air_neg_aux,
            DB_tlb_sig_fb->air_pos_aux,
            DB_tlb_sig_fb->dcbus_prch_rly_aux,

            DB_tlb_sig_fb->air_neg_impl_err,
            DB_tlb_sig_fb->air_pos_impl_err,
            DB_tlb_sig_fb->dcbus_prch_rly_impl_err,

            DB_tlb_sig_fb->dcbus_over_60v_impl_err,

            DB_tlb_sig_fb->any_impl_err,
            DB_tlb_sig_fb->any_impl_err_ltch,

            DB_tlb_sig_fb->tsal_green,
            DB_tlb_sig_fb->sample_tick);
}
/* Private functions ---------------------------------------------------------*/

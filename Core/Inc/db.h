/**
 * @file    db.h
 * @prefix  DB
 * @author  Simone Ruffini [simone.ruffini.work@gmail.com | simone.ruffini@squadracorse.com]
 * @date    Tue May 23 03:33:07 PM CEST 2023
 *
 * @brief Database for internal acquired signals hader file
 *
 * @license Licensed under "THE BEER-WARE LICENSE", Revision 69 (see LICENSE)
 */
#ifndef _DB_H_
#define _DB_H_

/* Includes ------------------------------------------------------------------*/
#include <inttypes.h>

/* Exported types ------------------------------------------------------------*/
// clang-format off
struct DB_SHTDWN_FB {
    uint8_t sd_mid_in_to_ams_err_rly;             /*!< shutdown line status from middle section input to AMS ERROR realy - OFF(0)/ON(!0) */
    uint8_t ams_err_rly_to_imd_err_rly;           /*!< shutdown line status from AMS ERROR relay to IMD ERROR relay - OFF(0)/ON(!0) */
    uint8_t imd_err_rly_to_sd_prch_rly;           /*!< shutdown line status from IMD ERROR relay to shutdown precharge relay - OFF(0)/ON(!0) */
    double  sd_prch_rly_to_sd_mid_out_V;          /*!< shutdown line status from shutdown precharge realy to middle section otput - Volts*/
    uint8_t sd_fnl_in_to_sd_dly_caps;             /*!< shutdown line status from shutdown final section input to shutdown delay caps - OFF(0)/ON(!0) */
    double  sd_dly_caps_to_sd_fin_out_airs_V;     /*!< shutdown line status from shutdown delay capacitors to shutdown final section output AIRs - Volts */
    uint32_t sample_tick;                         /*!< sample tick */
    char const * const csv_header_string;         /*!< CSV header string representation of struct */
};

struct DB_TLB_SIG_FB {
    uint8_t ams_err;     /*!< TLB signal feedback: AMS error control signal - OFF(0)/ON(!0) - (ON == ERROR) */
    uint8_t imd_err;     /*!< TLB signal feedback: IMD error control signal - OFF(0)/ON(!0) - (ON == ERROR) */
    uint8_t sd_prch_rly; /*!< TLB signal feedback: shutdown precharge relay control signal - OFF(0)/ON(!0) */

    uint8_t shrt2gnd_air_neg; /*!< TLB signal feedback: short to ground of AIR negative - OFF(0)/ON(!0) - (ON == ERROR) */
    uint8_t shrt2gnd_air_pos; /*!< TLB signal feedback: short to ground of AIR positive - OFF(0)/ON(!0) - (ON == ERROR) */
    uint8_t shrt2gnd_airs;    /*!< TLB signal feedback: short to ground of an AIR (either positive or negative) - OFF(0)/ON(!0) - (ON == ERROR) */

    uint8_t dcbus_over_60v; /*!< TLB signal feedback: high voltage DC bus (post AIR+) over 60V - OFF(0)/ON(!0) - (ON == ACTIVE) */

    uint8_t air_neg_int_sd_rel;        /*!< TLB signal feedback: air negative closed, intentional state signal, shutdown reliant - OFF(0)/ON(!0) - (ON == CLOSED) */
    uint8_t air_pos_int_sd_rel;        /*!< TLB signal feedback: air positive closed, intentional state signal, shutdown reliant - OFF(0)/ON(!0) - (ON == CLOSED) */
    uint8_t dcbus_prch_rly_int_sd_rel; /*!< TLB signal feedback: high voltage DC bus precharge relay closed, intentional state signal, shutdown reliant - OFF(0)/ON(!0) - (ON == CLOSED) */

    uint8_t air_neg_aux;        /*!< TLB signal feedback: inverted (n == not), AIR negaitve closed, auxiliary output (actual state) - OFF(0)/ON(!0) - (ON == CLOSED) */
    uint8_t air_pos_aux;        /*!< TLB signal feedback: inverted (n == not), AIR positive closed, auxiliary output (actual state) - OFF(0)/ON(!0) - (ON == CLOSED) */
    uint8_t dcbus_prch_rly_aux; /*!< TLB signal feedback: inverted (n == not), high voltage DC bus precharge relay closed, auxiliary output (actual state) - OFF(0)/ON(!0) - (ON == CLOSED) */

    uint8_t air_neg_impl_err;        /*!< TLB signal feedback: AIR negative implausibility error (AIR_NEX_AUX != AIR_NEG_INT) - OFF(0)/ON(!0) - (ON == ERROR) */
    uint8_t air_pos_impl_err;        /*!< TLB signal feedback: AIR positive implausibility error (AIR_POS_AUX != AIR_POS_INT) - OFF(0)/ON(!0) - (ON == ERROR) */
    uint8_t dcbus_prch_rly_impl_err; /*!< TLB signal feedback: high voltage DC bus precharge reelay implausibility error (DCBUS_PRCH_RLY_AUX != DCBUS_PRCH_RLY_INT) - OFF(0)/ON(!0) - (ON == ERROR) */

    uint8_t dcbus_over_60v_impl_err; /*!< TLB signal feedback: high voltage DC bus over 60V implausibility error - OFF(0)/ON(!0) - (ON == ERROR) */

    uint8_t any_impl_err;      /*!< TLB signal feedback: any type/general-case implausibility error in TLB - OFF(0)/ON(!0) - (ON == ERROR) */
    uint8_t any_impl_err_ltch; /*!< TLB signal feedback: any type/general case implausibility error in TLB, latched value - OFF(0)/ON(!0) - (ON == ERROR) */

    uint8_t tsal_green;   /*!< TLB signal feedback: Tractive System Active Light green status - OFF(0)/ON(!0) - (ON == ACTIVE)*/

    uint32_t sample_tick;                   /*!< sample tick */
    char const * const csv_header_string;   /*!< CSV header string representation of struct */
};
// clang-format on

/* Exported constants --------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/**
* @brief database structure containing shutdown feedback signals @ref struct DB_SHTDWN_FB
*/
extern struct DB_SHTDWN_FB DB_shtdwn_fb;
extern struct DB_TLB_SIG_FB DB_tlb_sig_fb;

/* Exported macros -----------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/**
 * @brief Convert a @ref DB_SHTDWN_FB struct to into a CSV string stored in buf
 * @param db_shtdwn_fb The struct of type @ struct DB_SHTDWN_FB that will be converted
 * @param buf Buffer array of char of at least 256 items that will containt the header string
 */
void DB_SHTDWN_FB_ToStringCSV(struct DB_SHTDWN_FB *DB_shtdwn_fb, char buf[static 256]);

/**
 * @brief Convert a @ref DB_SHTDWN_FB struct to into a CSV string stored in buf
 * @param db_shtdwn_fb The struct of type @ struct DB_SHTDWN_FB that will be converted
 * @param buf Buffer array of char of at least 256 items that will containt the output of conversion
 */
void DB_TLB_SIG_FB_ToStringCSV(struct DB_TLB_SIG_FB *DB_tlb_sig_fb, char buf[static 256]);

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private Macros -----------------------------------------------------------*/
#endif  //_DB_H_

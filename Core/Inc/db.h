/**
 * @file db.h
 * @prefix DB
 * @author Simone Ruffini [simone.ruffini.work@gmail.com]
 * @date Tue May 23 03:33:07 PM CEST 2023
 * @brief Database for internal acquired signals hader file
 *
 * @license Licensed under "THE BEER-WARE LICENSE", Revision 69 (see LICENSE)
 */
#ifndef _DB_H_
#define _DB_H_

/* Includes ------------------------------------------------------------------*/
#include <inttypes.h>

/* Exported types ------------------------------------------------------------*/

struct DB_SHTDWN_FB{
    uint8_t sd_mid_in_to_ams_err_rly;       /*!< shutdown line status from middle section input to AMS ERROR realy */
    uint8_t ams_err_rly_to_imd_err_rly;     /*!< shutdown line status from AMS ERROR relay to IMD ERROR relay */
    uint8_t imd_err_rly_to_sd_prch_rly;     /*!< shutdown line status from IMD ERROR relay to shutdown precharge relay */
    uint8_t sd_prch_rly_to_sd_mid_out;      /*!< shutdown line status from shutdown precharge realy to middle section otput */
    uint8_t sd_fnl_in_to_sd_dly_caps;       /*!< shutdown line status from shutdown final section input to shutdown delay caps */
    uint8_t sd_dly_caps_to_sd_fin_out_airs; /*!< shutdown line status from shutdown delay capacitors to shutdown final section output AIRs */
    uint32_t sample_tick;                   /*!< sample tick */
};

/* Exported constants --------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/**
* @brief database structure containing shutdown feedback signals @ref struct DB_SHTDWN_FB
*/
extern struct DB_SHTDWN_FB db_shtdwn_fb ;

/* Exported macros -----------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private Macros -----------------------------------------------------------*/
#endif //_DB_H_

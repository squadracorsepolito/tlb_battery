/**
 * @file feedback_timebase.h
 * @group FBTMBS
 * @author Simone Ruffini [simone.ruffini.work@gmail.com]
 * @date Tue May 23 03:33:07 PM CEST 2023
 * @brief Header file for feedback reading using timebase library of stmlibs
 *
 * @license Licensed under "THE BEER-WARE LICENSE", Revision 69 (see LICENSE)
 */

#ifndef _FEEDBACK_TIMEBASE_H_
#define _FEEDBACK_TIMEBASE_H_

/* Includes ------------------------------------------------------------------*/
#include "timebase.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/

extern TIMEBASE_HandleTypeDef hfbtmbs;  /*!< Export the handle feedback timebase */

/* Exported macros -----------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/**
* @brief Initialization function for feedback timebase tied to @ref
*        feedback_timebase handle
*/
void FBTMBS_init(void);

/**
* @brief Timebase routines runner
* @long  Executes the routines for feedback timebase
* @note  Must be called inside main
*/
void FBTMBS_routines_runner(void);

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private Macros -----------------------------------------------------------*/

#endif //_FEEDBACK_TIMEBASE_H_

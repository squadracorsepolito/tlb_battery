/* USER CODE BEGIN Header */
/**
 * @file      adc.h
 * @prefix    ADC
 * @author    Simone Ruffini [simone.ruffini.work@gmail.com | simone.ruffini@squadracorse.com]
 * @date      Thu Jun  1 07:14:33 PM CEST 2023
 *
 * @brief   This file contains all the function prototypes for the adc.c file
 *
 * @license Licensed under "THE BEER-WARE LICENSE", Revision 69 (see LICENSE)
 */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include <stdint.h>

/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc1;

/* USER CODE BEGIN Private defines */
//extern uint32_t adc_raw_data_filtered[2];
//extern uint32_t adc_raw_data[2];
/* USER CODE END Private defines */

void MX_ADC1_Init(void);

/* USER CODE BEGIN Prototypes */

/**
 * @brief Get the Filtered Raw Value of an ADC channel given the ADC peripheral
 * @param hadc Pointer to the ADC peripheral @ref ADC_HandleTypeDef
 * @param adc_channel Channel Number of the given value in the ADC @see main.h
 * @return - adc filtered raw value of the channel in uin32_t form must apply later ranging and covnersions
 *         - -1U in case of error
 */
uint32_t ADC_Get_Filtered_Raw(ADC_HandleTypeDef* hadc, uint8_t adc_channel);

/**
 * @brief Get the Filtered Real Value of an ADC channel given the ADC peripheral
 * @param hadc Pointer to the ADC peripheral @ref ADC_HandleTypeDef
 * @param adc_channel Channel Number of the given value in the ADC @see main.h
 * @return - adc filtered real/phisycal value of the channel
 *         - DBL_MAX in case of error
 */
double ADC_Get_Filtered_Real(ADC_HandleTypeDef* hadc, uint8_t adc_channel);

/**
 * @brief Transform an ADC raw value into it's real form given the ADC_channel and ADC peripheral it was sampled from
 * @param hadc Pointer to the ADC peripheral @ref ADC_HandleTypeDef
 * @param adc_channel Channel Number of the given value in the ADC @see main.h
 * @param adc_raw Raw adc value sampled by the adc
 * @return Real value of the sampled value. The meaning of this value depends on the measured channel
 */
double ADC_RawToReal(ADC_HandleTypeDef* hadc, uint8_t adc_channel, uint32_t raw_adc_value);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */


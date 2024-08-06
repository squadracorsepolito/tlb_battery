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

enum ADC_Channel {
    ADC_Channel0 = 0,
    ADC_Channel1 ,
    ADC_Channel2 ,
    ADC_Channel3 ,
    ADC_Channel4 ,
    ADC_Channel5 ,
    ADC_Channel6 ,
    ADC_Channel7 ,
    ADC_Channel8 ,
    ADC_Channel9 ,
    ADC_Channel10 ,
    ADC_Channel11 ,
    ADC_Channel12 ,
    ADC_Channel13 ,
    ADC_Channel14 ,
    ADC_Channel15 ,
    ADC_Channel16 ,
    ADC_Channel17 ,
    ADC_Channel18 ,
    ADC_Channel_NUM
};

/* USER CODE END Private defines */

void MX_ADC1_Init(void);

/* USER CODE BEGIN Prototypes */

#define ADC_ADC1_VREF_V (3.3f) /*!< ADC1 voltage reference in millivolt float*/

/**
* TODO: Calculate correct alpha value using nyquist *4 filtering at the sampling speed
* RC = 100ms => FREQcut_off = 10Hz
* alpha = sampling_period / (RC + sampling_period)
*       = 1ms/(100ms+1ms) = 1/101 = 0.00999 ~ 1*10E-2
*/
#define ADC_ADC1_IIR_ALPHA \
    (0.1) /*!< ADC1 Infinite Impulse Response filter (1 order) alpha value (smooth factor)
                                         @NOTE if the sampling changes update this value */

/**
 * @brief Get the resolution of the ADC in bits given the ADC Handle pointer
 * @param HADC_INSTANCE_PTR ADC Handle pointer of type @ref ADC_HandleTypeDef*
 * @return Resolution in bits for the ADC used if it exists otherwise 0
 */
#define ADC_GET_RESOLUTION_BITS(HADC_INSTANCE_PTR)                         \
    (ADC_GET_RESOLUTION(HADC_INSTANCE_PTR) == ADC_RESOLUTION_12B   ? 12U   \
     : ADC_GET_RESOLUTION(HADC_INSTANCE_PTR) == ADC_RESOLUTION_10B ? (10U) \
     : ADC_GET_RESOLUTION(HADC_INSTANCE_PTR) == ADC_RESOLUTION_8B  ? (8U)  \
     : ADC_GET_RESOLUTION(HADC_INSTANCE_PTR) == ADC_RESOLUTION_6B  ? (6U)  \
                                                                   : (0U))

/**
 * @brief return the raw value read by the ADC1 on given channel 
 * @param channel @ref enum ADC_Channel
 * @return converted raw value of channel 
 *         -1 in case wrong channel given
 */
int16_t ADC_ADC1_getChannelRaw(enum ADC_Channel channel);


/**
 * @brief return the raw filtered value read by the ADC1 on given channel 
 * @param channel @ref enum ADC_Channel
 * @return converted raw filtered value of channel 
 *         -1 in case wrong channel given
 */
int16_t ADC_ADC1_getChannelRawFiltered(enum ADC_Channel channel);

/**
 * @brief  Convert ADC raw value to volts given the adc voltage reference in
 *         volts and its resolution
 * @param  ADC_RAW_VALUE The sampled value from the ADC
 * @param  ADC_RESOLUTION_BITS The resolution of the adc in bits (e.g: 12U)
 * @param  ADC_VREF_mV The ADC reference voltage in millivolts
 * @return Analog rappresentation in volts (float) of ADC_RAW_VALUE
 */
#define ADC_CONV_RAW_TO_V(ADC_RAW_VALUE, ADC_RESOLUTION_BITS, ADC_VREF_V) \
    ((float)(ADC_VREF_V) / ((1U) << (ADC_RESOLUTION_BITS)) * ADC_RAW_VALUE)

// TODO: remove old stuff
///**
// * @brief Get the Filtered Raw Value of an ADC channel given the ADC peripheral
// * @param hadc Pointer to the ADC peripheral @ref ADC_HandleTypeDef
// * @param adc_channel Channel Number of the given value in the ADC @see main.h
// * @return - adc filtered raw value of the channel in uin32_t form must apply later ranging and covnersions
// *         - -1U in case of error
// */
//uint32_t ADC_Get_Filtered_Raw(ADC_HandleTypeDef *hadc, uint8_t adc_channel);
//
///**
// * @brief Get the Filtered Real Value of an ADC channel given the ADC peripheral
// * @param hadc Pointer to the ADC peripheral @ref ADC_HandleTypeDef
// * @param adc_channel Channel Number of the given value in the ADC @see main.h
// * @return - adc filtered real/phisycal value of the channel
// *         - DBL_MAX in case of error
// */
//double ADC_Get_Filtered_Real(ADC_HandleTypeDef *hadc, uint8_t adc_channel);
//
///**
// * @brief Transform an ADC raw value into it's real form given the ADC_channel and ADC peripheral it was sampled from
// * @param hadc Pointer to the ADC peripheral @ref ADC_HandleTypeDef
// * @param adc_channel Channel Number of the given value in the ADC @see main.h
// * @param adc_raw Raw adc value sampled by the adc
// * @return Real value of the sampled value. The meaning of this value depends on the measured channel
// */
//double ADC_RawToReal(ADC_HandleTypeDef *hadc, uint8_t adc_channel, uint32_t raw_adc_value);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */


/* USER CODE BEGIN Header */
/**
 * @file      adc.c
 * @prefix    ADC
 * @author    Simone Ruffini [simone.ruffini.work@gmail.com | simone.ruffini@squadracorse.com]
 * @date      Thu Jun  1 07:14:33 PM CEST 2023
 * 
 * @brief   This file provides code for the configuration
 *          of the ADC instances.
 * 
 * @license Licensed under "THE BEER-WARE LICENSE", Revision 69 (see LICENSE)
 * @notes ADC1 CONFIGURATION: 
 *        - hadc1.Init.ExternalTrigConv: who triggers the conversion
 *          - ADC_EXTERNALTRIGCONV_T2_TRGO => TIM2 triggers the conversion when
 *          TRGO fires (check TIM2 for details)
 *        - ADCCLK: PCLCK2/hadc1.Init.ClockPrescaler
 *          - PCLCK2 why? because ADC1/2/3 peripherals are on the APB2 bus (check stm32f446xx.h file)
 *        - hadc1.Init.ScanConvMode: if ENABLES means multiple channels/ranks will be sampled once triggered 
 *          - hadc1.Init.NbrOfConversion: number of channels/ranks that will be sampled
 *        - hadc1.Init.ContinuousConvMode = DISABLED, why? because timer triggers the conversion
 *        - hadc1.Init.DiscontinuousConvMode = DISABLED, why? all channels need to be converted in sequence without halts
 *        - hadc1.Init.DMAContinuousRequests = ENABLED, why? DMA requests can continue to be generated after the conversion sequence ends (otherwise the DMA is disabled and must be re-enabled)
 *          - NOTE: this should be disabled if the timer is not in continuos mode or there is no retriggering via a timer (DMA remains active)
 *          - NOTE: this value is effective only if a DMA request is configured
 *              - hdma_adc1.Init.Mode = DMA_CIRCULAR; -> DMA must be configured in circular mode. Otherwise an overrun will be triggered when DMA buffer maximum pointer is reached.
 *              - hdma_adc1.Init.MemInc = DMA_MINC_ENABLE; -> increase the memory address after each transfer
 *        - hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV; EndOfConversion event flag will be activated at the end of conversion of the sequence of channels/ranks instead of a single conversion (NOTE: interrupts must be enabled)
 *          - NOTE: IF DMA is enabled then setting EOCSelection as that will enable overrun detection feature
          - hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD
            - Peripheral data allignemtn for the DMA is halfword because the ADC Data Register (ADC.Instance.DR) is 16 bit wide.
              This means that the trasnfer to memory will be necessary a halfword too!
                  - hdma_adc1.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD
              Therefore since the DMA is set with memory autoincrease and circular mode (see above) this means that
              the autoincrease will be by HALWORDS. 
            @NOTE Therefore when HAL_ADC_Start_DMA(ADC_HandleTypeDef* hadc, uint32_t* pData, uint32_t Length) is called:
              - sizeof(pData[0]) == hdma_adc1.Init.MemDataAlignment
                otherwise, if greater (uint32_t pData[N]), the DMA will save data in the vector each 16 bits: 
                a pData item will hold two values -> pData[0] = {[adc_conv1],[adc_conv0]};
 * @note ADC TIMINGS:
 *       - CHANNEL/RANK TOTAL SAMPLING CYCLES = sConfig.SamplingTime + hadc1.Init.Resolution(#ofbits) + 2 or 3 cycles (depending on resolution)
 *       - CHANNEL/RANK TOTAL SAMPLING TIME = (CHANNEL/RANK TOTAL SAMPLING CYCLES)/ADCCLK
 *       - SAMPLING FREQUENCY = the one of the timer
 * 
 * @see https://wiki.st.com/stm32mcu/wiki/Getting_started_with_ADC
 * @see https://deepbluembedded.com/stm32-adc-tutorial-complete-guide-with-examples/
 */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "adc.h"

/* USER CODE BEGIN 0 */
#include "main.h"

#include <float.h>
#include <inttypes.h>
#include <tim.h>

double adc_raw_data_filtered[2] = {0};
uint16_t adc_raw_data[2]        = {0};

/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

/* ADC1 init function */
void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV6;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_TRGO;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 2;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

    // Start ADC IN DMA MODE
    HAL_TIM_Base_Start(&TIM_DMA1_HandleTypeDef);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_raw_data, hadc1.Init.NbrOfConversion);

  /* USER CODE END ADC1_Init 2 */

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* ADC1 clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PA0-WKUP     ------> ADC1_IN0
    PA1     ------> ADC1_IN1
    */
    GPIO_InitStruct.Pin = SD_FB_SD_DLY_CAPS_TO_SD_FIN_OUT_AIRS_ADC1_IN_Pin|SD_FB_SD_PRCH_RLY_TO_SD_MID_OUT_ADC1_IN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* ADC1 DMA Init */
    /* ADC1 Init */
    hdma_adc1.Instance = DMA2_Stream0;
    hdma_adc1.Init.Channel = DMA_CHANNEL_0;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc1.Init.Mode = DMA_CIRCULAR;
    hdma_adc1.Init.Priority = DMA_PRIORITY_LOW;
    hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(adcHandle,DMA_Handle,hdma_adc1);

    /* ADC1 interrupt Init */
    HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);
  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();

    /**ADC1 GPIO Configuration
    PA0-WKUP     ------> ADC1_IN0
    PA1     ------> ADC1_IN1
    */
    HAL_GPIO_DeInit(GPIOA, SD_FB_SD_DLY_CAPS_TO_SD_FIN_OUT_AIRS_ADC1_IN_Pin|SD_FB_SD_PRCH_RLY_TO_SD_MID_OUT_ADC1_IN_Pin);

    /* ADC1 DMA DeInit */
    HAL_DMA_DeInit(adcHandle->DMA_Handle);

    /* ADC1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(ADC_IRQn);
  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

// clang-format off
#define ADC_RESOLUTION_TO_NUMB_BITS(ADC_RESOLUTION)             \
    (ADC_RESOLUTION == ADC_RESOLUTION_12B ? 12U                 \
         : (ADC_RESOLUTION == ADC_RESOLUTION_10B ? 10U          \
                : (ADC_RESOLUTION == ADC_RESOLUTION_8B ? 8U     \
                    : (ADC_RESOLUTION == ADC_RESOLUTION_6B ? 6U \
                        : -1U))))
// clang-format on

/**
 * @brief Implements a first-order IIR single pole low pass filter 
 * @long  Y[n] = alpha * X[n] + (1-alpha)Y[n-1]
 *        Y[n]   = new filtered sample
 *        Y[n-1] = previous filtered samples (history) 
 *        X[n]   = current sample
 * @param alpha smoohting factor
 * @param X_n currently sampled value
 * @param Y_n_min1 previous sample already filtered (history)
 * @return truncated filtered sample (Y[n])
 * @see https://en.wikipedia.org/wiki/Low-pass_filter#math_Q
 * @see https://www.youtube.com/watch?v=QRMe02kzVkA
 */
static double _ADC_IIR_first_order(double alpha, double X_n, double Y_n_min1) {
    // Y[n] = alpha * X[n] + (1-alpha)Y[n-1]
    //      = Y[n-1] + alpha * (X[n]-Y[n-1])
    return (uint32_t)(Y_n_min1 + alpha * (X_n - Y_n_min1));
}

/**
 * @brief noise filtering function to be applied on new samples
 * @param new_sample Recently convered value
 * @param prev_filtered_sample Previous filtered value in history (already filtered)
 *
 */
static uint32_t _ADC_sample_filtering(uint32_t new_sample, double prev_filtered_sample) {
    /**
    * Values on CAN bus are sent every 100ms
    * RC = 100ms => FREQcut_off = 10Hz
    * alpha = sampling_period / (RC + sampling_period)
    *       = 1ms/(100ms+1ms) = 1/101 = 0.00999 ~ 1*10E-2
    */
#define IIR_ALPHA (0.1)  //(0.01)
    return _ADC_IIR_first_order(IIR_ALPHA, (double)new_sample, prev_filtered_sample);
}

/**
  * @brief  Regular conversion complete callback in non blocking mode 
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *         the configuration information for the specified ADC.
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
    for (int i = 0; i < hadc1.Init.NbrOfConversion; ++i) {
        //adc_raw_data_filtered[i] = _ADC_sample_filtering(adc_raw_data[i], adc_raw_data_filtered[i]);
        adc_raw_data_filtered[i] = (double)adc_raw_data[i];
    }
}

/**
 * @brief Transform RAW adc values to a real and physical counterpart
 * @details real_output[phisycal_unit] = offset[phisycal_value] + adc_raw_value[steps] * LSB_weight[Volts/steps] * gain[phisycal_unit/Volts]
 * @param adc_raw_data ADC raw data 
 * @param adc_res_bit ADC resolution, number of bits
 * @param adc_FSR_mv ADC full-scale input range (in milli Volts) proportional to the reference voltage (usually 3300mV or else VDDA voltage on uC)
 * @param adc_sig_res_bit ADC significant/usable resolution bits, i.e. a 12 bit reading could have only the first 8 bit significant 
 * @note adc_sig_res_bit <= adc_res_bit and both greater then 0
 * @param offset Offest of the real measure the raw data is representing, has the same physical unit of the output 
 * @param gain Multiplier coefficent that maps 1mV change to a physical change, e.g. 0.005 degC/mV means 1mV change is 0.005 degrees celsius change
 * @return real_output from the formula above
 */
static double _ADC_RawToReal(uint32_t adc_raw_data,
                             uint8_t adc_res_bit,
                             double adc_FSR_mv,
                             uint8_t adc_sig_res_bit,
                             double offset,
                             double gain) {
    assert_param(adc_sig_res_bit > 0);
    assert_param(adc_res_bit > 0);
    assert_param(adc_sig_res_bit <= adc_res_bit);
    // If the adc resolution is 12 bit (adc_res_bit = 12) but the usable
    // and significant bits of the adc are the first 8 (adc_sig_res_bit = 8)
    // We mask the adc_raw_data with 0x1111_1111_0000
    uint32_t raw_data_significant_bits_mask = ~((1U << (adc_res_bit - adc_sig_res_bit)) - 1U);
    uint32_t adc_raw_data_significant       = adc_raw_data & raw_data_significant_bits_mask;
    double LSb_weight                       = adc_FSR_mv / (1U << adc_res_bit);  // Least Significan Bit weight
    return offset + (adc_raw_data_significant * LSb_weight * gain);
}

uint32_t ADC_Get_Filtered_Raw(ADC_HandleTypeDef *hadc, uint8_t adc_channel) {
    if (hadc->Instance == SD_FB_ADC_Handle.Instance) {
        switch (adc_channel) {
            case SD_FB_SD_DLY_CAPS_TO_SD_FIN_OUT_AIRS_ADC1_IN_ADC_CHNL:
                return adc_raw_data_filtered[0];
                break;
            case SD_FB_SD_PRCH_RLY_TO_SD_MID_OUT_ADC1_IN_ADC_CHNL:
                return adc_raw_data_filtered[1];
                break;
            default:
                // Should not be here
                return -1U;
                break;
        }
    } else
        return -1U;  // Should not be here
}

double ADC_Get_Filtered_Real(ADC_HandleTypeDef *hadc, uint8_t adc_channel) {
    uint32_t raw = ADC_Get_Filtered_Raw(hadc, adc_channel);
    if (raw != (-1U))
        return ADC_RawToReal(hadc, adc_channel, raw);
    else
        return DBL_MAX;
}

double ADC_RawToReal(ADC_HandleTypeDef *hadc, uint8_t adc_channel, uint32_t raw_adc_value) {
    assert_param(IS_ADC_CHANNEL(adc_channel));
    if (hadc->Instance == SD_FB_ADC_Handle.Instance) {
        switch (adc_channel) {
            case SD_FB_SD_DLY_CAPS_TO_SD_FIN_OUT_AIRS_ADC1_IN_ADC_CHNL:
            case SD_FB_SD_PRCH_RLY_TO_SD_MID_OUT_ADC1_IN_ADC_CHNL:
                return _ADC_RawToReal(raw_adc_value,
                                      ADC_RESOLUTION_TO_NUMB_BITS(hadc->Init.Resolution),
                                      ADC_VDDA_mV,
                                      ADC_RESOLUTION_TO_NUMB_BITS(hadc->Init.Resolution),
                                      SD_FB_ADC_OFFSET,
                                      SD_FB_ADC_GAIN);
                break;
            default:
                // Should not be here
                return 0;
                break;
        }
    } else
        return 0;  // Should not be here
}
/* USER CODE END 1 */

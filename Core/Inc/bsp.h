/**
 * @file    BSP.h
 * @author  Simone Ruffini [simone.ruffini@squadracorsepolito.com, simone.ruffini.work@gmail.com]
 * @date    Thu Jul  4 03:00:17 PM CEST 2024
 * @updated Thu Jul  4 03:00:17 PM CEST 2024
 * @prefix  varius depending on module
 *
 * @brief   Header files for board support package functions
 *
 * @license Licensed under "THE BEER-WARE LICENSE", Revision 69 
 *          see LICENSE file in the root directory of this software component
 */

/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef _BSP_H_
#define _BSP_H_

/* ---------- Includes ------------------------------------------------------*/
#include "adc.h"
#include "gpio.h"

/* # Shared -----------------------------------------------------------------*/

/* ---------- Exported types ------------------------------------------------*/
//  GPIO (General Purpuse Input Output)
struct GPIO_Tuple {
    GPIO_TypeDef *GPIO_Port;
    uint16_t GPIO_Pin;
};

/* ---------- Exported constants --------------------------------------------*/

/* ---------- Exported variables --------------------------------------------*/

extern volatile uint32_t ERR_flags;

/* ---------- Exported macros -----------------------------------------------*/

/* ---------- Exported functions --------------------------------------------*/

/* ---------- Private types -------------------------------------------------*/

/* ---------- Private variables ---------------------------------------------*/

/* ---------- Private constants ---------------------------------------------*/

/* ---------- Private Macros ------------------------------------------------*/

/* # SDC Sensing ------------------------------------------------------------*/

/* ---------- Exported types ------------------------------------------------*/

/**
 * @brief SDC Sensing Probes
 * @details responds to isOpen / isClosed
 */
enum SDC_SENS_Probe {
    SDC_SENS_TSAC_InitialIn,   /*!< ShutDown Circuit Sensing on TSAC Initial Input */
    SDC_SENS_Post_AMS_IMD_Rly, /*!< ShutDown Circuit Sensing after AMS/IMD Error relays */
    SDC_SENS_TSAC_FinalIn,     /*!< ShutDown Circuit Sensing on TSAC Final Input */
    SDC_SENS_Probe_NUM         /*!< Number of SDC SENS enum values */
};

/**
 * @brief SDC Analog Value Sensing Probes
 */
enum SDC_ANAL_SENS_Probe {
    SDC_ANAL_SENS_TSAC_InitialIn, /*!< ShutDown Circuit Analog Sensing on TSAC Initial Input */
    SDC_ANAL_SENS_TSAC_FinalIn,   /*!< ShutDown Circuit Analog Sensing on TSAC Final Input */
    SDC_ANAL_SENS_Probe_NUM       /*!< Number of SDC SENS enum values */
};

/* ---------- Exported constants --------------------------------------------*/
#define SDC_SENS_SAMPLING_PERIOD_MS      (100U);
#define SDC_ANAL_SENS_SAMPLING_PERIOD_MS (300U);
/* ---------- Exported variables --------------------------------------------*/

extern uint32_t SDC_SENS_probe_values;
extern float SDC_ANAL_SENS_probe_values[SDC_ANAL_SENS_Probe_NUM];

/* ---------- Exported macros -----------------------------------------------*/

/* ---------- Exported functions --------------------------------------------*/
/**
 * @brief samples and returns the SDC probe status
 * @param probe SDC probe
 * @return 0 the probe reads a open circuit
 *         1 the probe reads a closed circuit (SDC Active)
 */
uint8_t SDC_SENS_Probe_sampleStatus(enum SDC_SENS_Probe probe);

uint8_t SDC_SENS_Probe_getStatus(enum SDC_SENS_Probe probe);
void SDC_SENS_Routine(void);
/**
 * @brief samples and returns the analog value read by the SDC probe
 * @param probe SDC probe
 * @return physical value read by the probe in Volts 
 */
float SDC_ANAL_SENS_Probe_sampleVoltage(enum SDC_ANAL_SENS_Probe probe);

float SDC_ANAL_SENS_Probe_getVoltage(enum SDC_ANAL_SENS_Probe probe);
void SDC_ANAL_SENS_Routine(void);
/* ---------- Private types -------------------------------------------------*/

/* ---------- Private variables ---------------------------------------------*/

/* ---------- Private constants ---------------------------------------------*/

/* ---------- Private Macros ------------------------------------------------*/

/* # Internal Signals Sensing  ----------------------------------------------*/

/* ---------- Exported types ------------------------------------------------*/

/**
 * @brief Internal Signals Probes
 * @details responds to isActive / isInactive
 */
enum SIG_SENS_Probe {
    SIG_SENS_AMS_Err_Latched,             /*!< Accumulator Managemenet System Error - Latched signal */
    SIG_SENS_IMD_Err_Latched,             /*!< Insulation Monitoring Device Error - Latched signal */
    SIG_SENS_SDCPrechBypassRlyCmd,        /*!< ShutDownCircuit Precharge Bypass Relay Command signal */
    SIG_SENS_DCBusOver60V,                /*!< DC-Bus Over 60V signal */
    SIG_SENS_TSAL_Green,                  /*!< Tractive System Active Light Green signal*/
    SIG_SENS_STG_on_AIR_Pos_MechStateSig, /*!< Short To Gnd on "AIR Positive Mechanical State" Signal */
    SIG_SENS_STG_on_AIR_Neg_MechStateSig, /*!< Short To Gnd on "AIR Negative Mechanical State" Signal */
    SIG_SENS_AnyImpl_Latched,             /*!< Any Implausibility Error - Latched signal */
    SIG_SENS_Impl_HVRlysState,            /*!< Implausibility Error HV Relays (AIR Pos/Min, DCBUS Precharge relay) command vs mechanical state differ */
    SIG_SENS_Impl_DCBus,                  /*!< Implausibility Error DCBus: DCBus less then 60V but AIRs closed */
    SIG_SENS_Probe_NUM                    /*!< Number of SIG_SENS enum values */
};

/* ---------- Exported constants --------------------------------------------*/
#define SIG_SENS_SAMPLING_PERIOD_MS (1U);

/* ---------- Exported variables --------------------------------------------*/
extern uint32_t SIG_SENS_probe_values;

/* ---------- Exported macros -----------------------------------------------*/

/* ---------- Exported functions --------------------------------------------*/
uint8_t SIG_SENS_Probe_sampleStatus(enum SIG_SENS_Probe probe);
uint8_t SIG_SENS_Probe_getStatus(enum SIG_SENS_Probe probe);
void SIG_SENS_Routine(void);

/* ---------- Private types -------------------------------------------------*/

/* ---------- Private variables ---------------------------------------------*/

/* ---------- Private constants ---------------------------------------------*/

/* ---------- Private Macros ------------------------------------------------*/

/* # HV Relays Sensing  -----------------------------------------------------*/

/* ---------- Exported types ------------------------------------------------*/

/*
 * @brief HV Relays Sensing Probes
 * @details responds to isStateOn/isStateOff
 */
enum HVRLYS_SENS_Probe {
    HVRLYS_SENS_AIR_Pos_Cmd,
    HVRLYS_SENS_AIR_Pos_MechStateClosed,
    HVRLYS_SENS_AIR_Neg_Cmd,
    HVRLYS_SENS_AIR_Neg_MechStateClosed,
    HVRLYS_SENS_DCBus_PrechRly_Cmd,
    HVRLYS_SENS_DCBus_PrechRly_MechStateClosed,
    HVRLYS_SENS_Probe_NUM
};

/* ---------- Exported constants --------------------------------------------*/
#define HVRLYS_SENS_SAMPLING_PERIOD_MS (1U);

/* ---------- Exported variables --------------------------------------------*/

extern uint32_t HVRLYS_SENS_probe_values;

/* ---------- Exported macros -----------------------------------------------*/

/* ---------- Exported functions --------------------------------------------*/
uint8_t HVRLYS_SENS_Probe_sampleStatus(enum HVRLYS_SENS_Probe probe);
uint8_t HVRLYS_SENS_Probe_getStatus(enum HVRLYS_SENS_Probe probe);
void HVRLYS_SENS_Routine(void);

/* ---------- Private types -------------------------------------------------*/

/* ---------- Private variables ---------------------------------------------*/

/* ---------- Private constants ---------------------------------------------*/

/* ---------- Private Macros ------------------------------------------------*/

/* # Logging ----------------------------------------------------------------*/

/* ---------- Exported types ------------------------------------------------*/

/* ---------- Exported constants --------------------------------------------*/

#define VCP_UART_Handle               huart1 /*!< Virtual COMM Port UART handle */
#define VCP_TX_LOG_BUF_MAX_TIMEOUT_ms (30U)  /*!< ~27ms are necessary to send 400 chars via UART@115kbit/s */

/* ---------- Exported variables --------------------------------------------*/

/* ---------- Exported macros -----------------------------------------------*/

/* ---------- Exported functions --------------------------------------------*/

/* ---------- Private types -------------------------------------------------*/

/* ---------- Private variables ---------------------------------------------*/

/* ---------- Private constants ---------------------------------------------*/

/* ---------- Private Macros ------------------------------------------------*/

/* # Main CAN Bus Communication ---------------------------------------------*/

/* ---------- Exported types ------------------------------------------------*/

/* ---------- Exported constants --------------------------------------------*/

#define MCB_Handle                           hcan1
#define MCB_TxAttemptsMailboxesFull          (3U)
#define MCB_AttemptRetryDelayMsMailboxesFull (1U)

/* ---------- Exported variables --------------------------------------------*/

/* ---------- Exported macros -----------------------------------------------*/

/* ---------- Exported functions --------------------------------------------*/

void MCB_send_msg(uint32_t id);
void MCB_SendMessagesRoutine(void);

/* ---------- Private types -------------------------------------------------*/

/* ---------- Private variables ---------------------------------------------*/

/* ---------- Private constants ---------------------------------------------*/

/* ---------- Private Macros ------------------------------------------------*/

/* # Status Leds ------------------------------------------------------------*/

/* ---------- Exported types ------------------------------------------------*/
enum STAT_LED_Device {
    STAT_LED_KEEP_ALIVE, /*!< Keep Alive Led */
    STAT_LED_USER1,      /*!< Status Led User 1 */
    STAT_LED_USER2,      /*!< Status Led User 2 */
    STAT_LED_WARN,       /*!< Warning Led */
    STAT_LED_ERR,        /*!< Error Led */
    STAT_LED_Device_NUM
};

/* ---------- Exported constants --------------------------------------------*/

/* ---------- Exported variables --------------------------------------------*/
#define STAT_LED_KEEP_ALIVE_BLINK_PERIOD_MS (1000U);
#define STAT_LED_ERR_PERSISTENCY_MS         (50U);

/* ---------- Exported macros -----------------------------------------------*/

/* ---------- Exported functions --------------------------------------------*/
void STAT_LED_toggle(enum STAT_LED_Device device);
void STAT_LED_enable(enum STAT_LED_Device device);
void STAT_LED_disable(enum STAT_LED_Device device);
void STAT_LED_Routine(void);

/* ---------- Private types -------------------------------------------------*/

/* ---------- Private variables ---------------------------------------------*/

/* ---------- Private constants ---------------------------------------------*/

/* ---------- Private Macros ------------------------------------------------*/
/* # CHAPTER ----------------------------------------------*/

/* ---------- Exported types ------------------------------------------------*/

/* ---------- Exported constants --------------------------------------------*/

/* ---------- Exported variables --------------------------------------------*/

/* ---------- Exported macros -----------------------------------------------*/

/* ---------- Exported functions --------------------------------------------*/

/* ---------- Private types -------------------------------------------------*/

/* ---------- Private variables ---------------------------------------------*/

/* ---------- Private constants ---------------------------------------------*/

/* ---------- Private Macros ------------------------------------------------*/

#endif

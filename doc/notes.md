# TSAL Latch Board Notes
## Pin Renaming And Notes

|Pin #| Old name                     | New Name                               | Notes                                                                                                           |
|-----|------------------------------|------------------------------------    |-------                                                                                                          |
|PB9  |CAN_TX                        |SN65HVD23x_D_CAN1_TX                    | CAN BUS 2                                                                                                       |
|PB8  |CAN_RX                        |SN65HVD23x_R_CAN1_RX                    | CAN BUS 2                                                                                                       |
|PB3  |SWO                           |SYS_JTDO-SWO                            |JTAG/SWD signal: serial wire output                                                                              |
|PC12 |SD_reading_3                  |SD_FDBK_IMD_ERR_RELAY_OUT_GPIO_IN       | shutdown feedback signal: output from RELAY drived by IMD ERROR circuitry                                       |
|PC11 |ctrl_relay_prec_SD_direct_3.3V|SD_FDBK_PRCHRG_SD_CTRL_REALY_GPIO_IN    |                                                                                                                 |
|PC10 |SD_reading_5                  |SD_FDBK_SD_FNL_IN_GPIO_IN               |                                                                                                                 |
|PA15 |JTDI                          |SYS_JTDI                                |JTAG/SWD signal:                                                                                                 |
|PA14 |SWCLK                         |SYS_JTCK-SWCLK                          |JTAG/SWD signal: serial wire Clock                                                                               |
|PA13 |SEDIO                         |SYS_JTMS-SWDIO                          |JTAG/SWD signal: serfial wire data input output                                                                  |
|PA11 |HV_signal_N_3.3V              |HV_FDBK_n60V_GPIO_IN                    |                                                                                                                 |
|PA10 |AIR_POS_Closed_N_3.3V         |AIR_POS_FDBK_nCLOSED_GPIO_IN            |                                                                                                                 |
|PA9  |AIR_NEG_Closed_N_3.3V         |AIR_NEG_FDBK_nCLOSED_GPIO_IN            |                                                                                                                 |
|PA8  |IMP_AIR_POS_3.3V              |AIR_POS_FDBK_IMPL_ERR_GPIO_IN           |                                                                                                                 |
|PC9  |IMP_PREC_HV_3.3V              |HV_PRCHRG_FDBK_IMPL_ERR_GPIO_IN         |                                                                                                                 |
|PC8  |IMP_AIR_NEG_3.3V              |                                        |                                                                                                                 |
|PC7  |IMP_HV_3.3V                   |                                        |                                                                                                                 |
|PC6  |IMP_OUT_N_3.3V                |                                        |                                                                                                                 |
|PB15 |LATCH_IMP_OUT_3.3V            |                                        |                                                                                                                 |
|PB14 |SAL_GREEN_STATUS_3.3V         |                                        |                                                                                                                 |
|PB13 |SCS_ERROR_N_3.3Vo             |                                        |                                                                                                                 |
|PB12 |PREC_Closed_N_3.3V            |                                        |                                                                                                                 |
|PB10 |Check_SCS_AIR_POS_3.3V        |                                        |                                                                                                                 |
|PB2  |Check_SCS_AIR_NEG_3.3V        |                                        |                                                                                                                 |
|PB1  |gpio_GREEN                    |LED_GREEN_GPIO_OUT                      |led color green - GPIO OUT                                                                                       |
|PB0  |gpio_BLUE                     |LED_BLUE_GPIO_OUT                       |led color blue - GPIO out                                                                                        |
|PC5  |ORANGE_LED                    |LED_ORANGE_GPIO_OUT                     |led color orange - GPIO out                                                                                      |
|PC4  |gpio_RED                      |LED_RED_GPIO_OUT                        |led color red - GPIO out                                                                                         |
|PA7  |PREC_TS_INTENTIONAL_STATE_3.3V|BMS_CMD_FDBK_TS_PRCHRG_RELAY_OUT_GPIO_IN|bms hv command feedback: tractive system prcharge signal relay output (intentiona state) - GPIO input reading    |
|PA6  |AIR_NEG_INTENTIONAL_STATE_3.3V|BMS_CMD_FDBK_AIR_NEG_RELAY_OUT_GPIO_IN  |bms hv command feedback: air negative ON/OFF signal realay output (intentional state) - GPIO input reading       |
|PA5  |AIR_POS_INTENTIONAL_STATE_3.3V|BMS_CMD_FDBK_AIR_POS_REALY_OUT_GPIO_IN  |bms hv command feedback: air positive ON/OFF signal realay output (intentional state) - GPIO input reading       |
|PA4  |AMS_ERR_neg_3.3V              |TLB_FDBK_nAMS_ERR_GPIO_IN               | TLB circuitry feedback: inverted (n == not) AMS error control signal (off == ERROR) - GPIO input reading        |
|PA3  |USART_RX                      |T_VCP_RX_USART_RX                       |target virtual comm port rx - usart rx                                                                           |
|PA2  |USART_TX                      |T_VCP_TX_USART_TX                       |target virtual comm port tx - usart tx                                                                           |
|PA1  |SD_reading_4_ADC              |SD_FDBK_SD_MID_OUT_ADC1_IN              |shutdown feedback signal: shutdown middle section otput - ADC input reading (0->3.3V, tipycal 3.0V)              |
|PA0  |SD_reading_6_ADC              |SD_FDBK_SD_PRCHRG_CAPS_ADC1_IN          |sutdown feedback signal: shutdown precharge capacitors - ADC input reading reading (0->3.3V, tipycal 3.0V)       |
|PC3  |SD_readinng_1                 |SD_FDBK_SD_MID_IN_GPIO_IN               |shutdown feedback signal: shutdown middle section input - GPIO input reading (on/off)                            |
|PC1  |SD_reading_2                  |SD_FDBK_AMS_ERR_RELAY_OUT_GPIO_IN       |shutdown feedback signal: output from RELAY drived by AMS ERROR circuitry - GPIO input reading (on/off)          |
|PC0  |IMD_ERR_neg_3.3V              |TLB_FDBK_nIMD_ERR_GPIO_IN               |TLB circuitry feedback: inverted (n == not) IMD error control signal (off == ERROR) - GPIO input reading         |

## Additional Schematics Changes
- IN1, OUT1 -> MID_IN, MID_OUT
- IN2,OUT2 -> FNL_IN, FNL_OUT

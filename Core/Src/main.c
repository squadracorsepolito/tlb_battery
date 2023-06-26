/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "adc.h"
#include "can.h"
#include "dma.h"
#include "gpio.h"
#include "tim.h"
#include "usart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "db.h"
#include "feedback_timebase.h"

#include <stdio.h>
#include <string.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
#define VCP_TX_LOG_BUF_MAX_TIMEOUT_MS (30U)  //~27ms are necessary to send 400 chars via UART@115kbit/s

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void disable_led() {
    HAL_GPIO_WritePin(LED_GREEN_GPIO_OUT_GPIO_Port, LED_GREEN_GPIO_OUT_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_RED_GPIO_OUT_GPIO_Port, LED_RED_GPIO_OUT_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_ORANGE_GPIO_OUT_GPIO_Port, LED_ORANGE_GPIO_OUT_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_BLUE_GPIO_OUT_GPIO_Port, LED_BLUE_GPIO_OUT_Pin, GPIO_PIN_RESET);
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_TIM2_Init();
    MX_TIM6_Init();
    MX_ADC1_Init();
    MX_CAN1_Init();
    MX_USART2_UART_Init();
    /* USER CODE BEGIN 2 */
    FBTMBS_init();
    HAL_CAN_Start(&hcan1);
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    char log_buf[400]           = {0};  // init logging buffer
    volatile uint32_t cnt_200ms = HAL_GetTick() + 200U;
    volatile uint32_t cnt_100ms = HAL_GetTick() + 100U;
    volatile uint32_t cnt_10ms  = HAL_GetTick() + 10U;

    //memset((void *)&(tlb_batt_sig_fb_can_msg.data),0,8);
    //memset((void *)&(tlb_batt_shtdwn_fb_can_msg.data),0,8);
    memset(&(tlb_batt_sig_fb_can_msg.data), 0, 8);
    memset(&(tlb_batt_shtdwn_fb_can_msg.data), 0, 8);

    // Send CSV header to VCP if enabled
    sprintf(log_buf, "%s,", DB_shtdwn_fb.csv_header_string);
    HAL_UART_Transmit(&VCP_UART_Handle, (uint8_t *)log_buf, strlen(log_buf), VCP_TX_LOG_BUF_MAX_TIMEOUT_MS);
    sprintf(log_buf, "%s;\n\r", DB_tlb_sig_fb.csv_header_string);
    HAL_UART_Transmit(&VCP_UART_Handle, (uint8_t *)log_buf, strlen(log_buf), VCP_TX_LOG_BUF_MAX_TIMEOUT_MS);

    while (1) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        // Execute the feedback timebase routines runner
        //FBTMBS_routines_runner();
        if (HAL_GetTick() >= cnt_10ms) {
            cnt_10ms = HAL_GetTick() + 10U;
        }

        if (HAL_GetTick() >= cnt_100ms) {
            cnt_100ms = HAL_GetTick() + 100U;

            _sample_fb();

            CAN_update_tlb_batt_shtdwn_fb(&DB_shtdwn_fb, &(tlb_batt_shtdwn_fb_can_msg.tlb_batt_shtdwn_fb));

            CAN_update_tlb_batt_sig_fb(&DB_tlb_sig_fb, &(tlb_batt_sig_fb_can_msg.tlb_batt_sig_fb));

            // Print to uart
            #if 0
            sprintf(log_buf,
                    "[%x %x %x %x | %x %x %x %x ]",
                    tlb_batt_shtdwn_fb_can_msg.data[0],
                    tlb_batt_shtdwn_fb_can_msg.data[1],
                    tlb_batt_shtdwn_fb_can_msg.data[2],
                    tlb_batt_shtdwn_fb_can_msg.data[3],
                    tlb_batt_shtdwn_fb_can_msg.data[4],
                    tlb_batt_shtdwn_fb_can_msg.data[5],
                    tlb_batt_shtdwn_fb_can_msg.data[6],
                    tlb_batt_shtdwn_fb_can_msg.data[7]);
            HAL_UART_Transmit(&VCP_UART_Handle, (uint8_t *)log_buf, strlen(log_buf), VCP_TX_LOG_BUF_MAX_TIMEOUT_MS);
            #endif
            DB_SHTDWN_FB_ToStringCSV(&DB_shtdwn_fb, log_buf);
            strcat(log_buf, ",");
            HAL_UART_Transmit(&VCP_UART_Handle, (uint8_t *)log_buf, strlen(log_buf), VCP_TX_LOG_BUF_MAX_TIMEOUT_MS);
            DB_TLB_SIG_FB_ToStringCSV(&DB_tlb_sig_fb, log_buf);
            strcat(log_buf, ";\n\r");
            HAL_UART_Transmit(&VCP_UART_Handle, (uint8_t *)log_buf, strlen(log_buf), VCP_TX_LOG_BUF_MAX_TIMEOUT_MS);

            // Send via can
            if (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) > 0) {
                CAN_TxHeaderTypeDef can_header;
                can_header.StdId              = 0x07;
                can_header.ExtId              = 0x50;
                can_header.IDE                = CAN_ID_STD;
                can_header.RTR                = CAN_RTR_DATA;
                can_header.DLC                = 3; //sizeof(struct CAN_tlb_batt_shtdwn_fb);
                can_header.TransmitGlobalTime = DISABLE;
                uint32_t used_mailbox;
                uint8_t can_data[8] = {0};
                CAN_serialize_tlb_batt_shtdwn_fb(&(tlb_batt_shtdwn_fb_can_msg.tlb_batt_shtdwn_fb),can_data);
                HAL_CAN_AddTxMessage(&hcan1, &can_header, can_data, &used_mailbox);

                can_header.StdId = 0x08;
                can_header.DLC   = sizeof(struct CAN_tlb_batt_sig_fb);
                CAN_serialize_tlb_batt_sig_fb(&(tlb_batt_sig_fb_can_msg.tlb_batt_sig_fb),can_data);
                HAL_CAN_AddTxMessage(&hcan1, &can_header, can_data, &used_mailbox);
            } else {
                HAL_UART_Transmit(
                    &VCP_UART_Handle, (uint8_t *)"problem", strlen("problem"), VCP_TX_LOG_BUF_MAX_TIMEOUT_MS);
            }
        }

        if (HAL_GetTick() >= cnt_200ms) {
            cnt_200ms = HAL_GetTick() + 200U;
        }
    }
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
  */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM       = 4;
    RCC_OscInitStruct.PLL.PLLN       = 180;
    RCC_OscInitStruct.PLL.PLLP       = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ       = 2;
    RCC_OscInitStruct.PLL.PLLR       = 2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Activate the Over-Drive mode
  */
    if (HAL_PWREx_EnableOverDrive() != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
  */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line) {
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
     * ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

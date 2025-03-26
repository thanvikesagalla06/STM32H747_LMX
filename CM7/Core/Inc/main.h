/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define OSC32_OUT_Pin GPIO_PIN_15
#define OSC32_OUT_GPIO_Port GPIOC
#define OSC32_IN_Pin GPIO_PIN_14
#define OSC32_IN_GPIO_Port GPIOC
//#define LED_Pin GPIO_PIN_8
//#define LED_GPIO_Port GPIOB
#define CEC_CK_MCO1_Pin GPIO_PIN_8
#define CEC_CK_MCO1_GPIO_Port GPIOA
#define OSC_OUT_Pin GPIO_PIN_1
#define OSC_OUT_GPIO_Port GPIOH
#define OSC_IN_Pin GPIO_PIN_0
#define OSC_IN_GPIO_Port GPIOH
/* USER CODE BEGIN Private defines */

#define ATT1_Pin GPIO_PIN_7
#define ATT1_GPIO_Port GPIOC
#define ATT2_Pin GPIO_PIN_3
#define ATT2_GPIO_Port GPIOD
#define ATT3_Pin GPIO_PIN_9
#define ATT3_GPIO_Port GPIOB
#define ATT4_Pin GPIO_PIN_8
#define ATT4_GPIO_Port GPIOB
#define ATT5_Pin GPIO_PIN_12
#define ATT5_GPIO_Port GPIOA

#define RF_LD_Pin GPIO_PIN_8
#define RF_LD_GPIO_Port GPIOF
#define RF_LE_Pin GPIO_PIN_6
#define RF_LE_GPIO_Port GPIOC
#define RF_DATA_Pin GPIO_PIN_13
#define RF_DATA_GPIO_Port GPIOJ
#define RF_CLK_Pin GPIO_PIN_4
#define RF_CLK_GPIO_Port GPIOA

#define LO_LE_Pin GPIO_PIN_12
#define LO_LE_GPIO_Port GPIOD
#define LO_DATA_Pin GPIO_PIN_15
#define LO_DATA_GPIO_Port GPIOB
#define LO_CLK_Pin GPIO_PIN_14
#define LO_CLK_GPIO_Port GPIOB
#define LO_LD_Pin GPIO_PIN_13
#define LO_LD_GPIO_Port GPIOD

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

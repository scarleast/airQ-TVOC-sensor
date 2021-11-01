/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f0xx_hal.h"

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
#define PWR_EN_Pin GPIO_PIN_0
#define PWR_EN_GPIO_Port GPIOA
#define POWER_EN_ADC_Pin GPIO_PIN_1
#define POWER_EN_ADC_GPIO_Port GPIOA
#define BAT_ADC_Pin GPIO_PIN_4
#define BAT_ADC_GPIO_Port GPIOA
#define Key1_Pin GPIO_PIN_7
#define Key1_GPIO_Port GPIOA
#define Key1_EXTI_IRQn EXTI4_15_IRQn
#define Key2_Pin GPIO_PIN_0
#define Key2_GPIO_Port GPIOB
#define Key2_EXTI_IRQn EXTI0_1_IRQn
#define Key3_Pin GPIO_PIN_1
#define Key3_GPIO_Port GPIOB
#define Key3_EXTI_IRQn EXTI0_1_IRQn
#define LED0_Pin GPIO_PIN_2
#define LED0_GPIO_Port GPIOB
#define CCS_WAKE_Pin GPIO_PIN_12
#define CCS_WAKE_GPIO_Port GPIOB
#define CCS_INT_Pin GPIO_PIN_13
#define CCS_INT_GPIO_Port GPIOB
#define CCS_INT_EXTI_IRQn EXTI4_15_IRQn
#define CCS_RST_Pin GPIO_PIN_14
#define CCS_RST_GPIO_Port GPIOB
#define CCS_ADD_Pin GPIO_PIN_15
#define CCS_ADD_GPIO_Port GPIOB
#define WB3S_KEY_Pin GPIO_PIN_8
#define WB3S_KEY_GPIO_Port GPIOA
#define WB3S_EN_Pin GPIO_PIN_11
#define WB3S_EN_GPIO_Port GPIOA
#define WB3S_CEN_Pin GPIO_PIN_12
#define WB3S_CEN_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

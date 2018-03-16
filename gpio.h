/**
  ******************************************************************************
  * File Name          : gpio.h
  * Description        : This file contains all the functions prototypes for 
  *                      the gpio  
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __gpio_H
#define __gpio_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */
#define SD_CS_PIN    GPIO_PIN_4
#define SD_CS_PORT   GPIOB

#define TP_CS_PIN    GPIO_PIN_5
#define TP_CS_PORT   GPIOB

#define LCD_CS_PIN    GPIO_PIN_6
#define LCD_CS_PORT   GPIOB   
   
#define RS_PIN   GPIO_PIN_4
#define R_W_PIN GPIO_PIN_13
#define E_PIN   GPIO_PIN_14
#define DB0_PIN GPIO_PIN_15
#define DB1_PIN GPIO_PIN_1
#define DB2_PIN GPIO_PIN_2
#define DB3_PIN GPIO_PIN_12
#define DB4_PIN GPIO_PIN_11
#define DB5_PIN GPIO_PIN_12
#define DB6_PIN GPIO_PIN_5
#define DB7_PIN GPIO_PIN_6
   
#define RS_PORT  GPIOC
#define R_W_PORT GPIOB
#define E_PORT   GPIOB
#define DB0_PORT GPIOB
#define DB1_PORT GPIOB
#define DB2_PORT GPIOB
#define DB3_PORT GPIOB
#define DB4_PORT GPIOA
#define DB5_PORT GPIOA
#define DB6_PORT GPIOC
#define DB7_PORT GPIOC

#define PEN_SENSOR_PIN  GPIO_PIN_0
#define PEN_SENSOR_PORT GPIOB
   
#define BUTTON_PORT  GPIOA
#define BUTTON_PIN   GPIO_PIN_6
   
/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */
void WS0010_Pin_RW_Write(int lev);

void WS0010_Pin_RW_Write(int lev);
void WS0010_Set_Data(unsigned char  data);
void WS0010_Pin_Enable_Write(int lev);
void set_data_dir_write();

void enable_drv8825();
void disable_drv8825();
void init_drv8825();
void toggle_led();
void init_micro_step();
void init_button();
void init_touch_sensor();

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

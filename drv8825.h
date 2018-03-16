/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __drv8825_H
#define __drv8825_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */
#define STEP_Z_PIN    GPIO_PIN_3
#define STEP_Z_PORT   GPIOC

#define DIR_X_PIN    GPIO_PIN_9
#define DIR_X_PORT   GPIOA

#define DIR_Y_PIN    GPIO_PIN_8
#define DIR_Y_PORT   GPIOB   

#define DIR_Z_PIN    GPIO_PIN_2
#define DIR_Z_PORT   GPIOC

#define M0_Z_PIN    GPIO_PIN_9
#define M0_Z_PORT   GPIOB

#define M1_Z_PIN    GPIO_PIN_1
#define M1_Z_PORT   GPIOA

#define M2_Z_PIN    GPIO_PIN_4
#define M2_Z_PORT   GPIOA
   
#define M0_XY_PIN    GPIO_PIN_10
#define M0_XY_PORT   GPIOA

#define M1_XY_PIN    GPIO_PIN_3
#define M1_XY_PORT   GPIOB

#define M2_XY_PIN    GPIO_PIN_5
#define M2_XY_PORT   GPIOB

#define ENABLE_Z_PIN    GPIO_PIN_7
#define ENABLE_Z_PORT   GPIOB
   
#define ENABLE_XY_PIN    GPIO_PIN_4
#define ENABLE_XY_PORT   GPIOB

#define RST_SLEEP_Z_PIN    GPIO_PIN_1
#define RST_SLEEP_Z_PORT   GPIOC
 
#define RST_SLEEP_XY_PIN    GPIO_PIN_10
#define RST_SLEEP_XY_PORT   GPIOB

#define FAULT_Z_PIN    GPIO_PIN_0
#define FAULT_Z_PORT   GPIOC

#define FAULT_X_PIN    GPIO_PIN_7
#define FAULT_X_PORT   GPIOC
   
#define FAULT_Y_PIN    GPIO_PIN_3
#define FAULT_Y_PORT   GPIOA

#define  ENABLE_XT_DRV8825_PIN     GPIO_PIN_4
#define  ENABLE_XT_DRV8825_PORT    GPIOB

#define   FULL_STEP 0   
#define   HALF_STEP 1
#define   STEP_1_4 2
#define   STEP_1_8 3
#define   STEP_1_16 4
#define   STEP_1_32 5

extern int g_z_step_diff_up;
extern int g_z_step_diff_down;
   
void drv8825_init_z_pins();
void drv8825_init_xy_pins();

void drv8825_set_u_step_mode_z(int mode);
void drv8825_set_u_step_mode_xy(int mode);

void drv8825_enable_xy();
void drv8825_disable_xy();
void drv8825_init_xy();


   
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif 

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

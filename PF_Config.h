/**
  ******************************************************************************
  * @file    PF_Config.h
  * @author  Waveshare Team
  * @version 
  * @date    13-October-2014
  * @brief     Header file for PF_Config.c
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, WAVESHARE SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  ******************************************************************************
  */



/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _USE_PF_CONFIG_H_
#define _USE_PF_CONFIG_H_

/* Includes ------------------------------------------------------------------*/
#include "MacroAndConst.h"

extern uint16_t g_hwDevId;

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
extern void system_init(void);
extern void sd_card_config(void);
extern void display_images(void);

#endif

/*-------------------------------END OF FILE-------------------------------*/


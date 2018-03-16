/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body

/* Includes ------------------------------------------------------------------*/
// max speed 5 mms
// 

#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "gpio.h"
#include "tim.h"
#include "sdio.h"
#include "ws0010.h"
#include "fatfs.h"
#include "utils.h"
#include "rtc.h"

// to log:
// g_skipLinesCnt
char author[80]="Alexander_Barunin_aabzel@yandex.ru";

static void Error_Handler(void);
 
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);


void read_TouchSensor()
{
  HAL_GPIO_WritePin(SD_CS_PORT, SD_CS_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(TP_CS_PORT, TP_CS_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);  
  tp_init();
  
  HAL_GPIO_WritePin(SD_CS_PORT, SD_CS_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(TP_CS_PORT, TP_CS_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);  
}

void init_rand()
{
  int a=0, signe,r ;
  a = rand();
  a = rand(); 
  
  for(int a=0;a<10;a++) {
    r = rand();
    signe = 1-r%3;//-1  0 1
  }
}

void init_system() {
  g_offsetX = 0.0;
  g_offsetY = 0.0;
  g_corrDiff.X=0;
  g_corrDiff.Y=0;
  g_stepLen = (PI_RAD*g_rSheave*STEP_ANGLE)/(180*U_STEP_MODE);//=0.056875
  
  init_flags();  
  
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM5_Init();
  
  MX_RTC_Init();

  MX_SDIO_SD_Init();
  MX_FATFS_Init();
  
  WS0010_Init();
  
  init_rand();
}

int main(void)
{
  delay_ms(1000);

  if(HAL_Init()!= HAL_OK) {
    Error_Handler(); 
  }

  SystemClock_Config();

  init_system();

  set_graph_mode();
  glcd_text57("Verify\n\rG-code...\n\r");
  display_buf();
  
  g_res = initTxtFile();
  if (g_res) {
   int a =-1;
   errorAlarm(g_res);
  }
  
  clear_buf();
  reset_caret();
  glcd_text57("verify\n\rdone!");
  display_buf();

  MX_FREERTOS_Init();

  vTaskStartScheduler();
  
  while (1) {

  }

}



static void Error_Handler(void)
{
  while(1)
  {
  }
}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  
  /* Enable HSI Oscillator and activate PLL with HSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI| RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  
    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
 
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    int a=1;
    //assert_failed(__FILE__, __LINE__);
  }

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 3, 0);
}



#ifdef USE_FULL_ASSERT


void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

}

#endif




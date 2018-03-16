/**
  ******************************************************************************
  * File Name          : TIM.c
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
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

/* Includes ------------------------------------------------------------------*/
#include "tim.h"
#include "utils.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim5;


/* USER CODE BEGIN 0 */
void setTim2Period(U32 period) //X
{
  if(4<period)
  {
    __HAL_TIM_SET_COUNTER(&htim2, 0) ;
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, period/2) ;
    __HAL_TIM_SET_AUTORELOAD(&htim2, period) ;
  }
}

void setTim5Period(U32 period) //Y
{
  if (4<period) {
    __HAL_TIM_SET_COUNTER(&htim5, 0) ;
    __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, period/2) ;
    __HAL_TIM_SET_AUTORELOAD(&htim5, period) ;
  }
}


/* TIM1 init function */
void MX_TIM1_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 100;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period            = 0xFFFF;
  htim1.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  
  HAL_TIM_Base_Init(&htim1);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig);
  
  HAL_TIM_Base_Start(&htim1);

}


void MX_TIM2_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim2.Instance           = TIM2;
  htim2.Init.Prescaler     = 42;
  htim2.Init.CounterMode   = TIM_COUNTERMODE_UP;
  htim2.Init.Period        = 200000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htim2);
  
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);
  
  HAL_TIM_OC_Init(&htim2);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_OC1;
  sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
  
  HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);

  sConfigOC.OCMode     = TIM_OCMODE_PWM1;
  sConfigOC.Pulse      = 100000;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1);
  
  HAL_TIM_Base_Start_IT(&htim2);

  HAL_TIM_OC_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_1);
  
  HAL_TIM_Base_Stop(&htim2);
  
  setTim2Period(200000);
  
  
  test_timer2();
}

void test_timer2()
{
  setTim2Period(200000);
  HAL_TIM_Base_Start(&htim2);
  HAL_TIM_Base_Start_IT(&htim2);
//10hz
  HAL_TIM_OC_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_1);

  HAL_Delay(3000);  
  HAL_TIM_Base_Stop(&htim2);
}

void test_timer5()
{
  setTim5Period(200000);
  HAL_TIM_Base_Start(&htim5);
  HAL_TIM_Base_Start_IT(&htim5);

  HAL_TIM_OC_Start(&htim5, TIM_CHANNEL_1);
  HAL_TIM_OC_Start_IT(&htim5, TIM_CHANNEL_1);

  HAL_Delay(3000);  
  HAL_TIM_Base_Stop(&htim5);
}


/* TIM5 init function */
void MX_TIM5_Init(void)
{
 // TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim5.Instance           = TIM5;
  htim5.Init.Prescaler     = 42;
  htim5.Init.CounterMode   = TIM_COUNTERMODE_UP;
  htim5.Init.Period        = 200000;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htim5);
  
  HAL_TIM_OC_Init(&htim5);

 // sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
 // HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_OC1;
  sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
  
  HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig);

  sConfigOC.OCMode     = TIM_OCMODE_PWM1;
  sConfigOC.Pulse      = 100000;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  
  HAL_TIM_OC_ConfigChannel(&htim5, &sConfigOC, TIM_CHANNEL_1);

  HAL_TIM_Base_Start_IT(&htim5);
  
  HAL_TIM_OC_Start(&htim5, TIM_CHANNEL_1);
  HAL_TIM_OC_Start_IT(&htim5, TIM_CHANNEL_1);
  
  HAL_TIM_Base_Stop(&htim5);
  
  setTim5Period(200000);
  
  test_timer5();
}


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim_base->Instance==TIM1)
  {
  /* USER CODE BEGIN TIM1_MspInit 0 */

  /* USER CODE END TIM1_MspInit 0 */
    /* Peripheral clock enable */
    __TIM1_CLK_ENABLE();

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
    HAL_NVIC_SetPriority(TIM1_CC_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
  /* USER CODE BEGIN TIM1_MspInit 1 */

  /* USER CODE END TIM1_MspInit 1 */
  }
  if(htim_base->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspInit 0 */

  /* USER CODE END TIM2_MspInit 0 */
    /* Peripheral clock enable */
    __TIM2_CLK_ENABLE();
  
    /**TIM2 GPIO Configuration    
    PA5     ------> TIM2_CH1 
    */
    GPIO_InitStruct.Pin       = GPIO_PIN_5;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
  /* USER CODE BEGIN TIM2_MspInit 1 */

  /* USER CODE END TIM2_MspInit 1 */
  }
  else if(htim_base->Instance==TIM5)
  {
  /* USER CODE BEGIN TIM5_MspInit 0 */

  /* USER CODE END TIM5_MspInit 0 */
    /* Peripheral clock enable */
    __TIM5_CLK_ENABLE();
  
    /**TIM5 GPIO Configuration    
    PA0     ------> TIM5_CH1 
    */
    GPIO_InitStruct.Pin       = GPIO_PIN_0;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(TIM5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM5_IRQn);
  /* USER CODE BEGIN TIM5_MspInit 1 */

  /* USER CODE END TIM5_MspInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim_base->Instance==TIM1)
  {
  /* USER CODE BEGIN TIM1_MspDeInit 0 */

  /* USER CODE END TIM1_MspDeInit 0 */
    /* Peripheral clock disable */
    __TIM1_CLK_DISABLE();

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM1_UP_TIM10_IRQn);

    HAL_NVIC_DisableIRQ(TIM1_CC_IRQn);

  }
  if(htim_base->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspDeInit 0 */

  /* USER CODE END TIM2_MspDeInit 0 */
    /* Peripheral clock disable */
    __TIM2_CLK_DISABLE();
  
    /**TIM2 GPIO Configuration    
    PA5     ------> TIM2_CH1 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM2_IRQn);

  /* USER CODE BEGIN TIM2_MspDeInit 1 */

  /* USER CODE END TIM2_MspDeInit 1 */
  }
  else if(htim_base->Instance==TIM5)
  {
  /* USER CODE BEGIN TIM5_MspDeInit 0 */

  /* USER CODE END TIM5_MspDeInit 0 */
    /* Peripheral clock disable */
    __TIM5_CLK_DISABLE();
  
    /**TIM5 GPIO Configuration    
    PA3     ------> TIM5_CH4 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_3);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM5_IRQn);

  /* USER CODE BEGIN TIM5_MspDeInit 1 */

  /* USER CODE END TIM5_MspDeInit 1 */
  }
  /* USER CODE BEGIN TIM1_MspDeInit 1 */

  /* USER CODE END TIM1_MspDeInit 1 */
} 

/* USER CODE BEGIN 1 */
void HAL_TIM_OC_MspInit(TIM_HandleTypeDef* htim_oc)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim_oc->Instance==TIM2)
  {
  } else if (htim_oc->Instance==TIM5)
  {
  }
}

void HAL_TIM_OC_MspDeInit(TIM_HandleTypeDef* htim_oc)
{
  if(htim_oc->Instance==TIM2)
  {

  }
  else if(htim_oc->Instance==TIM5)
  {

  }
} 

void HAL_TIM_ErrorCallback(TIM_HandleTypeDef *htim)
{
  if(TIM2==htim->Instance) {
  }
  if (TIM4==htim->Instance) {
    int a=1;
  }
  if(TIM5==htim->Instance) {
  }
}

/* USER CODE BEGIN 1 */
/**
  * @brief  Period elapsed callback in non blocking mode 
  * @param  htim: pointer to a TIM_HandleTypeDef structure that contains
  *                the configuration information for TIM module.
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if(TIM2==htim->Instance) { 
    if(1==g_curMovement.dirX) {
      //HAL_GPIO_WritePin(LD5_BOARD_RED_PORT,   LD5_BOARD_RED_PIN, GPIO_PIN_RESET);
      //HAL_GPIO_WritePin(LD4_BOARD_GREEN_PORT, LD4_BOARD_GREEN_PIN, GPIO_PIN_RESET);
    }
    if(-1==g_curMovement.dirX) {
      //HAL_GPIO_WritePin(LD4_BOARD_GREEN_PORT, LD4_BOARD_GREEN_PIN, GPIO_PIN_RESET);
      //HAL_GPIO_WritePin(LD5_BOARD_RED_PORT, LD5_BOARD_RED_PIN, GPIO_PIN_RESET);
    }
  }
  
  if (TIM3 == htim->Instance) {
    //int b=1;
    //tick_soft_timer();
  }
  
  if (TIM4==htim->Instance) {
    //int a=1;
  }
    
  if(TIM5==htim->Instance) {
    if(1==g_curMovement.dirY) {
      //HAL_GPIO_WritePin(LD6_BOARD_BLUE_PORT, LD6_BOARD_BLUE_PIN, GPIO_PIN_RESET); 
      //HAL_GPIO_WritePin(LD3_BOARD_ORANGE_PORT, LD3_BOARD_ORANGE_PIN, GPIO_PIN_RESET);
    }
    if(-1==g_curMovement.dirY) {
      //HAL_GPIO_WritePin(LD3_BOARD_ORANGE_PORT, LD3_BOARD_ORANGE_PIN, GPIO_PIN_RESET);
      //HAL_GPIO_WritePin(LD6_BOARD_BLUE_PORT, LD6_BOARD_BLUE_PIN, GPIO_PIN_RESET); 
    }
   
  }
}


/**
  * @brief  Output Compare callback in non blocking mode 
  * @param  htim: pointer to a TIM_HandleTypeDef structure that contains
  *                the configuration information for TIM module.
  * @retval None
  */
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (TIM2==htim->Instance) {
    if (1==g_curMovement.dirX) {
      //HAL_GPIO_WritePin(LD5_BOARD_RED_PORT, LD5_BOARD_RED_PIN, GPIO_PIN_SET);
      //HAL_GPIO_WritePin(LD4_BOARD_GREEN_PORT, LD4_BOARD_GREEN_PIN, GPIO_PIN_RESET);
      g_Xpos++;
    }
    if (-1==g_curMovement.dirX) {
      //HAL_GPIO_WritePin(LD4_BOARD_GREEN_PORT, LD4_BOARD_GREEN_PIN, GPIO_PIN_SET);
      //HAL_GPIO_WritePin(LD5_BOARD_RED_PORT, LD5_BOARD_RED_PIN, GPIO_PIN_RESET);
      g_Xpos--;
    }
    
    if (g_curMovement.amountOfstepsX<=0) {
      HAL_TIM_Base_Stop(&htim2);
      __HAL_TIM_SET_COUNTER(&htim2, 0) ;
      flag.xMotionDone=1;
      //HAL_GPIO_WritePin(LD4_BOARD_GREEN_PORT, LD4_BOARD_GREEN_PIN, GPIO_PIN_RESET);
      //HAL_GPIO_WritePin(LD5_BOARD_RED_PORT, LD5_BOARD_RED_PIN, GPIO_PIN_RESET);
    }
    g_curMovement.amountOfstepsX--;
  }
  
  if (TIM4==htim->Instance) {
    int a=1;
  }
  
  if (TIM5==htim->Instance) {
    if (1==g_curMovement.dirY) {
      //HAL_GPIO_WritePin(LD6_BOARD_BLUE_PORT, LD6_BOARD_BLUE_PIN, GPIO_PIN_SET); 
      //HAL_GPIO_WritePin(LD3_BOARD_ORANGE_PORT, LD3_BOARD_ORANGE_PIN, GPIO_PIN_RESET); 
      g_Ypos++;
    }
    if (-1==g_curMovement.dirY) {
      //HAL_GPIO_WritePin(LD3_BOARD_ORANGE_PORT, LD3_BOARD_ORANGE_PIN, GPIO_PIN_SET);
      //HAL_GPIO_WritePin(LD6_BOARD_BLUE_PORT, LD6_BOARD_BLUE_PIN, GPIO_PIN_RESET); 
      g_Ypos--;
    }
    if(g_curMovement.amountOfstepsY<=0) {
      HAL_TIM_Base_Stop(&htim5);
      __HAL_TIM_SET_COUNTER(&htim5, 0) ;
      flag.yMotionDone=1;
      //HAL_GPIO_WritePin(LD6_BOARD_BLUE_PORT, LD6_BOARD_BLUE_PIN, GPIO_PIN_RESET); 
      //HAL_GPIO_WritePin(LD3_BOARD_ORANGE_PORT, LD3_BOARD_ORANGE_PIN, GPIO_PIN_RESET);
    }
    g_curMovement.amountOfstepsY--;
  }
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

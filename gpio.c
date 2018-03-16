
#include "gpio.h"
#include "utils.h"

void WS0010_Pin_RS_Write(int lev)
{
  if(lev==1){
    HAL_GPIO_WritePin(RS_PORT  ,RS_PIN, GPIO_PIN_SET);
  }else{
    HAL_GPIO_WritePin(RS_PORT  ,RS_PIN, GPIO_PIN_RESET);
  }
}

void WS0010_Pin_RW_Write(int lev)
{
  if(lev==1){
    HAL_GPIO_WritePin(R_W_PORT  ,R_W_PIN, GPIO_PIN_SET);
  }else{
    HAL_GPIO_WritePin(R_W_PORT  ,R_W_PIN, GPIO_PIN_RESET);
  }
}

void WS0010_Pin_Enable_Write(int lev)
{
  if(lev==1){
    HAL_GPIO_WritePin(E_PORT  ,E_PIN, GPIO_PIN_SET);
  }else{
    HAL_GPIO_WritePin(E_PORT  ,E_PIN, GPIO_PIN_RESET);
  }
}


void set_data_dir_write()
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  GPIO_InitStruct.Pin = DB0_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(DB0_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = DB1_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(DB1_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = DB2_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(DB2_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = DB3_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(DB3_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = DB4_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(DB4_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = DB5_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(DB5_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = DB6_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(DB6_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = DB7_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(DB7_PORT, &GPIO_InitStruct);
}

uint8_t read_data()
{
  uint8_t data;
  GPIO_PinState state;
  
  state = HAL_GPIO_ReadPin(DB0_PORT, DB0_PIN);
  if (state) {
    data |= (1<<0);
  }
  state = HAL_GPIO_ReadPin(DB1_PORT, DB1_PIN);
  if (state) {
    data |= (1<<1);
  }
  state = HAL_GPIO_ReadPin(DB2_PORT, DB2_PIN);
  if (state) {
    data |= (1<<2);
  }
  state = HAL_GPIO_ReadPin(DB3_PORT, DB3_PIN);
  if (state) {
    data |= (1<<3);
  }
  state = HAL_GPIO_ReadPin(DB4_PORT, DB4_PIN);
  if (state) {
    data |= (1<<4);
  }
  state = HAL_GPIO_ReadPin(DB5_PORT, DB5_PIN);
  if (state) {
    data |= (1<<5);
  }
  state = HAL_GPIO_ReadPin(DB6_PORT, DB6_PIN);
  if (state) {
    data |= (1<<6);
  }
  state = HAL_GPIO_ReadPin(DB7_PORT, DB7_PIN);
  if (state) {
    data |= (1<<7);
  }
  return data;
  
}

void set_data_dir_read()
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  GPIO_InitStruct.Pin = DB0_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(DB0_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = DB1_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(DB1_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = DB2_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(DB2_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = DB3_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(DB3_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = DB4_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(DB4_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = DB5_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(DB5_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = DB6_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(DB6_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = DB7_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(DB7_PORT, &GPIO_InitStruct);  
}


void WS0010_Set_Data(unsigned char  data)
{
  set_data_dir_write();
  
  if(data&(1<<0)){
    HAL_GPIO_WritePin(DB0_PORT ,DB0_PIN, GPIO_PIN_SET);
  }else{
    HAL_GPIO_WritePin(DB0_PORT ,DB0_PIN, GPIO_PIN_RESET);
  }
  
  if(data&(1<<1)){
    HAL_GPIO_WritePin(DB1_PORT ,DB1_PIN, GPIO_PIN_SET);
  }else{
    HAL_GPIO_WritePin(DB1_PORT ,DB1_PIN, GPIO_PIN_RESET);
  }
  if(data&(1<<2)){
    HAL_GPIO_WritePin(DB2_PORT ,DB2_PIN, GPIO_PIN_SET);
  }else{
    HAL_GPIO_WritePin(DB2_PORT ,DB2_PIN, GPIO_PIN_RESET);
  }

  if(data&(1<<3)){
    HAL_GPIO_WritePin(DB3_PORT ,DB3_PIN, GPIO_PIN_SET);
  }else{
    HAL_GPIO_WritePin(DB3_PORT ,DB3_PIN, GPIO_PIN_RESET);
  }

  if(data&(1<<4)){
    HAL_GPIO_WritePin(DB4_PORT ,DB4_PIN, GPIO_PIN_SET);
  }else{
    HAL_GPIO_WritePin(DB4_PORT ,DB4_PIN, GPIO_PIN_RESET);
  }

  if(data&(1<<5)){
    HAL_GPIO_WritePin(DB5_PORT ,DB5_PIN, GPIO_PIN_SET);
  }else{
    HAL_GPIO_WritePin(DB5_PORT ,DB5_PIN, GPIO_PIN_RESET);
  }

  if(data&(1<<6)){
    HAL_GPIO_WritePin(DB6_PORT ,DB6_PIN, GPIO_PIN_SET);
  }else{
    HAL_GPIO_WritePin(DB6_PORT ,DB6_PIN, GPIO_PIN_RESET);
  }

  if(data&(1<<7)){
    HAL_GPIO_WritePin(DB7_PORT ,DB7_PIN, GPIO_PIN_SET);
  }else{
    HAL_GPIO_WritePin(DB7_PORT ,DB7_PIN, GPIO_PIN_RESET);
  }  
}

void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();
  __GPIOC_CLK_ENABLE();
   __GPIOD_CLK_ENABLE();
   
  
  GPIO_InitStruct.Pin = RS_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(RS_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = R_W_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(R_W_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = E_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(E_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = DB0_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(DB0_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = DB1_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(DB1_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = DB2_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(DB2_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = DB3_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(DB3_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = DB4_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(DB4_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = DB5_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(DB5_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = DB6_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(DB6_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = DB7_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(DB7_PORT, &GPIO_InitStruct);
  
  
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  
  HAL_GPIO_WritePin(GPIOC  ,GPIO_PIN_12, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOC  ,GPIO_PIN_12, GPIO_PIN_RESET);
  
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  
  HAL_GPIO_WritePin(GPIOD  ,GPIO_PIN_2, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD  ,GPIO_PIN_2, GPIO_PIN_RESET);
  
 HAL_GPIO_WritePin(RS_PORT  ,RS_PIN, GPIO_PIN_SET);
 HAL_GPIO_WritePin(R_W_PORT ,R_W_PIN, GPIO_PIN_RESET);
 HAL_GPIO_WritePin(E_PORT   ,E_PIN, GPIO_PIN_SET);
 HAL_GPIO_WritePin(DB0_PORT ,DB0_PIN, GPIO_PIN_RESET);
 HAL_GPIO_WritePin(DB1_PORT ,DB1_PIN, GPIO_PIN_RESET);
 HAL_GPIO_WritePin(DB2_PORT ,DB2_PIN, GPIO_PIN_RESET);
 HAL_GPIO_WritePin( DB3_PORT ,DB3_PIN, GPIO_PIN_RESET);
 HAL_GPIO_WritePin(DB4_PORT ,DB4_PIN, GPIO_PIN_RESET);
 HAL_GPIO_WritePin(DB5_PORT ,DB5_PIN, GPIO_PIN_RESET);
 HAL_GPIO_WritePin(DB6_PORT ,DB6_PIN, GPIO_PIN_RESET);
 HAL_GPIO_WritePin(DB7_PORT, DB7_PIN, GPIO_PIN_RESET);
 
 
 init_drv8825_z_pins();
 init_drv8825_xy_pins();
 
 init_button();
 
 init_touch_sensor();
}

/* USER CODE BEGIN 2 */
void init_button()
{
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_PinState state;
  
  /* GPIO Ports Clock Enable */
  __GPIOA_CLK_ENABLE();

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin  = BUTTON_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(BUTTON_PORT, &GPIO_InitStruct);  
  
  state = HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN);
  
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);  
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (BUTTON_PIN == GPIO_Pin) {
    flag.penSensor = 1;
  }
  if (GPIO_PIN_13 == GPIO_Pin) {
    flag.btnPressed = 1;
    flag.firstTouch = 0;
  }
}


void init_touch_sensor()
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* GPIO Ports Clock Enable */
  __GPIOB_CLK_ENABLE();

  GPIO_InitStruct.Pin = PEN_SENSOR_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(PEN_SENSOR_PORT, &GPIO_InitStruct);  
  
  HAL_GPIO_ReadPin(PEN_SENSOR_PORT, PEN_SENSOR_PIN);
  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}
/* USER CODE END 2 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

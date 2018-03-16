
#include "gpio.h"
#include "drv8825.h"

int g_z_step_diff_up=7400;
int g_z_step_diff_down=2000;


void drv8825_enable_xy()
{
  HAL_GPIO_WritePin(ENABLE_XT_DRV8825_PORT, ENABLE_XT_DRV8825_PIN, GPIO_PIN_RESET);
}

void drv8825_disable_xy()
{
  HAL_GPIO_WritePin(ENABLE_XT_DRV8825_PORT, ENABLE_XT_DRV8825_PIN, GPIO_PIN_SET);
}

void drv8825_init_xy()
{
  
}

void init_drv8825_xy_pins()
{
  GPIO_InitTypeDef GPIO_InitStruct;
  __GPIOA_CLK_ENABLE();

  GPIO_InitStruct.Pin   = DIR_X_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(DIR_X_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin   = DIR_Y_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(DIR_Y_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin   = M2_XY_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(M2_XY_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin   = M1_XY_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(M1_XY_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin   = M0_XY_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(M0_XY_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin   = ENABLE_XY_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(ENABLE_XY_PORT, &GPIO_InitStruct);
  
  
  GPIO_InitStruct.Pin   = RST_SLEEP_XY_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(RST_SLEEP_XY_PORT, &GPIO_InitStruct);
  
  
  GPIO_InitStruct.Pin   = FAULT_X_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(FAULT_X_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin   = FAULT_Y_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(FAULT_Y_PORT, &GPIO_InitStruct);
  
  HAL_GPIO_WritePin(DIR_X_PORT, DIR_X_PIN, GPIO_PIN_RESET);  
  HAL_GPIO_WritePin(DIR_Y_PORT, DIR_Y_PIN, GPIO_PIN_RESET);
  
  HAL_GPIO_WritePin(RST_SLEEP_XY_PORT, RST_SLEEP_XY_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(ENABLE_XY_PORT, ENABLE_XY_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(M0_XY_PORT, M0_XY_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(M1_XY_PORT, M1_XY_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(M2_XY_PORT, M2_XY_PIN, GPIO_PIN_RESET);
  
  drv8825_set_u_step_mode_xy(STEP_1_32);

}


void init_drv8825_z_pins()
{
  GPIO_InitTypeDef GPIO_InitStruct;

  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();
  __GPIOC_CLK_ENABLE();
  
  GPIO_InitStruct.Pin   = STEP_Z_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(STEP_Z_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin   = DIR_Z_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(DIR_Z_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin   = M2_Z_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(M2_Z_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin   = M1_Z_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(M1_Z_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin   = M0_Z_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(M0_Z_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin   = ENABLE_Z_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(ENABLE_Z_PORT, &GPIO_InitStruct);
  
  
  GPIO_InitStruct.Pin   = RST_SLEEP_Z_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(RST_SLEEP_Z_PORT, &GPIO_InitStruct);
  
  
  GPIO_InitStruct.Pin   = FAULT_Z_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(FAULT_Z_PORT, &GPIO_InitStruct);

  HAL_GPIO_WritePin(DIR_Z_PORT, DIR_Z_PIN, GPIO_PIN_RESET);  
  HAL_GPIO_WritePin(STEP_Z_PORT, STEP_Z_PIN, GPIO_PIN_RESET);
  
  HAL_GPIO_WritePin(RST_SLEEP_Z_PORT, RST_SLEEP_Z_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(ENABLE_Z_PORT, ENABLE_Z_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(M0_Z_PORT, M0_Z_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(M1_Z_PORT, M1_Z_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(M2_Z_PORT, M2_Z_PIN, GPIO_PIN_RESET);
  
  drv8825_set_u_step_mode_z(STEP_1_32);


}

void drv8825_set_u_step_mode_z(int mode)
{
  if (mode==FULL_STEP) {
    HAL_GPIO_WritePin(M0_Z_PORT, M0_Z_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M1_Z_PORT, M1_Z_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M2_Z_PORT, M2_Z_PIN, GPIO_PIN_RESET);
  }
  if (mode==HALF_STEP) {
    HAL_GPIO_WritePin(M0_Z_PORT, M0_Z_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(M1_Z_PORT, M1_Z_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M2_Z_PORT, M2_Z_PIN, GPIO_PIN_RESET);
  }
  if (mode==STEP_1_4) {
    HAL_GPIO_WritePin(M0_Z_PORT, M0_Z_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M1_Z_PORT, M1_Z_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(M2_Z_PORT, M2_Z_PIN, GPIO_PIN_RESET);
  }  
  if (mode==STEP_1_8) {
    HAL_GPIO_WritePin(M0_Z_PORT, M0_Z_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(M1_Z_PORT, M1_Z_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(M2_Z_PORT, M2_Z_PIN, GPIO_PIN_RESET);
  }  
  if (mode==STEP_1_16) {
    HAL_GPIO_WritePin(M0_Z_PORT, M0_Z_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M1_Z_PORT, M1_Z_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M2_Z_PORT, M2_Z_PIN, GPIO_PIN_SET);
  }  
  if (mode==STEP_1_32) {
    HAL_GPIO_WritePin(M0_Z_PORT, M0_Z_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(M1_Z_PORT, M1_Z_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(M2_Z_PORT, M2_Z_PIN, GPIO_PIN_SET);
  }  
}

void drv8825_set_u_step_mode_xy(int mode)
{
  if (mode==FULL_STEP) {
    HAL_GPIO_WritePin(M0_XY_PORT, M0_XY_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M1_XY_PORT, M1_XY_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M2_XY_PORT, M2_XY_PIN, GPIO_PIN_RESET);
  }
  if (mode==HALF_STEP) {
    HAL_GPIO_WritePin(M0_XY_PORT, M0_XY_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(M1_XY_PORT, M1_XY_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M2_XY_PORT, M2_XY_PIN, GPIO_PIN_RESET);
  }
  if (mode==STEP_1_4) {
    HAL_GPIO_WritePin(M0_XY_PORT, M0_XY_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M1_XY_PORT, M1_XY_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(M2_XY_PORT, M2_XY_PIN, GPIO_PIN_RESET);
  }  
  if (mode==STEP_1_8) {
    HAL_GPIO_WritePin(M0_XY_PORT, M0_XY_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(M1_XY_PORT, M1_XY_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(M2_XY_PORT, M2_XY_PIN, GPIO_PIN_RESET);
  }  
  if (mode==STEP_1_16) {
    HAL_GPIO_WritePin(M0_XY_PORT, M0_XY_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M1_XY_PORT, M1_XY_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M2_XY_PORT, M2_XY_PIN, GPIO_PIN_SET);
  }  
  if (mode==STEP_1_32) {
    HAL_GPIO_WritePin(M0_XY_PORT, M0_XY_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(M1_XY_PORT, M1_XY_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(M2_XY_PORT, M2_XY_PIN, GPIO_PIN_SET);
  }  
}

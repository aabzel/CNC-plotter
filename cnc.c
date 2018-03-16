

#include "cnc.h"
#include "tim.h"
#include "gpio.h"
#include "utils.h"
#include "drv8825.h"
#include "ws0010.h"

#include <math.h>

#include <stdio.h>

int g_upCnt=0;
int g_downCnt=0;

int g_Xpos=0;
int g_Ypos=0;
double g_curAngle=0.0;
double g_curVeloc=0.0;
double g_curSpeed_x=0.0;
double g_curSpeed_y=0.0;

double g_velocity=4;// [mm/s]  5 <6   0.003m/s= 30mm/s
 
double g_maxVel=5.0;//6<7
double g_minVel=1.0;

double  g_stepLen;// 0.056875 ustep=2

struct simpleShift g_simpleShift;

struct RemainsErrorT errorCompensetion;
//double  g_stepLen=0.056875; // [mm]   0.05625<  <0.0575
                 
int g_skipLinesCnt=0;
int g_procLinesCnt=0;

//const int uSterMode=8;
double  g_stepXperiod=0.0;
double  g_stepYperiod=0.0;

double g_realCurXpos=0.0;
double g_realCurYpos=0.0;
double g_realCurZpos=0.0;

double g_virtCurXpos=0.0;
double g_virtCurYpos=0.0;
double g_virtCurZpos=0.0;

double g_aimXpos = 0.0;//[mm]
double g_aimYpos = 0.0;//[mm]

double g_offsetX = 0.0 ;// [steps]
double g_offsetY = 0.0 ;// [steps]


struct RemainsErrorT remainsError;


double g_offsetXerrorUp   = 0.0;
double g_offsetXerrorDown = 0.0;
double g_offsetYerrorUp   = 0.0;
double g_offsetYerrorDown = 0.0;

double g_xOffsetError=0.0;// [steps]
double g_yOffsetError=0.0;// [steps]

double g_compensationX=0.0;// [steps]
double g_compensationY=0.0;// [steps]

struct Vector g_corrDiff;

double g_rSheave = R_SHEAVE;

double g_displacementLength = 0.0;//[m]
double g_curDist = 0.0;
double g_totalDist = 0.0;

double g_displacementTime=0.0; 

double g_StepTimerClkPeriod =0.000001;
double g_StepTimerClkFreq   =1000000.0;

double g_totalTime =0.0;

double  g_velX =0.0  ;
double  g_velY =0.0  ;

double g_amountOfstepsX=0.0 ;
double g_amountOfstepsY=0.0 ;

int  g_dirX=0  ;
int  g_dirY=0 ;

double g_periodsX=0.0;
double g_periodsY=0.0;

struct movementT g_txMove;
struct movementT g_curMovement;

void proc_horizin_motion()
{
  if (g_penState == ST_PEN_DOWN) {
     draw_down();
  }
  //enable_drv8825();
  if (1==g_curMovement.dirX) {
    HAL_GPIO_WritePin(DIR_X_PORT, DIR_X_PIN, GPIO_PIN_SET); 
    draw_x_left();
  }
  if (-1==g_curMovement.dirX) {
   HAL_GPIO_WritePin(DIR_X_PORT, DIR_X_PIN, GPIO_PIN_RESET); 
   draw_x_right();
  }
 
  if (1==g_curMovement.dirY) {
   HAL_GPIO_WritePin(DIR_Y_PORT, DIR_Y_PIN, GPIO_PIN_SET); 
   draw_y_left();
  }
  
  if (-1==g_curMovement.dirY) {
   HAL_GPIO_WritePin(DIR_Y_PORT, DIR_Y_PIN, GPIO_PIN_RESET); 
   draw_y_right();
  }
  
  if ((1==g_curMovement.amountOfstepsX)  || (1==g_curMovement.amountOfstepsY) ) {
    int a=2;
  }
  
  if ((0 < g_curMovement.amountOfstepsX) && 
      (0 < g_curMovement.amountOfstepsY)) {
    flag.xMotionDone=0;
    flag.yMotionDone=0; 
    flag.zMotionDone=1; 
    setTim2Period(g_curMovement.timPeriodX);
    setTim5Period(g_curMovement.timPeriodY);
    HAL_TIM_Base_Start(&htim2);
    HAL_TIM_Base_Start(&htim5);
  }
  else if ((0==g_curMovement.amountOfstepsX) && 
           (0<g_curMovement.amountOfstepsY))  {
    flag.xMotionDone=1;
    flag.yMotionDone=0;
    flag.zMotionDone=1; 
    setTim5Period(g_curMovement.timPeriodY);//????
    HAL_TIM_Base_Stop(&htim2);
    __HAL_TIM_SET_COUNTER(&htim2, 0) ;
    
    HAL_TIM_Base_Start(&htim5);
  }
  else if ((0<g_curMovement.amountOfstepsX) && 
           (0==g_curMovement.amountOfstepsY)) {
    flag.xMotionDone=0;
    flag.yMotionDone=1;
    flag.zMotionDone=1; 
    setTim2Period(g_curMovement.timPeriodX);
    HAL_TIM_Base_Start(&htim2);
    HAL_TIM_Base_Stop(&htim5);
    __HAL_TIM_SET_COUNTER(&htim5, 0) ;
  }else if ((0==g_curMovement.amountOfstepsX) && 
            (0==g_curMovement.amountOfstepsY)) {
    flag.xMotionDone=1;
    flag.yMotionDone=1;
    flag.zMotionDone=1; 
    HAL_TIM_Base_Stop(&htim2);
    HAL_TIM_Base_Stop(&htim5);
    __HAL_TIM_SET_COUNTER(&htim2, 0) ;
    __HAL_TIM_SET_COUNTER(&htim5, 0) ;
  }  
}

void proc_vertical_motion()
{
    GPIO_PinState state;
    
    flag.xMotionDone=1;
    flag.yMotionDone=1;
    flag.zMotionDone=1;
    
    if (0==g_curMovement.penPosition) {
      flag.btnPressed = 0;
      if (!flag.firstTouch) {
        // wait button
        flag.pressBtnNotif = 1;
        flag.notifitationOccured = 1;
        state = HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN);
        while ( state==GPIO_PIN_SET) {
          state = HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN);
          flag.pressBtnNotif = 1;
          vTaskDelay(50);
       //   HAL_GPIO_TogglePin(PEN_POSITION_GPIO_PORT, PEN_POSITION_GPIO_PIN); 
          clear_buf();
          glcd_text57(" Button!");
          //display_buf_diff();
        }
        clear_buf();
        glcd_text57("  OK!\n\r Write.");
        //display_buf_diff();

        flag.pressBtnNotif = 0;
        flag.firstTouch    = 1;
        reset_clock();
      }
      drv8825_enable_xy();
      // write
      g_penState = ST_PEN_DOWN;
      glcd_text57("       d\n\r");
      put_down_pen();
      clear_buf();
      draw_down();
   
      g_downCnt++;
      flag.penPos = 1;
      
      vTaskDelay( 150);
    }
    if (10 == g_curMovement.penPosition) {
      // rise 
      g_penState = ST_PEN_UP;
      glcd_text57("       u\n\r");
      put_up_pen();
      clear_buf();
      draw_up();
      flag.penPos = 0;
      g_upCnt++;
      
      vTaskDelay( 175);
    }
}


void launthMotion(struct movementT *movement)
{
  memset(&g_curMovement, 0x00, sizeof(g_curMovement) );
  memcpy(&g_curMovement, movement, sizeof(g_curMovement) );

  g_curAngle = g_curMovement.angle;
  g_curVeloc = g_curMovement.speed;
  g_curSpeed_x = g_curMovement.speedX;
  g_curSpeed_y = g_curMovement.speedY;
  
  if (VERTICAL_MOTION == g_curMovement.typeOfMotion) {  
    
    sprintf ( g_tempStr, " %02d:%02dz\n\r", g_curTime.Minutes, g_curTime.Seconds);
    glcd_text57(g_tempStr);
    proc_vertical_motion();
  }
  
  if (HORIZONTAL_MOTION==g_curMovement.typeOfMotion) {
    sprintf ( g_tempStr, " %02d:%02dh\n\r", g_curTime.Minutes, g_curTime.Seconds);
    glcd_text57(g_tempStr);
    proc_horizin_motion();
  }
}


uint8_t check_motion(struct movementT *move)
{
  if (HORIZONTAL_MOTION==move->typeOfMotion) {
    if ((0==move->amountOfstepsX) && (0==move->amountOfstepsY)) {
      return 1;
    }
    
    if (0==move->amountOfstepsX) {
      if (0==move->timPeriodX) {
        return 2;
      }
    }
    
    if (0==move->amountOfstepsY) {
      if (0==move->timPeriodY) {
        return 3;
      }
    }
    
    if ((0xFFFFFFFF/2)<move->timPeriodX) {
      return 4;
    }
    if ((0xFFFFFFFF/2)<move->timPeriodY) {
      return 5;
    }
  }
  if (VERTICAL_MOTION==move->typeOfMotion) {
    if (11<move->penPosition) {
      return 4;
    }
  }
  return 0;

}

void stop_all()
{
  g_curMovement.amountOfstepsX=0;
  g_curMovement.amountOfstepsY=0;
  flag.xMotionDone=1;
  flag.yMotionDone=1;
  flag.zMotionDone=1; 
  HAL_TIM_Base_Stop(&htim2);
  HAL_TIM_Base_Stop(&htim5);
  setTim2Period(0);
  setTim5Period(0);
  __HAL_TIM_SET_COUNTER(&htim2, 0) ;
  __HAL_TIM_SET_COUNTER(&htim5, 0) ;  
}


void compensate_remain(struct movementT *move, double *xError, double *yError)
{
  double offset =1.0;  
  
  if (offset<*xError) {
    if(2<move->amountOfstepsX){
      move->amountOfstepsX--;
      *xError -=offset;
      errorCompensetion.x -=1.0;
    }
  } else if (*xError < -offset) {
      move->amountOfstepsX++;
      *xError += offset;
      errorCompensetion.x +=1.0;
  } else{
    int a=1;
  }
  if (offset < *yError) {
    if (2 < move->amountOfstepsY) {
      move->amountOfstepsY--;
      *yError -= offset;
      errorCompensetion.y -=1.0;
    }
  }else if(*yError < -offset) {
      move->amountOfstepsY++;
      *yError+=offset;
      errorCompensetion.y +=1.0;
  }else{
    int a=1;
  }
}


void put_down_pen()
{
  GPIO_PinState pinState;
  int a;
  HAL_GPIO_WritePin(DIR_Z_PORT, DIR_Z_PIN, GPIO_PIN_SET);
  while (1) {
    HAL_GPIO_WritePin(STEP_Z_PORT, STEP_Z_PIN, GPIO_PIN_RESET);
    delay_ms(1);
    HAL_GPIO_WritePin(STEP_Z_PORT, STEP_Z_PIN, GPIO_PIN_SET);
    delay_ms(1);
    
    pinState = HAL_GPIO_ReadPin(PEN_SENSOR_PORT, PEN_SENSOR_PIN);
    if (GPIO_PIN_RESET == pinState ) {
      return ;
    }
  }
}


void put_up_pen()
{
  GPIO_PinState pinState;
  int a;
  HAL_GPIO_WritePin(DIR_Z_PORT, DIR_Z_PIN, GPIO_PIN_RESET);
  pinState = HAL_GPIO_ReadPin(PEN_SENSOR_PORT, PEN_SENSOR_PIN);
  while (pinState==GPIO_PIN_RESET) {
    HAL_GPIO_WritePin(STEP_Z_PORT, STEP_Z_PIN, GPIO_PIN_RESET);
    delay_ms(1);
    HAL_GPIO_WritePin(STEP_Z_PORT, STEP_Z_PIN, GPIO_PIN_SET);
    delay_ms(1);
    pinState = HAL_GPIO_ReadPin(PEN_SENSOR_PORT, PEN_SENSOR_PIN);
  }
  
  for(a=0; a<g_z_step_diff_up; a++){
    HAL_GPIO_WritePin(STEP_Z_PORT, STEP_Z_PIN, GPIO_PIN_RESET);
    delay_ms(1);
    HAL_GPIO_WritePin(STEP_Z_PORT, STEP_Z_PIN, GPIO_PIN_SET);
    delay_ms(1);
  }
}

void remove_pen()
{
  GPIO_PinState pinState;
  int a;
  HAL_GPIO_WritePin(DIR_Z_PORT, DIR_Z_PIN, GPIO_PIN_RESET);
  pinState = HAL_GPIO_ReadPin(PEN_SENSOR_PORT, PEN_SENSOR_PIN);

  for(a=0; a<10000; a++) {
    HAL_GPIO_WritePin(STEP_Z_PORT, STEP_Z_PIN, GPIO_PIN_RESET);
    delay_ms(1);
    HAL_GPIO_WritePin(STEP_Z_PORT, STEP_Z_PIN, GPIO_PIN_SET);
    delay_ms(1);
  }
}



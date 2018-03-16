
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __cnc_H
#define __cnc_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "utils.h"


   

#define SPR (400.0)
#define STEP_ANGLE (360.0/SPR)
#define R_SHEAVE (0.625)// [mm]0.5 <0.75 1<1.5 
#define U_STEP_MODE (32.0)
   
   
//typeOfMotion
#define VERTICAL_MOTION      0x3C
#define HORIZONTAL_MOTION    0xC3
   
#define SIZE_OF_MOVE_QUEUE    (20)
     
   
struct  simpleShift {
  double deltaX;
  double deltaY;
  double deltaZ;
  double xylength;
  double angle ;
};//

extern struct simpleShift g_simpleShift;

struct movementT{
  uint8_t typeOfMotion;//1
  int amountOfstepsX; //4
  int amountOfstepsY; //4
  uint32_t timPeriodX;     //4
  uint32_t timPeriodY;     //4
  int dirX;           //4
  int dirY;           //4
  double targetXpos;   //6
  double targetYpos;   //6
  int penPosition;//4
  double angle; //6
  double speed; //6
  double speedX; //6
  double speedY; //6
};           //88

extern struct movementT g_txMove;
extern struct movementT g_curMovement;

extern double g_xOffsetError;// [steps]
extern double g_yOffsetError;// [steps]

//extern const int uSterMode ;   
   
extern double g_curSpeed_x ;
extern double g_curSpeed_y ;

extern double g_offsetX ;
extern double g_offsetY ;
extern double g_curAngle;
extern double g_curVeloc;

extern int g_upCnt;
extern int g_downCnt;

struct RemainsErrorT{
  double x;
  double y;
};    

extern struct RemainsErrorT remainsError;
extern struct RemainsErrorT errorCompensetion;

extern double g_compensationX;
extern double g_compensationY;

extern double g_rSheave;

extern int g_Xpos;
extern int g_Ypos;

extern double g_curXpos;
extern double g_curYpos;
   
extern double g_realCurXpos ;
extern double g_realCurYpos ;
extern double g_realCurZpos ;

extern double g_aimXpos;
extern double g_aimYpos;

struct Vector{
  double X;
  double Y;
};

extern struct Vector g_corrDiff;

extern double g_displacementLength;
extern double g_curDist;
extern double g_totalDist;

extern double g_displacementTime ; 

extern double g_velocity ;
extern double g_maxVel;
extern double g_minVel;

extern double g_totalTime  ;

extern double  g_velX  ;
extern double  g_velY  ;

extern double  g_stepLen;

extern double g_amountOfstepsX ;
extern double g_amountOfstepsY ;

extern double g_StepTimerClkPeriod ;
extern double g_StepTimerClkFreq;

extern int  g_dirX  ;
extern int  g_dirY ;

extern double  g_stepXperiod;
extern double  g_stepYperiod;

extern double g_periodsX;
extern double g_periodsY;

   
extern double g_virtCurXpos ;
extern double g_virtCurYpos ;
extern double g_virtCurZpos ;
  
extern int g_skipLinesCnt; 
extern int g_procLinesCnt;

uint8_t check_motion(struct movementT* move);

void stop_all();
void compensate_remains(struct movementT *move, 
                        struct RemainsErrorT *error);
void proc_vertical_motion();

void put_down_pen();
void put_up_pen();


#ifdef __cplusplus
}
#endif
#endif /* __cnc_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

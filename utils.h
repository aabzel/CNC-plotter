
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __utils_H
#define __utils_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "cnc.h"
#include "fifo.h"
   
#define SIZE_OF_ARRAY(Array) (sizeof(Array)/sizeof(Array[0]))

   
#define  INCORRECT_FILE_CONTENT_ERROR (1)
#define  MOUNT_ERROR (2)
#define  QUEUE_UNDEF_ERROR (3)
#define  X_DRV_ERROR (4)
#define  Y_DRV_ERROR (5)   
#define  UNKNOWN_CMD_ERROR (6) 
#define  INPUT_ERROR (7)
#define  LOAD_FIFO_ERROR (8)
#define  GENERAL_ERROR (9)
#define  CANNOT_READ_GCODE_LINE_ERROR (10)

#define LINE_FEED  0x0A

#define PI_RAD 3.14159265
   
#define ST_PEN_UP   0x00
#define ST_PEN_DOWN 0x01

#define DFLT_DISTORTION_PRECENT (0.01)

typedef uint32_t U32;
typedef uint16_t U16;
typedef uint8_t U8;

typedef int32_t I32;
typedef int16_t I16;
typedef int8_t I8;   

typedef struct{
  U8 xMotionDone;
  U8 yMotionDone;
  U8 zMotionDone;
  U8 firstTouch;
  U8 btnPressed;
  U8 queueAccess;
  U8 fullSdCardRead;
  U8 penPos;
  U8 loadQueueEmpty;
  U8 done;
  U8 pressBtnNotif;
  U8 notifitationOccured;
  U8 penSensor;
}status;

struct vector {
  double x;
  double y;
  double z;
};

double calcAngleBetweenVectors(struct vector *v1, struct vector  *v2);

extern status flag;

extern RTC_TimeTypeDef g_curTime;

extern int g_stackDepth;
extern HAL_StatusTypeDef  g_statusHal;   
extern uint8_t     g_fileOpen;

extern char g_readLineStr[110];

   
extern int g_amounOfByteInFile;
extern int g_numberOfBytesRead;
extern int g_amountOfLine;
extern int g_curLine;
extern int g_lengthOfLine;


extern uint8_t g_penState;

extern int g_maxFiFoLen;

extern U8 g_status, g_resuls, globalError;

extern int g_error    ;
extern int g_errorCnt;

extern char g_tempStr[100];

void drawDirrection(double angle, int penPos);
void pre_parse_xy_cmd(char *gCodeLineStr, int lenOfCmd) ;
void parseGcodeCmd(char *gCodeLineStr, int lenOdCmd);
void parseZcmd(char *gCodeLineStr, int lenOdCmd); 
void parseXYcmd(char *gCodeLineStr, int lenOfCmd);

int getNumber( char *str, int *number, int len);
U8 initTxtFile();
double mySign(double x);
U8 isCardPresent();
void calcPerCent(int numerator, int denominator,int *integerPerCent, int *fractionalPerCent);
double absFloat(double x);

void errorAlarm(U16 errorCode);
void init_flags();
double calcAngleBetweenVectors(struct vector *v1, struct vector  *v2);
double calcCurvature(struct fifo_t *fifo, int depth);


void init_var();
void drawVelocity(double vel) ;
void drawVel(double  speed_x, double speed_y);
void proc_simple_shift() ;

#ifdef __cplusplus
}
#endif
#endif /* __utils_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

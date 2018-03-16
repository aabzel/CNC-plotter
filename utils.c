

#include "utils.h"

#include "gpio.h"
#include <math.h>

#include "cnc.h"
#include "fifo.h"
#include "fatfs.h"

#include "sdio.h"
#include "cnc_Task.h"
#include "readSDcard_Task.h"

#include <stdio.h>


RTC_TimeTypeDef g_curTime;

int g_stackDepth=0;
char g_readLineStr[110];
 
double g_distortionPercent = DFLT_DISTORTION_PRECENT ; 

int g_curSample_i=0, g_maxProcLen, g_maxFiFoLen;
U8 g_status, g_resuls, globalError;
HAL_StatusTypeDef  g_statusHal;

uint8_t g_penState, g_cur_pen_proc_pos = 10;

uint8_t    g_fileOpen=0;

int32_t    g_Timer = 32*500; 
int32_t    g_Period= 200;

int g_amounOfByteInFile = 0;
int g_numberOfBytesRead = 0;
int g_amountOfLine      = 0;
int g_curLine           = 0;
int g_lengthOfLine      = 0;

double g_waveFrontAngleFiltered = 0;

int g_errorCnt         = 0;
int g_error            = 0;

int g_maxInd           = 0;
double g_timeDiff       = 0;
double g_waveFrontAngle = 0;

char g_tempStr[100];

status flag;

void  parseGcodeCmd(char *gCodeLineStr, int lenOdCmd)
{
  if (0 < lenOdCmd) {
    switch (gCodeLineStr[0]) {
      case 'X':
        parseXYcmd(gCodeLineStr, lenOdCmd);
        break;
      case 'Z':
        parseZcmd(gCodeLineStr, lenOdCmd);
        break;
      case 'G':
        //parseGcmd(gCodeLineStr, lenOdCmd);
        break;
    default:
      errorAlarm(GENERAL_ERROR);
    }  
  }
}

void  pre_parse_Gcode_cmd(char *gCodeLineStr, int lenOdCmd)
{
  if (0<lenOdCmd) {
    switch (gCodeLineStr[0]) {
      case 'X':
        pre_parse_xy_cmd(gCodeLineStr, lenOdCmd);
        g_amountOfLine++;
        break;
      case 'Z':
        //parseZcmd(gCodeLineStr, lenOdCmd);
        g_amountOfLine++;
        break;
      case 'G':
        //parseGcmd(gCodeLineStr, lenOdCmd);
        break;  
    default:
      errorAlarm(INCORRECT_FILE_CONTENT_ERROR);
    }  
  }
}

int getNumber( char *str, int *number, int len)
{
  int numOfItems=0;
  for(int i=0; i<len; i++)
  {  
    if(!(('0'<=*str) && (*str <= '9')) && (*str != '-') && (*str != '+'))
    { 
      str++;
    }
    else
    {
      break;
    }
  }
  numOfItems=sscanf(str, "%d", number) ;
  if (1==numOfItems) {
    return 0;
  }
  // No int found
  return -1; 
}

double degToRad(double angleDeg)
{
  return (angleDeg*PI_RAD/180.0);
}

struct Vector rotateVector(struct Vector *inVector, double angle_deg )
{
  struct Vector tempVector;
  double angle_rad;
  angle_rad=degToRad(angle_deg);

  tempVector.X = (cos(angle_rad)*inVector->X)-(sin(angle_rad)*inVector->Y);
  tempVector.Y = (sin(angle_rad)*inVector->X)+(cos(angle_rad)*inVector->Y);
  return tempVector;
}


double absFloat(double x)
{
  if (x<0) {
    return -x;
  } else {
    return x;
  }
}

void pre_parse_xy_cmd(char *gCodeLineStr, int lenOfCmd) 
{
  char tempCh;
  U8 checkStat=1;
  char *subStr;
  int numOfItems=0;
  float xPos=0.0, yPos=0.0;
  
  char patternX[5] = "X";  
  char patternY[5] = "Y";
  
  subStr =(char *) strstr((char *) gCodeLineStr,(char *) "\r" );
  *subStr='\0';
  
  subStr =(char *) strstr((char *) gCodeLineStr,(char *) patternX );
  numOfItems=sscanf((subStr+1), "%f ", &xPos) ;
  if (1!=numOfItems) {
    errorAlarm(INPUT_ERROR);
  }

  subStr =(char *) strstr((char *) gCodeLineStr,(char *) patternY );
  numOfItems=sscanf((subStr+1), "%f ",  &yPos) ;
  if (1!=numOfItems) {
    errorAlarm(INPUT_ERROR);
  }
  
  g_aimXpos = xPos;
  g_aimYpos = yPos;
  
  // calc diff
  g_corrDiff.X = g_aimXpos - g_virtCurXpos;
  g_corrDiff.Y = g_aimYpos - g_virtCurYpos;
  
  if ((0==g_corrDiff.X) &&(0==g_corrDiff.Y) ) {
    g_skipLinesCnt++;
    return;
  }
  
  g_displacementLength=sqrt((g_corrDiff.X)*(g_corrDiff.X)+(g_corrDiff.Y)*(g_corrDiff.Y));
  g_totalDist += g_displacementLength;
  
  g_displacementTime = g_displacementLength /( g_velocity);
  g_totalTime += g_displacementTime;
  
  g_virtCurXpos = g_aimXpos;
  g_virtCurYpos = g_aimYpos;
}

double xstep, ystep;

void parseXYcmd(char *gCodeLineStr, int lenOfCmd) {
  char tempCh;
  U8 checkStat=1;
  char *subStr;
  int numOfItems=0;
  float xPos=0.0, yPos=0.0;
  //numOfItems=sscanf(gCodeLineStr, "%f %f", xPos, yPos) ;
   memset(&g_simpleShift, 0x00, sizeof(g_simpleShift));
  char patternX[5] = "X";
  char patternY[5] = "Y";
  
  subStr =(char *) strstr((char *) gCodeLineStr,(char *) "\r" );
  *subStr='\0';
  xstep=0;
  ystep=0;
  subStr =(char *) strstr((char *) gCodeLineStr,(char *) patternX );
  numOfItems=sscanf((subStr+1), "%f ", &xPos) ;
  if (1!=numOfItems) {
    errorAlarm(INPUT_ERROR);
  }
  
  subStr =(char *) strstr((char *) gCodeLineStr,(char *) patternY );
  numOfItems = sscanf((subStr+1), "%f ",  &yPos) ;
  if ( 1!=numOfItems) {
    errorAlarm(INPUT_ERROR);
  }
  
  g_aimXpos = xPos;
  g_aimYpos = yPos;
  
  // calc diff
  g_corrDiff.X = g_aimXpos - g_virtCurXpos;
  g_corrDiff.Y = g_aimYpos - g_virtCurYpos;
  
  if((0.0 == g_corrDiff.X) && (0.0 == g_corrDiff.Y) ) {
    g_skipLinesCnt++;
    return;
  }else if( 10.0< g_corrDiff.X) {
    int a=1;
  }else if (50.0< g_corrDiff.Y) { 
    int debug=1;
  }

  g_displacementLength = sqrt((g_corrDiff.X)*(g_corrDiff.X)+(g_corrDiff.Y)*(g_corrDiff.Y));
  if (20.0 < g_displacementLength) {
    int a=0;
  }
  g_curDist += g_displacementLength;
  
  
  
  //rotate
  //g_corrDiff = rotateVector(&g_corrDiff, -45.0);
  
  g_simpleShift.deltaX = g_corrDiff.X;
  g_simpleShift.deltaY = g_corrDiff.Y;
  g_simpleShift.deltaZ = 0.0;
  g_simpleShift.xylength = g_displacementLength;
  
  struct vector vector1,vector2;
  vector1.x = 1.0;
  vector1.y = 0.0;
  vector1.z = 0.0;
  
  vector2.x = g_corrDiff.X;
  vector2.y = g_corrDiff.Y;
  vector2.z = 0.0;
  
  g_simpleShift.angle = calcAngleBetweenVectors(&vector1, &vector2);
  

  if (flag.queueAccess) {
      fifo_result_t fifo_result; 
      
      fifo_result= fifo_push(&objFifoLoaded, &g_simpleShift);
      if ( FIFO_SUCCESS != fifo_result) {
        int a = 4;
        errorAlarm(LOAD_FIFO_ERROR);
      }
  }

  g_virtCurXpos = g_aimXpos;
  g_virtCurYpos = g_aimYpos;
}

void parseZcmd(char *gCodeLineStr, int lenOfCmd)
{
  int z = 0, res;
  memset(&g_simpleShift, 0x00, sizeof(g_simpleShift));
  res = getNumber(gCodeLineStr, &z, lenOfCmd);
  if(res) {
    errorAlarm(GENERAL_ERROR);
  }
  if (0==z) {
    // move down Pen
    flag.queueAccess = 1;
    g_penState =ST_PEN_DOWN;
  }
  if (10 == z) {
    // elevate Pen
    g_penState = ST_PEN_UP;
  }
  g_virtCurZpos = z;
  
  g_simpleShift.deltaZ = z;
  g_simpleShift.deltaX = 0;
  g_simpleShift.deltaY = 0;
  g_simpleShift.xylength = 0; 
  
  fifo_result_t fifo_result; 
      
  fifo_result= fifo_push(&objFifoLoaded, &g_simpleShift);
  if ( FIFO_SUCCESS != fifo_result) {
     int a = 4;
     errorAlarm(LOAD_FIFO_ERROR);
  }    
}

double locAngle;


void add_rand_effect(struct movementT *move)
{
  int signe=0, delta=0,originalVal=0,allowableVar=0;
  if( 99==move->penPosition) {
    if (HORIZONTAL_MOTION==move->typeOfMotion) {
      if (ST_PEN_DOWN==g_cur_pen_proc_pos) {
        if(1==move->dirX ){
          if(0<move->amountOfstepsX) {
            originalVal = move->amountOfstepsX;
            signe = 1-rand()%3;//-1  0 1
            while(signe==-1)
            {
              signe = 1-rand()%3;//-1  0 1
            }
            allowableVar= move->amountOfstepsX*100.0/(100.0-g_distortionPercent);
            if (0 < allowableVar) {
              delta = rand()%allowableVar;
              move->amountOfstepsX += signe*delta;
              if(move->amountOfstepsX<0){
                move->amountOfstepsX = originalVal;
              }
            } else {
              int a=1;
            }

          }else{
              int a=2;
          }
        }
//        if (1==move->dirX){
//          if(0<move->amountOfstepsY) {
//            originalVal = move->amountOfstepsY;
//            signe = 1-rand()%3;//-1  0 1
//
//            allowableVar= move->amountOfstepsY*100.0/(100.0-g_distortionPercent);
//            if (0<allowableVar) {
//              delta = rand()%allowableVar;
//              move->amountOfstepsY += signe*delta;
//              if (move->amountOfstepsY<0) {
//                move->amountOfstepsY = originalVal;
//              }
//            } else {
//              int a=1;
//            }
//          
//          }else{
//              int a=4;
//          }
//        }
        
      }         
    } 
  }
  int a=5;
}

void proc_simple_shift() 
{
  U8 checkStat;
  fifo_result_t fifo_result;
  struct simpleShift locSimpleShift;

  g_sizeOfLoadedQueue = get_size_of_queue(&objFifoLoaded);
  
   if (!g_sizeOfLoadedQueue) {
     flag.loadQueueEmpty=1;
     return;
  }
      
  g_sizeOfMovementQueue = GetSizeOfMovementQueue();
  
  if (g_sizeOfMovementQueue < (SIZE_OF_MOVEMENT_QUEUE/3)) {
    
      fifo_result = fifo_pull( &objFifoLoaded, &locSimpleShift);
      if ( FIFO_SUCCESS != fifo_result ) {
        errorAlarm(LOAD_FIFO_ERROR);
      }
  
      if (0 == locSimpleShift.xylength) {
          g_txMove.typeOfMotion   = VERTICAL_MOTION; 
          g_txMove.amountOfstepsX = 0;
          g_txMove.amountOfstepsY = 0;
          g_txMove.dirX           = 0;
          g_txMove.dirY           = 0;  
          g_txMove.targetXpos     = 0;
          g_txMove.targetYpos     = 0;
          g_txMove.penPosition    = (int) locSimpleShift.deltaZ;
          g_txMove.timPeriodX     = 0;
          g_txMove.timPeriodY     = 0;
          g_txMove.angle          = locAngle;
          g_txMove.speed          = 0.0;
          g_txMove.speedX         = 0.0;
          g_txMove.speedY         = 0.0; 
          if (g_txMove.penPosition==10) {
            g_cur_pen_proc_pos = ST_PEN_UP;
          }
          if (g_txMove.penPosition==0) {
            g_cur_pen_proc_pos = ST_PEN_DOWN;
          }
            
      } else {
        g_txMove.typeOfMotion   = HORIZONTAL_MOTION;
        // calc g_velocity
        //curvature=calcCurvature(&objFifoLoaded ,5);
        double curvature=0.0;
        if (0 == flag.fullSdCardRead) {
            curvature = calcCurvature(&objFifoLoaded, 3);
        } else { 
            curvature = 0.3;
        }
      
        g_velocity = g_minVel+ (1.0-curvature)*(g_maxVel-g_minVel);
        //drawVelocity(g_velocity);
    
        g_displacementTime = locSimpleShift.xylength / g_velocity;
        g_totalTime += g_displacementTime;
      
        g_velX = (locSimpleShift.deltaX) / g_displacementTime;
        g_velY = (locSimpleShift.deltaY) / g_displacementTime;
  
        //cals dirrection
        struct vector v1, v2;
      
        v1.x = 1.0; v1.y=0.0; v1.z=0.0;
        v2.x = locSimpleShift.deltaX; v2.y=locSimpleShift.deltaY; v2.z=0.0;
        locAngle = calcAngleBetweenVectors(&v1, &v2);
      
        // how many steps required?
        g_amountOfstepsX = absFloat((locSimpleShift.deltaX)/(g_stepLen));
        g_amountOfstepsY = absFloat((locSimpleShift.deltaY)/(g_stepLen));
  
        // in whith direction shaft must be spun
        g_dirX = mySign( (locSimpleShift.deltaX));
        g_dirY = mySign( (locSimpleShift.deltaY));
  
        //calc time between steps 
        if (1.0 <= g_amountOfstepsX ) {
          g_periodsX = g_displacementTime/( g_amountOfstepsX ) ;
        } else {
          g_periodsX = 10*g_StepTimerClkPeriod ;
        }

        if (1.0 <= g_amountOfstepsY ) {
          g_periodsY = g_displacementTime/(g_amountOfstepsY );
        } else {
          g_periodsY = 10*g_StepTimerClkPeriod ;
        }
  
        g_StepTimerClkPeriod = 1.0/g_StepTimerClkFreq;

        // fill in the movement structure
        g_txMove.angle          = locAngle;
        g_txMove.speed          = g_velocity;
        g_txMove.amountOfstepsX = floor( g_amountOfstepsX+0.5);  //floor ceil 
        g_txMove.amountOfstepsY = floor( g_amountOfstepsY+0.5);
        g_txMove.amountOfstepsX = floor( g_amountOfstepsX);  //floor ceil 
        g_txMove.amountOfstepsY = floor( g_amountOfstepsY);  
        g_txMove.dirX           = g_dirX;
        g_txMove.dirY           = g_dirY;
        g_txMove.timPeriodX   = g_periodsX/g_StepTimerClkPeriod;
        g_txMove.timPeriodY   = g_periodsY/g_StepTimerClkPeriod;
        g_txMove.targetXpos     = g_aimXpos;
        g_txMove.targetYpos     = g_aimYpos;
        g_txMove.penPosition    = 99;
        g_txMove.speedX         = g_velX;
        g_txMove.speedY         = g_velY; 
  
        xstep = 1.0*g_txMove.amountOfstepsX;
        ystep = 1.0*g_txMove.amountOfstepsY;
        g_offsetX = g_amountOfstepsX - xstep; //[steps] remain
        g_offsetY = g_amountOfstepsY - ystep; //[steps] remain
        g_xOffsetError += g_offsetX;//[steps]
        g_yOffsetError += g_offsetY;//[steps] 

        //compensate_remain(&g_txMove, &g_xOffsetError, &g_yOffsetError);  
      }
  
      //add_rand_effect(&g_txMove);
      
      checkStat = check_motion(&g_txMove);
      if(!checkStat ) {
        //put movement in queue
        if (flag.queueAccess) {
          BaseType_t status;
          status= xQueueSend(xQueueMovement, &g_txMove, 0 );
          if(pdPASS!=status) {
            errorAlarm(GENERAL_ERROR);
          }
        }
      }else {
        g_skipLinesCnt++;
        g_error=5;
      }    
  }else {
      vTaskDelay( 1 );
  }
}

U8 initTxtFile()
{
  int intgPart, partialPart, prevIntPerCent=0;
    
  g_res=isCardPresent();
  if (g_res) {
    return 1;
  }
  
  int sizeOfArray = SIZE_OF_ARRAY(g_readLineStr);
 
  memset(g_readLineStr, 0xFF, sizeOfArray);
  
  g_amountOfLine      = 0 ;
  g_amounOfByteInFile = 0 ;
 
  //HAL_GPIO_WritePin(CARD_ACCESS_GPIO_PORT, CARD_ACCESS_GPIO_PIN, GPIO_PIN_SET); 
  g_res = f_mount(&fileSystem, SD_Path, 1);
  //HAL_GPIO_WritePin(CARD_ACCESS_GPIO_PORT, CARD_ACCESS_GPIO_PIN, GPIO_PIN_RESET); 
  if (FR_OK != g_res) { 
    //WS0010_PrintString("mout error");
    return MOUNT_ERROR;
  }
  glcd_text57("Verify\n\r");  
  //WS0010_PrintString("mout OK");
  g_res= openTxtFile();
  if (!g_res) {
    //find out the amount of lines in the file
    g_amounOfByteInFile =f_size (&g_testFile);

    g_fileOpen=1;
    while (g_numberOfBytesRead < g_amounOfByteInFile) {
      //HAL_IWDG_Refresh(&hiwdg);
      calcPerCent(g_numberOfBytesRead, g_amounOfByteInFile, &intgPart, &partialPart);
      if ( (0==intgPart%1) && (prevIntPerCent != intgPart)) {
        clear_buf();
        reset_caret();
        //glcd_rect(0, 0, 126, 62, 0, 1);
        sprintf ( g_tempStr, "\rVerify\n\r%d.%d %%\n\r",  intgPart, partialPart);
        //glcd_text57(g_tempStr); 
        int lenght;
        lenght = 124*((intgPart+0.1*partialPart)/100.0);
        //glcd_rect(0, 17, lenght, 25, 1, 1);
        glcd_text57(g_tempStr); 
        //glcd_rect(0, 17, lenght, 25, 1, 1);
        //WS0010_PrintString("Verify\n\rG-code");
        int bar=40*intgPart/100;
        draw_line(0, 15, bar, 15, 1);
        //glcd_rect(0, 8, bar, 15, 1, 1);
        //glcd_text57(g_tempStr); 

        prevIntPerCent = intgPart;
        display_buf_diff();
      }

      g_res = readLine(g_readLineStr, &g_lengthOfLine);
      if (!g_res) {
        //g_amountOfLine++;
        pre_parse_Gcode_cmd(g_readLineStr, g_lengthOfLine);
      } else {
        return 3;
      }
    }
          
    g_res= closeTxtFile();
    if (g_res) {  
      return 4;
    } 

  } else {
    return 2;
  }

  return 0;
}



void save_statistics()
{
  uint8_t stat=0;
  
 // HAL_GPIO_WritePin(CARD_ACCESS_GPIO_PORT, CARD_ACCESS_GPIO_PIN, GPIO_PIN_SET); 
  g_res = f_open(&g_logFile, &g_logFileName[0],  FA_OPEN_EXISTING  | FA_WRITE  );
 // HAL_GPIO_WritePin(CARD_ACCESS_GPIO_PORT, CARD_ACCESS_GPIO_PIN, GPIO_PIN_RESET);
  
  if ((FR_OK ==g_res)|| (FR_EXIST==g_res)) {
    stat=0;
  }
  else {
    stat=1;
    return;
  }  
  //Move to end of the file to append data 
  g_res = f_lseek(&g_logFile, f_size(&g_logFile));
  if (FR_OK ==g_res) {
    stat=0;
  }
  else {
    stat=1;
    return;
  }  
  
  stat= f_puts( "\n", &g_logFile);
  if (-1 == stat) {
    g_error=1;
  }
  
  sprintf ( g_tempStr, "Duration: %02d:%02d", g_curTime.Minutes, g_curTime.Seconds);
  stat= f_puts( g_tempStr, &g_logFile);
  if (-1 == stat) {
    g_error=1;
  }
  
  sprintf ( g_tempStr, " length: %d [mm]",(int) g_totalDist); 
  stat= f_puts( g_tempStr, &g_logFile);
  if (-1 == stat) {
    g_error=2;
  }
  
  sprintf ( g_tempStr, " speed: %d [mm/s]",(int) g_velocity); 
  stat= f_puts( g_tempStr, &g_logFile);
  if (-1 == stat) {
    g_error=3;
  }
  
  sprintf ( g_tempStr, " up movements: %d",(int) g_upCnt); 
  stat= f_puts( g_tempStr, &g_logFile);
  if (-1 == stat) {
    g_error=4;
  }
  
  sprintf ( g_tempStr, " down movements: %d",(int) g_downCnt);
  stat= f_puts( g_tempStr, &g_logFile);
  if (-1 == stat) {
    g_error=5;
  }
  
  sprintf ( g_tempStr, " X_offset: %d [steps]"
                     , (int) g_xOffsetError ); 
  stat= f_puts( g_tempStr, &g_logFile);
  if (-1 == stat) {
    g_error=6;
  }
  sprintf ( g_tempStr, " Y_offset: %d [steps]"                
                     , (int) g_yOffsetError ); 
  stat= f_puts( g_tempStr, &g_logFile);
  if (-1 == stat) {
    g_error=7;
  }
  
  //HAL_GPIO_WritePin(CARD_ACCESS_GPIO_PORT, CARD_ACCESS_GPIO_PIN, GPIO_PIN_SET); 
  g_res = f_close(&g_logFile);
  //HAL_GPIO_WritePin(CARD_ACCESS_GPIO_PORT, CARD_ACCESS_GPIO_PIN, GPIO_PIN_RESET); 
  
  if (FR_OK ==g_res) {
    stat=0;
  }
  else {
    stat=1;
  }
}

double mySign(double x)
{
  if (0<x) return 1.0;
  if (x < 0) return -1.0;
  return 0.0;
}

U8 isCardPresent()
{
  GPIO_PinState PinState;
  //PinState= HAL_GPIO_ReadPin(CARD_SENSOR_PORT, CARD_SENSOR_PIN);
  if(GPIO_PIN_RESET ==PinState)
  {
    return 0;
  }
  return 0;
}

void errorAlarm(U16 errorCode)
{
  //HAL_GPIO_WritePin(ERROR_GPIO_PORT, ERROR_GPIO_PIN, GPIO_PIN_SET); 
  //clear();
  switch (errorCode) {
  case CANNOT_READ_GCODE_LINE_ERROR:
    glcd_text57("SDread\n\rError.\n\r");
    //displayDes();
    break;
  case INCORRECT_FILE_CONTENT_ERROR:
    //glcd_text57("Incorrect file content.\n\r");
    //displayDes();
    break;
  case MOUNT_ERROR: 
    //glcd_text57("Card mount error.\n\r");
    //displayDes();
    break;
    
  case  X_DRV_ERROR: 
    //glcd_text57("X drv error.\n\r");
    //displayDes();
    break;
  case  LOAD_FIFO_ERROR:
    //glcd_text57("Load fifo error.\n\r");
    //displayDes();
    break;
   
  case  Y_DRV_ERROR: 
    //glcd_text57("Y drv error.\n\r");
    //displayDes();
    break;
  default:  
    //glcd_text57("Error.\n\r");
    //displayDes();
  }
  while (1) {
      HAL_Delay(200);

  }
}


void checkStack()
{
  g_stackDepth++;
  checkStack();
}


void display_real_pos()
{ 
  //glcd_text57("\n\r");
  sprintf ( g_tempStr, "X: %010.6f mm \n\r", g_realCurXpos);
  //glcd_text57(g_tempStr); 
  sprintf ( g_tempStr, "Y: %010.6f mm \n\r", g_realCurYpos);
  //glcd_text57(g_tempStr); 
  sprintf ( g_tempStr, "Z: %010.6f mm \n\r", g_realCurZpos);
  //glcd_text57(g_tempStr); 
}

void calcPerCent(int numerator, int denominator,int *integerPerCent, int *fractionalPerCent)
{
  *integerPerCent = numerator*100/denominator;
  *fractionalPerCent = numerator*1000/denominator - *integerPerCent*10;
}

void init_flags()
{
  flag.firstTouch    = 0;
  flag.btnPressed    = 0;
  flag.xMotionDone   = 0;
  flag.yMotionDone   = 0;
  flag.zMotionDone   = 0;
  flag.queueAccess   = 0;
  flag.fullSdCardRead    = 0;
  flag.penPos        = 0;
  flag.done          = 0;
  flag.pressBtnNotif = 0;
  flag.notifitationOccured=0;
}

double norm(struct vector *v)
{
  //Евклидова норма:    
  double norm;
  norm=sqrt( (v->x)*(v->x)+(v->y)*(v->y) );
  return norm;
}

double dotProduct(struct vector *v1, struct vector  *v2) {
  double dot=0.0;
  dot = (v1->x)*(v2->x)+(v1->y)*(v2->y)+(v1->z)*(v2->z);
  return dot;
}

struct vector cross(struct vector *a, struct vector  *b)
{
  struct vector v;
  v.x=   a->y*b->z - a->z*b->y ;
  v.y= -(a->x*b->z - a->z*b->x);
  v.z=   a->x*b->y - a->y*b->x ;
  return v;
}

double  calcAngleBetweenVectors(struct vector *v1, struct vector  *v2)
{
//  betta rad  0.....3.14159
  double betta=0.0,norm1,norm2,absBetta;
  double dotPr;
  double acosArg;
  struct vector v3;
  norm1=norm( v1 );
  norm2=norm( v2 );
  if ( 0.0==norm1 || 0.0==norm2 ) {
    return 0.0;
  } else {
       dotPr = dotProduct(v1, v2);
       acosArg = dotPr/(norm1*norm2);
       if ((-1.0 <acosArg) && (acosArg <1.0)) {
         absBetta = acos(acosArg); 
         //scalar multiplication gives just module of  the angle.
         //vector multiplication gives the sign of the angle.
         v3 = cross(v1, v2);
         if( (0.0==v3.x) &&  (0.0==v3.y) && (0.0==v3.z)){
           betta = absBetta;
         } else {
           betta = mySign(v3.z)*absBetta;
         }
       }
       return betta;
  }
}


double calcCurvature(struct fifo_t *fifo, int depth)
{
  int cnt=depth, n=0, first=1;
  struct simpleShift locSimpleShift;
  fifo_result_t fifo_result;
  struct vector vPrev, vCur;
  double angleDiff = 0.0;
  double totalAngleDiff = 0.0;
  double totalMaxAngleDiff = 0.0;
  double curvature = 0.0;
  
  while (0 < cnt) {
    fifo_result = fifo_show(fifo, &locSimpleShift, n);
    if (FIFO_SUCCESS != fifo_result) {
       errorAlarm(LOAD_FIFO_ERROR);
    }
    if (0 < locSimpleShift.xylength) {
      cnt--;
      if (first) {
        first = 0;
        vPrev.x = locSimpleShift.deltaX;
        vPrev.y = locSimpleShift.deltaY;
        vPrev.z = 0.0;
      } else {
        vCur.x = locSimpleShift.deltaX;
        vCur.y = locSimpleShift.deltaY;
        vCur.z = 0.0;
        
        angleDiff = absFloat( calcAngleBetweenVectors(&vPrev, &vCur) );
        totalAngleDiff += angleDiff;
        totalMaxAngleDiff += PI_RAD;
        
        vPrev.x = vCur.x;
        vPrev.y = vCur.y;
        vPrev.z = 0.0;    
      }
    }
    n++;
  }
  curvature = totalAngleDiff/totalMaxAngleDiff;
  return curvature;
}



void init_var()
{
  struct vector v1, v2;
  double angle=0.0;
  g_corrDiff.X=0;
  g_corrDiff.Y=0;
  /* USER CODE END 1 */
  
  remainsError.x=0.0;
  remainsError.y=0.0; 
  
  v1.x=1.0; v1.y=0.0; v1.z=0.0;
  v2.x=1.0; v2.y=0.0; v2.z=0.0;
  angle= calcAngleBetweenVectors(&v1, &v2);
  
  v1.x=1.0; v1.y=0.0; v1.z=0.0;
  v2.x=0.0; v2.y=1.0; v2.z=0.0;
  angle= calcAngleBetweenVectors(&v1, &v2);
  
  v1.x=1.0; v1.y=0.0; v1.z=0.0;
  v2.x=0.0; v2.y=-1.0; v2.z=0.0;
  angle= calcAngleBetweenVectors(&v1, &v2);
  
  v1.x=1.0; v1.y=0.0; v1.z=0.0;
  v2.x=-0.5; v2.y=0.866; v2.z=0.0;
  angle= calcAngleBetweenVectors(&v1, &v2);
  
  v1.x=1.0; v1.y=0.0; v1.z=0.0;
  v2.x=-0.5; v2.y=-0.866; v2.z=0.0;
  angle= calcAngleBetweenVectors(&v1, &v2);
  
  v1.x=1.0; v1.y=0.0; v1.z=0.0;
  v2.x=-1.0; v2.y=0.0; v2.z=0.0;
  angle= calcAngleBetweenVectors(&v1, &v2);
  
  v1.x=-1.0; v1.y=0.0; v1.z=0.0;
  v2.x= 1.0; v2.y=0.0; v2.z=0.0;
  angle= calcAngleBetweenVectors(&v1, &v2);
}

void drawVelocity(double vel) {
  
  int barLen    = 20;
  int curbarLen = (vel/g_maxVel)*barLen;
  int xStart = 31;
  int xEnd   = xStart + 4;
  
  int yStart = 35;
  int yEnd   = yStart + barLen;
  glcd_rect(xStart, yStart, xEnd , yEnd, 0, 1);
 
  glcd_rect(xStart, yStart+barLen-curbarLen, xEnd , yEnd, 1, 1);

}

void drawDirrection(double angle, int penPos) 
{
  int x,y,radius,xCen,yCen,xEnd,yEnd;
  radius=10;
  x = radius*cos(angle);
  y = radius*sin(angle);
  
  xCen=17;
  xEnd=xCen+x;
  yCen=45;
  yEnd=yCen+y;
  glcd_line( xCen,  yCen,  xEnd,  yEnd, 1);
  glcd_circle(xCen, yCen, radius, 0, 1);
  if(penPos){
    glcd_circle(xCen, yCen, radius/3, 1, 1);
  }else{
    glcd_circle(xCen, yCen, radius/3, 0, 1);
  }
  
}
  

void drawQueues(int sizeOfMovementQueue, int sizeOfLoadedQueue)
{
//  int barLen    = 20;
//  int curbarLen = (sizeOfLoadedQueue*barLen/LOAD_FIFO_WORK_SIZE);
//  
//  int xStart = 37;
//  int xEnd   = xStart + 4;//41
//  
//  int yStart = 35;
//  int yEnd   = yStart + barLen;//51
//
//  glcd_rect(xStart, yStart, xEnd , yEnd, 0, 1);
//  glcd_rect(xStart, yStart+barLen-curbarLen, xEnd , yEnd, 1, 1);
//  
//  barLen    = 20;
//
//  curbarLen = (sizeOfMovementQueue*barLen/(SIZE_OF_MOVEMENT_QUEUE/2));
//  
//  xStart = 43;
//  xEnd   = xStart + 4;//
//  
//  yStart = 35;
//  yEnd   = yStart + barLen;//
//  
//  glcd_rect(xStart, yStart, xEnd , yEnd, 0, 1);
//  glcd_rect(xStart, yStart+barLen-curbarLen, xEnd , yEnd, 1, 1);
  
}

void drawVel(double  speed_x, double speed_y)
{
  int xCen=60;  
  int yCen=45;
  
  int radius=10;
  int diffx=(radius*speed_x/g_maxVel); 
  int diffy=(radius*speed_y/g_maxVel);
  
  int xEnd=xCen+diffx; 
  int yEnd=yCen+diffy;
  glcd_line( xCen,  yCen,  xEnd,  yEnd, 1);
  glcd_circle(xCen, yCen, radius, 0, 1);
}
//cnc_Task.c

#include "cnc_Task.h"
#include "readSDcard_Task.h"
#include "utils.h"
#include "fatfs.h"
#include "cnc.h"
//#include "iwdg.h"
#include "drv8825.h"


QueueHandle_t xQueueMovement=NULL;

//-----------------------------------------------------------------------
void Cnc_vTask(void *pvParameters)
{
  U8 motionStat=0;
  int intgPart;
  U32 iterCnt=0;
  int iter=0, typeSize=0;
  struct movementT rxMovement;
  BaseType_t status;
  typeSize = sizeof(rxMovement);

  xQueueMovement = xQueueCreate( SIZE_OF_MOVEMENT_QUEUE, typeSize );
  if (NULL == xQueueMovement) {
    errorAlarm(QUEUE_UNDEF_ERROR);
  }

  for (;;) {
    // HAL_IWDG_Refresh(&hiwdg);
    // get Movement from queue
    status = xQueueReceive( xQueueMovement, &rxMovement, 1    );
    if(pdTRUE==status) {
      clear_buf();
      intgPart = ((g_procLinesCnt + g_skipLinesCnt)*100) / g_amountOfLine;
      int bar = 40*intgPart/100;
      draw_line(0, 15, bar, 15, 1); 
      
      launthMotion(&rxMovement);
      
      g_procLinesCnt++;
      //wait the ent of motion
      motionStat = flag.xMotionDone+flag.yMotionDone+flag.zMotionDone; 
      iterCnt = 0;
      
      while(3 != motionStat) {
        vTaskDelay(1);
        g_sizeOfMovementQueue = GetSizeOfMovementQueue();
        g_sizeOfLoadedQueue = get_size_of_queue(&objFifoLoaded);
        motionStat = flag.xMotionDone+flag.yMotionDone+flag.zMotionDone; 
        iterCnt++;
        if(14000 < iterCnt) {
          stop_all();
          motionStat=4;
          break;
        }
      }
      stop_all();
      
      if(HORIZONTAL_MOTION==rxMovement.typeOfMotion) {
        g_realCurXpos = rxMovement.targetXpos;
        g_realCurYpos = rxMovement.targetYpos;
      }
      if (VERTICAL_MOTION==rxMovement.typeOfMotion) {
        g_realCurZpos = rxMovement.penPosition;
      }
      

      
    } else {
      if ( (flag.done) && (50<intgPart) ) {
        clear_buf();
        glcd_text57("  Done!");
        drv8825_disable_xy();
        
        remove_pen();
        break;
        //display_buf_diff();
      }
    }
    iter++;
  }
  
  for(;;){}
}
 
portBASE_TYPE StartCncTask()
{
  return xTaskCreate(Cnc_vTask, "CNC", 1000, NULL, 6, NULL);
}
 
int GetSizeOfMovementQueue()
{
  int  freeSpace;
  freeSpace = uxQueueSpacesAvailable( xQueueMovement );
  return SIZE_OF_MOVEMENT_QUEUE-freeSpace;
}



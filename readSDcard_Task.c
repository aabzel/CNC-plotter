//readSDcard_Task.c

#include "readSDcard_Task.h"
#include "glcd_library_1_0.h"
#include "cnc_Task.h"
#include "fifo.h"
#include "utils.h"
#include "gpio.h"
#include "fatfs.h"
#include "rtc.h"
//#include "iwdg.h"

int g_sizeOfLoadedQueue=0;
int g_sizeOfMovementQueue=0;
//-----------------------------------------------------------------------
void ReadCard_vTask(void *pvParameters)
{
  vTaskDelay( 100 );
  
  g_virtCurXpos = 0;
  g_virtCurYpos = 0;
  
  initLoadFifo();
  
  reset_clock();
  
  g_res = openTxtFile();
  
  if (!g_res) {
    glcd_text57("\rSD open OK!");
    g_curLine = 0;
    flag.loadQueueEmpty = 0;
    while ( g_curLine < g_amountOfLine  ) {
      //HAL_IWDG_Refresh(&hiwdg);

      g_sizeOfMovementQueue = GetSizeOfMovementQueue();
      g_sizeOfLoadedQueue = get_size_of_queue(&objFifoLoaded);
      
      if (g_sizeOfLoadedQueue < LOAD_FIFO_WORK_SIZE ) {        
            // read next gCodeLine
            memset(g_readLineStr, 0x00, sizeof(g_readLineStr));
            g_lengthOfLine =0;
            
            g_res = readLine(g_readLineStr, &g_lengthOfLine);
            if (!g_res) {                      
              parseGcodeCmd(g_readLineStr, g_lengthOfLine);
              g_curLine++;            
            } else {
              errorAlarm(CANNOT_READ_GCODE_LINE_ERROR);
              //show g_curLine  1775  
             // HAL_GPIO_WritePin(ERROR_GPIO_PORT, ERROR_GPIO_PIN, GPIO_PIN_SET); 
              g_error=1;
            }
       } else {
         //There are enough elements to predict curvature
         proc_simple_shift();
       } 
    }
    flag.fullSdCardRead=1;
    g_sizeOfLoadedQueue = get_size_of_queue(&objFifoLoaded);
    while(0<g_sizeOfLoadedQueue) {
      g_sizeOfLoadedQueue = get_size_of_queue(&objFifoLoaded);
      proc_simple_shift();
    }
    flag.done=1;
    g_res = closeTxtFile();
    save_statistics();
  } else {
     // print Error
     // HAL_GPIO_WritePin(ERROR_GPIO_PORT, ERROR_GPIO_PIN, GPIO_PIN_SET); 
     // glcd_text57("\rSD open error");
  }

  if (g_res) {  
      g_error=2;
  }

  for (;;) {
      vTaskDelay( 1 );
  }
}

portBASE_TYPE StartReadCardTask()
{
  return xTaskCreate(ReadCard_vTask, "READ_SD", 1280, NULL, 5, NULL);
}




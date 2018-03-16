//ind_Task.c

#include "ind_Task.h"
#include "utils.h"

#include "glcd_library_1_0.h"
#include "readSDcard_Task.h"
#include "rtc.h"

unsigned char g_line;
unsigned char g_col;
//-----------------------------------------------------------------------
void ind_vTask(void *pvParameters)
{
  int cmpRes;
  uint8_t realByte,   desByte, realPix, desPix; 
  int length;
  int intgPart ,  partial;
   
  char textptr[300]="ind. task...";

  for(;;) {
    vTaskDelay( 5 );
    //HAL_IWDG_Refresh(&hiwdg);
   // clear();
   // resetCaret();
    HAL_RTC_GetTime(&hrtc, &g_curTime, RTC_FORMAT_BIN);
    display_buf_diff();
    //glcd_rect(0, 0, 126, 62, 0, 1);  
    if ( !flag.notifitationOccured ) {
      vTaskDelay( 1 );
      continue;  
    }
    if (flag.pressBtnNotif) {
    //  clear();
    //  resetCaret();
 //     glcd_text57("  Set the initial \n\rposition of the pen \n\r and press button \n\r to start drawing.\n\r");
//      sprintf ( g_tempStr, "%d[mm/s]d%d,u%d\n\r", 
//                (int) g_maxVel, g_down_position, g_up_position); 
//      glcd_text57(g_tempStr);
    } else {
//      sprintf ( g_tempStr, "%02d:%02d \n\r", g_curTime.Minutes, g_curTime.Seconds);
//                glcd_text57(g_tempStr);
    
    //  calcPerCent(g_curLine, g_amountOfLine, &intgPart, &partial);
    //  sprintf ( g_tempStr, "Done:%d.%d%% of code\n\r", intgPart, partial);
              
    //  length = 124*((intgPart+0.1*partial)/100.0);
    //  glcd_rect(0, 9, length, 17, 1, 1);
    //  glcd_text57(g_tempStr);
              
    //  calcPerCent(g_curDist, g_totalDist, &intgPart, &partial);
    //  sprintf ( g_tempStr, "%d.%d%% %d/%d mm\n\r", intgPart, partial, (int) g_curDist,(int) g_totalDist); 
                         
    //  length = 124*((intgPart+0.1*partial)/100.0);
    //  glcd_rect(0, 17, length, 25, 1, 1);
    //  glcd_text57(g_tempStr);   
      
   //   sprintf ( g_tempStr, "%d[mm/s]d%d,u%d\n\r", 
    //            (int) g_maxVel, g_down_position, g_up_position); 
    //  glcd_text57(g_tempStr);
      
     // if (flag.penPos) {
     //     drawDirrection( g_curAngle, 1);
     // } else {
     //     drawDirrection( g_curAngle, 0);
     // }
     // drawVelocity(g_curVeloc);
   
     // g_sizeOfMovementQueue = GetSizeOfMovementQueue();
     // g_sizeOfLoadedQueue = get_size_of_queue(&objFifoLoaded);    
     // drawQueues(g_sizeOfMovementQueue, g_sizeOfLoadedQueue);
       
     // drawVel(g_curSpeed_x,g_curSpeed_y);
    }
    
    if (flag.done) {
      glcd_x=2;
      glcd_y=37;
      //glcd_text812("          Done!\n\r");
    }
    
    //displayDes() ;
    
    // есть ли различия?
//    cmpRes=memcmp (realBuffer, desBuffer, 1024);   
//        
//    if (cmpRes) {
//      for (g_line=0; g_line < SSD1305_LCDHEIGHT_64/8; g_line++) {
//        for (g_col=0; g_col < SSD1305_LCDWIDTH_128; g_col++) {
//           realByte= getByteStick(g_col, g_line, REAL_DISP_MEM);
//           desByte = getByteStick(g_col, g_line, DESIRE_DISP_MEM);
//                         
//           if (realByte != desByte) {
//              // set & show only single pixel
//              displayByteStick( g_col,   g_line,  desByte);
//           }else{
//             //vTaskDelay( 1 );
//           }               
//        }
//      }
//    } else {
//      vTaskDelay( 5 );
//    }
    // отрисовать пиксели с различиями
    // обновить массив текущего отображения
  }
}
//-----------------------------------------------------------------------
portBASE_TYPE StartIndTask()
{
  return xTaskCreate(ind_vTask, "Ind", 500, NULL, 1, NULL);
}
//-----------------------------------------------------------

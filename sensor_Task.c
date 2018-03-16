// sensor_Task.c

#include "sensor_Task.h"
#include "utils.h"
//#include "sensors.h"
//#include "iwdg.h"
#include "nec_fsm.h"
#include "gpio.h"

QueueHandle_t xQueueIRpkt = NULL;

void perform_ir_cmd(struct msgNecIRcmdT inIrNecMsg) {
  toggle_led();
  if (0x80 == inIrNecMsg.addr) {
    //on LEDs
    switch(inIrNecMsg.cmd) {
      case 0x12: //off
        break;
      case 0x01: //MUTE
        break;
      case 0x1E: //CH+
        break;  
      case 0x03: //CH-
        break; 
      case 0x0C: //ZOOM
        break;   
      case 0x0E: //JUMP
        break;     
      case 0x0D: //0
        break;  
      case 0x04: //1
        //g_down_position++;
        break;
      case 0x06: //3
       // g_up_position++;
        break;
      case 0x07: //4
       // g_down_position--;
        break;
      case 0x05: //2
        break; 
      case 0x08: //5
        break;   
      case 0x09: //6
       // g_up_position--;
        break;  
      case 0x1F: //9
        break;    
      case 0x1A://vel++
        g_maxVel = g_maxVel+1.0;
        break;
      case 0x02://vel--
        g_maxVel=g_maxVel-1.0;
        if(g_maxVel <= g_minVel) {
          g_maxVel = 2*g_minVel;
        }
        break;  
      default:
    }
  }
  

}

void proc_ir_commands() {
  BaseType_t  status;
  struct msgNecIRcmdT inIrNecMsg;
  
  status = xQueueReceive(xQueueIRpkt, 
                         &inIrNecMsg, 
                         (TickType_t) 0);
    
  if( pdTRUE == status ) {
    perform_ir_cmd(inIrNecMsg);
  }
}


//-----------------------------------------------------------------------
void sensor_vTask(void *pvParameters)
{
  U8 senrorResult=0;
  GPIO_PinState state;
  int typeSize;
                    
  typeSize = sizeof(struct msgNecIRcmdT);
  xQueueIRpkt = xQueueCreate( SIZE_OF_IR_QUEUE, typeSize );
  if (NULL==xQueueIRpkt) {
    errorAlarm(QUEUE_UNDEF_ERROR);
  }
  
  vTaskDelay( 1000);  
  for (;;) {
    
    state = HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN);
    if(state==GPIO_PIN_RESET) {
       flag.btnPressed = 1;
       flag.firstTouch = 0;
    }
    //HAL_IWDG_Refresh(&hiwdg);
	    
//    senrorResult = readDrvStatus(X_AXIS);
//    if(!senrorResult) {
//            //if(flag.firstTouch)
//            //errorAlarm(X_DRV_ERROR);
//    }
//    senrorResult = readDrvStatus(Y_AXIS);
//    if(!senrorResult) {
//                  //if(flag.firstTouch)
//                      //errorAlarm(Y_DRV_ERROR);
//    }
//    senrorResult = readBtn();
    // read limit sensors
         
   // proc_ir_commands();
         
    vTaskDelay( 200);  
  }
}
//-----------------------------------------------------------------------
portBASE_TYPE StartSensorTask()
{
  return xTaskCreate(sensor_vTask, "SENSOR", 100, NULL, 2, NULL);
}

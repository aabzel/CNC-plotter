//readSDcard_Task.h

#ifndef __CNC_TASK_HEADER
#define __CNC_TASK_HEADER

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include "queue.h"

#define SIZE_OF_MOVEMENT_QUEUE (25) //50

extern QueueHandle_t xQueueMovement;

void Cnc_vTask(void *pvParameters);
portBASE_TYPE StartCncTask();
int GetSizeOfMovementQueue();


#endif // __CNC_TASK_HEADER
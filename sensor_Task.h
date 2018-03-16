//sensor_Task.h

#ifndef __SENSOR_TASK_HEADER
#define __SENSOR_TASK_HEADER

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include "queue.h"

#define SIZE_OF_IR_QUEUE (10)

extern QueueHandle_t xQueueIRpkt;

void sensor_vTask(void *pvParameters);
portBASE_TYPE StartSensorTask();

#endif // __SENSOR_TASK_HEADER
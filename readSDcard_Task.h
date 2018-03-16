//readSDcard_Task.h

#ifndef __READ_SD_TASK_HEADER
#define __READ_SD_TASK_HEADER

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "queue.h"



extern int g_sizeOfLoadedQueue;
extern int g_sizeOfMovementQueue;

extern void ReadCard_vTask(void *pvParameters);
extern portBASE_TYPE StartReadCardTask();

#endif // __READ_SD_TASK_HEADER
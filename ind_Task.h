//ind_Task.h

#ifndef __IND_TASK_HEADER
#define __IND_TASK_HEADER

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include "queue.h"

void ind_vTask(void *pvParameters);
portBASE_TYPE StartIndTask();

#endif // __IND_TASK_HEADER
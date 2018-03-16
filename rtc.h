
//rtc.h

#ifndef __rtc_H
#define __rtc_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx_hal.h"

extern RTC_HandleTypeDef hrtc;

void reset_clock();
extern void _Error_Handler(char *, int);

void MX_RTC_Init(void);


#ifdef __cplusplus
}
#endif
#endif /*__ rtc_H */


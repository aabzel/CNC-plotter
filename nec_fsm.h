
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __nec_fsm_H
#define __nec_fsm_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

   
enum nec_fsm_inputs { 
  NEC_IN_0_FALL, 
  NEC_IN_1_RISE,
  NEC_IN_2_T056,
  NEC_IN_3_T169, 
  NEC_IN_4_T225, 
  NEC_IN_5_T450, 
  NEC_IN_6_T900, 
  NEC_IN_7_BIT33, 

  NEC_IN_CNT
};

enum nec_fsm_state { 
  NEC_ST_0_WAIT_SYNC1,  
  NEC_ST_1_SYNC1,  
  NEC_ST_2_SYNC1_9_MS,  
  NEC_ST_3_WAIT_SYNC2,  
  NEC_ST_4_SYNC2_2_25_MS,  
  NEC_ST_5_SYNC2_4_5_MS,  
  NEC_ST_6_DATA_BIT,  
  NEC_ST_7_DATA_BIT_560_US, 
  NEC_ST_8_WAIT_BIT_PAUSE,  
  NEC_ST_9_BIT_PAUSE_560_US,  
  NEC_ST_10_BIT_PAUSE_1_69_MS,  
  NEC_ST_11_ACK_BIT,   
  NEC_ST_12_ACK_BIT_PASSED_560_US,  

  NEC_ST_CNT
};


enum nec_fsm_action { 
  NEC_OUT_0_NONE, 
  NEC_OUT_1_STOP_TIMER, 
  NEC_OUT_2_START_TIMER,
  NEC_OUT_3_SAVE_0,
  NEC_OUT_4_SAVE_1,
  NEC_OUT_5_RST,
  NEC_OUT_6_DONE,
  
  NEC_OUT_CNT
};
 
struct softTimerT
{
  unsigned int cnt;
  unsigned int maxCnt;
};

struct msgNecIRcmdT 
{ 
  char addr; 
  char cmd;
};

extern enum nec_fsm_state g_cur_state ;

extern enum nec_fsm_state state_table[NEC_IN_CNT][NEC_ST_CNT];
extern enum nec_fsm_action action_table[NEC_IN_CNT][NEC_ST_CNT];
  
void proc_nec_fsm(enum nec_fsm_inputs input) ;
void tick_soft_timer(); 

#ifdef __cplusplus
}
#endif
#endif /*__ __nec_fsm_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

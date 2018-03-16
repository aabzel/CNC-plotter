
/* Includes ------------------------------------------------------------------*/
#include "nec_fsm.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "tim.h"
#include "sensor_Task.h"

struct softTimerT softTimer;
enum nec_fsm_state g_cur_state = NEC_ST_0_WAIT_SYNC1;
int g_timeOffset=5;
char nec_addr, inv_nec_addr, nec_cmd, inv_nec_cmd;


int nec_data[40];
int cur_bit=0;
int max_cur_bit=0;

struct msgNecIRcmdT irNecMsg;

int putIRcmdInQueue() {
  BaseType_t status;
  BaseType_t xHigherPriorityTaskWoken;
  xHigherPriorityTaskWoken = pdFALSE;
  irNecMsg.addr= nec_addr;
  irNecMsg.cmd = nec_cmd;
  
  if(xQueueIRpkt){
    status = xQueueSendFromISR( xQueueIRpkt, &irNecMsg, &xHigherPriorityTaskWoken );
    if(pdTRUE==status){
      return 0;
    }else{
      return 1;
    }
  }

}

void stop_timer()
{
//  HAL_TIM_Base_Stop(&htim3);
//  __HAL_TIM_SET_COUNTER(&htim3, 0);
//  softTimer.cnt=0;
}

void start_timer()
{
//  softTimer.cnt=0;
//  __HAL_TIM_SET_COUNTER(&htim3, 0);
//  HAL_TIM_Base_Start(&htim3);
}

int check_pkt()
{
  nec_addr=0;
  inv_nec_addr=0; 
  nec_cmd=0; 
  inv_nec_cmd=0;
  
  for(int i=0;i<8;i++) {
    if(nec_data[i]){
      nec_addr |=(1<<i);
    }else{
      nec_addr &=~(1<<i);
    }
  }
  for(int i=0;i<8;i++) {
    if(nec_data[i+8]){
      inv_nec_addr |=(1<<i);
    }else{
      inv_nec_addr &=~(1<<i);
    }
  }  
  
  for(int i=0;i<8;i++) {
    if(nec_data[i+16]){
      nec_cmd |=(1<<i);
    }else{
      nec_cmd &=~(1<<i);
    }
  }
  for(int i=0;i<8;i++) {
    if(nec_data[i+24]){
      inv_nec_cmd |=(1<<i);
    }else{
      inv_nec_cmd &=~(1<<i);
    }
  }  
  char cmdDiff=nec_cmd-(~inv_nec_cmd);
  char addrDiff = nec_addr-(~inv_nec_addr);

  if(!cmdDiff){
     if (!addrDiff )  {
       return 0;
     }else{ 
       return 1;
     }
  } else{
    return 1;
  }
}

void reset_nec_bus()
{
  stop_timer();
  cur_bit = 0;
  for (int i=0; i<4*8; i++) {
    nec_data[i] = 0xFFFFFFFF;
  }
  softTimer.cnt=0;
  g_cur_state = NEC_ST_0_WAIT_SYNC1;
}

void proc_nec_fsm(enum nec_fsm_inputs input) {
  enum nec_fsm_action action;
  action = action_table[input][g_cur_state];
  g_cur_state = state_table[input][g_cur_state];
  
  switch(action)
  {
    case NEC_OUT_1_STOP_TIMER: 
      stop_timer();
    break;
    case NEC_OUT_2_START_TIMER: 
      start_timer();
      
    break;
    case NEC_OUT_3_SAVE_0: 
      nec_data[cur_bit]=0;
      cur_bit++;
      if(max_cur_bit<cur_bit)max_cur_bit=cur_bit ;
      if (32==cur_bit) {
        proc_nec_fsm(NEC_IN_7_BIT33);
      }
      start_timer(); 
    break;
    case NEC_OUT_4_SAVE_1: 
      nec_data[cur_bit]=1;
      cur_bit++;
      if(max_cur_bit<cur_bit)max_cur_bit=cur_bit ;
      if (32==cur_bit) {
        proc_nec_fsm(NEC_IN_7_BIT33);
      }
      start_timer(); 
    break;
    
    case NEC_OUT_5_RST:
      reset_nec_bus();
    break;
    
    case NEC_OUT_6_DONE:
      int ret = 0;
      ret = check_pkt();
	  if (!ret) {
		putIRcmdInQueue();  
	  } else {
		  //log error to file
		  ret = 5;
	  }
    break;
      
    case NEC_OUT_0_NONE:
    break;
	
  default:
  }
}


void tick_soft_timer()
{
   softTimer.cnt++;
   if(softTimer.maxCnt<softTimer.cnt) {
     softTimer.maxCnt=softTimer.cnt;
   }
   int timeout=0;
   enum nec_fsm_inputs fsm_input;
   
   if(40==softTimer.cnt ) {
       timeout=1;
       fsm_input = NEC_IN_2_T056;
   }
   if(120==softTimer.cnt) {
       timeout=1;
       fsm_input = NEC_IN_3_T169;
   }   
   if(200==softTimer.cnt) {
       timeout=1;
       fsm_input = NEC_IN_4_T225;
   }   
   if(350==softTimer.cnt) {
       timeout=1;
       fsm_input = NEC_IN_5_T450;
   }
   if(650==softTimer.cnt) {
       timeout=1;
       fsm_input = NEC_IN_6_T900;
   }
   
   if(timeout) {
     proc_nec_fsm(fsm_input);
   }
}


enum nec_fsm_state state_table[NEC_IN_CNT][NEC_ST_CNT]={
//                     NEC_ST_0_WAIT_SYNC1   NEC_ST_1_SYNC1       NEC_ST_2_SYNC1_9_MS  NEC_ST_3_WAIT_SYNC2     NEC_ST_4_SYNC2_2_25_MS  NEC_ST_5_SYNC2_4_5_MS  NEC_ST_6_DATA_BIT         NEC_ST_7_DATA_BIT_560_US  NEC_ST_8_WAIT_BIT_PAUSE    NEC_ST_9_BIT_PAUSE_560_US    NEC_ST_10_BIT_PAUSE_1_69_MS  NEC_ST_11_ACK_BIT                NEC_ST_12_ACK_BIT_PASSED_560_US    
/* NEC_IN_0_FALL */   { NEC_ST_0_WAIT_SYNC1, NEC_ST_0_WAIT_SYNC1, NEC_ST_3_WAIT_SYNC2, NEC_ST_3_WAIT_SYNC2,    NEC_ST_4_SYNC2_2_25_MS, NEC_ST_5_SYNC2_4_5_MS, NEC_ST_0_WAIT_SYNC1,      NEC_ST_8_WAIT_BIT_PAUSE,  NEC_ST_8_WAIT_BIT_PAUSE,   NEC_ST_9_BIT_PAUSE_560_US,   NEC_ST_10_BIT_PAUSE_1_69_MS, NEC_ST_0_WAIT_SYNC1,             NEC_ST_0_WAIT_SYNC1 },
/* NEC_IN_1_RISE */   { NEC_ST_1_SYNC1,      NEC_ST_1_SYNC1,      NEC_ST_2_SYNC1_9_MS, NEC_ST_0_WAIT_SYNC1,    NEC_ST_11_ACK_BIT,      NEC_ST_6_DATA_BIT,     NEC_ST_6_DATA_BIT,        NEC_ST_7_DATA_BIT_560_US, NEC_ST_0_WAIT_SYNC1,       NEC_ST_6_DATA_BIT,           NEC_ST_6_DATA_BIT,           NEC_ST_11_ACK_BIT,               NEC_ST_12_ACK_BIT_PASSED_560_US},
/* NEC_IN_2_T056 */   { NEC_ST_0_WAIT_SYNC1, NEC_ST_1_SYNC1,      NEC_ST_0_WAIT_SYNC1, NEC_ST_3_WAIT_SYNC2,    NEC_ST_0_WAIT_SYNC1,    NEC_ST_0_WAIT_SYNC1,   NEC_ST_7_DATA_BIT_560_US, NEC_ST_0_WAIT_SYNC1,      NEC_ST_9_BIT_PAUSE_560_US, NEC_ST_0_WAIT_SYNC1,         NEC_ST_0_WAIT_SYNC1,         NEC_ST_12_ACK_BIT_PASSED_560_US, NEC_ST_0_WAIT_SYNC1 },
/* NEC_IN_3_T169 */   { NEC_ST_0_WAIT_SYNC1, NEC_ST_1_SYNC1,      NEC_ST_0_WAIT_SYNC1, NEC_ST_3_WAIT_SYNC2,    NEC_ST_0_WAIT_SYNC1,    NEC_ST_0_WAIT_SYNC1,   NEC_ST_0_WAIT_SYNC1,      NEC_ST_0_WAIT_SYNC1,      NEC_ST_0_WAIT_SYNC1,       NEC_ST_10_BIT_PAUSE_1_69_MS, NEC_ST_0_WAIT_SYNC1,         NEC_ST_0_WAIT_SYNC1,             NEC_ST_0_WAIT_SYNC1 },
/* NEC_IN_4_T225 */   { NEC_ST_0_WAIT_SYNC1, NEC_ST_1_SYNC1,      NEC_ST_0_WAIT_SYNC1, NEC_ST_4_SYNC2_2_25_MS, NEC_ST_0_WAIT_SYNC1,    NEC_ST_0_WAIT_SYNC1,   NEC_ST_0_WAIT_SYNC1,      NEC_ST_0_WAIT_SYNC1,      NEC_ST_0_WAIT_SYNC1,       NEC_ST_0_WAIT_SYNC1,         NEC_ST_0_WAIT_SYNC1,         NEC_ST_0_WAIT_SYNC1,             NEC_ST_0_WAIT_SYNC1 },
/* NEC_IN_5_T450 */   { NEC_ST_0_WAIT_SYNC1, NEC_ST_1_SYNC1,      NEC_ST_0_WAIT_SYNC1, NEC_ST_0_WAIT_SYNC1,    NEC_ST_5_SYNC2_4_5_MS,  NEC_ST_0_WAIT_SYNC1,   NEC_ST_0_WAIT_SYNC1,      NEC_ST_0_WAIT_SYNC1,      NEC_ST_0_WAIT_SYNC1,       NEC_ST_0_WAIT_SYNC1,         NEC_ST_0_WAIT_SYNC1,         NEC_ST_0_WAIT_SYNC1,             NEC_ST_0_WAIT_SYNC1 },
/* NEC_IN_6_T900 */   { NEC_ST_0_WAIT_SYNC1, NEC_ST_2_SYNC1_9_MS, NEC_ST_0_WAIT_SYNC1, NEC_ST_0_WAIT_SYNC1,    NEC_ST_0_WAIT_SYNC1,    NEC_ST_0_WAIT_SYNC1,   NEC_ST_0_WAIT_SYNC1,      NEC_ST_0_WAIT_SYNC1,      NEC_ST_0_WAIT_SYNC1,       NEC_ST_0_WAIT_SYNC1,         NEC_ST_0_WAIT_SYNC1,         NEC_ST_0_WAIT_SYNC1,             NEC_ST_0_WAIT_SYNC1 },
/* NEC_IN_7_BIT33 */  { NEC_ST_0_WAIT_SYNC1, NEC_ST_0_WAIT_SYNC1, NEC_ST_0_WAIT_SYNC1, NEC_ST_0_WAIT_SYNC1,    NEC_ST_0_WAIT_SYNC1,    NEC_ST_0_WAIT_SYNC1,   NEC_ST_0_WAIT_SYNC1,      NEC_ST_0_WAIT_SYNC1,      NEC_ST_0_WAIT_SYNC1,       NEC_ST_0_WAIT_SYNC1,         NEC_ST_0_WAIT_SYNC1,         NEC_ST_0_WAIT_SYNC1,             NEC_ST_0_WAIT_SYNC1 }
};

enum nec_fsm_action action_table[NEC_IN_CNT][NEC_ST_CNT]={
//                     NEC_ST_0_WAIT_SYNC1     NEC_ST_1_SYNC1  NEC_ST_2_SYNC1_9_MS    NEC_ST_3_WAIT_SYNC2   NEC_ST_4_SYNC2_2_25_MS  NEC_ST_5_SYNC2_4_5_MS  NEC_ST_6_DATA_BIT  NEC_ST_7_DATA_BIT_560_US NEC_ST_8_WAIT_BIT_PAUSE  NEC_ST_9_BIT_PAUSE_560_US  NEC_ST_10_BIT_PAUSE_1_69_MS  NEC_ST_11_ACK_BIT  NEC_ST_12_ACK_BIT_PASSED_560_US    
/* NEC_IN_0_FALL */   { NEC_OUT_5_RST,         NEC_OUT_5_RST,  NEC_OUT_2_START_TIMER, NEC_OUT_0_NONE,       NEC_OUT_0_NONE,         NEC_OUT_0_NONE,        NEC_OUT_5_RST,     NEC_OUT_2_START_TIMER,   NEC_OUT_0_NONE,          NEC_OUT_0_NONE,            NEC_OUT_0_NONE,              NEC_OUT_5_RST,     NEC_OUT_5_RST},
/* NEC_IN_1_RISE */   { NEC_OUT_2_START_TIMER, NEC_OUT_0_NONE, NEC_OUT_0_NONE,        NEC_OUT_5_RST,        NEC_OUT_2_START_TIMER,  NEC_OUT_2_START_TIMER, NEC_OUT_0_NONE,    NEC_OUT_0_NONE,          NEC_OUT_5_RST,           NEC_OUT_3_SAVE_0,          NEC_OUT_4_SAVE_1,            NEC_OUT_0_NONE,    NEC_OUT_0_NONE},
/* NEC_IN_2_T056 */   { NEC_OUT_5_RST,         NEC_OUT_0_NONE, NEC_OUT_5_RST,         NEC_OUT_0_NONE,       NEC_OUT_5_RST,          NEC_OUT_5_RST,         NEC_OUT_0_NONE,    NEC_OUT_5_RST,           NEC_OUT_0_NONE,          NEC_OUT_5_RST,             NEC_OUT_5_RST,               NEC_OUT_0_NONE,    NEC_OUT_5_RST},
/* NEC_IN_3_T169 */   { NEC_OUT_5_RST,         NEC_OUT_0_NONE, NEC_OUT_5_RST,         NEC_OUT_0_NONE,       NEC_OUT_5_RST,          NEC_OUT_5_RST,         NEC_OUT_5_RST,     NEC_OUT_5_RST,           NEC_OUT_5_RST,           NEC_OUT_0_NONE,            NEC_OUT_5_RST,               NEC_OUT_5_RST,     NEC_OUT_5_RST},
/* NEC_IN_4_T225 */   { NEC_OUT_5_RST,         NEC_OUT_0_NONE, NEC_OUT_5_RST,         NEC_OUT_0_NONE,       NEC_OUT_5_RST,          NEC_OUT_5_RST,         NEC_OUT_5_RST,     NEC_OUT_5_RST,           NEC_OUT_5_RST,           NEC_OUT_5_RST,             NEC_OUT_5_RST,               NEC_OUT_5_RST,     NEC_OUT_5_RST},
/* NEC_IN_5_T450 */   { NEC_OUT_5_RST,         NEC_OUT_0_NONE, NEC_OUT_5_RST,         NEC_OUT_5_RST,        NEC_OUT_0_NONE,         NEC_OUT_5_RST,         NEC_OUT_5_RST,     NEC_OUT_5_RST,           NEC_OUT_5_RST,           NEC_OUT_5_RST,             NEC_OUT_5_RST,               NEC_OUT_5_RST,     NEC_OUT_5_RST},
/* NEC_IN_6_T900 */   { NEC_OUT_5_RST,         NEC_OUT_0_NONE, NEC_OUT_5_RST,         NEC_OUT_5_RST,        NEC_OUT_5_RST,          NEC_OUT_5_RST,         NEC_OUT_5_RST,     NEC_OUT_5_RST,           NEC_OUT_5_RST,           NEC_OUT_5_RST,             NEC_OUT_5_RST,               NEC_OUT_5_RST,     NEC_OUT_5_RST},
/* NEC_IN_7_BIT33 */  { NEC_OUT_5_RST,         NEC_OUT_5_RST,  NEC_OUT_5_RST,         NEC_OUT_5_RST,        NEC_OUT_5_RST,          NEC_OUT_5_RST,         NEC_OUT_6_DONE,    NEC_OUT_6_DONE,          NEC_OUT_5_RST,           NEC_OUT_5_RST,             NEC_OUT_5_RST,               NEC_OUT_5_RST,     NEC_OUT_5_RST}
};

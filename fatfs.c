/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

#include "fatfs.h"
#include "gpio.h"
#include "utils.h"

uint8_t retSD;    /* Return value for SD */
char SD_Path[4];  /* SD logical drive path */
FATFS fileSystem;


/* USER CODE BEGIN Variables */



FIL g_testFile;
FIL g_logFile;
char g_fileName[20] = "ZIG.TXT";
char g_logFileName[20] ="log.txt";
FRESULT g_res;
/* USER CODE END Variables */    

void MX_FATFS_Init(void) 
{
  /*## FatFS: Link the SD driver ###########################*/
  //привязываю драйвер:
  retSD = FATFS_LinkDriver(&SD_Driver, SD_Path);

  /* USER CODE BEGIN Init */
  /* additional user code for init */     
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC 
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
  return 0;
  /* USER CODE END get_fattime */  
}

/* USER CODE BEGIN Application */
uint8_t openTxtFile()
{
  uint8_t stat=0;
  
  g_res = f_open(&g_testFile, &g_fileName[0],  FA_OPEN_EXISTING  | FA_READ );

  
  if ((FR_OK ==g_res)|| (FR_EXIST==g_res))
  {
    stat=0;
  }
  else
  {
    stat=1;
  }
}  

FRESULT closeTxtFile()
{
  FRESULT stat=0;
  
  //HAL_GPIO_WritePin(CARD_ACCESS_GPIO_PORT, CARD_ACCESS_GPIO_PIN, GPIO_PIN_SET); 
  g_res = f_close(&g_testFile);
  //HAL_GPIO_WritePin(CARD_ACCESS_GPIO_PORT, CARD_ACCESS_GPIO_PIN, GPIO_PIN_RESET); 
  
  if (FR_OK ==g_res)
  {
    stat=0;
  }
  else
  {
    stat=1;
  }
} 
  
//add lineLength
uint8_t readLine(char *readLineStr, int *lenOfLine )
{
  char readBuff[3]={0,0,0};
  int i=0,readCnt;
  FRESULT res;
  *lenOfLine=0;
  while(1) {  
    //HAL_GPIO_WritePin(CARD_ACCESS_GPIO_PORT, CARD_ACCESS_GPIO_PIN, GPIO_PIN_SET); 
    res= f_read (&g_testFile, readBuff,1 , &readCnt);
    //HAL_GPIO_WritePin(CARD_ACCESS_GPIO_PORT, CARD_ACCESS_GPIO_PIN, GPIO_PIN_RESET); 
    
    if (FR_OK != res) {
      return 1;
    }
    if (1!=readCnt) {
      return 2;
    }
    g_numberOfBytesRead++;
    if (LINE_FEED == readBuff[0]) {
      readLineStr[i]='\0';
      *lenOfLine = i-1;
      return 0;
    }else{
      readLineStr[i]=readBuff[0];
      i++;
    }
    if (100<i) {
      return 3;
    }
  }
  
}
    
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

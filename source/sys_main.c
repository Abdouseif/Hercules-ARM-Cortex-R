/** @file sys_main.c 
*   @brief Application main file
*   @date 11-Dec-2018
*   @version 04.07.01
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/* 
* Copyright (C) 2009-2018 Texas Instruments Incorporated - www.ti.com 
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


/* USER CODE BEGIN (0) */
#include "can.h"
#include "adc.h"
#include "stdlib.h"
#include "sci.h"
/* USER CODE END */

/* Include Files */

#include "sys_common.h"

/* USER CODE BEGIN (1) */
unsigned char command[9];
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
#define D_SIZE 9
      uint8_t tx_data2[D_SIZE]={'H','E','R','C','U','L','E','S','\0'};
      unsigned int NumberOfChars,value;
       uint8_t tx_data[2];
       uint8_t Rx_data[2]={0};
    //uint8_t* tx_data={'S','E','I','F'};
    uint32_t checkPackets(uint8_t * src_packet,uint8_t * dest_packet , uint32_t psize);

/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
      adcData_t adc_data;
      adcData_t *adc_data_ptr=& adc_data;
       adcInit();
       canInit();
       sciInit();
     while(1)
     {
         adcStartConversion(adcREG1,adcGROUP1);
               while(!adcIsConversionComplete(adcREG1,adcGROUP1));
               adcGetData(adcREG1,1U,adc_data_ptr);
               *tx_data= adc_data_ptr->value;
         canTransmit(canREG1,canMESSAGE_BOX1,tx_data);
         if( canIsRxMessageArrived(canREG1,canMESSAGE_BOX2)){
            sciSend(scilinREG,2,(unsigned char *)"w");
            sciSend(scilinREG,2,(unsigned char *)"\r\n");
            canTransmit(canREG1,canMESSAGE_BOX1,tx_data);


               canGetData(canREG1,canMESSAGE_BOX2,Rx_data);
               //canGetData(canREG1,canMESSAGE_BOX2,Rx_data2);
               value= (unsigned int)*Rx_data;
               NumberOfChars=ltoa(value,(char *)command);
               sciSend(scilinREG,2,(unsigned char *)"0x");
               sciSend(scilinREG,NumberOfChars,command);
               sciSend(scilinREG,2,(unsigned char *)"\r\n");
         }



     }
/* USER CODE END */


}


/* USER CODE BEGIN (4) */
uint32_t checkPackets(uint8_t * src_packet,uint8_t * dest_packet , uint32_t psize)
{
    uint32_t err=0;
    uint32_t cnt=psize;
    while(cnt--)
    {
        if((*src_packet++)!=(*dest_packet++))
        {
            err++;
        }

    }
    return err;
}

void canMessageNotification( canBASE_t *node,uint32_t messageBox){return;}
void canErroeNotification( canBASE_t *node,uint32_t notification){return;}
void esmGroup1Notification(int bit){return;}
void esmGroup2Notification(int bit){return;}

/* USER CODE END */

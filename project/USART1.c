 /************************************************************************************************************
 * @attention
 *
 * Firmware Disclaimer Information
 *
 * 1. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, which is supplied by Holtek Semiconductor Inc., (hereinafter referred to as "HOLTEK") is the
 *    proprietary and confidential intellectual property of HOLTEK, and is protected by copyright law and
 *    other intellectual property laws.
 *
 * 2. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, is confidential information belonging to HOLTEK, and must not be disclosed to any third parties
 *    other than HOLTEK and the customer.
 *
 * 3. The program technical documentation, including the code, is provided "as is" and for customer reference
 *    only. After delivery by HOLTEK, the customer shall use the program technical documentation, including
 *    the code, at their own risk. HOLTEK disclaims any expressed, implied or statutory warranties, including
 *    the warranties of merchantability, satisfactory quality and fitness for a particular purpose.
 *
 * <h2><center>Copyright (C) Holtek Semiconductor Inc. All rights reserved</center></h2>
 ************************************************************************************************************/
//Generated by HT32CodeConfig V1.1.4 Build 241129
//-----------------------------------------------------------------------------
#include "USART1.h"
//-----------------------------------------------------------------------------
__ALIGN4 _USART1_STRUCT rxd_scomm1;
__ALIGN4 _USART1_STRUCT txd_scomm1;


//-----------------------------------------------------------------------------
void USART1_Configuration(void)
{
  USART_InitTypeDef USART_InitStruct;
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  CKCUClock.Bit.USART1   = 1;
  CKCUClock.Bit.AFIO     = 1;
  CKCUClock.Bit.PA      = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);

  GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_5, GPIO_PR_UP);  //Turn on UxART Rx internal pull up resistor to prevent unknow state

  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_4, AFIO_FUN_USART_UART);  // Config AFIO mode
  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_5, AFIO_FUN_USART_UART);  // Config AFIO mode

  USART_InitStruct.USART_BaudRate = 115200;
  USART_InitStruct.USART_WordLength = USART_WORDLENGTH_8B;
  USART_InitStruct.USART_StopBits = USART_STOPBITS_1;
  USART_InitStruct.USART_Parity = USART_PARITY_NO;
  USART_InitStruct.USART_Mode = USART_MODE_NORMAL;
  USART_Init(HT_USART1, &USART_InitStruct);
  USART_RxCmd(HT_USART1, ENABLE);
  USART_TxCmd(HT_USART1, ENABLE);

  USART_IntConfig(HT_USART1, USART_INT_RXDR, ENABLE);

  NVIC_EnableIRQ(USART1_IRQn);
  USART1_init_buffer();
}

//-----------------------------------------------------------------------------
void USART1_init_buffer (void)
{
  rxd_scomm1.read_pt = 0;
  rxd_scomm1.write_pt = 0;
  rxd_scomm1.cnt = 0;

  txd_scomm1.read_pt = 0;
  txd_scomm1.write_pt = 0;
  txd_scomm1.cnt = 0;
}

//-----------------------------------------------------------------------------
void USART1_IRQHandler(void)
{
  // Tx, move data from buffer to UART FIFO
  if ((HT_USART1->SR) & USART_FLAG_TXC)
  {
    if (!txd_scomm1.cnt)
    {
      USART_IntConfig(HT_USART1, USART_INT_TXC, DISABLE);
    }
    else
    {
      u16 i;
      for (i = 0; i < USART1_FIFO_LEN; i++)   //have FIFO?
      {
        USART_SendData(HT_USART1, txd_scomm1.buffer[txd_scomm1.read_pt]);
        txd_scomm1.read_pt = (txd_scomm1.read_pt + 1) % USART1_BUF_SIZE;
        txd_scomm1.cnt--;
        if (!txd_scomm1.cnt)
          break;
      }
    }
  }

  // Rx, move data from UART FIFO to buffer
  if ((HT_USART1->SR) & USART_FLAG_RXDR)
  {
    rxd_scomm1.buffer[rxd_scomm1.write_pt] = USART_ReceiveData(HT_USART1);

    rxd_scomm1.write_pt = (rxd_scomm1.write_pt + 1) % USART1_BUF_SIZE;
    rxd_scomm1.cnt++;
  }

}

//-----------------------------------------------------------------------------
void USART1_analyze_data(void)
{
  u8 tmp;
  if(!rxd_scomm1.cnt)
    return;

  //...to be add
  tmp = rxd_scomm1.buffer[rxd_scomm1.read_pt];
  if( (tmp != 0xAA) )            //for example
  {
    NVIC_DisableIRQ(USART1_IRQn);
    rxd_scomm1.cnt--;  //throw invalid data
    NVIC_EnableIRQ(USART1_IRQn);
    rxd_scomm1.read_pt = (rxd_scomm1.read_pt + 1) % USART1_BUF_SIZE;
    return;
  }
  else if(rxd_scomm1.cnt >= 8)    //for example
  {
    //add your code here

  }
}

//-----------------------------------------------------------------------------
void USART1_tx_data(u8 *pt, u8 len)
{
  while(len--)
  {
    txd_scomm1.buffer[txd_scomm1.write_pt] = *pt++;
    txd_scomm1.write_pt = (txd_scomm1.write_pt + 1) % USART1_BUF_SIZE;
    NVIC_DisableIRQ(USART1_IRQn);
    txd_scomm1.cnt++;
    NVIC_EnableIRQ(USART1_IRQn);
  }

  if(txd_scomm1.cnt)
    USART_IntConfig(HT_USART1,  USART_INT_TXC , ENABLE);
}

void USART1_SendByte(u8 byte)
{
	while (USART_GetFlagStatus(HT_USART1, USART_FLAG_TXC) == RESET); 
	USART_SendData(HT_USART1, byte);
}

void USART1_SendArray(u8* Array, uint16_t Length) {

	for (uint16_t i = 0; i < Length; i++)
	{
		USART1_SendByte(Array[i]);
	}
}

//-----------------------------------------------------------------------------
void USART1_test(void)
{
  u8 i,test_array[8];
  for(i=0; i<8; i++)
  {
    test_array[i] = i;
  }
  USART1_tx_data(test_array, 8);
}

//-----------------------------------------------------------------------------
void USART1_ClearBuffer(void) {
	memset(rxd_scomm1.buffer, 0, sizeof(rxd_scomm1.buffer));
	USART1_init_buffer();
}

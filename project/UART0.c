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
#include "UART0.h"
//-----------------------------------------------------------------------------
void UART0_Configuration(void)
{
  USART_InitTypeDef USART_InitStruct;
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  CKCUClock.Bit.UART0   = 1;
  CKCUClock.Bit.AFIO    = 1;
  CKCUClock.Bit.PC      = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);

  GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_5, GPIO_PR_UP);  //Turn on UxART Rx internal pull up resistor to prevent unknow state

  AFIO_GPxConfig(GPIO_PC, AFIO_PIN_4, AFIO_FUN_USART_UART);  // Config AFIO mode
  AFIO_GPxConfig(GPIO_PC, AFIO_PIN_5, AFIO_FUN_USART_UART);  // Config AFIO mode

  USART_InitStruct.USART_BaudRate = 115200;
  USART_InitStruct.USART_WordLength = USART_WORDLENGTH_8B;
  USART_InitStruct.USART_StopBits = USART_STOPBITS_1;
  USART_InitStruct.USART_Parity = USART_PARITY_NO;
  USART_InitStruct.USART_Mode = USART_MODE_NORMAL;
  USART_Init(HT_UART0, &USART_InitStruct);
  USART_RxCmd(HT_UART0, ENABLE);
  USART_TxCmd(HT_UART0, ENABLE);

}

//-----------------------------------------------------------------------------



/**
  * @name  		AT_Manage.c
  * @describe	AT指令操作封装
  * @data  		2025/3/17
  * @author 	liangziyue
  */
#include "AT_Manage.h"

/**
  * @name  		AT_Analyse	
  * @describe	分析AT指令返回值内容
  * @param  	u8* Receive	返回的内容 | char* Target	目标值
  * @return 	符合目标值 --> true	| 不符合目标值 --> false
  * @author 	liangziyue
  */
bool AT_Analyse(u8* Receive, char* Target) {
	char* res = strstr((char*)Receive, Target);
	if (res != NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**
  * @name  		AT_Command	
  * @describe	发送AT指令，并判断结果是否符合预期
  * @param  	char* Send	AT指令，不需要带\r\n  | char* Target  期待返回的结果  | uint32_t timeout  超时时间
  * @return 	符合预期结果且不超时 --> true	|	不符合结果或者超时 --> false
  * @author 	liangziyue
  */
bool AT_Command(char* Send, char* Target, uint32_t timeout) {
	USART1_ClearBuffer();

	char* result = (char*)malloc(strlen(Send) + strlen("\r\n"));
	if (result == NULL)
	{
		return false;
	}
	strcpy(result, Send);
	strcat(result, "\r\n");
	USART1_tx_data((u8*)result, strlen(result));
	free(result);
	
	while (timeout)
	{
		Delay_ms(1);
		timeout--;
		if (AT_Analyse(rxd_scomm1.buffer, Target))
		{
			return true;
		}
	}
	return false;
}

/**
  * @name  		AT_CommandGet	
  * @describe	使用于需要获取返回信息的AT命令
  * @param  	char* Send 命令 | char* Target 成功的标志 | char* Return 需要获取返回值的标志	| uint32_t timeout 超时时间
  * @return 	char* （使用完后，记得把返回值free后并且置NULL，否则会导致内存泄露！！！！！）
  * @author 	liangziyue
  */
char* AT_CommandGet(char* Send, char* Target, char* Return, uint32_t timeout) {
	USART1_ClearBuffer();

	char* result = (char*)malloc(strlen(Send) + strlen("\r\n"));
	if (result == NULL)
	{
		printf("AT Command malloc fail\r\n");
		return result;
	}
	strcpy(result, Send);
	strcat(result, "\r\n");
	USART1_tx_data((u8*)result, strlen(result));
	free(result);
	
	while (timeout)
	{
		Delay_ms(1);
		timeout--;
		if (AT_Analyse(rxd_scomm1.buffer, Target))
		{
			USART_TxRxCmd(HT_USART1, USART_CMD_RX, DISABLE);
			char* find_ret = strstr((char*)rxd_scomm1.buffer, Return);
			if (find_ret == NULL)
			{
				printf("AT	No related string found\r\n");
				return find_ret;
			}
			size_t length = strcspn(find_ret, "\r\n");
			char* return_str = (char*)malloc(length * sizeof(char) + sizeof('\0'));
			if (return_str == NULL)
			{
				printf("AT malloc fail\r\n");
				return return_str;
			}
			strncpy(return_str, find_ret, length);
			strcat(return_str, "\0");
			USART_TxRxCmd(HT_USART1, USART_CMD_RX, ENABLE);
			return return_str;
		}
	}
	
	printf("AT Time OUT \r\n");
	return NULL;
}

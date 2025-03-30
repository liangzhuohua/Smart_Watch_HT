/**
  * @name  		AT_Manage.c
  * @describe	ATָ�������װ
  * @data  		2025/3/17
  * @author 	liangziyue
  */
#include "AT_Manage.h"

/**
  * @name  		AT_Analyse	
  * @describe	����ATָ���ֵ����
  * @param  	u8* Receive	���ص����� | char* Target	Ŀ��ֵ
  * @return 	����Ŀ��ֵ --> true	| ������Ŀ��ֵ --> false
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
  * @describe	����ATָ����жϽ���Ƿ����Ԥ��
  * @param  	char* Send	ATָ�����Ҫ��\r\n  | char* Target  �ڴ����صĽ��  | uint32_t timeout  ��ʱʱ��
  * @return 	����Ԥ�ڽ���Ҳ���ʱ --> true	|	�����Ͻ�����߳�ʱ --> false
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
  * @describe	ʹ������Ҫ��ȡ������Ϣ��AT����
  * @param  	char* Send ���� | char* Target �ɹ��ı�־ | char* Return ��Ҫ��ȡ����ֵ�ı�־	| uint32_t timeout ��ʱʱ��
  * @return 	char* ��ʹ����󣬼ǵðѷ���ֵfree������NULL������ᵼ���ڴ�й¶������������
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

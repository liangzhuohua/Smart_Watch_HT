/**
  * @name  		air724.c
  * @describe	air724   USART1
  * @data  		2025/3/17
  * @author 	liangziyue
  */
#include "Air724.h"
#include "MQTT_Analyse.h"

char temp[1024];

/**
  * @name  		Air_CallPhoneInit	
  * @describe	打电话初始化
  * @param  	void
  * @return 	void
  * @author 	liangziyue
  */
void Air_CallPhoneInit(void) {
	
//	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_6, SET);
//	Delay_s(2);
//	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_6, RESET);
//	Delay_s(10);
	
	printf("ATE0 %d\r\n", AT_Command("ATE0", "OK", 200));
	printf("AT %d\r\n", AT_Command("AT", "OK", 200));
	printf("ICCID %d\r\n", AT_Command("AT+ICCID", "OK", 200));
	printf("CGATT %d\r\n", AT_Command("AT+CGATT?", "1", 200));
	printf("AT+SETVOLTE=1 %d\r\n", AT_Command("AT+SETVOLTE=1", "OK", 200));
	printf("AT+CLIP=1 %d\r\n", AT_Command("AT+CLIP=1", "OK", 200));
}

/**
  * @name  		Air_CallPhone	
  * @describe	打电话
  * @param  	num	电话号码 | text 电话要讲的内容
  * @return 	void
  * @author 	liangziyue
  */
void Air_CallPhone(char* num, char* text) {
	sprintf(temp, "ATD%s", num);
	bool res = AT_Command(temp, "CONNECT", 100000);
	printf("ATD Phone %d\r\n", res);
	sprintf(temp, "AT+CTTS=2,\"%s\"", text);
	Delay_s(2);
	if (res == true)
	{
		printf("Speek %s %d\r\n", temp, AT_Command(temp, "OK", 1000));
	}
	else
	{
		printf("Speek ERROR\r\n");
	}
}

/**
  * @name  		Air_SendMessagesInit	
  * @describe	发短信初始化
  * @param  	void
  * @return 	void
  * @author 	liangziyue
  */
void Air_SendMessagesInit(void) {

	printf("Reset\r\n");
	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_6, SET);
	Delay_s(2);
	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_6, RESET);
	Delay_s(10);
	
	printf("ATE0 %d\r\n", AT_Command("ATE0", "OK", 200));
	printf("AT %d\r\n", AT_Command("AT", "OK", 1000));
	printf("ICCID %d\r\n", AT_Command("AT+ICCID", "OK", 1000));
	printf("CGATT %d\r\n", AT_Command("AT+CGATT?", "1", 1000));
	printf("AT+CSMS=1 %d\r\n", AT_Command("AT+CSMS=1", "OK", 1000));
	printf("AT+CMGF=1 %d\r\n", AT_Command("AT+CMGF=1", "OK", 1000));
	printf("AT+CSMP=1 %d\r\n", AT_Command("AT+CSMP=17,167,0,8", "OK", 1000));
}

/**
  * @name  		Air_SendMessages	
  * @describe	发送短信
  * @param  	num	电话号码
  * @return 	void
  * @author 	liangziyue
  */
void Air_SendMessages(char* num) {

	memset(temp, 0, sizeof(temp));
	// AT+CMGS="13424733810"
	sprintf(temp, "AT+CMGS=\"%s\"", num);
	bool res = AT_Command(temp, ">", 100000);
	printf("AT+CMGS %d\r\n", res);
	//u6211u6454u5012u4e86		后要加0x1A代表结束
	u8 hex[9] = {0x62, 0x11, 0x64, 0x54, 0x50, 0x12, 0x4E, 0x86, 0x1A};

	Delay_s(2);
	USART1_ClearBuffer();
	USART1_SendArray(hex, 9);
	Delay_s(1);
	printf("\r\n%s\r\n", rxd_scomm1.buffer);
}

/**
  * @name  		Air_LocationInit	
  * @describe	或许定位初始化
  * @param  	void
  * @return 	void
  * @author 	liangziyue
  */
void Air_LocationInit(void) {
	printf("ATE0 %d\r\n", AT_Command("ATE0", "OK", 200));		
	printf("AT %d\r\n", AT_Command("AT", "OK", 1000));
	printf("ICCID %d\r\n", AT_Command("AT+ICCID", "OK", 1000));
	printf("CGATT %d\r\n", AT_Command("AT+CGATT?", "1", 1000));
	printf("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"	%d\r\n", AT_Command("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"", "OK", 1000));	// 设置PDP承载类型，固定格式
	printf("AT+SAPBR=3,1,\"APN\",\"\"	%d", AT_Command("AT+SAPBR=3,1,\"APN\",\"\"", "OK", 1000));						// 设置PDP承载的APN格式，固定格式
	printf("AT+SAPBR=1,1	%d", AT_Command("AT+SAPBR=1,1", "OK", 1000));												// 激活PDP，固定格式
	printf("AT+SAPBR=2,1	%d", AT_Command("AT+SAPBR=2,1", "OK", 1000));
}

/**
  * @name  		Air_LocationByCipgsm	
  * @describe	基站定位
  * @param  	void
  * @return 	void
  * @author 	liangziyue
  */
void Air_LocationByCipgsm(void) {
	char* location = AT_CommandGet("AT+CIPGSMLOC=1,1", "OK", "+CIPGSMLOC:", 60000);
	if (location == NULL)
	{
		printf("Get location fail\r\n");
	}
	printf("location = %s\r\n", location);
	free(location);
	location = NULL;
}

/**
  * @name  		Air_LocationByWifi	
  * @describe	Wifi定位
  * @param  	void
  * @return 	void
  * @author 	liangziyue
  */
void Air_LocationByWifi(void) {
	char* location = AT_CommandGet("AT+WIFILOC=1,1", "OK", "+WIFILOC:", 60000);
	if (location == NULL)
	{
		printf("Get location fail\r\n");
	}
	printf("location = %s\r\n", location);
	free(location);
	location = NULL;
}

/**
  * @name  		Air_LocationByGPS	
  * @describe	GPS定位
  * @param  	void
  * @return 	void
  * @author 	liangziyue
  */
void Air_LocationByGPS(void) {
	printf("AT+CGNSPWR=1	%d", AT_Command("AT+CGNSPWR=1", "OK", 1000));	// 开启GPS
	printf("AT+CGNSAID=1,1,1,1	%d", AT_Command("AT+CGNSAID=1,1,1,1", "OK", 1000));	// 开启辅助定位，加快定位速度
	char* location = AT_CommandGet("AT+CGNSINF", "OK", "+CGNSINF", 120000);			// 开始定位
	if (location == NULL)
	{
		printf("Get location fail\r\n");
	}
	printf("location = %s\r\n", location);
	free(location);
	location = NULL;	
}

/**
  * @name  		Air_MQTTInit	
  * @describe	MQTT初始化
  * @param  	ip 服务器ip地址，port服务器端口，user用户名，password密码
  * @return 	void
  * @author 	liangziyue
  */
void Air_MQTTInit(char* ip, char* port, char* user, char* password) {
	
	printf("Reset\r\n");
	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_6, SET);
	Delay_s(2);
	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_6, RESET);
	Delay_s(8);
	
	
	printf("ATE0 %d\r\n", AT_Command("ATE0", "OK", 200));
	printf("AT %d\r\n", AT_Command("AT", "OK", 1000));
	printf("ICCID %d\r\n", AT_Command("AT+ICCID", "OK", 1000));
	printf("CGATT %d\r\n", AT_Command("AT+CGATT?", "1", 1000));
	printf("AT+CSTT	%d\r\n", AT_Command("AT+CSTT=\"\",\"\",\"\"", "OK", 1000));		// 设置APN
	printf("AT+CIICR %d\r\n", AT_Command("AT+CIICR", "OK", 1000));
	
	USART1_ClearBuffer();
	USART1_tx_data((u8*)"AT+CIFSR\r\n", strlen("AT+CIFSR\r\n"));
	Delay_ms(500);
	printf("AT+CIFSR %s\r\n", rxd_scomm1.buffer);
	
	memset(temp, 0, sizeof(temp));
	sprintf(temp, "AT+MCONFIG=%s,%s,%s", user, user, password);		// 设置MQTT参数连接
	printf("%s %d\r\n", temp, AT_Command(temp, "OK", 1000));
	
	memset(temp, 0, sizeof(temp));
	sprintf(temp, "AT+MIPSTART=%s,%s", ip, port);
	printf("%s %d\r\n", temp, AT_Command(temp, "CONNECT", 1000));
	
	printf("AT+MCONNECT=1,120 %d\r\n",AT_Command("AT+MCONNECT=1,120", "CONNECT OK", 1000));
	
}

/**
  * @name  		Air_MQTTSub	
  * @describe	MQTT订阅消息
  * @param  	subtopic订阅主题
  * @return 	void
  * @author 	liangziyue
  */
void Air_MQTTSub(MQTT_Handler_t* mqtt_handler, char* topic) {
	mqtt_handler->topic = (char*)malloc(strlen(topic) * sizeof(char) + sizeof('\0'));
	strcpy(mqtt_handler->topic, topic);
	strcat(mqtt_handler->topic, "\0");
	printf("mqtt_handler->topic = %s\r\n", mqtt_handler->topic);
	memset(temp, 0, sizeof(temp));
	sprintf(temp, "AT+MSUB=%s,0", topic);
	printf("subtopic=%s %d\r\n", topic, AT_Command(temp, "SUBACK", 1000));
}

/**
  * @name  		Air_MQTTPub	
  * @describe	MQTT发布消息
  * @param  	pubtopic发布主题，message要发布的消息内容
  * @return 	void
  * @author 	liangziyue
  */
void Air_MQTTPub(char* pubtopic, char* message) {
	memset(temp, 0, sizeof(temp));
	uint16_t lenght = strlen(message);
	sprintf(temp, "AT+MPUBEX=%s,0,0,%d", pubtopic, lenght);
	printf("AT+MPUBEX  %d", AT_Command(temp, ">", 1000));
	printf("Pub  %d", AT_Command(message, "OK", 1000));
}


bool Air_MQTTLoop(MQTT_Handler_t* mqtt_handle) {
	char* result = NULL;
	char* temp = NULL;
	if (rxd_scomm1.cnt == 0)
	{
		return false;
	}
	result = strstr((char*)rxd_scomm1.buffer, "+MSUB:");
	if (result == NULL)
	{
		return false;
	}
	temp = strstr((char*)rxd_scomm1.buffer, "{");
	if (temp == NULL)
	{
		return false;
	}
	temp = strstr((char*)rxd_scomm1.buffer, "}");
	if (temp == NULL)
	{
		return false;
	}
	size_t length = strcspn(result, "}") + 1;
	char* analyse = (char*)malloc(length * sizeof(char) + sizeof('\0'));
	strncpy(analyse, result, length);
	strcat(analyse, "\0");
	char* analyse_json = strstr(analyse, "{");
//	printf("analyse=%s\r\n", analyse);
	
	
	if (!MQTT_AnalyseData(analyse_json, mqtt_handle))
	{
		printf("MQTT_AnalyseData Fail\r\n");
		return false;
	}
	printf("topic: %s\r\n", mqtt_handle->topic);
	printf("key: %s\r\n", mqtt_handle->key);
	printf("value: %s\r\n", mqtt_handle->value);
	
	// 分析完后要把analyse清除掉，防止内存泄露
	free(analyse);
	analyse = NULL;
	USART1_ClearBuffer();
	return true;
}

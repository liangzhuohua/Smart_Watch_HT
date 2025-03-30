#ifndef __AIR724_H__
#define __AIR724_H__

#include "ht32.h"
#include <string.h>
#include "Delay.h"
#include "AT_Manage.h"
#include "GPIO.h"


typedef struct {
	char* topic;
	char* key;
	char* value;
}MQTT_Handler_t;


void Air_CallPhoneInit(void);
void Air_CallPhone(char* num, char* text);
void Air_SendMessagesInit(void);
void Air_SendMessages(char* num);
void Air_LocationInit(void);
void Air_LocationByCipgsm(void);
void Air_LocationByWifi(void);
void Air_LocationByGPS(void);
void Air_MQTTInit(char* ip, char* port, char* user, char* password);
void Air_MQTTSub(MQTT_Handler_t* mqtt_handler, char* topic);
void Air_MQTTPub(char* pubtopic, char* message);
bool Air_MQTTLoop(MQTT_Handler_t* mqtt_handle);

#endif

#include "MQTT_Analyse.h"

bool MQTT_AnalyseData(char* json_str, MQTT_Handler_t* mqtt_handle) {
	cJSON* json = NULL;
	cJSON* item = NULL;
	printf("json_str = %s\r\n", json_str);
	json = cJSON_Parse(json_str);
	if (json == NULL)
	{
		printf("JSON Parse fail\r\n");
		return false;
	}
	item = cJSON_GetObjectItem(json, "msg");
	mqtt_handle->key = (char*)malloc(sizeof(char) * strlen("msg") + sizeof('\0'));
	if (mqtt_handle->key == NULL)
	{
		printf("JSON key malloc fail\r\n");
		return false;
	}
	mqtt_handle->key = "msg\0";
	
	mqtt_handle->value = (char*)malloc(sizeof(char) * strlen(item->valuestring));
	if (mqtt_handle->value == NULL)
	{
		printf("JSON value malloc fail\r\n");
		return false;
	}
	strcpy(mqtt_handle->value, item->valuestring);
	cJSON_Delete(json);
	
	return true;
	
}

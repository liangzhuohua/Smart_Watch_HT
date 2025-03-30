#ifndef __MQTT_ANALYSE_H__
#define __MQTT_ANALYSE_H__

#include "air724.h"
#include "ht32.h"
#include "cJSON.h"
#include "USART1.h"
#include "string.h"
#include <stdbool.h>

bool MQTT_AnalyseData(char* json_str, MQTT_Handler_t* mqtt_handle);

#endif







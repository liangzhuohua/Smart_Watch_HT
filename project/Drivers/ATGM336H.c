#include "ATGM336H.h"

ATGM_LOCATIONSTR LocationStr;
ATGM_LOACTION Location;

char ATGM_AnalyseBuffer[ATGM_AnalyseBufferSize];

void ATGM_Init(void) {
	LocationStr.IsReady = false;
	LocationStr.Latitude = NULL;
	LocationStr.Longitude = NULL;
	LocationStr.UTCTime = NULL;
	USART0_Configuration();
	}

bool ATGM_AnalyseLocation(void) {
	if (ATGM_AnalyseBufferReady == false)
	{
		return false;
	}
	char* ret1 = strstr(ATGM_AnalyseBuffer, "GNRMC");
	size_t length = strcspn(ret1, "\r\n");
	char* ret2 = (char*)malloc(length * sizeof(char) + sizeof('\0'));
	strncpy(ret2, ret1, length);
	strcat(ret2, "\0");	
	char* ret3 = strstr(ret2, "A");
	if (ret3 == NULL)
	{
		LocationStr.IsReady = false;
		printf("Location failure\r\n");
		free(ret2);
		ret2 = NULL;
		return false;
	}
	char* token = strtok(ret2, ",");
	uint8_t i = 0;
	while (token != NULL)
	{
//		printf("%s\r\n", token);
		switch (i)
		{
		case 1:
		{
			if (LocationStr.UTCTime != NULL)
			{
				free(LocationStr.UTCTime);
				LocationStr.UTCTime = NULL;
			}
			LocationStr.UTCTime = (char*)malloc(sizeof(token) + sizeof('\0'));			
			strcpy(LocationStr.UTCTime, token);
			strcat(LocationStr.UTCTime, "\0");
			break;
		}
			/* code */

		case 2:
		{
			if (strcmp(token, "A"))
			{
				LocationStr.IsReady = false;
			}
			else
			{
				LocationStr.IsReady = true;
			}
			break;
		}

		case 3:
		{

			if (LocationStr.Latitude != NULL)
			{
				free(LocationStr.Latitude);
				LocationStr.Latitude = NULL;
			}
			LocationStr.Latitude = (char*)malloc(sizeof(token) + sizeof('\0'));
			strcpy(LocationStr.Latitude, token);
			strcat(LocationStr.Latitude, "\0");
			break;
		}

		case 5:
		{
			if (LocationStr.Longitude != NULL)
			{
				free(LocationStr.Longitude);
				LocationStr.Longitude = NULL;
			}
			LocationStr.Longitude = (char*)malloc(sizeof(token) + sizeof('\0'));
			strcpy(LocationStr.Longitude, token);
			strcat(LocationStr.Longitude, "\0");
			break;
		}
		default:
			break;
		}
		token = strtok(NULL, ",");
		i++;
	}
	free(ret2);
	ret2 = NULL;
	ATGM_TransforLocation();
//	printf("UTCTime %s\r\n", LocationStr.UTCTime);
//	printf("Latitude %s\r\n", LocationStr.Latitude);
//	printf("Longitude %s\r\n", LocationStr.Longitude);
//	printf("IsReady %d\r\n", LocationStr.IsReady);	
	return true;
}

void ATGM_TransforLocation(void) {
	ATGM_LOACTION Location;
	Location.Latitude = 0;
	Location.Longitude = 0;

	uint16_t temp1 = 0;
	uint16_t temp2 = 0;

	Location.Latitude = strtod(LocationStr.Latitude, NULL);
	printf("Latitude\t%f\r\n", Location.Latitude);
	Location.Longitude = strtod(LocationStr.Longitude, NULL);
	printf("Longitude\t%f\r\n", Location.Longitude);

	// 纬度信息处理
	// 五位纬度信息
	if ((Location.Latitude - 10000.0) >= 0)
	{
		temp1 = (((uint16_t)Location.Latitude / 10000) % 10) * 100 + (((uint16_t)Location.Latitude / 1000) % 10) * 10 + ((uint16_t)Location.Latitude / 100) % 10;
		Location.Latitude = Location.Latitude - (float)temp1 * 100;
		Location.Latitude = (float)temp1 + Location.Latitude / 60;
	}
	else	// 四位纬度信息
	{
		temp1 = (((uint16_t)Location.Latitude / 1000) % 10) * 10 + ((uint16_t)Location.Latitude / 100) % 10;
		Location.Latitude = Location.Latitude - (float)temp1 * 100;
		Location.Latitude = (float)temp1 + Location.Latitude / 60;	
	}

	// 经度信息处理
	// 五位经度信息
	if ((Location.Longitude - 10000.0) >= 0)
	{
		temp2 = (((uint16_t)Location.Longitude / 10000) % 10) * 100 + (((uint16_t)Location.Longitude / 1000) % 10) * 10 + ((uint16_t)Location.Longitude / 100) % 10;
		Location.Longitude = Location.Longitude - (float)temp2 * 100;
		Location.Longitude = (float)temp2 + Location.Longitude / 60;
	}
	else   // 四位经度信息
	{
		// 前两位需要单独拿出来组成一个数
		temp2 = (((uint16_t)Location.Longitude / 1000) % 10) * 10 + ((uint16_t)Location.Longitude / 100) % 10;
		Location.Longitude = Location.Longitude - (float)temp2 * 100;
		Location.Longitude = (float)temp2 + Location.Longitude / 60;
		
	}
	printf("Latitude_T\t%f\r\n", Location.Latitude);
	printf("Longitude_T\t%f\r\n", Location.Longitude);

}

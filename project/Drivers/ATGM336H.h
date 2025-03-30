#ifndef __ATGM336_H__
#define __ATGM336_H__

#include "ht32.h"
#include "USART0.h"
#include "UART0.h"
#include <string.h>
#include <stdlib.h>

#define ATGM_AnalyseBufferSize		256

extern char ATGM_AnalyseBuffer[ATGM_AnalyseBufferSize];

typedef struct {
	char* UTCTime;
	char* Latitude;			// γ��
	char* Longitude;		// ����
	bool IsReady;
}ATGM_LOCATIONSTR;

// GPS ����ϵ
typedef struct {
	float Latitude;			// γ��
	float Longitude;		// ����
}ATGM_LOACTION;


extern ATGM_LOCATIONSTR LocationStr;
extern ATGM_LOACTION Location;

void ATGM_Init(void);
bool ATGM_AnalyseLocation(void);
void ATGM_TransforLocation(void);

#endif

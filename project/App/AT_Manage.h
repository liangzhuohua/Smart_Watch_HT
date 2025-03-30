#ifndef __AT_MANAGE_H__
#define __AT_MANAGE_H__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "USART1.h"
#include "Delay.h"

bool AT_Command(char* Send, char* Target, uint32_t timeout);
char* AT_CommandGet(char* Send, char* Target, char* Return, uint32_t timeout);

#endif

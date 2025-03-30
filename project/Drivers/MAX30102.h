#ifndef _MAX30102_H
#define _MAX30102_H
 
#include "ht32.h"              // Device header
#include "delay.h"
#include "I2C0.h"
#include "EXTI.h"
#include <stdbool.h>
#include "algorithm.h"




 
#define I2C_WRITE_ADDR 0x57
#define I2C_READ_ADDR 0x57
 
//register addresses
#define REG_INTR_STATUS_1 0x00
#define REG_INTR_STATUS_2 0x01
#define REG_INTR_ENABLE_1 0x02
#define REG_INTR_ENABLE_2 0x03
#define REG_FIFO_WR_PTR 0x04
#define REG_OVF_COUNTER 0x05
#define REG_FIFO_RD_PTR 0x06
#define REG_FIFO_DATA 0x07
#define REG_FIFO_CONFIG 0x08
#define REG_MODE_CONFIG 0x09
#define REG_SPO2_CONFIG 0x0A
#define REG_LED1_PA 0x0C
#define REG_LED2_PA 0x0D
#define REG_PILOT_PA 0x10
#define REG_MULTI_LED_CTRL1 0x11
#define REG_MULTI_LED_CTRL2 0x12
#define REG_TEMP_INTR 0x1F
#define REG_TEMP_FRAC 0x20
#define REG_TEMP_CONFIG 0x21
#define REG_PROX_INT_THRESH 0x30
#define REG_REV_ID 0xFE
#define REG_PART_ID 0xFF
 
#define MAX_BRIGHTNESS 255


extern int32_t n_sp02;                 //这个变量一会用来存放获得的血氧值
extern int8_t ch_spo2_valid;           //这个变量用来指明获得的血氧值是否有效
extern int32_t n_heart_rate;           //这个变量一会用来存放获得的心率值
extern int8_t  ch_hr_valid;            //这个变量用来指明获得的心率值是否有效

void MAX30102_Reset(void);
void MAX30102_WriteByte(uint8_t reg_addr, uint8_t byte);
uint8_t MAX30102_ReadByte(uint8_t reg_addr, uint8_t* byte);
uint8_t MAX30102_ReadLen(uint8_t reg_addr, uint8_t* data, uint8_t size);
void MAX30102_Init(void);
void MAX30102_ReadFIFOBytes(void);
void MAX30102_GetSpo2AndHeartRate(void);

#endif


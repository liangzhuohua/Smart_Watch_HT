 /************************************************************************************************************
 * @attention
 *
 * Firmware Disclaimer Information
 *
 * 1. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, which is supplied by Holtek Semiconductor Inc., (hereinafter referred to as "HOLTEK") is the
 *    proprietary and confidential intellectual property of HOLTEK, and is protected by copyright law and
 *    other intellectual property laws.
 *
 * 2. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, is confidential information belonging to HOLTEK, and must not be disclosed to any third parties
 *    other than HOLTEK and the customer.
 *
 * 3. The program technical documentation, including the code, is provided "as is" and for customer reference
 *    only. After delivery by HOLTEK, the customer shall use the program technical documentation, including
 *    the code, at their own risk. HOLTEK disclaims any expressed, implied or statutory warranties, including
 *    the warranties of merchantability, satisfactory quality and fitness for a particular purpose.
 *
 * <h2><center>Copyright (C) Holtek Semiconductor Inc. All rights reserved</center></h2>
 ************************************************************************************************************/
//Generated by HT32CodeConfig V1.1.4 Build 241129
//-----------------------------------------------------------------------------
#include "ht32.h"
#include "UART0.h"
#include "USART1.h"
#include "USART0.h"
#include "GPIO.h"
#include "Air724.h"
#include "MAX30102.h"
#include "ATGM336H.h"
#include "QMI8658A.h"
#include "OLED.h"
#include "IMU.h"
//-----------------------------------------------------------------------------

int main(void)
{
  #if (HTCFG_STACK_USAGE_ANALYSIS == 1)
  /* !!! NOTICE !!!
     Please update the Keil HT32 PACK and HT32 Firmware Library to the latest version to make sure the
     Stack Usage Analysis function works properly.
  */
  /*
    Set HTCFG_STACK_USAGE_ANALYSIS as 1 in the "ht32xxxxxx_conf.h" to enable Stack Usage Analysis feature.
    This feature is only applicable to the Keil MDK-ARM. Please call the "StackUsageAnalysisInit()" function
    in the begin of the "main()".
    The "StackUsageAnalysisInit()" parameter shall be the start address of the vector table.
    Under Keil Debug mode, tick "View > Watch Window > HT32 Stack Usage Analysis" to show the stack usage
    information. Those information is only valid after calling "StackUsageAnalysisInit()" function.
  */
  StackUsageAnalysisInit(0);
  #endif

	UART0_Configuration();
	USART1_Configuration();
	GPIO_Configuration();
	
	printf("hello\r\n");
	
//	Air_CallPhoneInit();
//	Air_CallPhone("13424733810", "江少锋你好 李锡鸿你好 李锡鸿你好");

//	Air_SendMessagesInit();
//	Air_SendMessages("13424733810");

//	Air_LocationInit();
//	Air_LocationByGPS();
//	
//	Air_MQTTInit("116.205.165.132", "1883", "test", "123456");
//	MQTT_Handler_t test;
//	
//	Air_MQTTSub(&test, "test");
//	printf("test->topic: %s\r\n", test.topic);

//	Air_MQTTPub("test", "{\"test\": \"Hello\"}");
	
	
//	MAX30102_Init();
	ATGM_Init();
//	OLED_Init();
//	OLED_ShowString(1, 1, (u8*)"hello", 16, 1);
//	OLED_Refresh();

	
	float gyro[3];
	float accel[3]; // 加速度

	float euler_angle[3] = {0, 0, 0};
	if (QMI_Init())
	{
		printf("陀螺仪初始化失败\r\n");
	}
	
	while (1)
	{
		
		QMI_ReadXYZ(accel, gyro);
//		sprintf(temp, "acc x: %.2f\r\n", accel[0]);
//		OLED_ShowString(1, 1, (u8*)temp, 8, 1);
//		
//		sprintf(temp, "acc y: %.2f\r\n", accel[1]);
//		OLED_ShowString(1, 8, (u8*)temp, 8, 1);
//		
//		
//		sprintf(temp, "acc z: %.2f\r\n", accel[2]);
//		OLED_ShowString(1, 16, (u8*)temp, 8, 1);	

//		sprintf(temp, "gyro x: %.2f\r\n", gyro[0]);
//		OLED_ShowString(1, 32, (u8*)temp, 8, 1);
//		
//		sprintf(temp, "gyro y: %.2f\r\n", gyro[1]);
//		OLED_ShowString(1, 40, (u8*)temp, 8, 1);
//		
//		
//		sprintf(temp, "gyro z: %.2f\r\n", gyro[2]);
//		OLED_ShowString(1, 48, (u8*)temp, 8, 1);

		if (g_imu_init)
		{
			imu_get_eulerian_angles(accel, gyro, euler_angle, IMU_DELTA_T);
		}

//		sprintf(temp, "x: %.2f\r\n", euler_angle[0]);
//		OLED_ShowString(1, 1, (u8*)temp, 16, 1);
//		sprintf(temp, "y: %.2f\r\n", euler_angle[1]);
//		OLED_ShowString(1, 16, (u8*)temp, 16, 1);
//		sprintf(temp, "z: %.2f\r\n", euler_angle[2]);
//		OLED_ShowString(1, 32, (u8*)temp, 16, 1);		
		
//		OLED_Refresh();
//		printf("acc x: %f\r\n", accel[0]);
//		printf("acc y: %f\r\n", accel[1]);
//		printf("acc z: %f\r\n", accel[2]);
//		printf("gyro x: %f\r\n", gyro[0]);
//		printf("gyro y: %f\r\n", gyro[1]);
//		printf("gyro z: %f\r\n", gyro[2]);
//		Air_MQTTLoop(&test);
//		MAX30102_GetSpo2AndHeartRate();
//		
//		printf("血氧值：%d , 心率：%d ----ch_spo2_valid：%d,ch_hr_valid:%d\r\n", n_sp02, n_heart_rate, ch_spo2_valid, ch_hr_valid);

		
//		printf("%s\r\n", rxd_scomm0.buffer);
		
		if (ATGM_AnalyseLocation())
		{
			printf("ATGM_AnalyseLocation yes\r\n");
		}
//		
		Delay_s(1);
		if(HT_CKCU->APBCCR1 & (1 << 4))
		WDT_Restart();
		//USART1_test(); //Can be removed
	
	}
}

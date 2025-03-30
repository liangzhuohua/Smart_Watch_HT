#ifndef __QMI8658A_H__
#define __QMI8658A_H__

#include "ht32.h"
#include "I2C1.h"
#include <stdbool.h>
#include <string.h>
#include "Delay.h"

/* ȫ�ֱ��������� */
extern volatile uint8_t g_imu_init;   


#define QMI8658_ADDR            0X6A                        /* SA0=1 ��ַΪ��0X6A, SA0=0 ��ַΪ��0X6B */        
#define ONE_G                   9.807f                      /* ���ٶȵ�λת��ʹ�� */
#define M_PI                    (3.14159265358979323846f)   /* �����ǵ�λת��ʹ�� */
#define MAX_CALI_COUNT          100                         /* �������� */

#define QMI8658_DISABLE_ALL     (0x0)                       /* �����Ǻͼ��ٶȼƾ���ʹ�� */
#define QMI8658_ACC_ENABLE      (0x1)                       /* ʹ�ܼ��ٶȼ� */
#define QMI8658_GYR_ENABLE      (0x2)                       /* ʹ�������� */
#define QMI8658_ACCGYR_ENABLE   (QMI8658_ACC_ENABLE | QMI8658_GYR_ENABLE)/* �����Ǻͼ��ٶȼƾ�ʹ�� */



/****************************************************************************************************************/
/* QMI8658A�ļĴ�����ַ */
/* ��ϸ˵������QMI8658A�����ֲ� */
enum Qmi8658AReg
{
    Register_WhoAmI = 0,
    Register_Revision,
    Register_Ctrl1,
    Register_Ctrl2,
    Register_Ctrl3,
    Register_Reserved,
    Register_Ctrl5,
    Register_Reserved1,
    Register_Ctrl7,
    Register_Ctrl8,
    Register_Ctrl9,
    Register_Cal1_L = 11,
    Register_Cal1_H,
    Register_Cal2_L,
    Register_Cal2_H,
    Register_Cal3_L,
    Register_Cal3_H,
    Register_Cal4_L,
    Register_Cal4_H,
    Register_FifoWmkTh = 19, 
    Register_FifoCtrl = 20,
    Register_FifoCount = 21,
    Register_FifoStatus = 22,
    Register_FifoData = 23,
    Register_StatusInt = 45,
    Register_Status0,
    Register_Status1,
    Register_Timestamp_L = 48,
    Register_Timestamp_M,
    Register_Timestamp_H,
    Register_Tempearture_L = 51,
    Register_Tempearture_H,
    Register_Ax_L = 53,
    Register_Ax_H,
    Register_Ay_L,
    Register_Ay_H,
    Register_Az_L,
    Register_Az_H,
    Register_Gx_L = 59,
    Register_Gx_H,
    Register_Gy_L,
    Register_Gy_H,
    Register_Gz_L,
    Register_Gz_H,
    Register_COD_Status = 70,
    Register_dQW_L = 73,
    Register_dQW_H,
    Register_dQX_L,
    Register_dQX_H,
    Register_dQY_L,
    Register_dQY_H,
    Register_dQZ_L,
    Register_dQZ_H,
    Register_dVX_L,
    Register_dVX_H,
    Register_dVY_L,
    Register_dVY_H,
    Register_dVZ_L,
    Register_dVZ_H,
    Register_TAP_Status = 89,
    Register_Step_Cnt_L = 90,
    Register_Step_Cnt_M = 91,
    Register_Step_Cnt_H = 92,
    Register_Reset = 96
};

/****************************************************************************************************************/
/* ��ϸ˵������QMI8658A�����ֲ� Ctrl9��ϸ����˵�� */
enum Ctrl9Command
{
    Ctrl9_Cmd_Ack                   = 0X00,
    Ctrl9_Cmd_RstFifo               = 0X04,
    Ctrl9_Cmd_ReqFifo               = 0X05, /* Get FIFO data from Device */
    Ctrl9_Cmd_WoM_Setting           = 0x08, /* ���ò������˶����� */
    Ctrl9_Cmd_AccelHostDeltaOffset  = 0x09, /* ���ļ��ٶȼ�ƫ�� */
    Ctrl9_Cmd_GyroHostDeltaOffset   = 0x0A, /* ����������ƫ�� */
    Ctrl9_Cmd_CfgTap                = 0x0C, /* ����TAP��� */
    Ctrl9_Cmd_CfgPedometer          = 0x0D, /* ���üƲ��� */
    Ctrl9_Cmd_Motion                = 0x0E, /* �����κ��˶�/���˶�/�����˶���� */
    Ctrl9_Cmd_RstPedometer          = 0x0F, /* ���üƲ��������������� */
    Ctrl9_Cmd_CopyUsid              = 0x10, /* �� USID �� FW �汾���Ƶ� UI �Ĵ��� */
    Ctrl9_Cmd_SetRpu                = 0x11, /* ���� IO ���� */
    Ctrl9_Cmd_AHBClockGating        = 0x12, /* �ڲ� AHB ʱ���ſؿ��� */
    Ctrl9_Cmd_OnDemandCalivration   = 0xA2, /* �����ǰ���У׼ */
    Ctrl9_Cmd_ApplyGyroGains        = 0xAA  /* �ָ���������������� */
};

/* ��ͨ�˲����Ŀ����͹ر� */
enum qmi8658_LpfConfig
{
    Qmi8658Lpf_Disable,
    Qmi8658Lpf_Enable
};

/* �Լ�ģʽ�Ŀ����͹ر� */
enum qmi8658_StConfig
{
    Qmi8658St_Disable,
    Qmi8658St_Enable
};

/* ���ٶȼƺ������ǵĵ�ͨ������ģʽѡ�� */
enum qmi8658_LpfMode
{
    A_LSP_MODE_0 = 0x00 << 1,
    A_LSP_MODE_1 = 0x01 << 1,
    A_LSP_MODE_2 = 0x02 << 1,
    A_LSP_MODE_3 = 0x03 << 1,

    G_LSP_MODE_0 = 0x00 << 5,
    G_LSP_MODE_1 = 0x01 << 5,
    G_LSP_MODE_2 = 0x02 << 5,
    G_LSP_MODE_3 = 0x03 << 5
};

/* ���ٶȼ�����ѡ�� */
enum qmi8658_accrange
{
    Qmi8658accrange_2g = 0x00 << 4,
    Qmi8658accrange_4g = 0x01 << 4,
    Qmi8658accrange_8g = 0x02 << 4,
    Qmi8658accrange_16g = 0x03 << 4
};

/* ���ٶȼ�ODR�������ѡ�� */
enum qmi8658_accodr
{
    Qmi8658accodr_8000Hz = 0x00,
    Qmi8658accodr_4000Hz = 0x01,
    Qmi8658accodr_2000Hz = 0x02,
    Qmi8658accodr_1000Hz = 0x03,
    Qmi8658accodr_500Hz = 0x04,
    Qmi8658accodr_250Hz = 0x05,
    Qmi8658accodr_125Hz = 0x06,
    Qmi8658accodr_62_5Hz = 0x07,
    Qmi8658accodr_31_25Hz = 0x08,
    Qmi8658accodr_LowPower_128Hz = 0x0c,
    Qmi8658accodr_LowPower_21Hz = 0x0d,
    Qmi8658accodr_LowPower_11Hz = 0x0e,
    Qmi8658accodr_LowPower_3Hz = 0x0f
};

/* ����������ѡ�� */
enum qmi8658_gyrrange
{
    Qmi8658gyrrange_16dps = 0 << 4,
    Qmi8658gyrrange_32dps = 1 << 4,
    Qmi8658gyrrange_64dps = 2 << 4,
    Qmi8658gyrrange_128dps = 3 << 4,
    Qmi8658gyrrange_256dps = 4 << 4,
    Qmi8658gyrrange_512dps = 5 << 4,
    Qmi8658gyrrange_1024dps = 6 << 4,
    Qmi8658gyrrange_2048dps = 7 << 4
};

/* �������������ѡ�� */
enum qmi8658_gyrodr
{
    Qmi8658gyrodr_8000Hz = 0x00,
    Qmi8658gyrodr_4000Hz = 0x01,
    Qmi8658gyrodr_2000Hz = 0x02,
    Qmi8658gyrodr_1000Hz = 0x03,
    Qmi8658gyrodr_500Hz = 0x04,
    Qmi8658gyrodr_250Hz = 0x05,
    Qmi8658gyrodr_125Hz = 0x06,
    Qmi8658gyrodr_62_5Hz = 0x07,
    Qmi8658gyrodr_31_25Hz = 0x08
};


typedef struct
{
    unsigned char ensensors;        /* ������ʹ�ܱ�־ */
    enum qmi8658_accrange accrange; /* ���ü��ٶȼƵ����� */
    enum qmi8658_accodr accodr;     /* ���ü��ٶȼƵ�ODR������� */     
    enum qmi8658_gyrrange gyrrange; /* ���������ǵ����� */
    enum qmi8658_gyrodr gyrodr;     /* ���������ǵ�ODR������� */     
    unsigned char syncsample;       /* ͬ��������־ */
} qmi8658_config;

typedef struct
{
    qmi8658_config cfg;             /* ��¼���������� */
    unsigned short ssvt_a;          /* ���ٶȼƵ�λ������� */
    unsigned short ssvt_g;          /* �����ǵ�λ������� */
    float imu[6];                   /* ��¼��һ�εļ��ٶȼƺ������ǵ�������� */
} qmi8658_state;


void QMI_WriteByte(uint8_t reg, uint8_t data);
uint8_t QMI_ReadByte(uint8_t reg);
void QMI_ReadNBytes(uint8_t reg, uint8_t* data, uint8_t len);
void QMI_WriteNBytes(uint8_t reg, uint8_t* data, uint8_t len);
bool QMI_CheckWhoAmI(void);
bool QMI_Calibration(void);
void QMI_Reset(void);
void QMI_ReadStatus(void);
uint8_t QMI_ReadState0(void);
uint8_t QMI_ReadState1(void);
void QMI_ConfigACC(enum qmi8658_accrange range, enum qmi8658_accodr odr, enum qmi8658_LpfConfig lpfEnable, enum qmi8658_StConfig stEnable);
void QMI_ConfigGYRO(enum qmi8658_gyrrange range, enum qmi8658_gyrodr odr, enum qmi8658_LpfConfig lpfEnable, enum qmi8658_StConfig stEnable);
int QMI_SendCt19Cmd(enum Ctrl9Command cmd);
float QMI_GetTemperature(void);
void QMI_ReadSensorData(float *acc, float *gyro);
void QMI_ReadXYZ(float *acc, float *gyro);
void QMI_EableAhbClock(bool enable);
void QMI_EnableSensors(unsigned char enableFlags);
void QMI_ConfigReg(unsigned char low_power);
bool QMI_Init(void);

#endif

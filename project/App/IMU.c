#include "IMU.h"

#define ACCZ_SAMPLE     80                      /* 350 */

float Kp    = 20;                               /* �������� */
float Ki    = 0.01f;                            /* �������� */
float exInt = 0.0f;
float eyInt = 0.0f;
float ezInt = 0.0f;                             /* ��������ۼ� */

static float q0 = 1.0f;                         /* ��Ԫ�� */
static float q1 = 0.0f;
static float q2 = 0.0f;
static float q3 = 0.0f;	
static float rMat[3][3];                        /* ��ת���� */

static float maxError = 0.f;                    /* ������ */
bool isGravityCalibrated = false;               /* �Ƿ�УУ׼��� */
//static float baseAcc[3] = {0.0f, 0.0f, 1.0f};   /* ��̬���ٶ� */

static float imu_inv_sqrt(float x);             /* ���ٿ�ƽ���� */

/**
 * @brief   ���㾲̬���ٶ�
 * @param   acc�����ٶȼ�ԭʼ����
 * @retval  ��
 */
static void imu_calbaseacc(float* acc)          
{
    static unsigned short int cnt = 0;
    static float accZMin = 1.5;
    static float accZMax = 0.5;
    static float sumAcc[3] = {0.f};

    if(cnt == 0)
    {
        accZMin = acc[2];
        accZMax = acc[2];

        for(unsigned char i=0; i<3; i++)
        sumAcc[i] = 0.0f;
    }

    for(unsigned char i = 0; i < 3; i++)
    sumAcc[i] += acc[i];

    if(acc[2] < accZMin)    accZMin = acc[2];
    if(acc[2] > accZMax)    accZMax = acc[2];

    if(++cnt >= ACCZ_SAMPLE) /* �������� */
    {
        cnt = 0;
        maxError = accZMax - accZMin;
        if(maxError < 100.0f)
        {
//            for(unsigned char i=0; i<3; i++)
//            {
//                baseAcc[i] = sumAcc[i] / ACCZ_SAMPLE;
//            }
            isGravityCalibrated = true;
            
        }
        for(unsigned char i=0; i<3; i++)
        {
            sumAcc[i] = 0.f;
        }
    }
}

/**
 * @brief   ������ת����
 * @param   ��
 * @retval  ��
 */
void imu_computerotationmatrix(void)
{
    float q1q1 = q1 * q1;
    float q2q2 = q2 * q2;
    float q3q3 = q3 * q3;

    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
    float q0q3 = q0 * q3;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q2q3 = q2 * q3;

    rMat[0][0] = 1.0f - 2.0f * q2q2 - 2.0f * q3q3;
    rMat[0][1] = 2.0f * (q1q2 + -q0q3);
    rMat[0][2] = 2.0f * (q1q3 - -q0q2);

    rMat[1][0] = 2.0f * (q1q2 - -q0q3);
    rMat[1][1] = 1.0f - 2.0f * q1q1 - 2.0f * q3q3;
    rMat[1][2] = 2.0f * (q2q3 + -q0q1);

    rMat[2][0] = 2.0f * (q1q3 + -q0q2);
    rMat[2][1] = 2.0f * (q2q3 - -q0q1);
    rMat[2][2] = 1.0f - 2.0f * q1q1 - 2.0f * q2q2;
}

/**
 * @brief   ��ȡŷ��������
 * @note    ��̬�����ں�, �����㷨�������˲��㷨��û��ʹ��Kalman�˲��㷨
 *          ������֤�ú����ĵ���Ƶ��Ϊ: IMU_DELTA_T , ����YAW����Ӧ��ƫ��/ƫС
 * @param   gyro  : 3������������
 * @param   acc   : 3����ٶ�����
 * @param   rpy   : ŷ���Ǵ��buf
 * @param   dt    : ����Ƶ��
 * @retval  ��
 */
void imu_get_eulerian_angles(float acc[3], float gyro[3], float *rpy , float dt) 
{
    float normalise;
    float ex, ey, ez;
    float halfT = 0.5f * dt;
    float accBuf[3] = {0.f};
    float tempacc[3];
    tempacc[0] = acc[0];
    tempacc[1] = acc[1];
    tempacc[2] = acc[2];
    static unsigned short int uiPostureInitWaitTimer = 200; /* �ȴ�һС��ʱ�䣬�ȽǶȳ�ʼ���ȶ����� */
    
    if(uiPostureInitWaitTimer > 0)
    {
        uiPostureInitWaitTimer--;
        Kp = 20;                                            /* ��ʼ�������Ƕȵ�ʱ���KPֵ��󣬱��ڿ���������ʵ��ֵ */
    }
    else
    {
        Kp = 1.0;
    }
    /* ��ת���� */
    gyro[0] = gyro[0];   
    gyro[1] = gyro[1];  
    gyro[2] = gyro[2]; 

    /* ���ٶȼ������Чʱ,���ü��ٶȼƲ���������*/
    if((acc[0] != 0.0f) || (acc[1] != 0.0f) || (acc[2] != 0.0f))
    {
        /* ��λ�����ټƲ���ֵ */
        normalise = imu_inv_sqrt(acc[0] * acc[0] + acc[1] * acc[1] + acc[2] * acc[2]);
        acc[0] *= normalise;
        acc[1] *= normalise;
        acc[2] *= normalise;

        /* ���ټƶ�ȡ�ķ������������ټƷ���Ĳ�ֵ����������˼��� */
        ex = (acc[1] * rMat[2][2] - acc[2] * rMat[2][1]);
        ey = (acc[2] * rMat[2][0] - acc[0] * rMat[2][2]);
        ez = (acc[0] * rMat[2][1] - acc[1] * rMat[2][0]);

        /* ����ۼƣ�����ֳ������ */
        exInt += Ki * ex * dt ;  
        eyInt += Ki * ey * dt ;
        ezInt += Ki * ez * dt ;

        /* �ò���������PI����������ƫ�����������ݶ����е�ƫ���� */
        gyro[0] += Kp * ex + exInt;
        gyro[1] += Kp * ey + eyInt;
        gyro[2] += Kp * ez + ezInt;
    }
    /* һ�׽����㷨����Ԫ���˶�ѧ���̵���ɢ����ʽ�ͻ��� */
    float q0Last = q0;
    float q1Last = q1;
    float q2Last = q2;
    float q3Last = q3;
    q0 += (-q1Last * gyro[0] - q2Last * gyro[1] - q3Last * gyro[2]) * halfT;
    q1 += ( q0Last * gyro[0] + q2Last * gyro[2] - q3Last * gyro[1]) * halfT;
    q2 += ( q0Last * gyro[1] - q1Last * gyro[2] + q3Last * gyro[0]) * halfT;
    q3 += ( q0Last * gyro[2] + q1Last * gyro[1] - q2Last * gyro[0]) * halfT;

    /* ��λ����Ԫ�� */
    normalise = imu_inv_sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= normalise;
    q1 *= normalise;
    q2 *= normalise;
    q3 *= normalise;

    imu_computerotationmatrix();     /* ������ת���� */

    /*����roll pitch yaw ŷ����*/
    /* pitch�Ƕȼ��㲻����ԭ��ŷ���Ǽ��㷽��-asinf(rMat[2][0]) * RAD2DEG��������atan2f(rMat[2][0], rMat[2][2]) * RAD2DEG
    ԭ����Ӧ�ò�ͬ������ֻҪ��Ҫ֪��ң��������б�Ƕ� */
    rpy[0] = asinf(rMat[2][0]) * RAD2DEG;               /* pitch */ 
    rpy[1] = atan2f(rMat[2][1], rMat[2][2]) * RAD2DEG;  /* roll */ 
    rpy[2] = -atan2f(rMat[1][0], rMat[0][0]) * RAD2DEG; /* yaw */ 

    if(0)//(!isGravityCalibrated)   /* δУ׼ */
    {
        accBuf[2] = tempacc[0]* rMat[2][0] + tempacc[1] * rMat[2][1] + tempacc[2] * rMat[2][2];	/*accz*/
        imu_calbaseacc(accBuf);         /* ���㾲̬���ٶ� */   
    }
}

/**
 * @brief   ��������
 * @param   x : ��������ֵ
 * @retval  �������
 */
float imu_inv_sqrt(float x)
{
    float halfx = 0.5f * x;
    float y = x;
    long i = *(long*)&y;
    i = 0x5f3759df - (i>>1);
    y = *(float*)&i;
    y = y * (1.5f - (halfx * y * y));
    return y;
}

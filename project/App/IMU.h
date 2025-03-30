#ifndef __IMU_H__
#define __IMU_H__

#include <math.h>
#include <stdbool.h>

#define IMU_DELTA_T     0.012f          /* ����ʱ�� */
#define DEG2RAD         0.017453293f    /* ��ת���� ��/180 */
#define RAD2DEG         57.29578f       /* ����ת�� 180/�� */

void imu_get_eulerian_angles(float acc[3], float gyro[3], float *rpy , float dt); /* ��ȡŷ���� */


#endif

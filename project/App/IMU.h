#ifndef __IMU_H__
#define __IMU_H__

#include <math.h>
#include <stdbool.h>

#define IMU_DELTA_T     0.012f          /* 采样时间 */
#define DEG2RAD         0.017453293f    /* 度转弧度 π/180 */
#define RAD2DEG         57.29578f       /* 弧度转度 180/π */

void imu_get_eulerian_angles(float acc[3], float gyro[3], float *rpy , float dt); /* 获取欧拉角 */


#endif

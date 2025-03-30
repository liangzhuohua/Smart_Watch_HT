#include "QMI8658A.h"

uint8_t QMI_write_buffer[2] = {0};

volatile uint8_t g_imu_init = 0;
qmi8658_state g_imu;

/**
  * @name  		QMI_WriteByte	
  * @describe	дһ���ֽڵ�QMI8568�ļĴ���
  * @param  	reg: �Ĵ�����ַ
  * @param		data: �Ĵ�������
  * @return 	void
  * @author 	liangziyue
  */
void QMI_WriteByte(uint8_t reg, uint8_t data) {
	QMI_write_buffer[0] = reg;
	QMI_write_buffer[1] = data;
	I2C1_write(QMI8658_ADDR, QMI_write_buffer, 2);
	I2C1_write_waiting();
}

/**
  * @name  		QMI_ReadByte	
  * @describe	��QMI8658ָ���Ĵ�����ַ����һ������
  * @param  	reg --> �Ĵ�����ַ
  * @return 	����������
  * @author 	liangziyue
  */
uint8_t QMI_ReadByte(uint8_t reg) {
	uint8_t temp = 0;
	I2C1_read(QMI8658_ADDR, reg, &temp, 1);
	I2C1_read_waiting();
	return temp;
}

/**
  * @name  		QMI_ReadNBytes	
  * @describe	��QMI8568��ȡN�ֽ�����
  * @param  	reg �Ĵ�����ַ  data ���ݴ洢buf  len���ݳ���
  * @return 	void
  * @author 	liangziyue
  */
void QMI_ReadNBytes(uint8_t reg, uint8_t* data, uint8_t len) {
	I2C1_read(QMI8658_ADDR, reg, data, len);
	I2C1_read_waiting();
}

void QMI_WriteNBytes(uint8_t reg, uint8_t* data, uint8_t len) {
	I2C1_write(QMI8658_ADDR, data, len);
	I2C1_write_waiting();
}

/**
  * @name  		QMI_CheckWhoAmI	
  * @describe	���QMI8658A��ID
  * @param  	void
  * @return 	true  |  false
  * @author 	liangziyue
  */
bool QMI_CheckWhoAmI(void) {
	uint8_t qmi8658_chip_id = 0;
    uint8_t qmi8658_revision_id = 0;
    uint8_t i = 0;
	
	while ((qmi8658_chip_id != 0x05) && (i < 5))
	{
		qmi8658_chip_id = QMI_ReadByte(Register_WhoAmI);
        if(qmi8658_chip_id == 0x05)
        {
            qmi8658_revision_id = QMI_ReadByte(Register_Revision); /* ��ȡ�豸ID */
            break;
        }
        i++;
	}
    if((qmi8658_chip_id == 0x05) && (qmi8658_revision_id == 0x7c))          /* ��ȡ��׼ȷ�ı�ʶ�����豸ID */
    {
        printf("qmi8658���豸��ʶ����%#x, �豸ID��%#x\r\n", qmi8658_chip_id, qmi8658_revision_id);
        return false;
    }
    else
    {
        return true;
    }
}

/**
  * @name  		QMI_Calibration	
  * @describe	�����Ǽ���
  * @param  	void
  * @return 	true У��ɹ� | false У��ʧ��
  * @author 	liangziyue
  */
bool QMI_Calibration(void) {
	uint8_t sta = 0;
	QMI_WriteByte(Register_Ctrl7, 0x00);	// �ر������ǣ����ٶȼ�
	QMI_WriteByte(Register_Ctrl9, 0xA2);	
	Delay_s(2);
	sta = QMI_ReadByte(Register_COD_Status);
	if (sta == 0x00)
	{
		return false;
	}
	else
	{
		return true;
	}
}

/**
  * @name  		QMI_Reset	
  * @describe	�����λ
  * @param  	void
  * @return 	void
  * @author 	liangziyue
  */
void QMI_Reset(void) {
	QMI_WriteByte(Register_Reset, 0xB0);
	Delay_ms(150);
}

void QMI_ReadStatus(void) {
	QMI_ReadByte(Register_StatusInt);
}

uint8_t QMI_ReadState0(void) {
	return QMI_ReadByte(Register_Status0);
}

uint8_t QMI_ReadState1(void) {
	return QMI_ReadByte(Register_Status1);
}

/**
  * @name  		QMI_ConfigACC	
  * @describe	���ü��ٶȼƲ���
  * @param  	range --> ����   
  * @param  	odr --> odr�������  
  * @param  	lpfEnable --> ��ͨ�˲��� ��Qmi8658Lpf_Enable �򿪣�Qmi8658Lpf_Disable �ر� 
  * @param  	stEnable  --> �������Լ� ��Qmi8658St_Enable �Լ죬Qmi8658St_Disable ���Լ�  
  * @return 	void
  * @author 	liangziyue
  */
void QMI_ConfigACC(enum qmi8658_accrange range, enum qmi8658_accodr odr, enum qmi8658_LpfConfig lpfEnable, enum qmi8658_StConfig stEnable) {
	unsigned char ctl_dada;
    switch (range)
    {
        case Qmi8658accrange_2g:
            g_imu.ssvt_a = (1 << 14);
            break;
        case Qmi8658accrange_4g:
            g_imu.ssvt_a = (1 << 13);
            break;
        case Qmi8658accrange_8g:
            g_imu.ssvt_a = (1 << 12);
            break;
        case Qmi8658accrange_16g:
            g_imu.ssvt_a = (1 << 11);
            break;
        default:
            range = Qmi8658accrange_8g;
            g_imu.ssvt_a = (1 << 12);
            break;
    }
    if (stEnable == Qmi8658St_Enable)
    {
        ctl_dada = (unsigned char)range | (unsigned char)odr | 0x80;
    } 
    else
    {
        ctl_dada = (unsigned char)range | (unsigned char)odr;
    }
    QMI_WriteByte(Register_Ctrl2, ctl_dada);
    /* set LPF & HPF */
    QMI_ReadNBytes(Register_Ctrl5, &ctl_dada, 1);
    ctl_dada &= 0xf0;
    if (lpfEnable == Qmi8658Lpf_Enable)
    {
        ctl_dada |= A_LSP_MODE_3;
        ctl_dada |= 0x01;
    }
    else
    {
        ctl_dada &= ~0x01;
    }
    QMI_WriteByte(Register_Ctrl5, ctl_dada);
}

/**
  * @name  		QMI_ConfigGYRO	
  * @describe	���������ǲ���
  * @param  	range 	--> ����
  * @param  	odr         ��odr�������
  * @param  	lpfEnable   ����ͨ�˲��� ��Qmi8658Lpf_Enable �򿪣�Qmi8658Lpf_Disable ��
  * @param  	stEnable    ���������Լ� ��Qmi8658St_Enable �Լ죬Qmi8658St_Disable ����
  * @return 	void
  * @author 	liangziyue
  */
void QMI_ConfigGYRO(enum qmi8658_gyrrange range, enum qmi8658_gyrodr odr, enum qmi8658_LpfConfig lpfEnable, enum qmi8658_StConfig stEnable) {
    /* Set the CTRL3 register to configure dynamic range and ODR */
    unsigned char ctl_dada;

    /* Store the scale factor for use when processing raw data */
    switch (range)
    {
        case Qmi8658gyrrange_16dps:
            g_imu.ssvt_g = 2048;
            break;
        case Qmi8658gyrrange_32dps:
            g_imu.ssvt_g = 1024;
            break;
        case Qmi8658gyrrange_64dps:
            g_imu.ssvt_g = 512;
            break;
        case Qmi8658gyrrange_128dps:
            g_imu.ssvt_g = 256;
            break;
        case Qmi8658gyrrange_256dps:
            g_imu.ssvt_g = 128;
            break;
        case Qmi8658gyrrange_512dps:
            g_imu.ssvt_g = 64;
            break;
        case Qmi8658gyrrange_1024dps:
            g_imu.ssvt_g = 32;
            break;
        case Qmi8658gyrrange_2048dps:
            g_imu.ssvt_g = 16;
            break;
        default:
            range = Qmi8658gyrrange_512dps;
            g_imu.ssvt_g = 64;
            break;
    }

    if (stEnable == Qmi8658St_Enable)
    {
        ctl_dada = (unsigned char)range | (unsigned char)odr | 0x80;
    }   
    else
    {
        ctl_dada = (unsigned char)range | (unsigned char)odr;
    }
    QMI_WriteByte(Register_Ctrl3, ctl_dada);

    /* Conversion from degrees/s to rad/s if necessary */
    /* set LPF & HPF */
    QMI_ReadNBytes(Register_Ctrl5, &ctl_dada, 1);
    ctl_dada &= 0x0f;
    if (lpfEnable == Qmi8658Lpf_Enable)
    {
        ctl_dada |= G_LSP_MODE_3;
        ctl_dada |= 0x10;
    }
    else
    {
        ctl_dada &= ~0x10;
    }
    QMI_WriteByte(Register_Ctrl5, ctl_dada);	
}


/**
 * @brief   ���������Register_Ctrl9�Ĵ���
 * @param   cmd  ���������Ctrl9Commandö��
 * @retval  ����ͽ��
 * @arg     0: �ɹ�
 * @arg     1: ʧ��
 */
int QMI_SendCt19Cmd(enum Ctrl9Command cmd) {
    unsigned char status1 = 0x00;
    unsigned short count = 0;
    unsigned char status_reg = Register_StatusInt;
    unsigned char cmd_done = 0x80;
    unsigned char retry = 0;
    int ret1 = 0;
    int ret2 = 0;

#if defined(QMI8658_SYNC_SAMPLE_MODE)   /* ͬ������ģʽ */
    if (g_imu.cfg.syncsample == 1)
    {
        status_reg = Register_Status1;
        cmd_done = 0x01;
    }
#endif
    while(retry++ < 3)
    {
        QMI_WriteByte(Register_Ctrl9, (unsigned char)cmd);     /* write commond to ctrl9*/

        QMI_ReadNBytes(status_reg, &status1, 1);
        while (((status1 & cmd_done) != cmd_done) && (count++ < 100))   /* read statusINT until bit7 is 1*/
        {
            Delay_ms(1);
            QMI_ReadNBytes(status_reg, &status1, 1);
        }
        if(count < 100)
        {
            ret1 = 1;
        }
        else
        {
            ret1 = 0;
        }

        QMI_WriteByte(Register_Ctrl9, Ctrl9_Cmd_Ack);          /* write commond  0x00 to ctrl */
        count = 0;
        QMI_ReadNBytes(status_reg, &status1, 1);
        while (((status1 & cmd_done) == cmd_done) && (count++ < 100))   /* read statusINT until bit7 is 0 */
        {
            Delay_ms(1);    
            QMI_ReadNBytes(status_reg, &status1, 1);
        }
        if(count < 100)
        {
            ret2 = 1;
        }
        else
        {
            ret2 = 0;
        }

        if((ret1==0) || (ret2==0))
        {
            continue;
        }
        else
        {
            break;
        }
    }

    if(ret1 && ret2)
    {
        return 0;
    }
    else
    {
        printf("atk_qmi8658_send_ctl9cmd fail cmd=%d\n", cmd);
        return 1;
    }
}

/**
 * @brief   ��ȡ�������¶�
 * @param   ��
 * @retval  �¶�ֵ,��λΪ��
 */
float QMI_GetTemperature(void) {
    short temp = 0;
    float temp_f = 0;
    uint8_t buf[2];

    QMI_ReadNBytes(Register_Tempearture_L, buf, 2);    /* ��ȡ�¶����� */    
    temp = ((short)buf[1] << 8) | buf[0];
    temp_f = (float)temp / 256.0f;
    return temp_f;
}

/**
 * @brief   ��ȡ������QMI8658�����Ǻͼ��ٶȵ����ݣ����жϣ� 
 * @param   acc  ���洢���ٶȼ�xyz������
 * @param   gyro ���洢������xyz������
 * @retval  ��
 */
void QMI_ReadSensorData(float *acc, float *gyro) {
    unsigned char buf_reg[12];
    short raw_acc_xyz[3];
    short raw_gyro_xyz[3];
    unsigned char axis = 0;
    static int cali_count = 0;
    static float offset_acc[3] = {0.0, 0.0, 0.0};
    static float offset_gyro[3] = {0.0, 0.0, 0.0};
    static float accel_calibration_sum[3] = {0.0f, 0.0f, 0.0f};
    static float gyro_calibration_sum[3] = {0.0f, 0.0f, 0.0f};

    float acc_raw[3];
    float gyro_raw[3];

    /* ��ȡ���ٶȼƺ����������� */
    QMI_ReadNBytes(Register_Ax_L, buf_reg, 12);
    
    raw_acc_xyz[0] = (short)((unsigned short)(buf_reg[1] << 8) | (buf_reg[0]));
    raw_acc_xyz[1] = (short)((unsigned short)(buf_reg[3] << 8) | (buf_reg[2]));
    raw_acc_xyz[2] = (short)((unsigned short)(buf_reg[5] << 8) | (buf_reg[4]));

    raw_gyro_xyz[0] = (short)((unsigned short)(buf_reg[7] << 8) | (buf_reg[6]));
    raw_gyro_xyz[1] = (short)((unsigned short)(buf_reg[9] << 8) | (buf_reg[8]));
    raw_gyro_xyz[2] = (short)((unsigned short)(buf_reg[11] << 8) | (buf_reg[10]));

    /* ���ٶȵ�λ��m/s2 */
    acc_raw[0] = (float)(raw_acc_xyz[0] * ONE_G) / g_imu.ssvt_a;
    acc_raw[1] = (float)(raw_acc_xyz[1] * ONE_G) / g_imu.ssvt_a;
    acc_raw[2] = (float)(raw_acc_xyz[2] * ONE_G) / g_imu.ssvt_a;

    /* �����ǵ�λ��rad/s */
    gyro_raw[0] = (float)(raw_gyro_xyz[0] * M_PI) / (g_imu.ssvt_g * 180);   /* *pi/180 */
    gyro_raw[1] = (float)(raw_gyro_xyz[1] * M_PI) / (g_imu.ssvt_g * 180);
    gyro_raw[2] = (float)(raw_gyro_xyz[2] * M_PI) / (g_imu.ssvt_g * 180);

    if (g_imu_init != 1)            /* flash���Ѿ��洢У׼��ǣ� */
    {
        if (cali_count == 0)        /* û�д洢�����¼��� */
        {
            memset((void *)accel_calibration_sum, 0, sizeof(accel_calibration_sum));
            memset((void *)gyro_calibration_sum, 0, sizeof(gyro_calibration_sum));
            cali_count++;
        }
        else if (cali_count < MAX_CALI_COUNT)
        {
            for (axis = 0; axis < 3; axis++)
            {
                if (axis == 2)
                {
                    accel_calibration_sum[axis] += (acc_raw[axis] - ONE_G);
                }
                else
                {
                    accel_calibration_sum[axis] += acc_raw[axis];
                }
                gyro_calibration_sum[axis] += gyro_raw[axis];
            }
            cali_count++;
        }
        else if (cali_count == MAX_CALI_COUNT)
        {
            for (axis = 0; axis < 3; axis++)
            {
                offset_gyro[axis] = (0.0f - (gyro_calibration_sum[axis] / (MAX_CALI_COUNT - 1)));
                offset_acc[axis] = (0.0f - (accel_calibration_sum[axis] / (MAX_CALI_COUNT - 1)));
            }
            g_imu_init = 1;

            cali_count++;
        }
    }
    else    /* �д洢��ֱ�Ӽ��� */
    {
        for (axis = 0; axis < 3; axis++)
        {
            acc[axis] = acc_raw[axis] + offset_acc[axis];
            gyro[axis] = gyro_raw[axis] + offset_gyro[axis];
        }
    }
}

/**
 * @brief   �ж����ݸ��º��ڶ�ȡ������QMI8658�����Ǻͼ��ٶȵ�����(�Ƽ�ʹ��)
 * @param   acc  : ���ٶȼ� X,Y,Z������;
 * @param   gyro : ������ X,Y,Z������;
 * @retval  ��
 */
void QMI_ReadXYZ(float *acc, float *gyro) {
    unsigned char status = 0;
    unsigned char data_ready = 0;
    int retry = 0;

    while (retry++ < 3)
    {
#if defined(QMI8658_SYNC_SAMPLE_MODE)
        atk_qmi8568_read_nbytes(Register_StatusInt, &status, 1);

        if (status & 0x01)
        {
            delay_us(12);       /* delay 12us <=500Hz�� 12us 1000Hz, 4us 2000Hz 2us > 2000Hz */
        }
        if ((status & 0x01) || (status & 0x03))
        {
            data_ready = 1;
            break;
        }
#else
        /* �����ٶȼƺ������������Ƿ���� */
        QMI_ReadNBytes(Register_Status0, &status, 1);
        if (status & 0x03)
        {
            data_ready = 1;
            break;
        }
#endif
    }
    if (data_ready)
    {
        QMI_ReadSensorData(acc, gyro);

        g_imu.imu[0] = acc[0];
        g_imu.imu[1] = acc[1];
        g_imu.imu[2] = acc[2];
        g_imu.imu[3] = gyro[0];
        g_imu.imu[4] = gyro[1];
        g_imu.imu[5] = gyro[2];
    }
    else
    {
        acc[0] = g_imu.imu[0];
        acc[1] = g_imu.imu[1];
        acc[2] = g_imu.imu[2];
        gyro[0] = g_imu.imu[3];
        gyro[1] = g_imu.imu[4];
        gyro[2] = g_imu.imu[5];
//      printf("���ݻ�δ׼����\r\n");    /* ����ʹ�� */
    }
}

/**
 * @brief   �����������ڲ�����ʱ�� 
 * @param   enable ��
 *          1 �� ���������������ڲ�ʱ��
 *          0 �� �رմ����������ڲ�ʱ��
 * @retval  ��
 */
void QMI_EableAhbClock(bool enable) {
    if (enable)
    {
        QMI_WriteByte(Register_Ctrl7, 0x00);
        QMI_WriteByte(Register_Cal1_L, 0x00);
        QMI_SendCt19Cmd(Ctrl9_Cmd_AHBClockGating);
    }
    else
    {
        QMI_WriteByte(Register_Ctrl7, 0x00);
        QMI_WriteByte(Register_Cal1_L, 0x01);
        QMI_SendCt19Cmd(Ctrl9_Cmd_AHBClockGating);
    }
}

/**
 * @brief   ʹ�������ǡ����ٶȼ� 
 * @param   enableFlags ��
 *          QMI8658_DISABLE_ALL  : ����ʹ��
 *          QMI8658_ACC_ENABLE   : ʹ�ܼ��ٶȼ�
 *          QMI8658_GYR_ENABLE   : ʹ��������
 *          QMI8658_ACCGYR_ENABLE: ʹ�������ǡ����ٶȼ�
 * @retval  ��
 */
void QMI_EnableSensors(unsigned char enableFlags) {
	
#if defined(QMI8658_SYNC_SAMPLE_MODE)
    g_imu.cfg.syncsample = 1;
    atk_qmi8658_enable_ahb_clock(0);
    atk_qmi8658_write_byte(Register_Ctrl7, enableFlags | 0x80);
#else
    QMI_WriteByte(Register_Ctrl7, enableFlags);    
#endif
    g_imu.cfg.ensensors = enableFlags & 0x03;

    Delay_ms(2);
}

/**
 * @brief   ����QMI8658�����Ǻͼ��ٶȼƵ����̡����Ƶ�ʲ�����
 * @param   low_power �� 0�� ����ģʽ 1���͹���ģʽ
 * @retval  ��
 */
void QMI_ConfigReg(unsigned char low_power) {
	
    QMI_EnableSensors(QMI8658_DISABLE_ALL);
    if (low_power)
    {
        g_imu.cfg.ensensors = QMI8658_ACC_ENABLE;           
        g_imu.cfg.accrange = Qmi8658accrange_8g;
        g_imu.cfg.accodr = Qmi8658accodr_LowPower_21Hz;
        g_imu.cfg.gyrrange = Qmi8658gyrrange_1024dps;
        g_imu.cfg.gyrodr = Qmi8658gyrodr_250Hz;
    }
    else
    {
        g_imu.cfg.ensensors = QMI8658_ACCGYR_ENABLE;    /* ʹ�������ǡ����ٶȼ� */
        g_imu.cfg.accrange = Qmi8658accrange_16g;       /* ��16g */
        g_imu.cfg.accodr = Qmi8658accodr_500Hz;         /* 500Hz���� */
        g_imu.cfg.gyrrange = Qmi8658gyrrange_128dps;    /* ��128dps */
        g_imu.cfg.gyrodr = Qmi8658gyrodr_500Hz;         /* 500Hz���� */
    }

    if (g_imu.cfg.ensensors & QMI8658_ACC_ENABLE)
    {
        QMI_ConfigACC(g_imu.cfg.accrange, g_imu.cfg.accodr, Qmi8658Lpf_Enable, Qmi8658St_Enable); /* ���ò������������ٶȼ��Լ�͵�ͨ�˲��� */
    }
    if (g_imu.cfg.ensensors & QMI8658_GYR_ENABLE)
    {
        QMI_ConfigGYRO(g_imu.cfg.gyrrange, g_imu.cfg.gyrodr, Qmi8658Lpf_Enable, Qmi8658St_Enable);/* ���ò����������������Լ�͵�ͨ�˲��� */
    }
}

/**
 * @brief   ��ʼ��QMI8658
 * @param   ��
 * @retval  ��ʼ�����
 * @arg     0: �ɹ�
 * @arg     1: ʧ��
 */
bool QMI_Init(void) {
    I2C1_Configuration();

	QMI_Reset();
    
    if(QMI_CheckWhoAmI())                      /* ����豸ID�Ƿ���ȷ */
    {
        return true;
    }
    if(QMI_Calibration())
    {
        return true;
    }
	
	
    printf("У׼�ɹ�����\r\n");

    QMI_WriteByte(Register_Ctrl1, 0x60);   /* I2C���� */
    QMI_WriteByte(Register_Ctrl7, 0x00);   /* �ر������ǡ����ٶȼ� */
    
    QMI_ConfigReg(0);                          /* ���������Ǻͼ��ٶȼƵ����̺�����������ʵȲ��� */
    QMI_EnableSensors(g_imu.cfg.ensensors);     /* ʹ�������ǡ����ٶȼ� */

    return false;
}

#include "MAX30102.h"

u8 write_buffer[2] = {0};

uint8_t tmp; 
uint8_t fifo_data[8] = {0};

//MAX30102参数
uint32_t aun_ir_buffer[500];     //此数组用于存放不断读取到的红外数据
int32_t n_ir_buffer_length=500;  //红外数据的个数
uint32_t aun_red_buffer[500];    //此数组用于存放不断读取到的红光数据
int32_t n_sp02;                 //这个变量一会用来存放获得的血氧值
int8_t ch_spo2_valid;           //这个变量用来指明获得的血氧值是否有效
int32_t n_heart_rate;           //这个变量一会用来存放获得的心率值
int8_t  ch_hr_valid;            //这个变量用来指明获得的心率值是否有效
u32 un_min, un_max, un_prev_data;



void MAX30102_Reset(void) {
	write_buffer[0] = REG_MODE_CONFIG;
	write_buffer[1] = 0x40;
	I2C0_write(I2C_WRITE_ADDR, write_buffer, 2);
	I2C0_write_waiting();
	I2C0_write(I2C_WRITE_ADDR, write_buffer, 2);
	I2C0_write_waiting();
}

void MAX30102_WriteByte(uint8_t reg_addr, uint8_t byte) {
	write_buffer[0] = reg_addr;
	write_buffer[1] = byte;
	I2C0_write(I2C_WRITE_ADDR, write_buffer, 2);
	I2C0_write_waiting();
}

uint8_t MAX30102_ReadByte(uint8_t reg_addr, uint8_t* byte) {
	I2C0_read(I2C_READ_ADDR, reg_addr, byte, 1);
	I2C0_read_waiting();
	return 0;
}

uint8_t MAX30102_ReadLen(uint8_t reg_addr, uint8_t* data, uint8_t size) {
	I2C0_read(I2C_READ_ADDR, reg_addr, data, size);
	I2C0_read_waiting();
	return 0;
}

void MAX30102_Init(void) {
	
	I2C0_Configuration();
	
	MAX30102_Reset();
    MAX30102_WriteByte(REG_INTR_ENABLE_1,0xC0);
    MAX30102_WriteByte(REG_INTR_ENABLE_2,0x00);
    MAX30102_WriteByte(REG_FIFO_WR_PTR,0x00);
    MAX30102_WriteByte(REG_OVF_COUNTER,0x00);
    MAX30102_WriteByte(REG_FIFO_RD_PTR,0x00);
    MAX30102_WriteByte(REG_FIFO_CONFIG,0x0F);
    MAX30102_WriteByte(REG_MODE_CONFIG,0x03);
    MAX30102_WriteByte(REG_SPO2_CONFIG,0x27);
    MAX30102_WriteByte(REG_LED1_PA,0x24);
    MAX30102_WriteByte(REG_LED2_PA,0x24);
	MAX30102_WriteByte(REG_PILOT_PA,0x7F);
}

void MAX30102_ReadFIFOBytes(void) {
	MAX30102_ReadByte(REG_INTR_STATUS_1, &tmp);
	MAX30102_ReadByte(REG_INTR_STATUS_2, &tmp);
    /* 这里读取了FIFO寄存器的6个字节的数据，因为FIFO寄存器6个字节的数据为一组
        前三字节是红光数据，用于计算心率，后三字节是红外数据，用于计算血氧 */
	MAX30102_ReadLen(REG_FIFO_DATA,fifo_data,6);
}

void MAX30102_GetSpo2AndHeartRate(void) {
	int i = 0;

    /* 第一个for循环，将数组aun_red_buffer和aun_ir_buffer中后400
       个数据移到数组的前400的位置 */
	for (i = 100; i < 500; i++)
	{
        aun_red_buffer[i-100] = aun_red_buffer[i];
        aun_ir_buffer[i-100] = aun_ir_buffer[i];		
	}
	un_prev_data = aun_red_buffer[i];  // 记录最后一个样本值
    /* 第二个for循环，将接收到的新数据从数组第400的位置开始存存100个 */
	for (i = 400; i < 500; i++)
	{
		while (GPIO_ReadInBit(HT_GPIOD, GPIO_PIN_1));			// 等待中断
		MAX30102_ReadFIFOBytes();		//开始读取FIFO寄存器的值，读取1组数据6个字节
		
		/* 下面两行将读取到的6字节数据，前三字节拼接起来存入aun_red_buffer数组中，
         后三字节拼接起来存入aun_ir_buffer数组中 */	
		aun_red_buffer[i] = (long)((long)((long)fifo_data[0]&0x03)<<16)|
                                         (long)fifo_data[1]<<8|(long)fifo_data[2];
        aun_ir_buffer[i] = (long)((long)((long)fifo_data[3]&0x03)<<16)|
                                         (long)fifo_data[4]<<8|(long)fifo_data[5];
		
		maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer,n_ir_buffer_length,
                                           aun_red_buffer,&n_sp02, &ch_spo2_valid,
                                           &n_heart_rate, &ch_hr_valid);
	}
} 


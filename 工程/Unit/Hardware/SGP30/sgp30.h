#ifndef __HX711_H
#define __HX711_H
#include "sys.h"

#define  SGP30_SCL   PAout(12)
#define  SGP30_SDA   PAout(15)


// USART GPIO ���ź궨��
#define  SGP30_SCL_GPIO_CLK        RCC_APB2Periph_GPIOA
#define  SGP30_SCL_GPIO_PORT       GPIOA
#define  SGP30_SCL_GPIO_PIN        GPIO_Pin_12

#define  SGP30_SDA_GPIO_SDA        RCC_APB2Periph_GPIOA
#define  SGP30_SDA_GPIO_PORT       GPIOA
#define  SGP30_SDA_GPIO_PIN        GPIO_Pin_15

#define  SGP30_SDA_READ()           GPIO_ReadInputDataBit(SGP30_SDA_GPIO_PORT, SGP30_SDA_GPIO_PIN)

#define SGP30_read  0xb1  //SGP30�Ķ���ַ
#define SGP30_write 0xb0  //SGP30��д��ַ


void SGP30_IIC_Start(void);				//����IIC��ʼ�ź�
void SGP30_IIC_Stop(void);	  			//����IICֹͣ�ź�
void SGP30_IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u16 SGP30_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 SGP30_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void SGP30_IIC_Ack(void);					//IIC����ACK�ź�
void SGP30_IIC_NAck(void);				//IIC������ACK�ź�
void SGP30_IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 SGP30_IIC_Read_One_Byte(u8 daddr,u8 addr);	
void SGP30_Init(void);				  
void SGP30_Write(u8 a, u8 b);
u32 SGP30_Read(void);



#endif


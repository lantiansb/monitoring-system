#include "stm32f10x.h"                  // Device header
#include  "dht11.h"
#include  "delay.h"
//����
unsigned int rec_data[4];


//����stm32��˵�������
void DH11_GPIO_Init_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; //�������
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}

//����stm32��˵��������
void DH11_GPIO_Init_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING; //��������
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}



//�������Ϳ�ʼ�ź�
void DHT11_Start(void)
{
	DH11_GPIO_Init_OUT(); //���ģʽ
	
	dht11_high; //������
	delay_us(30);
	
	dht11_low; //���͵�ƽ����18us
	delay_ms(20);
	
	dht11_high; //���ߵ�ƽ20~40us
	delay_us(30);
	
	DH11_GPIO_Init_IN(); //����ģʽ
}


//��ȡһ���ֽ�
char DHT11_Rec_Byte(void)
{
	unsigned char i = 0;
	unsigned char data;
	
	for(i=0;i<8;i++) //1�����ݾ���1���ֽ�byte��1���ֽ�byte��8λbit
	{
		while( Read_Data == 0); //��1bit��ʼ���͵�ƽ��ߵ�ƽ���ȴ��͵�ƽ����
		delay_us(30); //�ӳ�30us��Ϊ����������0������1��0ֻ��26~28us
		
		data <<= 1; //����
		
		if( Read_Data == 1 ) //�������30us���Ǹߵ�ƽ�Ļ���������1
		{
			data |= 1; //����+1
		}
		
		while( Read_Data == 1 ); //�ߵ�ƽ��͵�ƽ���ȴ��ߵ�ƽ����
	}
	
	return data;
}

//��ȡ����

void DHT11_REC_Data(void)
{
	unsigned int R_H,R_L,T_H,T_L;
	unsigned char RH,RL,TH,TL,CHECK;
	
	DHT11_Start(); //���������ź�
	dht11_high; //���ߵ�ƽ
	
	if( Read_Data == 0 ) //�ж�DHT11�Ƿ���Ӧ
	{
		while( Read_Data == 0); //�͵�ƽ��ߵ�ƽ���ȴ��͵�ƽ����
		while( Read_Data == 1); //�ߵ�ƽ��͵�ƽ���ȴ��ߵ�ƽ����
		
		R_H = DHT11_Rec_Byte();
		R_L = DHT11_Rec_Byte();
		T_H = DHT11_Rec_Byte();
		T_L = DHT11_Rec_Byte();
		CHECK = DHT11_Rec_Byte(); //����5������
		
		dht11_low; //�����һbit���ݴ�����Ϻ�DHT11�������� 50us
		delay_us(55); //������ʱ55us
		dht11_high; //��������������������߽������״̬��
		
		if(R_H + R_L + T_H + T_L == CHECK) //�ͼ���λ�Աȣ��ж�У����յ��������Ƿ���ȷ
		{
			RH = R_H;
			RL = R_L;
			TH = T_H;
			TL = T_L;
		}
	}
	rec_data[0] = RH;
	rec_data[1] = RL;
	rec_data[2] = TH;
	rec_data[3] = TL;
}

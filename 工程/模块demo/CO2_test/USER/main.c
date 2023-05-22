/*
1.��Ŀ���ƣ������콢��SGP30ģ��STM32F103C8T6���Գ���
2.��ʾģ�飺���ڷ�������,������9600
3.ʹ�������keil5 for ARM
4.������λ������
5.��Ŀ��ɣ�SGP30ģ��
6.��Ŀ���ܣ����ڷ��ز�����CO2,TVOC����
7.��Ҫԭ������ο�SGP30�����ֲ�
8.�����ַ��https://lssz.tmall.com ���Ա��������������콢�ꡱ
10.��Ȩ�����������콢�����г��������������Ȩ�����뱾���Ʒ���׳��ۣ��벻Ҫ����������׷���䷨�����Σ�
���߶���
	VCC--5V
	GND--GND
	SCL--PB0
	SDA--PB1
*/

#include "sys.h"
#include "delay.h"
#include "SGP30.h"
#include "usart.h"

u32 CO2Data,TVOCData;//����CO2Ũ�ȱ�����TVOCŨ�ȱ���

int main(void)
{
	u32 sgp30_dat;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();
	uart_init(9600);
	SGP30_Init();   //��ʼ��SGP30
	delay_ms(100);
	SGP30_Write(0x20,0x08);
	sgp30_dat = SGP30_Read();//��ȡSGP30��ֵ
	CO2Data = (sgp30_dat & 0xffff0000) >> 16;
	TVOCData = sgp30_dat & 0x0000ffff;	
	//SGP30ģ�鿪����Ҫһ��ʱ���ʼ�����ڳ�ʼ���׶ζ�ȡ��CO2Ũ��Ϊ400ppm��TVOCΪ0ppd�Һ㶨���䣬����ϵ��ÿ��һ��ʱ���ȡһ��
	//SGP30ģ���ֵ�����CO2Ũ��Ϊ400ppm��TVOCΪ0ppd�����͡����ڼ����...����ֱ��SGP30ģ���ʼ����ɡ�
	//��ʼ����ɺ�տ�ʼ�������ݻᲨ���Ƚϴ�������������һ��ʱ�����������ȶ���
	//�����ഫ�����Ƚ������ܻ���Ӱ�죬�������ݳ��ֲ����������ģ�����������˲�������
	while(CO2Data == 400 && TVOCData == 0)
	{
		SGP30_Write(0x20,0x08);
		sgp30_dat = SGP30_Read();//��ȡSGP30��ֵ
		CO2Data = (sgp30_dat & 0xffff0000) >> 16;//ȡ��CO2Ũ��ֵ
		TVOCData = sgp30_dat & 0x0000ffff;			 //ȡ��TVOCֵ
		printf("���ڼ����...\r\n");
		delay_ms(500);
	}
	while(1)
	{
		SGP30_Write(0x20,0x08);
		sgp30_dat = SGP30_Read();//��ȡSGP30��ֵ
		CO2Data = (sgp30_dat & 0xffff0000) >> 16;//ȡ��CO2Ũ��ֵ
		TVOCData = sgp30_dat & 0x0000ffff;       //ȡ��TVOCֵ
		printf("CO2:%dppm\r\nTVOC:%dppd\r\n",CO2Data,TVOCData);
		delay_ms(500);
	}
}



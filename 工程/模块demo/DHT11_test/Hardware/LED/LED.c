#include "stm32f10x.h"                  // Device header

void LED_Init(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//�������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//����ע�⣬GPIO���ú���֮��Ĭ���ǵ͵�ƽ
	GPIO_SetBits(GPIOA,GPIO_Pin_1 | GPIO_Pin_2);//���øߵ�ƽ,
}
void LED1_ON(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);//���õ͵�ƽ
}
void LED1_OFF(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_1);//���øߵ�ƽ
}
void LED2_ON(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);//���õ͵�ƽ
}
void LED2_OFF(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_1);//���øߵ�ƽ
}
//PA1�˿ڵĵ�ƽ��ת
void LED1_Turn(void)
{
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0)
	{
	   GPIO_SetBits(GPIOA,GPIO_Pin_1);//���øߵ�ƽ
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_1);//���õ͵�ƽ
	}
}
//PA2�˿ڵĵ�ƽ��ת
void LED2_Turn(void)
{
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)==0)
	{
	   GPIO_SetBits(GPIOA,GPIO_Pin_2);//���øߵ�ƽ
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_2);//���õ͵�ƽ
	}
}


#include "stm32f10x.h"                  // Device header

void LED_Init(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//这里注意，GPIO配置好了之后，默认是低电平
	GPIO_SetBits(GPIOA,GPIO_Pin_1 | GPIO_Pin_2);//设置高电平,
}
void LED1_ON(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);//设置低电平
}
void LED1_OFF(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_1);//设置高电平
}
void LED2_ON(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);//设置低电平
}
void LED2_OFF(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_1);//设置高电平
}
//PA1端口的电平翻转
void LED1_Turn(void)
{
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0)
	{
	   GPIO_SetBits(GPIOA,GPIO_Pin_1);//设置高电平
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_1);//设置低电平
	}
}
//PA2端口的电平翻转
void LED2_Turn(void)
{
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)==0)
	{
	   GPIO_SetBits(GPIOA,GPIO_Pin_2);//设置高电平
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_2);//设置低电平
	}
}


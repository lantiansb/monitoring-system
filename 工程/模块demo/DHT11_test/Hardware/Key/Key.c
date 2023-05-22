#include "stm32f10x.h" 
#include "delay.h"

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}

uint8_t Key_GetNum(void)
{
	uint8_t KeyNum=0;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0)
	{
		delay_ms(20);//消抖
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0);//检测是否松手
		delay_ms(20);//消抖
		KeyNum=1;
	}
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0)
	{
		delay_ms(20);//消抖
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0);//检测是否松手
		delay_ms(20);//消抖
		KeyNum=2;
	}
	
	return KeyNum;

}

#include "stm32f10x.h" 
#include "delay.h"

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

uint8_t Key_GetNum(void)
{
	uint8_t KeyNum=0;
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)==0)
	{
		delay_ms(20);//消抖
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)==0);//检测是否松手
		delay_ms(20);//消抖
		KeyNum=1;
	}
	
	return KeyNum;

}

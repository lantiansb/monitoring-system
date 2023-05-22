#include "stm32f10x.h"                  // Device header
#include "LED.h"

unsigned int timeCount = 0;	//发送间隔变量
unsigned short push_flag = 0;	//推送标志位

void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//首先开启总线时钟
	
	TIM_InternalClockConfig(TIM2);//选择开启TIM2的时钟，STM32中TIM的时钟均为72MHz
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//定义定时器初始化结构体
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择预分频系数
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//计数器计数模式：上升计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;//设置自动重装值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;//设置预分频值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//高级计数器才有的重复计数器，这里没有，写0即可
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);//回传初始化结构体
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);//
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//
	
	NVIC_InitTypeDef NVIC_InitStructure;//NVIC中断管理系统初始化结构体定义
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;//选择TIM2的中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//通道开启
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//设置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//设置响应优先级
	NVIC_Init(&NVIC_InitStructure);//回传NVIC初始化结构体
	
	TIM_Cmd(TIM2, ENABLE);//开启TIM2计时器
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		if(++timeCount >= 1000)
        {
            LED_Turn();
        }
		if(++timeCount >= 5000)
        {
            timeCount = 0;
            push_flag = 1;
        }
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

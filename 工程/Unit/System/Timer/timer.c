#include "stm32f10x.h"                  // Device header
#include "LED.h"

unsigned int timeCount = 0;	//���ͼ������
unsigned short push_flag = 0;	//���ͱ�־λ

void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//���ȿ�������ʱ��
	
	TIM_InternalClockConfig(TIM2);//ѡ����TIM2��ʱ�ӣ�STM32��TIM��ʱ�Ӿ�Ϊ72MHz
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//���嶨ʱ����ʼ���ṹ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//ѡ��Ԥ��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//����������ģʽ����������ģʽ
	TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;//�����Զ���װֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;//����Ԥ��Ƶֵ
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//�߼����������е��ظ�������������û�У�д0����
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);//�ش���ʼ���ṹ��
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);//
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//
	
	NVIC_InitTypeDef NVIC_InitStructure;//NVIC�жϹ���ϵͳ��ʼ���ṹ�嶨��
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;//ѡ��TIM2���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ͨ������
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//������Ӧ���ȼ�
	NVIC_Init(&NVIC_InitStructure);//�ش�NVIC��ʼ���ṹ��
	
	TIM_Cmd(TIM2, ENABLE);//����TIM2��ʱ��
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

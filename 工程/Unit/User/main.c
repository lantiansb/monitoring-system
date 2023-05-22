//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//Sys
#include "delay.h"
#include "usart.h"
#include "timer.h"

//����Э���
#include "onenet.h"

//�豸
#include "esp8266.h"
#include "OLED.h"

//Ӳ������
#include "SGP30.h"
#include "BH1750.h"
#include "DHT11.h"
#include "Key.h"
#include "Beep.h"
#include "LED.h"

//C��
#include <stdio.h>
#include <string.h>

extern char rec_data[4];//dht11�ı�������
extern unsigned int push_flag;	//���ͱ�־λ
    
const char *topics[] = {"/deviceA/commands", "test"};//mqtt������������

unsigned char onenet_buf[128];//onenet��������

u8 hum;//����ʪ��

float light;//�������ǿ��

u32 CO2_dat;//������̼Ũ��

u8 warring_flag, warring_flag_last;//������־��1-�¶ȣ�2-ʪ�ȣ�3-CO2��4-����ǿ��,warring_flag_last���ڸ��������������ʾҲ������

/*
************************************************************
*	�������ƣ�	Hardware_Init
*
*	�������ܣ�	Ӳ����ʼ��
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		��ʼ����Ƭ�������Լ�����豸
************************************************************
*/
void Hardware_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�жϿ�������������

	delay_init();									//systick��ʼ��
	
	Usart1_Init(115200);							//����1����ӡ��Ϣ��
	
	Usart2_Init(115200);							//����2������ESP8266��
    LED_Init();
    Beep_Init();
    //CO2�������õ���PA15��ʼĬ��ΪJTAG���Խţ���ر�JTAG����
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    SGP30_Init();                                   //CO2���ģ���ʼ��
    BH1750_Init();                                  //���ռ��ģ���ʼ��
	Timer_Init();                                   //��ʼ����ʱ��2
    OLED_Init();                                    //��Ļ��ʼ��
    OLED_Clear();
	UsartPrintf(USART_DEBUG, "Hardware init OK\r\n");
    OLED_ShowString(1, 1, "Init OK");
    Beep_ON();
    delay_ms(2000);
    OLED_Clear();
    Beep_OFF();
}

/*
************************************************************
*	�������ƣ�	main
*
*	�������ܣ�	������
*
*	��ڲ�����	��
*
*	���ز�����	0
*
*	˵����		
************************************************************
*/
int main(void)
{
/*************************** ��ʼ������ *************************/
//	unsigned char *dataPtr = NULL;

	Hardware_Init();				//��ʼ����ΧӲ��
	
	ESP8266_Init();					//��ʼ��ESP8266
	
	while(OneNet_DevLink())			//����OneNET
		delay_ms(500);
		
	OneNet_Subscribe(topics, 2);    //����
    
    OneNet_Publish(topics[1], "test!"); //test onenet
    
    LED_ON();//��ʾ��ʼ���ɹ�
    
    u32 sgp30_dat = 0;//�ݴ�SGP30���ص�ֵ
	
	while(1)
	{
        
		if(push_flag)				//���ͼ��5s
		{
/*************************** ��ȡ���������� *************************/
            DHT11_REC_Data();//��ȡ��ʪ�ȣ��ú�������ȫ���������洢
            light = LIght_Intensity();//��ȡ����ǿ��
            SGP30_Write(0x20,0x08);
            sgp30_dat = SGP30_Read();//��ȡSGP30��ֵ
            CO2_dat = (sgp30_dat & 0xffff0000) >> 16;//ȡ��CO2Ũ��ֵ
            SGP30_Read();
            
/*************************** ������Ϣ��� *************************/
			UsartPrintf(USART_DEBUG, "OneNet_Publish\r\n");
			sprintf((char *)onenet_buf, "temp:%d;hum:%d;CO2:%d;light:%.2f", rec_data[2], rec_data[0], CO2_dat, light);
                
			UsartPrintf(USART_DEBUG, (char *)onenet_buf);
			OneNet_Publish(topics[0], (const char *)onenet_buf);
            UsartPrintf(USART_DEBUG,"\r\n");	
            //��������
        if(rec_data[2]>30)
            warring_flag = 1;
        else if(rec_data[0] > 100)
            warring_flag = 2;
        else if(CO2_dat > 1000)
            warring_flag = 3;
        else if(light > 1000)
            warring_flag = 4;
        else
            warring_flag = 0;
/*************************** ������ʾ���� *************************/
        switch(warring_flag)
        {
            case 0:
                Beep_OFF();
                if(warring_flag_last != warring_flag)
                    OLED_Clear();
                OLED_ShowString(1, 1, "temp:");
                OLED_ShowNum(1, 6, rec_data[2], 2);
                OLED_ShowString(2, 1, "hum:");
                OLED_ShowNum(2, 6, rec_data[0], 2);
                OLED_ShowString(3, 1, "CO2:");
                OLED_ShowNum(3, 6, CO2_dat, 5);
                OLED_ShowString(4, 1, "lig:");
                OLED_ShowNum(4, 6, light, 5);   break;
            case 1:
//                Beep_ON();
//                OLED_Clear();
//                OLED_ShowString(1, 1, "temp:");
//                OLED_ShowNum(1, 6, rec_data[2], 3);
//                OLED_ShowString(2, 1, "temp warring!"); 
            break;
            case 2:
                Beep_ON();
                OLED_Clear();
                OLED_ShowString(1, 1, "hum:");
                OLED_ShowNum(1, 6, hum, 3);
                OLED_ShowString(2, 1, "hum warring!"); 
            break;
            case 3:
                Beep_ON();
                OLED_Clear();
                OLED_ShowString(1, 1, "CO2:");
                OLED_ShowNum(1, 6, CO2_dat, 5);
                OLED_ShowString(2, 1, "CO2 warring!"); break;
            case 4:
                Beep_ON();
                OLED_Clear();
                OLED_ShowString(1, 1, "lig:");
                OLED_ShowNum(1, 6, light, 2);
                OLED_ShowString(2, 1, "light warring!"); break;
        }
        warring_flag_last = warring_flag;
			push_flag = 0x00;
			ESP8266_Clear();
		}
        
/*************************** ���������Ϣ���� *************************/
//		dataPtr = ESP8266_GetIPD(0);
//		if(dataPtr != NULL)
//			OneNet_RevPro(dataPtr);
//		
		delay_ms(10);
	
	}

}

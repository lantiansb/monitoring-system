//单片机头文件
#include "stm32f10x.h"

//Sys
#include "delay.h"
#include "usart.h"
#include "timer.h"

//网络协议层
#include "onenet.h"

//设备
#include "esp8266.h"
#include "OLED.h"

//硬件驱动
#include "SGP30.h"
#include "BH1750.h"
#include "DHT11.h"
#include "Key.h"
#include "Beep.h"
#include "LED.h"

//C库
#include <stdio.h>
#include <string.h>

extern char rec_data[4];//dht11的保存数组
extern unsigned int push_flag;	//推送标志位
    
const char *topics[] = {"/deviceA/commands", "test"};//mqtt订阅主题数组

unsigned char onenet_buf[128];//onenet发布缓冲

u8 hum;//定义湿度

float light;//定义光照强度

u32 CO2_dat;//二氧化碳浓度

u8 warring_flag, warring_flag_last;//报警标志，1-温度，2-湿度，3-CO2，4-光照强度,warring_flag_last用于辅助，免得正常显示也卡清屏

/*
************************************************************
*	函数名称：	Hardware_Init
*
*	函数功能：	硬件初始化
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		初始化单片机功能以及外接设备
************************************************************
*/
void Hardware_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断控制器分组设置

	delay_init();									//systick初始化
	
	Usart1_Init(115200);							//串口1，打印信息用
	
	Usart2_Init(115200);							//串口2，驱动ESP8266用
    LED_Init();
    Beep_Init();
    //CO2传感器用到的PA15初始默认为JTAG调试脚，须关闭JTAG功能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    SGP30_Init();                                   //CO2检测模块初始化
    BH1750_Init();                                  //光照检测模块初始化
	Timer_Init();                                   //初始化定时器2
    OLED_Init();                                    //屏幕初始化
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
*	函数名称：	main
*
*	函数功能：	主函数
*
*	入口参数：	无
*
*	返回参数：	0
*
*	说明：		
************************************************************
*/
int main(void)
{
/*************************** 初始化所有 *************************/
//	unsigned char *dataPtr = NULL;

	Hardware_Init();				//初始化外围硬件
	
	ESP8266_Init();					//初始化ESP8266
	
	while(OneNet_DevLink())			//接入OneNET
		delay_ms(500);
		
	OneNet_Subscribe(topics, 2);    //订阅
    
    OneNet_Publish(topics[1], "test!"); //test onenet
    
    LED_ON();//表示初始化成功
    
    u32 sgp30_dat = 0;//暂存SGP30返回的值
	
	while(1)
	{
        
		if(push_flag)				//发送间隔5s
		{
/*************************** 获取传感器数据 *************************/
            DHT11_REC_Data();//获取温湿度，该函数调用全局数据来存储
            light = LIght_Intensity();//获取光照强度
            SGP30_Write(0x20,0x08);
            sgp30_dat = SGP30_Read();//读取SGP30的值
            CO2_dat = (sgp30_dat & 0xffff0000) >> 16;//取出CO2浓度值
            SGP30_Read();
            
/*************************** 发送信息组包 *************************/
			UsartPrintf(USART_DEBUG, "OneNet_Publish\r\n");
			sprintf((char *)onenet_buf, "temp:%d;hum:%d;CO2:%d;light:%.2f", rec_data[2], rec_data[0], CO2_dat, light);
                
			UsartPrintf(USART_DEBUG, (char *)onenet_buf);
			OneNet_Publish(topics[0], (const char *)onenet_buf);
            UsartPrintf(USART_DEBUG,"\r\n");	
            //报警部分
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
/*************************** 本地显示部分 *************************/
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
        
/*************************** 检验接收信息部分 *************************/
//		dataPtr = ESP8266_GetIPD(0);
//		if(dataPtr != NULL)
//			OneNet_RevPro(dataPtr);
//		
		delay_ms(10);
	
	}

}

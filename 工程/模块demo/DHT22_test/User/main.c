#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "dht22.h"
#include "OLED.h"
 
 int main(void)
 {	
	float temperature = 0;  	    
	float humidity = 0;  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);        //串口1，波特率9600
	delay_ms(1000);
	delay_ms(1000);
    OLED_Init();
     OLED_ShowString(1, 1, "test!");
	while(DHT22_Init())    //DHT22初始化，模块I/O检测
	{
		printf("DHT22_Check Error!\r\n");
	}		
	DHT22_Read_Data(&temperature,&humidity);	//DHT22的初次温湿度采集
  while(1)
	{
		/* 间隔2秒采集 */
		delay_ms(1000);	
		delay_ms(1000);
		/* 温湿度间隔采集 */
		DHT22_Read_Data(&temperature,&humidity);	
		/* 温湿度数据打印串口（print重定向到串口1输出） */
		printf("\r\ntemperature =  %.2f\r\n",temperature);	
		printf("humidity    =  %.2f\r\n",humidity);
	}
 }

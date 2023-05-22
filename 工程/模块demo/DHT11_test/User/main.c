#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "OLED.h"
#include  "dht11.h"
#include  "usart.h"

extern unsigned int rec_data[4];

int main()
{

  OLED_Init();
	uart_init(9600);
	
  OLED_ShowCHinese(0, 0, 0);							//温
	OLED_ShowCHinese(0, 16, 1); 						//度
	OLED_ShowCHinese(0, 32, 4);							//：
	OLED_ShowCHinese(0, 60, 5);							//.
	OLED_ShowCHinese(0, 90, 6);							//℃
	
	
	OLED_ShowCHinese(4, 0, 2);							//湿
	OLED_ShowCHinese(4, 16, 3); 						//度
	OLED_ShowCHinese(4, 32, 4);							//：
	OLED_ShowCHinese(4, 60, 5);							//.
	OLED_ShowCHinese(4, 90, 7);							//%
	while(1)
	{
		delay_s(1);
//		DHT11_REC_Data(); //接收温度和湿度的数据
		
	  OLED_ShowNum(1,7,rec_data[2],2);
		OLED_ShowNum(1,10,rec_data[3],1);
		OLED_ShowNum(3,7,rec_data[0],2);
		OLED_ShowNum(3,10,rec_data[1],2);
		printf("温度：%d\r\n",rec_data[3]); 
		printf("温度：%d\r\n",rec_data[2]); 
		printf("湿度：%d\r\n",rec_data[1]);
		printf("湿度：%d\r\n",rec_data[0]);
    printf("\r\n\n");	

	}
}

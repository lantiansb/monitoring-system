#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "dht22.h"
#include "OLED.h"
 
 int main(void)
 {	
	float temperature = 0;  	    
	float humidity = 0;  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);        //����1��������9600
	delay_ms(1000);
	delay_ms(1000);
    OLED_Init();
     OLED_ShowString(1, 1, "test!");
	while(DHT22_Init())    //DHT22��ʼ����ģ��I/O���
	{
		printf("DHT22_Check Error!\r\n");
	}		
	DHT22_Read_Data(&temperature,&humidity);	//DHT22�ĳ�����ʪ�Ȳɼ�
  while(1)
	{
		/* ���2��ɼ� */
		delay_ms(1000);	
		delay_ms(1000);
		/* ��ʪ�ȼ���ɼ� */
		DHT22_Read_Data(&temperature,&humidity);	
		/* ��ʪ�����ݴ�ӡ���ڣ�print�ض��򵽴���1����� */
		printf("\r\ntemperature =  %.2f\r\n",temperature);	
		printf("humidity    =  %.2f\r\n",humidity);
	}
 }

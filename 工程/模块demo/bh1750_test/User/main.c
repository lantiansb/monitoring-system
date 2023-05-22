#include "stm32f10x.h"                  // Device header
#include "string.h"
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "LED.h"
#include "bh1750.h"

float light;

int main(void)
{
    Serial_Init();
    BH1750_Init();
	while (1)
	{
        if(!i2c_CheckDevice(BH1750_Addr))
        {
            light = LIght_Intensity();
            printf("light = %.2f\r\n", light);
        }
        else
            printf("err!\r\n");
        Delay_ms(1000);
	}
}

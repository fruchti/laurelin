#include "main.h"

int main(void)
{
    LED_InitShiftRegister();

    // Delay a bit to make programming easier
    for(unsigned int i = 0; i < 10000000; i++)
    {
        __asm__ volatile("nop");
    }

    LightSensor_Init();
    LED_Init();

    int ct = 0;
    uint8_t *data = (uint8_t*)LED_PixelData;
    while(1)
    {
        __WFI();
        LightSensor_Poll();
        uint8_t brightness = LightSensor_RelativeBrightness * 255;
        if(LED_FrameFlag)
        {
            LED_FrameFlag = false;
            data[ct] = brightness;
            LED_Commit();
            
            ct += 3;
            if(ct >= LED_COLUMNS * LED_ROWS)
            {
                ct -= LED_COLUMNS * LED_ROWS;
                ct += 1;
            }
        }
    }

    return 0;
}


#include "main.h"

int main(void)
{
    LED_InitShiftRegister();
    LightSensor_Init();

    // Delay a bit to make programming easier
    for(unsigned int i = 0; i < 1000; i++)
    {
        LightSensor_Poll();
    }

    LED_Init();

    uint8_t *data = (uint8_t*)LED_PixelData;
    while(1)
    {
        __WFI();
        LightSensor_Poll();
        uint8_t brightness = LightSensor_RelativeBrightness * 255;
        if(LED_FrameFlag)
        {
            for(int i = 0; i < LED_COLUMNS * LED_ROWS; i++)
            {
                data[i] = brightness;
            }
            
            LED_FrameFlag = false;
            LED_Commit();
        }
    }

    return 0;
}


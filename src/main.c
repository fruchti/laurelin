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

    unsigned int counter = 0;
    const unsigned int blink_period = 1000;
    const unsigned int group_size = 6;
    while(1)
    {
        __WFI();
        LightSensor_Poll();

        if(LED_FrameFlag)
        {
            memset(LED_PixelData, 0, sizeof(LED_PixelData));

            for(unsigned int i = 0; i < LED_COUNT; i++)
            {
                uint8_t brightness = 0;
                if((counter / blink_period) <= (i % group_size))
                {
                    if(counter / (blink_period / 2) % 2)
                    {
                        brightness = 30;
                    }
                }

                switch(i / group_size)
                {
                    case 0:
                        LED_PixelData[i].r = brightness;
                        break;
                    case 1:
                        LED_PixelData[i].g = brightness;
                        break;
                    case 2:
                        LED_PixelData[i].b = brightness;
                        break;
                    case 3:
                        LED_PixelData[i].r = brightness;
                        LED_PixelData[i].g = brightness;
                        break;
                    case 4:
                        LED_PixelData[i].g = brightness;
                        LED_PixelData[i].b = brightness;
                        break;
                    case 5:
                        LED_PixelData[i].b = brightness;
                        LED_PixelData[i].r = brightness;
                        break;
                }

                counter++;
                if(counter > blink_period * (group_size + 2))
                {
                    counter = 0;
                }
            }

            LED_FrameFlag = false;
            LED_Commit();
        }
    }

    return 0;
}


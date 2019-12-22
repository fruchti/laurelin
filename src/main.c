#include "main.h"

int main(void)
{
    // Delay a bit to make programming easier
    for(int i = 0; i < 30000; i++)
    {
        __asm__ volatile("nop");
    }

    LED_Init();

    int ct = 0;
    uint8_t *data = (uint8_t*)LED_PixelData;
    while(1)
    {
        if(LED_FrameFlag)
        {
            LED_FrameFlag = false;
            data[ct]++;
            LED_Commit();
            if(data[ct] == 255)
            {
                ct += 3;
                if(ct >= LED_COLUMNS * LED_ROWS)
                {
                    ct -= LED_COLUMNS * LED_ROWS;
                    ct += 1;
                }
            }
        }
    }

    return 0;
}


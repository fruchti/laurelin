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

    while(1)
    {
        __WFI();
        LightSensor_Poll();
        Animation_Poll();
    }

    return 0;
}


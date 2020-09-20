#include "main.h"

int main(void)
{
    LED_InitShiftRegister();
    LightSensor_Init();

    bool powered_down = false;

    // Delay a bit to make programming easier
    for(unsigned int i = 0; i < 1000; i++)
    {
        LightSensor_Poll();
    }

    NVS_Load();
    LED_Init();
    Animation_Init();

    while(1)
    {
        __WFI();
        LightSensor_Poll();
        Animation_Poll();

        if(LightSensor_RelativeBrightness == 0 && !powered_down)
        {
            LED_Suspend();
            powered_down = true;
        }
        if(powered_down && LightSensor_RelativeBrightness > 0)
        {
            LED_WakeUp();
            powered_down = false;
        }
    }

    return 0;
}

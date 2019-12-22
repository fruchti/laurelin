#include "main.h"

int main(void)
{
    // Delay a bit to make programming easier
    for(int i = 0; i < 30000; i++)
    {
        __asm__ volatile("nop");
    }

    LED_Init();

    while(1)
    {
    }

    return 0;
}


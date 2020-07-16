#include <stdint.h>
#include "stm32f030x6.h"

void SystemInit(void)
{
    // Disable all interrupts
    RCC->CIR = 0x00000000;
}


#pragma once

#include "led.h"

#define ANIMATION_STEPS         65535

extern const unsigned int Animation_LEDOrder[LED_COUNT];

void Animation_Poll(void);
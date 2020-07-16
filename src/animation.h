#pragma once

#include "led.h"

#define ANIMATION_STEPS         65535

#define ANIMATION_DURATION_BOTTOM \
                                1000

#define ANIMATION_DURATION_TOP  5000

extern const unsigned int Animation_LEDOrder[LED_COUNT];

void Animation_Poll(void);
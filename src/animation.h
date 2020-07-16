#pragma once

#include "led.h"

#define ANIMATION_STEPS         65535
#define ANIMATION_BRIGHTNESS_BITS \
                                16
#define ANIMATION_MAX_BRIGHTNESS \
                                (1 << ANIMATION_BRIGHTNESS_BITS)

extern const unsigned int Animation_LEDOrder[LED_COUNT];

void Animation_Poll(void);
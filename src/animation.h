#pragma once

#include <stdbool.h>

#include "led.h"

#define ANIMATION_STEPS         (1 << 24)

#define ANIMATION_REFRESH_RATE  10

// Cycle time of the animation for the bottom end of the gradient (in seconds)
#define ANIMATION_CYCLE_TIME_BOTTOM \
                                (6 * 24 * 60 * 60)
// Cycle time of the animation for the top end of the gradient (in seconds)
#define ANIMATION_CYCLE_TIME_TOP \
                                (8 * 24 * 60 * 60)

extern const unsigned int Animation_LEDOrder[LED_COUNT];

void Animation_Init(void);
void Animation_Poll(void);
#pragma once

#include <stdbool.h>

#include "led.h"

#define ANIMATION_STEPS         (1 << 24)

#define ANIMATION_REFRESH_RATE  10

#define ANIMATION_DURATION_BOTTOM \
                                6048000

#define ANIMATION_DURATION_TOP  6652800

extern const unsigned int Animation_LEDOrder[LED_COUNT];

void Animation_Init(void);
void Animation_Poll(void);
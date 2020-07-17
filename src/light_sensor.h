#pragma once

#include <stdbool.h>

#include "stm32f030x6.h"
#include "pinning.h"

// ADC polling interval in milliseconds
#define LIGHTSENSOR_INTERVAL    250

// Resolution of the brightness output
#define LIGHTSENSOR_BITS        12
#define LIGHTSENSOR_MAX         (1 << LIGHTSENSOR_BITS)

// 'Forgetting factor' of the rolling brightness average
#define LIGHTSENSOR_LAMBDA_BITS 4

// Time until minimum and maximum value are completely reset (in seconds)
#define LIGHTSENSOR_LIMIT_RESET_TIME \
                                (24 * 60 * 60)

// Bounds for converting absolute to relative brightness: Consider everything
// near the minimum or maximum 0.0 or 1.0, respectively
#define LIGHTSENSOR_LOW_BOUND   ((unsigned int)(0.005 * LIGHTSENSOR_MAX))
#define LIGHTSENSOR_HIGH_BOUND  ((unsigned int)(0.9 * LIGHTSENSOR_MAX))

extern int LightSensor_RelativeBrightness;

void LightSensor_Init(void);
void LightSensor_Poll(void);

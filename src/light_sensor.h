#pragma once

#include <stdbool.h>

#include "stm32f030x6.h"
#include "pinning.h"

// ADC polling interval in milliseconds
#define LIGHTSENSOR_INTERVAL    500

// Resolution of the brightness output
#define LIGHTSENSOR_BITS        12
#define LIGHTSENSOR_MAX         (1 << LIGHTSENSOR_BITS)

// 'Forgetting factor' of the rolling brightness average
#define LIGHTSENSOR_LAMBDA_BITS 2

// Time until minimum and maximum value are completely reset (in seconds)
#define LIGHTSENSOR_LIMIT_RESET_TIME \
                                (48 * 60 * 60)

// Bounds for converting absolute to relative brightness: Consider everything
// near the minimum or maximum 0.0 or 1.0, respectively
#define LIGHTSENSOR_LOW_BOUND   ((unsigned int)(0.013 * LIGHTSENSOR_MAX))
#define LIGHTSENSOR_HIGH_BOUND  ((unsigned int)(1.0 * LIGHTSENSOR_MAX))

extern int LightSensor_RelativeBrightness;

void LightSensor_Init(void);
void LightSensor_Poll(void);

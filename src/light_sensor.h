#pragma once

#include <stdbool.h>

#include "stm32f030x6.h"
#include "pinning.h"

// ADC polling interval in milliseconds
#define LIGHTSENSOR_INTERVAL    250

// 'Forgetting factor' of the rolling brightness average
#define LIGHTSENSOR_LAMBDA      0.9f

// Bounds for converting absolute to relative brightness: Consider everything
// near the minimum or maximum 0.0 or 1.0, respectively
#define LIGHTSENSOR_LOW_BOUND   0.005f
#define LIGHTSENSOR_HIGH_BOUND  0.9f

extern float LightSensor_RelativeBrightness;

void LightSensor_Init(void);
void LightSensor_Poll(void);

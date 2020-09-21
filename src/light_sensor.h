#pragma once

#include <stdbool.h>

#include "stm32f030x6.h"
#include "pinning.h"

// ADC polling interval in milliseconds
#define LIGHTSENSOR_INTERVAL    500

// Resolution of the brightness output
#define LIGHTSENSOR_BITS        12
#define LIGHTSENSOR_MAX         ((1 << LIGHTSENSOR_BITS) - 1)

// 'Forgetting factor' of the rolling brightness average
#define LIGHTSENSOR_LAMBDA_BITS 2

// Bounds for converting absolute to relative brightness (empirically
// determined)
#define LIGHTSENSOR_LOW_BOUND   5
#define LIGHTSENSOR_HIGH_BOUND  ((int)(0.85 * LIGHTSENSOR_MAX))

extern int LightSensor_RelativeBrightness;

void LightSensor_Init(void);
void LightSensor_Poll(void);

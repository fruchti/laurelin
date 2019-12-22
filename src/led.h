#pragma once

#include "stm32f030x6.h"
#include "pinning.h"

#define LED_BITS                12
#define LED_ROWS                8   // Rows are driven by a shift register
#define LED_COLUMNS             12  // Columns are driven by the MCU directly

void LED_Init(void);


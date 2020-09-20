#pragma once

#include <stdbool.h>

#include "stm32f030x6.h"

typedef struct
__attribute__((packed, aligned(2)))
{
    unsigned int animation_step_bottom;
    unsigned int animation_step_top;
} NVS_Data_t;

extern NVS_Data_t *const NVS_Data;

// Returns true if the data was successfully loaded from flash and false if the
// defaults were restored instead
bool NVS_Load(void);

// Stores the current contents of NVS_Data to flash. Pass `false` as a parameter
// to skip saving unless it can be done without a flash page erase.
void NVS_Save(bool erase_if_needed);



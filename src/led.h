#pragma once

#include <stdbool.h>

#include "stm32f030x6.h"
#include "pinning.h"

#define LED_BITS                12  // BCM resolution in bits
#define LED_ROWS                8   // Rows are driven by a shift register
#define LED_COLUMNS             12  // Columns are driven by the MCU directly.
                                    // Set to 9 to free the SWD pins and enable
                                    // debugging.
#define LED_COUNT               (LED_ROWS * LED_COLUMNS / 3)

typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} __attribute__((packed)) LED_Colour_t;

// Pixel data, not displayed until LED_Commit() is called
extern LED_Colour_t LED_PixelData[LED_COUNT];

// Is set to true for every frame, can be used for timing or synchronisation
// externally. Will not be set to false within led.c.
extern volatile bool LED_FrameFlag;

void LED_InitShiftRegister(void);
void LED_Init(void);

// Enter power-saving mode (LEDs off)
void LED_Suspend(void);

// Leave power-saving mode
void LED_WakeUp(void);

// Display LED_PixelData, starting with the next frame
void LED_Commit(void);

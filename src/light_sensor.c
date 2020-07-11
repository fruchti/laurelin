#include "light_sensor.h"

volatile unsigned int LightSensor_Measurement;
volatile bool LightSensor_NewMeasurement = false;

// Rolling average of the brightness measurement
float LightSensor_AbsoluteBrightness = 0.5f;

// Maximum and minimum encountered so far
float LightSensor_MinimumBrightness = 1.0f;
float LightSensor_MaximumBrightness = 0.0f;

float LightSensor_RelativeBrightness;

static void LightSensor_Measure(void)
{
    // Use light sensor pin in output mode, drive high
    GPIOB->MODER = (GPIOB->MODER & ~(0x03 << PIN_LIGHT_SENSOR * 2))
        | (0x01 << PIN_LIGHT_SENSOR * 2);

    // Disable timer
    TIM14->SR = 0x0000;
    TIM14->CR1 = 0x0000;
    TIM14->CNT = 0;

    // Delay a bit to ensure capacitor is charged
    for(unsigned int i = 0; i < 100; i++)
    {
        __asm__ volatile("nop");
    }

    // Re-enable timer
    TIM14->CR1 = TIM_CR1_CEN;

    // Switch to alternate function mode
    GPIOB->MODER = (GPIOB->MODER & ~(0x03 << PIN_LIGHT_SENSOR * 2))
        | (0x02 << PIN_LIGHT_SENSOR * 2);
}

void LightSensor_Init(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;

    GPIOB->ODR |= (1 << PIN_LIGHT_SENSOR);
    GPIOB->AFR[0] &= ~(0x0f << (PIN_LIGHT_SENSOR * 4));

    // Enable input capture for light sensor pin, trigger on falling edge
    TIM14->CCMR1 = TIM_CCMR1_CC1S_0;
    TIM14->CCER = TIM_CCER_CC1E | TIM_CCER_CC1P;
    // Set up TIM14 for an oveflow interrupt at the configured interval
    TIM14->PSC = 48000 * LIGHTSENSOR_INTERVAL / 65535;
    TIM14->ARR = 65535;
    TIM14->DIER = TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM14_IRQn);
    
    LightSensor_Measure();
}

void LightSensor_Poll(void)
{
    if(LightSensor_NewMeasurement)
    {
        unsigned int measurement = LightSensor_Measurement;
        LightSensor_NewMeasurement = false;
        float brightness = 1.0f - measurement / 65535.0f;

        if(measurement != 65535 && brightness < LightSensor_MinimumBrightness)
        {
            LightSensor_MinimumBrightness = brightness;
        }
        if(brightness > LightSensor_MaximumBrightness)
        {
            LightSensor_MaximumBrightness = brightness;
        }

        LightSensor_AbsoluteBrightness = LIGHTSENSOR_LAMBDA * LightSensor_AbsoluteBrightness
            + (1.0f - LIGHTSENSOR_LAMBDA) * brightness;

        // Slowly move maximum and minimum back to 0.0 and 1.0, respectively
        LightSensor_MaximumBrightness *= LIGHTSENSOR_ALPHA;
        LightSensor_MinimumBrightness = 1.0f - LightSensor_MinimumBrightness;
        LightSensor_MinimumBrightness *= LIGHTSENSOR_ALPHA;
        LightSensor_MinimumBrightness = 1.0f - LightSensor_MinimumBrightness;

        // Scale and saturate to get relative brightness value
        float range = LightSensor_MaximumBrightness
            - LightSensor_MinimumBrightness;
        float low = LightSensor_MinimumBrightness
            + range * LIGHTSENSOR_LOW_BOUND;
        float high = LightSensor_MinimumBrightness
            + range * LIGHTSENSOR_HIGH_BOUND;
        LightSensor_RelativeBrightness = (LightSensor_AbsoluteBrightness - low)
            / (high - low);
        if(LightSensor_RelativeBrightness < 0.0f)
        {
            LightSensor_RelativeBrightness = 0.0f;
        }
        if(LightSensor_RelativeBrightness > 1.0f)
        {
            LightSensor_RelativeBrightness = 1.0f;
        }
    }
}

void TIM14_IRQHandler(void)
{
    if(TIM14->SR & TIM_SR_CC1IF)
    {
        LightSensor_Measurement = TIM14->CCR1;
    }
    else
    {
        LightSensor_Measurement = 65535;
    }
    LightSensor_NewMeasurement = true;

    LightSensor_Measure();
}
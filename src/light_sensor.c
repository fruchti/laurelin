#include "light_sensor.h"

volatile unsigned int LightSensor_Measurement;
volatile bool LightSensor_NewMeasurement = false;

// Rolling average of the brightness measurement
unsigned int LightSensor_AbsoluteBrightness = LIGHTSENSOR_MAX / 2;

// Maximum and minimum encountered so far
unsigned int LightSensor_MinimumBrightness = LIGHTSENSOR_MAX;
unsigned int LightSensor_MaximumBrightness = 0;

int LightSensor_RelativeBrightness;

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
    TIM14->PSC = 8000 * LIGHTSENSOR_INTERVAL / 65535;
    TIM14->ARR = 65535;
    TIM14->DIER = TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM14_IRQn);
    
    LightSensor_Measure();

    while(!LightSensor_NewMeasurement);
    LightSensor_Poll();
}

void LightSensor_Poll(void)
{
    if(LightSensor_NewMeasurement)
    {
        unsigned int measurement = LightSensor_Measurement;
        LightSensor_NewMeasurement = false;
        unsigned int brightness = ((1 << 31)
            / (measurement + 1)) >> (31 - 16);
        // LightSensor_AbsoluteBrightness = LIGHTSENSOR_LAMBDA * LightSensor_AbsoluteBrightness
        //     + (1.0f - LIGHTSENSOR_LAMBDA) * brightness;
        LightSensor_AbsoluteBrightness -= LightSensor_AbsoluteBrightness >> LIGHTSENSOR_LAMBDA_BITS;
        LightSensor_AbsoluteBrightness += brightness >> LIGHTSENSOR_LAMBDA_BITS;

        if(brightness < LightSensor_MinimumBrightness)
        {
            LightSensor_MinimumBrightness = brightness;
        }
        if(brightness > LightSensor_MaximumBrightness)
        {
            LightSensor_MaximumBrightness = brightness;
        }

        // Scale and saturate to get relative brightness value
        int range = LightSensor_MaximumBrightness
            - LightSensor_MinimumBrightness;
        int low = LightSensor_MinimumBrightness
            + LIGHTSENSOR_LOW_BOUND * range / LIGHTSENSOR_MAX;
        int high = LightSensor_MinimumBrightness
            + LIGHTSENSOR_HIGH_BOUND * range / LIGHTSENSOR_MAX;
        LightSensor_RelativeBrightness = ((int)LightSensor_AbsoluteBrightness - low)
            * LIGHTSENSOR_MAX / (high - low);
        if(LightSensor_RelativeBrightness < 0)
        {
            LightSensor_RelativeBrightness = 0;
        }
        if(LightSensor_RelativeBrightness > LIGHTSENSOR_MAX)
        {
            LightSensor_RelativeBrightness = LIGHTSENSOR_MAX;
        }

        // Slowly reset limit values
        static int decay_counter = 0;
        decay_counter++;
        if(decay_counter == LIGHTSENSOR_LIMIT_RESET_TIME * 1000
            / LIGHTSENSOR_INTERVAL / LIGHTSENSOR_MAX)
        {
            decay_counter = 0;
            LightSensor_MaximumBrightness -= 1;
            LightSensor_MinimumBrightness += 1;
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

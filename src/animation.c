#include "animation.h"
#include "animation_lut.h"
#include "light_sensor.h"

volatile bool Animation_FrameFlag = false;

const unsigned int Animation_LEDOrder[LED_COUNT] =
{
    19, 27, 21, 13, 0, 4, 24, 8, 12, 15, 6, 5, 28,
    29, 17, 3, 18, 26, 22, 10, 16, 20, 30, 1,
    25, 2, 14, 31, 7, 11, 9, 23
};

LED_Colour_t Animation_GetColour(unsigned int step, unsigned int brightness)
{
    const unsigned int lut_size = sizeof(Animation_ColourLUT)
        / sizeof(LED_Colour_t);

    unsigned int index = step * lut_size / ANIMATION_STEPS;

    LED_Colour_t colour = Animation_ColourLUT[index];

    colour.r = ((unsigned int)colour.r * brightness) >> LIGHTSENSOR_BITS;
    colour.g = ((unsigned int)colour.g * brightness) >> LIGHTSENSOR_BITS;
    colour.b = ((unsigned int)colour.b * brightness) >> LIGHTSENSOR_BITS;

    return colour;
}

void Animation_DrawGradient(unsigned int step_bottom, unsigned int step_top,
    unsigned int brightness)
{
    if(step_bottom >= ANIMATION_STEPS)
    {
        step_bottom = 2 * ANIMATION_STEPS - step_bottom;
    }
    if(step_top >= ANIMATION_STEPS)
    {
        step_top = 2 * ANIMATION_STEPS - step_top;
    }
    for(int i = 0; i < LED_COUNT; i++)
    {
        unsigned int step = ((LED_COUNT - 1 - i) * step_bottom + i * step_top)
            / (LED_COUNT - 1);
        LED_PixelData[Animation_LEDOrder[i]] = Animation_GetColour(step,
            brightness);
    }
}

void Animation_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_TIM17EN;

    TIM17->PSC = 8000 - 1;
    TIM17->ARR = 1000 / ANIMATION_REFRESH_RATE;
    TIM17->DIER = TIM_DIER_UIE;
    TIM17->CR1 = TIM_CR1_CEN;
    NVIC_EnableIRQ(TIM17_IRQn);
}

void Animation_Poll(void)
{
    if(!Animation_FrameFlag)
    {
        return;
    }
    Animation_FrameFlag = false;

    static unsigned int bottom = 0;
    static unsigned int top = 0;

    bottom += ANIMATION_STEPS / ANIMATION_CYCLE_TIME_BOTTOM
        / ANIMATION_REFRESH_RATE;
    top += ANIMATION_STEPS / ANIMATION_CYCLE_TIME_TOP
        / ANIMATION_REFRESH_RATE;

    bottom %= 2 * ANIMATION_STEPS;
    top %= 2 * ANIMATION_STEPS;

    Animation_DrawGradient(bottom, top, LightSensor_RelativeBrightness);
    LED_Commit();
}

void TIM17_IRQHandler(void)
{
    Animation_FrameFlag = true;
    TIM17->SR &= ~TIM_SR_UIF;
}
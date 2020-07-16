#include "animation.h"
#include "light_sensor.h"

const unsigned int Animation_LEDOrder[LED_COUNT] =
{
    19, 27, 21, 13, 0, 4, 24, 8, 12, 15, 6, 5, 28,
    29, 17, 3, 18, 26, 22, 10, 16, 20, 30, 1,
    25, 2, 14, 31, 7, 11, 9, 23
};

const LED_Colour_t Animation_ColourLUT[] =
{
    { .r = 255, .g = 51, .b = 51 },
    { .r = 255, .g = 51, .b = 56 },
    { .r = 255, .g = 51, .b = 61 },
    { .r = 255, .g = 51, .b = 65 },
    { .r = 255, .g = 51, .b = 70 },
    { .r = 255, .g = 51, .b = 75 },
    { .r = 255, .g = 51, .b = 80 },
    { .r = 255, .g = 51, .b = 85 },
    { .r = 255, .g = 51, .b = 89 },
    { .r = 255, .g = 51, .b = 94 },
    { .r = 255, .g = 51, .b = 99 },
    { .r = 255, .g = 51, .b = 104 },
    { .r = 255, .g = 51, .b = 109 },
    { .r = 255, .g = 51, .b = 113 },
    { .r = 255, .g = 51, .b = 118 },
    { .r = 255, .g = 51, .b = 123 },
    { .r = 255, .g = 51, .b = 128 },
    { .r = 255, .g = 51, .b = 133 },
    { .r = 255, .g = 51, .b = 137 },
    { .r = 255, .g = 51, .b = 142 },
    { .r = 255, .g = 51, .b = 147 },
    { .r = 255, .g = 51, .b = 152 },
    { .r = 255, .g = 51, .b = 157 },
    { .r = 255, .g = 51, .b = 161 },
    { .r = 255, .g = 51, .b = 166 },
    { .r = 255, .g = 51, .b = 171 },
    { .r = 255, .g = 51, .b = 176 },
    { .r = 255, .g = 51, .b = 181 },
    { .r = 255, .g = 51, .b = 185 },
    { .r = 255, .g = 51, .b = 190 },
    { .r = 255, .g = 51, .b = 195 },
    { .r = 255, .g = 51, .b = 200 },
    { .r = 255, .g = 51, .b = 205 },
    { .r = 255, .g = 51, .b = 209 },
    { .r = 255, .g = 51, .b = 214 },
    { .r = 255, .g = 51, .b = 219 },
    { .r = 255, .g = 51, .b = 224 },
    { .r = 255, .g = 51, .b = 229 },
    { .r = 255, .g = 51, .b = 233 },
    { .r = 255, .g = 51, .b = 238 },
    { .r = 255, .g = 51, .b = 243 },
    { .r = 255, .g = 51, .b = 248 },
    { .r = 255, .g = 51, .b = 253 },
    { .r = 253, .g = 51, .b = 255 },
    { .r = 248, .g = 51, .b = 255 },
    { .r = 243, .g = 51, .b = 255 },
    { .r = 238, .g = 51, .b = 255 },
    { .r = 233, .g = 51, .b = 255 },
    { .r = 229, .g = 51, .b = 255 },
    { .r = 224, .g = 51, .b = 255 },
    { .r = 219, .g = 51, .b = 255 },
    { .r = 214, .g = 51, .b = 255 },
    { .r = 209, .g = 51, .b = 255 },
    { .r = 205, .g = 51, .b = 255 },
    { .r = 200, .g = 51, .b = 255 },
    { .r = 195, .g = 51, .b = 255 },
    { .r = 190, .g = 51, .b = 255 },
    { .r = 185, .g = 51, .b = 255 },
    { .r = 181, .g = 51, .b = 255 },
    { .r = 176, .g = 51, .b = 255 },
    { .r = 171, .g = 51, .b = 255 },
    { .r = 166, .g = 51, .b = 255 },
    { .r = 161, .g = 51, .b = 255 },
    { .r = 157, .g = 51, .b = 255 },
    { .r = 152, .g = 51, .b = 255 },
    { .r = 147, .g = 51, .b = 255 },
    { .r = 142, .g = 51, .b = 255 },
    { .r = 137, .g = 51, .b = 255 },
    { .r = 133, .g = 51, .b = 255 },
    { .r = 128, .g = 51, .b = 255 },
    { .r = 123, .g = 51, .b = 255 },
    { .r = 118, .g = 51, .b = 255 },
    { .r = 113, .g = 51, .b = 255 },
    { .r = 109, .g = 51, .b = 255 },
    { .r = 104, .g = 51, .b = 255 },
    { .r = 99, .g = 51, .b = 255 },
    { .r = 94, .g = 51, .b = 255 },
    { .r = 89, .g = 51, .b = 255 },
    { .r = 85, .g = 51, .b = 255 },
    { .r = 80, .g = 51, .b = 255 },
    { .r = 75, .g = 51, .b = 255 },
    { .r = 70, .g = 51, .b = 255 },
    { .r = 65, .g = 51, .b = 255 },
    { .r = 61, .g = 51, .b = 255 },
    { .r = 56, .g = 51, .b = 255 },
    { .r = 51, .g = 51, .b = 255 },
    { .r = 51, .g = 56, .b = 255 },
    { .r = 51, .g = 61, .b = 255 },
    { .r = 51, .g = 65, .b = 255 },
    { .r = 51, .g = 70, .b = 255 },
    { .r = 51, .g = 75, .b = 255 },
    { .r = 51, .g = 80, .b = 255 },
    { .r = 51, .g = 85, .b = 255 },
    { .r = 51, .g = 89, .b = 255 },
    { .r = 51, .g = 94, .b = 255 },
    { .r = 51, .g = 99, .b = 255 },
    { .r = 51, .g = 104, .b = 255 },
    { .r = 51, .g = 109, .b = 255 },
    { .r = 51, .g = 113, .b = 255 },
    { .r = 51, .g = 118, .b = 255 },
    { .r = 51, .g = 123, .b = 255 },
    { .r = 51, .g = 128, .b = 255 },
    { .r = 51, .g = 133, .b = 255 },
    { .r = 51, .g = 137, .b = 255 },
    { .r = 51, .g = 142, .b = 255 },
    { .r = 51, .g = 147, .b = 255 },
    { .r = 51, .g = 152, .b = 255 },
    { .r = 51, .g = 157, .b = 255 },
    { .r = 51, .g = 161, .b = 255 },
    { .r = 51, .g = 166, .b = 255 },
    { .r = 51, .g = 171, .b = 255 },
    { .r = 51, .g = 176, .b = 255 },
    { .r = 51, .g = 181, .b = 255 },
    { .r = 51, .g = 185, .b = 255 },
    { .r = 51, .g = 190, .b = 255 },
    { .r = 51, .g = 195, .b = 255 },
    { .r = 51, .g = 200, .b = 255 },
    { .r = 51, .g = 205, .b = 255 },
    { .r = 51, .g = 209, .b = 255 },
    { .r = 51, .g = 214, .b = 255 },
    { .r = 51, .g = 219, .b = 255 },
    { .r = 51, .g = 224, .b = 255 },
    { .r = 51, .g = 229, .b = 255 },
    { .r = 51, .g = 233, .b = 255 },
    { .r = 51, .g = 238, .b = 255 },
    { .r = 51, .g = 243, .b = 255 },
    { .r = 51, .g = 248, .b = 255 },
    { .r = 51, .g = 253, .b = 255 },
    { .r = 51, .g = 255, .b = 253 },
    { .r = 51, .g = 255, .b = 248 },
    { .r = 51, .g = 255, .b = 243 },
    { .r = 51, .g = 255, .b = 238 },
    { .r = 51, .g = 255, .b = 233 },
    { .r = 51, .g = 255, .b = 229 },
    { .r = 51, .g = 255, .b = 224 },
    { .r = 51, .g = 255, .b = 219 },
    { .r = 51, .g = 255, .b = 214 },
    { .r = 51, .g = 255, .b = 209 },
    { .r = 51, .g = 255, .b = 205 },
    { .r = 51, .g = 255, .b = 200 },
    { .r = 51, .g = 255, .b = 195 },
    { .r = 51, .g = 255, .b = 190 },
    { .r = 51, .g = 255, .b = 185 },
    { .r = 51, .g = 255, .b = 181 },
    { .r = 51, .g = 255, .b = 176 },
    { .r = 51, .g = 255, .b = 171 },
    { .r = 51, .g = 255, .b = 166 },
    { .r = 51, .g = 255, .b = 161 },
    { .r = 51, .g = 255, .b = 157 },
    { .r = 51, .g = 255, .b = 152 },
    { .r = 51, .g = 255, .b = 147 },
    { .r = 51, .g = 255, .b = 142 },
    { .r = 51, .g = 255, .b = 137 },
    { .r = 51, .g = 255, .b = 133 },
    { .r = 51, .g = 255, .b = 128 },
    { .r = 51, .g = 255, .b = 123 },
    { .r = 51, .g = 255, .b = 118 },
    { .r = 51, .g = 255, .b = 113 },
    { .r = 51, .g = 255, .b = 109 },
    { .r = 51, .g = 255, .b = 104 },
    { .r = 51, .g = 255, .b = 99 },
    { .r = 51, .g = 255, .b = 94 },
    { .r = 51, .g = 255, .b = 89 },
    { .r = 51, .g = 255, .b = 85 },
    { .r = 51, .g = 255, .b = 80 },
    { .r = 51, .g = 255, .b = 75 },
    { .r = 51, .g = 255, .b = 70 },
    { .r = 51, .g = 255, .b = 65 },
    { .r = 51, .g = 255, .b = 61 },
    { .r = 51, .g = 255, .b = 56 },
    { .r = 51, .g = 255, .b = 51 },
    { .r = 56, .g = 255, .b = 51 },
    { .r = 61, .g = 255, .b = 51 },
    { .r = 65, .g = 255, .b = 51 },
    { .r = 70, .g = 255, .b = 51 },
    { .r = 75, .g = 255, .b = 51 },
    { .r = 80, .g = 255, .b = 51 },
    { .r = 85, .g = 255, .b = 51 },
    { .r = 89, .g = 255, .b = 51 },
    { .r = 94, .g = 255, .b = 51 },
    { .r = 99, .g = 255, .b = 51 },
    { .r = 104, .g = 255, .b = 51 },
    { .r = 109, .g = 255, .b = 51 },
    { .r = 113, .g = 255, .b = 51 },
    { .r = 118, .g = 255, .b = 51 },
    { .r = 123, .g = 255, .b = 51 },
    { .r = 128, .g = 255, .b = 51 },
    { .r = 133, .g = 255, .b = 51 },
    { .r = 137, .g = 255, .b = 51 },
    { .r = 142, .g = 255, .b = 51 },
    { .r = 147, .g = 255, .b = 51 },
    { .r = 152, .g = 255, .b = 51 },
    { .r = 157, .g = 255, .b = 51 },
    { .r = 161, .g = 255, .b = 51 },
    { .r = 166, .g = 255, .b = 51 },
    { .r = 171, .g = 255, .b = 51 },
    { .r = 176, .g = 255, .b = 51 },
    { .r = 181, .g = 255, .b = 51 },
    { .r = 185, .g = 255, .b = 51 },
    { .r = 190, .g = 255, .b = 51 },
    { .r = 195, .g = 255, .b = 51 },
    { .r = 200, .g = 255, .b = 51 },
    { .r = 205, .g = 255, .b = 51 },
    { .r = 209, .g = 255, .b = 51 },
    { .r = 214, .g = 255, .b = 51 },
    { .r = 219, .g = 255, .b = 51 },
    { .r = 224, .g = 255, .b = 51 },
    { .r = 229, .g = 255, .b = 51 },
    { .r = 233, .g = 255, .b = 51 },
    { .r = 238, .g = 255, .b = 51 },
    { .r = 243, .g = 255, .b = 51 },
    { .r = 248, .g = 255, .b = 51 },
    { .r = 253, .g = 255, .b = 51 },
    { .r = 255, .g = 253, .b = 51 },
    { .r = 255, .g = 248, .b = 51 },
    { .r = 255, .g = 243, .b = 51 },
    { .r = 255, .g = 238, .b = 51 },
    { .r = 255, .g = 233, .b = 51 },
    { .r = 255, .g = 229, .b = 51 },
    { .r = 255, .g = 224, .b = 51 },
    { .r = 255, .g = 219, .b = 51 },
    { .r = 255, .g = 214, .b = 51 },
    { .r = 255, .g = 209, .b = 51 },
    { .r = 255, .g = 205, .b = 51 },
    { .r = 255, .g = 200, .b = 51 },
    { .r = 255, .g = 195, .b = 51 },
    { .r = 255, .g = 190, .b = 51 },
    { .r = 255, .g = 185, .b = 51 },
    { .r = 255, .g = 181, .b = 51 },
    { .r = 255, .g = 176, .b = 51 },
    { .r = 255, .g = 171, .b = 51 },
    { .r = 255, .g = 166, .b = 51 },
    { .r = 255, .g = 161, .b = 51 },
    { .r = 255, .g = 157, .b = 51 },
    { .r = 255, .g = 152, .b = 51 },
    { .r = 255, .g = 147, .b = 51 },
    { .r = 255, .g = 142, .b = 51 },
    { .r = 255, .g = 137, .b = 51 },
    { .r = 255, .g = 133, .b = 51 },
    { .r = 255, .g = 128, .b = 51 },
    { .r = 255, .g = 123, .b = 51 },
    { .r = 255, .g = 118, .b = 51 },
    { .r = 255, .g = 113, .b = 51 },
    { .r = 255, .g = 109, .b = 51 },
    { .r = 255, .g = 104, .b = 51 },
    { .r = 255, .g = 99, .b = 51 },
    { .r = 255, .g = 94, .b = 51 },
    { .r = 255, .g = 89, .b = 51 },
    { .r = 255, .g = 85, .b = 51 },
    { .r = 255, .g = 80, .b = 51 },
    { .r = 255, .g = 75, .b = 51 },
    { .r = 255, .g = 70, .b = 51 },
    { .r = 255, .g = 65, .b = 51 },
    { .r = 255, .g = 61, .b = 51 },
    { .r = 255, .g = 56, .b = 51 },
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
    for(int i = 0; i < LED_COUNT; i++)
    {
        unsigned int step = ((LED_COUNT - 1 - i) * step_bottom + i * step_top)
            / (LED_COUNT - 1);
        LED_PixelData[Animation_LEDOrder[i]] = Animation_GetColour(step,
            brightness);
    }
}

void Animation_Poll(void)
{
    if(!LED_FrameFlag)
    {
        return;
    }
    LED_FrameFlag = false;
    Animation_DrawGradient(60000, 50000, LightSensor_RelativeBrightness);
    LED_Commit();
}
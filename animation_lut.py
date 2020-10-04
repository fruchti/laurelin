#!/usr/bin/env python

from colorsys import hsv_to_rgb

LUT_SIZE = 256
SATURATION = 0.8
VALUE = 1
RESOLUTION = 12     # in bits

print('const LED_Colour_t Animation_ColourLUT[] =')
print('{')
for i in range(LUT_SIZE):
    h = i / LUT_SIZE
    rgb = hsv_to_rgb(h, SATURATION, VALUE)
    # Gamma-correct
    rgb = tuple(pow(x, 2.2) for x in rgb)
    # Scale to BCM resolution
    (r, g, b) = tuple(round(2 ** RESOLUTION * x) for x in rgb)
    print('    {{ .r = {}, .g = {}, .b = {} }},'.format(r, g, b))
print('};')

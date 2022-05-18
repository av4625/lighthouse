#include <FastLED.h>
const uint8_t number_of_leds(16);
CRGB leds[number_of_leds];

void setup()
{
    FastLED.addLeds<NEOPIXEL, 0>(leds, number_of_leds);
    // Uses 5% space, pretty safe to remove for 16 LED's
    FastLED.setBrightness(
        calculate_max_brightness_for_power_vmA(
            leds, number_of_leds, 255, 5, 475));
}

uint8_t colour(0);
uint8_t i(0);

void loop()
{
    leds[i] = CHSV(colour, 255, 75);
    FastLED.show();
    fade_all(200);
    delay(75);
    colour += 10;
    i = ((i + 1) % number_of_leds);
}

void fade_all(const uint8_t nscale)
{
    for(uint8_t i = 0; i < number_of_leds; ++i)
    {
        leds[i].nscale8(nscale);
    }
}

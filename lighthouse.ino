#include <FastLED.h>

const uint8_t number_of_leds(16);
CRGBArray<number_of_leds> leds;

void setup()
{
    FastLED.addLeds<NEOPIXEL, 0>(leds, number_of_leds);
    // Uses 5% space, pretty safe to remove for 16 LED's
    FastLED.setBrightness(
        calculate_max_brightness_for_power_vmA(
            leds, number_of_leds, 255, 5, 475));
}

bool has_delay_elapsed(const short delay)
{
    static unsigned long previous_time(0);
    const unsigned long current_time(millis());

    if (current_time > (previous_time + delay))
    {
        previous_time = current_time;
        return true;
    }

    return false;
}

void cyon(
    const short delay,
    const uint8_t brightness,
    const bool is_white_only,
    const bool is_one_at_a_time)
{
    static uint8_t index(1);
    static uint8_t colour(0);

    if (has_delay_elapsed(delay))
    {
        if (is_one_at_a_time)
        {
            leds[index] = CHSV(
                colour,
                (is_white_only) ? 0 : 255,
                brightness);

            index = (index + 1) % number_of_leds;
        }
        else
        {
            // To get the correct groups of two for my dividers
            if (index % 2 != 1)
            {
                index = 1;
            }

            const uint8_t second_index((index + 1) % number_of_leds);

            // This is needed to wrap round to the start
            if (second_index < index)
            {
                leds[index] = CHSV(
                    colour,
                    (is_white_only) ? 0 : 255,
                    brightness);
                leds[second_index] = CHSV(
                    colour,
                    (is_white_only) ? 0 : 255,
                    brightness);
            }
            else
            {
                leds(index, (index + 1) % number_of_leds) = CHSV(
                    colour,
                    (is_white_only) ? 0 : 255,
                    brightness);
            }

            index = (index + 2) % number_of_leds;
        }

        FastLED.show();
        // Make this configurable?
        fade_all(75);
        // Make this configurable?
        colour += 8;
    }
}

void colour_rotation(
    const short delay, const uint8_t brightness, const bool is_one_at_a_time)
{
    static uint8_t colour(0);

    if (has_delay_elapsed(delay))
    {
        if (is_one_at_a_time)
        {
            for (CRGB& led : leds)
            {
                // 15 is 255 / 17 (1 extra led) to make it rotate
                led = CHSV(colour += 15, 255, brightness);
            }
        }
        else
        {
            for (uint8_t i = 1; i < number_of_leds; i += 2)
            {
                // 28.33333 is 255 / 9 (1 extra section) to make it rotate
                leds[i] = CHSV(colour, 255, brightness);

                /* Starting at 1 and have this if to get the correct groups of
                 * two for my dividers
                 */
                if (i != number_of_leds - 1)
                {
                    leds[i + 1] = CHSV(colour, 255, brightness);
                }
                else
                {
                    leds[0] = CHSV(colour, 255, brightness);
                }

                colour += 28;
            }
        }

        FastLED.show();
    }
}

void fade_all(const uint8_t nscale)
{
    leds.fadeToBlackBy(nscale);
}

uint8_t colour(0);
uint8_t i(0);

void loop()
{
    // cyon(250, 80, false, false);
    colour_rotation(200, 100, false);
}

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

            ++index %= number_of_leds;
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

void multiple_spinning_leds(
    const short delay,
    const uint8_t brightness,
    const bool is_white_only,
    const bool is_same_colour,
    const bool cyon_effect = true)
{
    // Adding this so we don't have to divide everytime for a "const"
    static const uint8_t half_leds(number_of_leds / 2);
    // Integer division round up
    static const uint8_t colour_change_per_led(
        (255 + (number_of_leds - 1)) / number_of_leds);
    static const uint8_t half_colour(colour_change_per_led * half_leds);

    static uint8_t index(0);
    static uint8_t colour(0);

    if (has_delay_elapsed(delay))
    {
        const uint8_t second_index((index + half_leds) % number_of_leds);

        if (is_same_colour)
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
            leds[index] = CHSV(
                colour,
                (is_white_only) ? 0 : 255,
                brightness);
            leds[second_index] = CHSV(
                colour + half_colour,
                (is_white_only) ? 0 : 255,
                brightness);
        }

        if (cyon_effect)
        {
            FastLED.show();
            // Make this configurable?
            fade_all(75);
        }
        else
        {
            set_black_except(index, second_index);
            FastLED.show();
        }

        ++index %= number_of_leds;
        colour += colour_change_per_led;
    }
}

void colour_rotation(
    const short delay, const uint8_t brightness, const bool is_one_at_a_time)
{
    // Integer division round up
    static const uint8_t colour_change_per_led(
        (255 + (number_of_leds - 1)) / number_of_leds);
    // Adding this so we don't have to compute a "constant" lots of times
    static const uint8_t colour_change_per_led_group(colour_change_per_led * 2);

    static uint8_t colour(0);

    if (has_delay_elapsed(delay))
    {
        uint8_t current_colour(colour);

        if (is_one_at_a_time)
        {
            for (CRGB& led : leds)
            {
                led = CHSV(
                    current_colour += colour_change_per_led, 255, brightness);
            }
        }
        else
        {
            for (uint8_t i = 1; i < number_of_leds; i += 2)
            {
                leds[i] = CHSV(
                    current_colour += colour_change_per_led_group,
                    255,
                    brightness);

                /* Starting at 1 and have this if to get the correct groups of
                 * two for my dividers
                 */
                if (i != number_of_leds - 1)
                {
                    leds[i + 1] = CHSV(current_colour, 255, brightness);
                }
                else
                {
                    leds[0] = CHSV(current_colour, 255, brightness);
                }
            }
        }

        FastLED.show();
        ++colour;
    }
}

void fade_in_and_out(
    const short delay, const uint8_t brightness, const bool is_white_only)
{
    static uint8_t colour(0);
    static uint8_t current_brightness(0);
    static bool fading_out(false);

    if (has_delay_elapsed(delay))
    {
        for (CRGB& led : leds)
        {
            // 15 is 255 / 17 (1 extra led) to make it rotate
            led = CHSV(colour, (is_white_only) ? 0 : 255, current_brightness);
        }

        FastLED.show();

        if (current_brightness == 0)
        {
            colour += 10;
            fading_out = false;
        }
        else if (current_brightness == brightness)
        {
            fading_out = true;
        }

        if (fading_out)
        {
            --current_brightness;
        }
        else
        {
            ++current_brightness;
        }
    }
}

// Must be set to black first
void led_flash_and_fade(
    const short delay,
    const uint8_t brightness,
    const bool is_white_only,
    const bool is_one_at_a_time,
    const bool cycle_colours,
    const bool random_led)
{
    static uint8_t current_index(1);
    static uint8_t current_colour(0);
    static uint8_t current_brightness(brightness);

    if (has_delay_elapsed(delay))
    {
        leds[current_index] = CHSV(
            current_colour, (is_white_only) ? 0 : 255, --current_brightness);

        if (!is_one_at_a_time)
        {
            uint8_t second_index;

            // Need this to work with my dividers
            if (current_index == 0)
            {
                second_index = number_of_leds - 1;
            }
            else if (current_index % 2 == 1)
            {
                second_index = (current_index + 1) % number_of_leds;
            }
            else
            {
                second_index = current_index - 1;
            }

            leds[second_index] = CHSV(
                current_colour, (is_white_only) ? 0 : 255, current_brightness);
        }

        FastLED.show();
    }

    if (current_brightness == 0)
    {
        current_brightness = brightness;

        if (random_led)
        {
            current_index = random8(0, number_of_leds - 1);
        }
        else
        {
            if (is_one_at_a_time)
            {
                ++current_index %= number_of_leds;
            }
            else
            {
                current_index = (current_index + 2) % number_of_leds;
            }
        }

        if (cycle_colours)
        {
            current_colour += 10;
        }
        else
        {
            current_colour = random8();
        }
    }
}

void fade_all(const uint8_t nscale)
{
    leds.fadeToBlackBy(nscale);
}

void set_all_black()
{
    leds = CRGB::Black;
}

void set_black_except(const uint8_t index_1, const uint8_t index_2)
{
    for (uint8_t i = 0; i < number_of_leds; ++i)
    {
        if (i != index_1 && i != index_2)
        {
            leds[i] = CRGB::Black;
        }
    }
}

void loop()
{
    // cyon(90, 125, true, true);
    colour_rotation(10, 100, true);
    // multiple_spinning_leds(90, 125, false, false);
    // fade_in_and_out(15, 125, false);
    // led_flash_and_fade(5, 125, false, true, true, false);
}

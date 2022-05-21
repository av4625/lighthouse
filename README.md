# lighthouse

## Description
Going to learn a bit of wood turning. Decided to mix it with my electronics
hobby, so I will make a lighthouse that has a Neopixel ring with 16 pixels at
the top. I will use a Digispark microcontroller to keep the size small although
the available memory is very limited.

## To-Do
- [ ] Fade one in and then one out and on to the next (and for two at a time)
    - [ ] White only
    - [ ] Multi coloured

- [x] Cyon effect (and for two at a time)
    - [x] White only
    - [x] Multi coloured (Change colour for each light)
    - [ ] Multi coloured (Change colour for each loop)
    - [ ] Don't fade to black, have all LED's fade to "dim"

- [x] Two LED's spinning (with and without cyon)
    - [x] Two LED's the same colour and change colour together
    - [x] White only
    - [x] Two LED's different colours but add 16 (255 / num LED's) each time so
          the colours stay in the same place

- [x] Random light sharp on and fade out (and for two at a time)
    - [x] White only
    - [x] Multi coloured (Random colour and rainbow pattern)

- [x] Always on but colour rotates (and for two at a time)
    - [ ] Fade in and out at the same time

- [ ] All on but the faded one rotates (like a reverse cyon (and for two at a time))
    - [ ] White only
    - [ ] Multi coloured

- [x] Fade in and out (Don't think I ike it)

- [ ] Make some change direction?

## Functions
Some (all) of the animation functions are probably way more configurable than
they will ever need to be. The animation functions are designed to be
continually called.

None of the functions block.

- `setup`: Adds the LED's and sets the max brightness that can be used for the
number of LED's given a max milliamps.

- `has_delay_elapsed`: Returns `true` if the passed in `delay` has elapsed
since the last call else `false`.
    - `delay`: The delay to check in milliseconds.

- `cyon`: A circular pattern that looks like it leaves a tail as the tail
gradually fades out.
    - `delay`: The millisecond wait moving to the next LED.
    - `brightness`: The brightness for the leading LED's. Value between 0 - 255.
    - `is_white_only`: If true only the colour white will be used else the
    colour will change through a rainbow of colours.
    - `is_one_at_a_time`: If true there will be one leading LED else there will
    a leading group of two LED's.

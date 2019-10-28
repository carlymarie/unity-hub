#include "effects.h"
Effects::Effects()
{
    FastLED.addLeds<NEOPIXEL_TYPE, NEOPIXEL_PIN, NEOPIXEL_ORDER>(pixels, NEOPIXEL_COUNT);
    resetStrip();
}
void Effects::weatherAlert(uint8_t _count, byte alerts[MAX_WEATHER_ALERTS])
{
    solid(0, 0, 0);
    for (int i = 0; i < _count; i++)
    {
        byte alert = alerts[i];
        switch (alert)
        {
        case WEATHER_ADVISORY:
            pixels[9] = CRGB(255, 127, 0);
            break;
        case WEATHER_SPECIAL:
            pixels[13] = CRGB(255, 0, 255);
            break;
        case WEATHER_WATCH:
            pixels[15] = CRGB(255, 255, 0);
            break;
        case WEATHER_WARNING:
            pixels[11] = CRGB(255, 0, 0);
            break;
        }
    }
    FastLED.show();
}
void Effects::sparkle(uint8_t _r, uint8_t _g, uint8_t _b, bool _random, bool _fade, uint16_t _delay)
{
    unsigned long ms = millis();
    if (ms - _sparkle.last_ms > _delay)
    {
        if (_random)
        {
            _sparkle.led = random(NEOPIXEL_COUNT);
            pixels[_sparkle.led] = CRGB(random(255), random(255), random(255));
            FastLED.show();
            pixels[_sparkle.led] = CRGB::Black;
        }
        else if (_fade)
        {
            pixels[_sparkle.led] = CRGB(_sparkle.color.r, _sparkle.color.g, _sparkle.color.b);
            _sparkle.run(_r, _g, _b);
            FastLED.show();
        }
        else
        {
            _sparkle.led = random(NEOPIXEL_COUNT);
            pixels[_sparkle.led] = CRGB(_r, _g, _b);
            FastLED.show();
            pixels[_sparkle.led] = CRGB::Black;
        }
        _sparkle.last_ms = ms;
    }
}
void Effects::meteorRain(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _size, uint8_t _decay, bool _random, uint16_t _delay)
{
    unsigned long ms = millis();
    if (ms - _meteorRain.last_ms > _delay)
    {
        if (_meteorRain.i < NEOPIXEL_COUNT + NEOPIXEL_COUNT)
        {
            _meteorRain.i++;
        }
        else
        {
            _meteorRain.i = 0;
        }

        // fade brightness all LEDs one step
        for (int j = 0; j < NEOPIXEL_COUNT; j++)
        {
            if ((!_random) || (random(10) > 5))
            {
                fadeToBlack(j, _decay);
            }
        }

        // draw meteor
        for (int j = 0; j < _size; j++)
        {
            if ((_meteorRain.i - j < NEOPIXEL_COUNT) && (_meteorRain.i - j >= 0))
            {
                pixels[_meteorRain.i - j] = CRGB(_r, _g, _b);
            }
        }
        FastLED.show();
        _meteorRain.last_ms = ms;
    }
}
void Effects::bounce(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _balls)
{
    if (!_bounce.isSetup)
    {
        _bounce.setup(_balls);
    }
    for (int i = 0; i < _balls; i++)
    {
        _bounce.TimeSinceLastBounce[i] = millis() - _bounce.ClockTimeSinceLastBounce[i];
        _bounce.Height[i] = 0.5 * _bounce.Gravity * pow(_bounce.TimeSinceLastBounce[i] / 1000, 2.0) + _bounce.ImpactVelocity[i] * _bounce.TimeSinceLastBounce[i] / 1000;

        if (_bounce.Height[i] < 0)
        {
            _bounce.Height[i] = 0;
            _bounce.ImpactVelocity[i] = _bounce.Dampening[i] * _bounce.ImpactVelocity[i];
            _bounce.ClockTimeSinceLastBounce[i] = millis();

            if (_bounce.ImpactVelocity[i] < 0.01)
            {
                _bounce.ImpactVelocity[i] = _bounce.ImpactVelocityStart;
            }
        }
        _bounce.Position[i] = round(_bounce.Height[i] * (NEOPIXEL_COUNT - 1) / _bounce.StartHeight);
    }

    for (int i = 0; i < _balls; i++)
    {
        pixels[_bounce.Position[i]] = CRGB(_r, _g, _b);
    }
    FastLED.show();
    solid(0, 0, 0);
}
void Effects::fire(uint16_t _delay)
{

    unsigned long ms = millis();
    if (ms - _fire.last_ms > _delay)
    {

        // Array of temperature readings at each simulation cell
        static byte heat[NEOPIXEL_COUNT];

        // Step 1.  Cool down every cell a little
        for (int i = 0; i < NEOPIXEL_COUNT; i++)
        {
            heat[i] = qsub8(heat[i], random8(0, ((_fire.cooling * 10) / NEOPIXEL_COUNT) + 2));
        }

        // Step 2.  Heat from each cell drifts 'up' and diffuses a little
        for (int k = NEOPIXEL_COUNT - 1; k >= 2; k--)
        {
            heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
        }

        // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
        if (random8() < _fire.sparking)
        {
            int y = random8(7);
            heat[y] = qadd8(heat[y], random8(160, 255));
        }

        // Step 4.  Map from heat cells to LED colors
        for (int j = 0; j < NEOPIXEL_COUNT; j++)
        {
            CRGB color = HeatColor(heat[j]);
            int pixelnumber;
            if (_fire.toggle)
            {
                pixelnumber = (NEOPIXEL_COUNT - 1) - j;
            }
            else
            {
                pixelnumber = j;
            }
            pixels[pixelnumber] = color;
        }
        FastLED.show();
        _fire.last_ms = ms;
    }
}
void Effects::single(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _led)
{
    pixels[_led] = CRGB(_r, _g, _b);
    FastLED.show();
}
void Effects::walk(uint8_t _r, uint8_t _g, uint8_t _b, uint16_t _delay)
{
    unsigned long ms = millis();
    if (ms - _walk.last_ms > _delay)
    {
        if (_walk.pixel == NEOPIXEL_COUNT)
        {
            _walk.pixel = 0;
        }
        pixels[_walk.last] = CRGB::Black;
        pixels[_walk.pixel] = CRGB(_r, _g, _b);
        FastLED.show();
        _walk.run();
        _walk.last_ms = ms;
    }
}
void Effects::whirl(uint8_t _r, uint8_t _g, uint8_t _b, uint16_t _delay)
{
    unsigned long ms = millis();
    if (ms - _whirl.last_ms > _delay)
    {
        if (_whirl.pixel_1 == _whirl.pixel_2)
        {
            _whirl.pixel_2 = NEOPIXEL_COUNT / 2;
        }
        if (_whirl.pixel_1 == NEOPIXEL_COUNT)
        {
            _whirl.pixel_1 = 0;
            _whirl.pixel_2 = NEOPIXEL_COUNT / 2;
        }
        if (_whirl.pixel_2 == NEOPIXEL_COUNT)
        {
            _whirl.pixel_2 = 0;
        }
        pixels[_whirl.last_1] = CRGB::Black;
        pixels[_whirl.pixel_1] = CRGB(_r, _g, _b);
        pixels[_whirl.last_2] = CRGB::Black;
        pixels[_whirl.pixel_2] = CRGB(_r, _g, _b);
        FastLED.show();
        _whirl.last_1 = _whirl.pixel_1;
        _whirl.pixel_1++;
        _whirl.last_2 = _whirl.pixel_2;
        _whirl.pixel_2++;
        _whirl.last_ms = ms;
    }
}
void Effects::wipe(uint8_t _r, uint8_t _g, uint8_t _b, uint16_t _delay)
{
    unsigned long ms = millis();
    if (ms - _wipe.last_ms > _delay)
    {
        if (_wipe.pixel == NEOPIXEL_COUNT)
        {
            _wipe.pixel = 0;
            _wipe.toggle = !_wipe.toggle;
        }
        if (_wipe.toggle)
        {
            pixels[_wipe.pixel] = CRGB::Black;
        }
        else
        {
            pixels[_wipe.pixel] = CRGB(_r, _g, _b);
        }
        FastLED.show();
        _wipe.pixel++;
        _wipe.last_ms = ms;
    }
}
void Effects::ripple(uint16_t _delay)
{
    unsigned long ms = millis();
    if (ms - _ripple.last_ms > _delay)
    {
        for (uint16_t l = 0; l < NEOPIXEL_COUNT; l++)
        {
            pixels[l] = CHSV(_ripple.currentBg, 255, 50); // strip.setPixelColor(l, Wheel(currentBg, 0.1));
        }

        if (_ripple.step == 0)
        {
            pixels[_ripple.center] = CHSV(_ripple.color, 255, 255); // strip.setPixelColor(center, Wheel(color, 1));
            _ripple.step++;
        }
        else
        {
            if (_ripple.step < _ripple.maxSteps)
            {

                pixels[_ripple.wrap(_ripple.center + _ripple.step)] = CHSV(_ripple.color, 255, pow(_ripple.fadeRate, _ripple.step) * 255); //   strip.setPixelColor(wrap(center + step), Wheel(color, pow(fadeRate, step)));
                pixels[_ripple.wrap(_ripple.center - _ripple.step)] = CHSV(_ripple.color, 255, pow(_ripple.fadeRate, _ripple.step) * 255); //   strip.setPixelColor(wrap(center - step), Wheel(color, pow(fadeRate, step)));
                if (_ripple.step > 3)
                {
                    pixels[_ripple.wrap(_ripple.center + _ripple.step - 3)] = CHSV(_ripple.color, 255, pow(_ripple.fadeRate, _ripple.step - 2) * 255); //   strip.setPixelColor(wrap(center + step - 3), Wheel(color, pow(fadeRate, step - 2)));
                    pixels[_ripple.wrap(_ripple.center - _ripple.step + 3)] = CHSV(_ripple.color, 255, pow(_ripple.fadeRate, _ripple.step - 2) * 255); //   strip.setPixelColor(wrap(center - step + 3), Wheel(color, pow(fadeRate, step - 2)));
                }
                _ripple.step++;
            }
            else
            {
                _ripple.step = -1;
            }
        }
        _ripple.run();
        FastLED.show();
        _ripple.last_ms = ms;
    }
}
// TODO: Finish This
void Effects::whoosh(uint8_t _r, uint8_t _g, uint8_t _b, uint16_t _delay)
{
    unsigned long ms = millis();
    if (ms - _whoosh.last_ms > _delay)
    {
        if (_whoosh.pixel == NEOPIXEL_COUNT)
        {
            _whoosh.pixel = 0;
        }
        for (int i = 0; i < 3; i++)
        {
            pixels[_whoosh.tail[i]] = CRGB(_r / (4 * i), _g / (4 * i), _b / (4 * i));
        }
        pixels[_whoosh.last] = CRGB(_r / 4, _g / 4, _b / 4);
        pixels[_whoosh.pixel] = CRGB(_r, _g, _b);
        FastLED.show();
        _whoosh.run();
        _whoosh.last_ms = ms;
    }
}
void Effects::rainbowCycle(uint16_t _delay)
{
    _rainbowCycle.pixel++;
    if (_rainbowCycle.pixel > NEOPIXEL_COUNT)
    {
        _rainbowCycle.pixel = 0;
        _rainbowCycle.wheelColor += _delay;
        if (_rainbowCycle.wheelColor >= 256)
            _rainbowCycle.wheelColor = 0;
    }
    pixels[_rainbowCycle.pixel] = wheel(((_rainbowCycle.pixel * 256 / NEOPIXEL_COUNT) + _rainbowCycle.wheelColor) & 255);
    FastLED.show();
}
void Effects::blink(uint8_t _r, uint8_t _g, uint8_t _b, uint16_t _delay)
{
    unsigned long ms = millis();
    if (ms - _blink.last_ms > _delay)
    {
        for (int i = 0; i < NEOPIXEL_COUNT; i++)
        {
            if (_blink.toggle)
            {
                pixels[i] = CRGB(_r, _g, _b);
            }
            else
            {
                pixels[i] = CRGB::Black;
            }
        }
        FastLED.show();
        _blink.toggle = !_blink.toggle;
        _blink.last_ms = ms;
    }
}
void Effects::pulse(uint8_t _r, uint8_t _g, uint8_t _b, uint16_t _delay)
{
    unsigned long ms = millis();
    if (ms - _pulse.last_ms > _delay)
    {
        for (int i = 0; i < NEOPIXEL_COUNT; i++)
        {
            pixels[i] = CRGB(_pulse.color[0], _pulse.color[1], _pulse.color[2]);
        }
        FastLED.show();
        _pulse.run(_r, _g, _b);
        _pulse.last_ms = ms;
    }
}
void Effects::colorCycle(uint16_t _delay)
{
    unsigned long ms = millis();
    if (ms - _colorCycle.last_ms > _delay)
    {
        _colorCycle.pixel++;
        if (_colorCycle.pixel > NEOPIXEL_COUNT)
        {
            _colorCycle.pixel = 0;
            _colorCycle.wheelColor++;
            if (_colorCycle.wheelColor == 256)
                _colorCycle.wheelColor = 0;
        }
        for (int i = 0; i < NEOPIXEL_COUNT; i++)
        {
            pixels[i] = wheel(((_colorCycle.pixel * 256 / NEOPIXEL_COUNT) + _colorCycle.wheelColor) & 255);
        }
        FastLED.show();
        _colorCycle.last_ms = ms;
    }
}
void Effects::rainbowFade(uint16_t _delay)
{
    unsigned long ms = millis();
    if (ms - _rainbowFade.last_ms > _delay)
    {
        for (int i = 0; i < NEOPIXEL_COUNT; i++)
        {
            pixels[i] = CRGB(_rainbowFade.color[0], _rainbowFade.color[1], _rainbowFade.color[2]);
        }
        FastLED.show();
        _rainbowFade.run();
        _rainbowFade.last_ms = ms;
    }
}
void Effects::solid(uint8_t _r, uint8_t _g, uint8_t _b)
{
    for (int i = 0; i < NEOPIXEL_COUNT; i++)
    {
        pixels[i] = CRGB(_r, _g, _b);
    }
    FastLED.show();
}
void Effects::cylon(uint8_t _r, uint8_t _g, uint8_t _b, uint16_t _delay)
{
    unsigned long ms = millis();
    if (ms - _cylon.last_ms > _delay)
    {
        uint8_t half = NEOPIXEL_COUNT / 2;
        uint8_t l = half + _cylon.pixel;
        uint8_t r = half - _cylon.pixel;
        pixels[_cylon.last_pixel[0]] = CRGB::Black;
        pixels[_cylon.last_pixel[1]] = CRGB::Black;

        pixels[l] = CRGB(_r, _g, _b);
        pixels[r] = CRGB(_r, _g, _b);

        _cylon.last_pixel[0] = l;
        _cylon.last_pixel[1] = r;
        FastLED.show();
        _cylon.run();
        _cylon.last_ms = ms;
    }
}
CRGB Effects::wheel(byte WheelPos)
{
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85)
    {
        return CRGB(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if (WheelPos < 170)
    {
        WheelPos -= 85;
        return CRGB(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return CRGB(WheelPos * 3, 255 - WheelPos * 3, 0);
}
void Effects::resetStrip()
{
    queue.reset();
    _walk.reset();
    _wipe.reset();
    _pulse.reset();
    _blink.reset();
    _rainbowCycle.reset();
    _colorCycle.reset();
    _rainbowFade.reset();
    _whirl.reset();
    _cylon.reset();
    _whoosh.reset();
    _sparkle.reset();
    solid(0, 0, 0);
}
void Effects::progress(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _progress)
{
    uint8_t pixel = map(_progress, 0, 100, 0, NEOPIXEL_COUNT);
    pixels[pixel] = CRGB(_r, _g, _b);
    FastLED.show();
}
void Effects::fadeToBlack(uint8_t led, uint8_t fadeValue)
{
    pixels[led].fadeToBlackBy(fadeValue);
}
void Effects::run()
{
    switch (queue.effect)
    {
    case EFFECT_PULSE:
        pulse(queue.color.r, queue.color.g, queue.color.b, queue.delay);
        break;
    case EFFECT_BLINK:
        blink(queue.color.r, queue.color.g, queue.color.b, queue.delay);
        break;
    case EFFECT_RAINBOW_FADE:
        rainbowFade(queue.delay);
        break;
    case EFFECT_RAINBOW_CYCLE:
        rainbowCycle(queue.delay);
        break;
    case EFFECT_WIPE:
        wipe(queue.color.r, queue.color.g, queue.color.b, queue.delay);
        break;
    case EFFECT_WALK:
        walk(queue.color.r, queue.color.g, queue.color.b, queue.delay);
        break;
    case EFFECT_WHOOSH:
        whoosh(queue.color.r, queue.color.g, queue.color.b, queue.delay);
        break;
    case EFFECT_CYLON:
        cylon(queue.color.r, queue.color.g, queue.color.b, queue.delay);
        break;
    case EFFECT_WHIRL:
        whirl(queue.color.r, queue.color.g, queue.color.b, queue.delay);
        break;
    case EFFECT_SPARKLE:
        sparkle(queue.color.r, queue.color.g, queue.color.b, queue.random, queue.fade, queue.delay);
        break;
    case EFFECT_SOLID:
        solid(queue.color.r, queue.color.g, queue.color.b);
        queue.reset();
        break;
    case EFFECT_RIPPLE:
        ripple(queue.delay);
        break;
    case EFFECT_FIRE:
        fire(queue.delay);
        break;
    case EFFECT_BOUNCE:
        bounce(queue.color.r, queue.color.g, queue.color.b, queue.balls);
        break;
    case EFFECT_WEATHER_ALERT:
        weatherAlert(queue.size, queue.alerts);
        queue.reset();
        break;
    case EFFECT_METEOR_RAIN:
        meteorRain(queue.color.r, queue.color.g, queue.color.b, queue.size, queue.decay, queue.random, queue.delay);
        break;
    case EFFECT_COLOR_CYCLE:
        colorCycle(queue.delay);
        break;
    }
}

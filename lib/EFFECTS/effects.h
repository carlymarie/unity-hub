#ifndef EFFECTS_H_
#define EFFECTS_H_

#include <configuration.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

// Ring Animations
#define EFFECT_NONE 0x00
#define EFFECT_PULSE 0x01
#define EFFECT_BLINK 0x02
#define EFFECT_RAINBOW_FADE 0x03
#define EFFECT_RAINBOW_CYCLE 0x04
#define EFFECT_WIPE 0x05
#define EFFECT_WALK 0x06
#define EFFECT_WHOOSH 0x07
#define EFFECT_CYLON 0x08
#define EFFECT_WHIRL 0x09
#define EFFECT_SPARKLE 0x10
#define EFFECT_SOLID 0x11
#define EFFECT_RIPPLE 0x12
#define EFFECT_FIRE 0x13
#define EFFECT_BOUNCE 0x14
#define EFFECT_WEATHER_ALERT 0x15
#define EFFECT_METEOR_RAIN 0x16
#define EFFECT_COLOR_CYCLE 0x17

// Weather Alerts
#define WEATHER_NONE 0x00
#define WEATHER_ADVISORY 0x01
#define WEATHER_WATCH 0x02
#define WEATHER_WARNING 0x03
#define WEATHER_SPECIAL 0x04

class Effects
{
    struct Color
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };
    struct fx
    {
        byte effect;
        Color color;
        int16_t delay;
        uint8_t balls;
        uint8_t size;
        uint8_t decay;
        byte alerts[MAX_WEATHER_ALERTS];
        bool fade;
        bool random;
        void reset()
        {
            effect = EFFECT_NONE;
            color = {0, 0, 0};
            delay = 0;
            random = false;
            fade = false;
            balls = 0;
            size = 0;
            decay = 0;
            for (int i = 0; i < MAX_WEATHER_ALERTS; i++)
            {
                alerts[i] = 0x00;
            }
        }
    };
    struct fx_bounce
    {
        bool isSetup;
        float Gravity = -9.81;
        int StartHeight = 1;
        float Height[NEOPIXEL_COUNT];
        float ImpactVelocityStart = sqrt(-2 * Gravity * StartHeight);
        float ImpactVelocity[NEOPIXEL_COUNT];
        float TimeSinceLastBounce[NEOPIXEL_COUNT];
        int Position[NEOPIXEL_COUNT];
        unsigned long ClockTimeSinceLastBounce[NEOPIXEL_COUNT];
        float Dampening[NEOPIXEL_COUNT];
        void setup(uint8_t _balls)
        {
            for (int i = 0; i < _balls; i++)
            {
                ClockTimeSinceLastBounce[i] = millis();
                Height[i] = StartHeight;
                Position[i] = 0;
                ImpactVelocity[i] = ImpactVelocityStart;
                TimeSinceLastBounce[i] = 0;
                Dampening[i] = 0.90 - float(i) / pow(_balls, 2);
                isSetup = true;
            }
        }
    };
    struct fx_fire
    {
        uint8_t sparking = 120;
        uint8_t cooling = 55;
        bool toggle = false;
        unsigned long last_ms;
    };
    struct fx_meteorRain
    {
        int i = 0;
        unsigned long last_ms;
    };
    struct fx_sparkle
    {
        unsigned long last_ms;
        bool toggle;
        uint8_t led;
        Color color;
        uint8_t i;
        void reset()
        {
            last_ms = 0;
            toggle = false;
            led = 0;
            color = {0, 0, 0};
            i = 0;
        }
        void run(uint8_t _r, uint8_t _g, uint8_t _b)
        {
            if (i == 0)
            {
                led = random(NEOPIXEL_COUNT);
            }
            if (!toggle)
            {
                i += 15;
            }
            else
            {
                i -= 15;
            }
            if (i <= 0)
            {
                i = 0;
                toggle = !toggle;
            }
            else if (i >= 255)
            {
                i = 255;
                toggle = !toggle;
            }
            color.r = (i / 255.0) * _r;
            color.g = (i / 255.0) * _g;
            color.b = (i / 255.0) * _b;
        }
    };
    struct fx_walk
    {
        unsigned long last_ms;
        int pixel;
        int last;
        void run()
        {
            last = pixel;
            pixel++;
        }
        void reset()
        {
            pixel = 0;
            last = 0;
            last_ms = 0;
        }
    };
    struct fx_whirl
    {
        unsigned long last_ms;
        uint8_t pixel_1;
        uint8_t pixel_2;
        uint8_t last_1;
        uint8_t last_2;
        void reset()
        {
            pixel_1 = 0;
            last_1 = 0;
            pixel_2 = 0;
            last_2 = 0;
            last_ms = 0;
        }
    };
    struct fx_wipe
    {
        unsigned long last_ms;
        uint8_t pixel;
        bool toggle;
        void reset()
        {
            pixel = 0;
            toggle = 0;
            last_ms = 0;
        }
    };
    struct fx_rainbowCycle
    {
        uint8_t pixel;
        int8_t wheelColor;
        void reset()
        {
            pixel = 0;
            wheelColor = 0;
        }
    };
    struct fx_colorCycle
    {
        unsigned long last_ms;
        uint8_t pixel;
        uint8_t last;
        int8_t wheelColor;
        void reset()
        {
            pixel = 0;
            last = 0;
            last_ms = 0;
            wheelColor = 0;
        }
    };
    struct fx_rainbowFade
    {
        unsigned long last_ms;
        uint8_t last_color;
        uint8_t color[3];
        uint8_t color_index;
        bool toggle;
        void reset()
        {
            last_ms = 0;
            last_color = 0;
            color_index = 0;
            toggle = false;
            color[0] = 0;
            color[1] = 0;
            color[2] = 0;
        }
        void run()
        {
            if (color_index > 2)
            {
                color_index = 0;
            }
            if (!toggle)
            {
                color[color_index]++;
            }
            else
            {
                color[color_index]--;
            }
            if (color[color_index] == 255 || color[color_index] == 0)
            {
                toggle = !toggle;
            }
            if (color[color_index] == 0)
            {
                color_index++;
            }
        }
    };
    struct fx_blink
    {
        unsigned long last_ms;
        bool toggle;
        void reset()
        {
            last_ms = 0;
            toggle = false;
        }
    };
    struct fx_pulse
    {
        unsigned long last_ms;
        bool toggle;
        int color[3];
        uint8_t i;
        void run(uint8_t _r, uint8_t _g, uint8_t _b)
        {
            if (!toggle)
            {
                i++;
            }
            else
            {
                i--;
            }
            if (i <= 0)
            {
                i = 0;
                toggle = !toggle;
            }
            else if (i >= 255)
            {
                i = 255;
                toggle = !toggle;
            }
            color[0] = (i / 255.0) * _r;
            color[1] = (i / 255.0) * _g;
            color[2] = (i / 255.0) * _b;
        }
        void reset()
        {
            color[0] = 0;
            color[1] = 0;
            color[2] = 0;
            toggle = false;
        }
    };
    struct fx_ripple
    {
        unsigned long last_ms;
        int color;
        int center = 0;
        int step = -1;
        int maxSteps = 16;
        float fadeRate = 0.8;
        int diff;
        uint32_t currentBg = random(256);
        uint32_t nextBg = currentBg;
        void run()
        {
            if (currentBg == nextBg)
            {
                nextBg = random(256);
            }
            else if (nextBg > currentBg)
            {
                currentBg++;
            }
            else
            {
                currentBg--;
            }
            if (step == -1)
            {
                center = random(NEOPIXEL_COUNT);
                color = random(256);
                step = 0;
            }
        }
        int wrap(int step)
        {
            if (step < 0)
                return NEOPIXEL_COUNT + step;
            if (step > NEOPIXEL_COUNT - 1)
                return step - NEOPIXEL_COUNT;
            return step;
        }
    };

    // TODO: Finish This
    struct fx_whoosh
    {
        unsigned long last_ms;
        int pixel;
        int last;
        uint8_t tail[3];
        void run()
        {
            last = pixel;
            pixel++;
        }
        void reset()
        {
            pixel = 0;
            last = 0;
            last_ms = 0;
        }
    };
    struct fx_cylon
    {
        unsigned long last_ms;
        uint8_t last_pixel[2];
        uint8_t pixel;
        bool toggle;
        void run()
        {
            uint8_t half = NEOPIXEL_COUNT / 2;
            if (pixel == half || pixel == 0)
            {
                toggle = !toggle;
            }
            if (!toggle)
            {
                pixel--;
            }
            else
            {
                pixel++;
            }
        }
        void reset()
        {
            pixel = 0;
            last_ms = 0;
            toggle = false;
            last_pixel[0] = 0;
            last_pixel[1] = 0;
        }
    };

  public:
    fx queue;
    Effects();
    void single(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _led);
    void fire(uint16_t _delay);
    void blink(uint8_t _r, uint8_t _g, uint8_t _b, uint16_t _delay);
    void cylon(uint8_t _r, uint8_t _g, uint8_t _b, uint16_t _delay);
    void pulse(uint8_t _r, uint8_t _g, uint8_t _b, uint16_t _delay);
    void walk(uint8_t _r, uint8_t _g, uint8_t _b, uint16_t _delay);
    void whirl(uint8_t _r, uint8_t _g, uint8_t _b, uint16_t _delay);
    void wipe(uint8_t _r, uint8_t _g, uint8_t _b, uint16_t _delay);
    void whoosh(uint8_t _r, uint8_t _g, uint8_t _b, uint16_t _delay);
    void solid(uint8_t _r, uint8_t _g, uint8_t _b);
    void rainbowCycle(uint16_t _delay);
    void rainbowFade(uint16_t _delay);
    void colorCycle(uint16_t _delay);
    void ripple(uint16_t _delay);
    void bounce(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _balls);
    void sparkle(uint8_t _r, uint8_t _g, uint8_t _b, bool _random, bool _fade, uint16_t _delay);
    void meteorRain(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _size, uint8_t _decay, bool _random_decay, uint16_t _delay);
    void progress(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _progress);
    void weatherAlert(uint8_t _count, byte alerts[MAX_WEATHER_ALERTS]);
    void fadeToBlack(uint8_t led, uint8_t fadeValue);
    void resetStrip();
    void run();

  private:
    CRGB pixels[NEOPIXEL_COUNT];
    CRGB wheel(byte WheelPos);
    fx_bounce _bounce;
    fx_meteorRain _meteorRain;
    fx_walk _walk;
    fx_whirl _whirl;
    fx_wipe _wipe;
    fx_blink _blink;
    fx_ripple _ripple;
    fx_pulse _pulse;
    fx_fire _fire;
    fx_rainbowCycle _rainbowCycle;
    fx_rainbowFade _rainbowFade;
    fx_whoosh _whoosh;
    fx_cylon _cylon;
    fx_sparkle _sparkle;
    fx_colorCycle _colorCycle;
};

#endif // EFFECTS_H_

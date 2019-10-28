#include "device.h"
void DEVICE::setup()
{
// put your setup code here, to run once:
#ifdef DEBUG_ENABLED
    Serial.begin(SERIAL_BAUD);
    Serial.println();
#endif
    rf.setup();
    wifi.setup();
    mqtt->setup();
    mqtt->client.setCallback([this](char *_topic, uint8_t *_payload, unsigned int _length) {
        callback(_topic, _payload, _length);
    });
}
void DEVICE::callback(char *_topic, uint8_t *_payload, unsigned int _length)
{
    DynamicJsonDocument data(1024);
    deserializeJson(data, _payload);
    if (data.containsKey("state") && !data.containsKey("brightness") && !data.containsKey("color"))
    {
        if (data["state"] == "OFF")
        {
            effect->resetStrip();
        }
    }
    if (data.containsKey("color"))
    {
        effect->queue.color.r = data["color"]["r"];
        effect->queue.color.g = data["color"]["g"];
        effect->queue.color.b = data["color"]["b"];
    }
    if (data.containsKey("effect"))
    {
        if (data["effect"] == "Rainbow Cycle")
        {
            effect->queue.effect = EFFECT_RAINBOW_CYCLE;
            effect->queue.delay = 10;
        }
        if (data["effect"] == "Rainbow Fade")
        {
            effect->queue.effect = EFFECT_RAINBOW_FADE;
            effect->queue.delay = 0;
        }
        if (data["effect"] == "Pulse")
        {
            effect->queue.effect = EFFECT_PULSE;
            effect->queue.delay = 10;
        }
        if (data["effect"] == "Blink")
        {
            effect->queue.effect = EFFECT_BLINK;
            effect->queue.delay = 1000;
        }
        if (data["effect"] == "Wipe")
        {
            effect->queue.effect = EFFECT_WIPE;
            effect->queue.delay = 25;
        }
        if (data["effect"] == "Walk")
        {
            effect->queue.effect = EFFECT_WALK;
            effect->queue.delay = 20;
        }
        if (data["effect"] == "Cylon")
        {
            effect->queue.effect = EFFECT_CYLON;
            effect->queue.delay = 25;
        }
        if (data["effect"] == "Whirl")
        {
            effect->queue.effect = EFFECT_WHIRL;
            effect->queue.delay = 30;
        }
        if (data["effect"] == "Sparkle")
        {
            effect->queue.effect = EFFECT_SPARKLE;
            effect->queue.fade = true;
            effect->queue.random = false;
            effect->queue.delay = 5;
        }
        if (data["effect"] == "Solid")
        {
            effect->resetStrip();
        }
        if (data["effect"] == "Ripple")
        {
            effect->queue.effect = EFFECT_RIPPLE;
            effect->queue.delay = 25;
        }
        if (data["effect"] == "Fire")
        {
            effect->queue.effect = EFFECT_FIRE;
            effect->queue.delay = 25;
        }
        if (data["effect"] == "Bounce")
        {
            effect->queue.effect = EFFECT_BOUNCE;
            effect->queue.balls = 3;
        }
        if (data["effect"] == "Meteor Rain")
        {
            effect->queue.effect = EFFECT_METEOR_RAIN;
            effect->queue.size = 24;
            effect->queue.decay = 100;
            effect->queue.random = true;
            effect->queue.delay = 50;
        }
        if (data["effect"] == "Color Cycle")
        {
            effect->queue.effect = EFFECT_COLOR_CYCLE;
            effect->queue.delay = 50;
        }
    }
    if (data.containsKey("brightness"))
    {
        FastLED.setBrightness(data["brightness"]);
        FastLED.show();
    }
    String response;
    serializeJson(data, response);
    DEBUG_PRINT(F("[INFO] "), false);
    DEBUG_PRINT(response, true);
    
    if (!mqtt->client.publish(MQTT_STATE_TOPIC.c_str(), response.c_str()))
    {
        DEBUG_PRINT(F("[ERROR] Unable to publish to: "), false);
        DEBUG_PRINT(MQTT_STATE_TOPIC, true);
    }
}
void DEVICE::run()
{
    // put your main code here, to run repeatedly:
    if (wifi.run())
    {
        if (mqtt->run())
        {
            rf.run();
            effect->run();
        }
    }
#ifdef DEBUG_ENABLED
    dbg->run();
#endif
}
#include "ota.h"

void OTA::setup()
{
    ArduinoOTA.setPort(OTA_PORT);
    ArduinoOTA.setHostname(MQTT_DEVICE_NAME.c_str());
    ArduinoOTA.setPassword(OTA_PASS);
    ArduinoOTA.onStart([this]() {
        DEBUG_PRINT(F("Beginning OTA"), true);
    });
    ArduinoOTA.onEnd([this]() {
        DEBUG_PRINT(F("\nOTA Complete"), true);
    });
    ArduinoOTA.onProgress([this](unsigned int _progress, unsigned int _total) {
        DEBUG_PRINT(F("Uploading... "), true);
        DEBUG_PRINT(F("["), false);
        DEBUG_PRINT((_progress / (_total / 100)), false);
        DEBUG_PRINT(F("%"), false);
        DEBUG_PRINT(F("]"), true);
    });
    ArduinoOTA.onError([](ota_error_t _error) {
        switch (_error)
        {
        case OTA_AUTH_ERROR:
            DEBUG_PRINT(F("OTA Failed: Unable to authenticate"), true);
            break;
        case OTA_BEGIN_ERROR:
            DEBUG_PRINT(F("OTA Failed: Unable to start"), true);
            break;
        case OTA_CONNECT_ERROR:
            DEBUG_PRINT(F("OTA Failed: Unable to connect"), true);
            break;
        case OTA_RECEIVE_ERROR:
            DEBUG_PRINT(F("OTA Failed: Unable to receive"), true);
            break;
        case OTA_END_ERROR:
            DEBUG_PRINT(F("OTA Failed: Unable to end"), true);
            break;
        }
    });
    ArduinoOTA.begin();
}
void OTA::run()
{
    ArduinoOTA.handle();
}

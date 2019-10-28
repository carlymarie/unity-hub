#ifndef WIFI_H_
#define WIFI_H_
#include <Arduino.h>
#include <helpers.h>
#include <ESP8266WiFi.h>
#include <configuration.h>
#ifdef DEBUG_ENABLED
#include <ota.h>
#endif
class WIFI
{
public:
  void setup();
  bool run();

private:
  bool isBooting;
  uint8_t last_status;

#ifdef DEBUG_ENABLED
  OTA ota;
#endif
};
#endif // WIFI_H_
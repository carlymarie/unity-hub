#ifndef DEVICE_H_
#define DEVICE_H_
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <wifi.h>
#include <rf.h>
class DEVICE
{
public:
  void setup();
  void run();

private:
  RF rf;
  WIFI wifi;
  void callback(char *, uint8_t *, unsigned int);
};
#endif // DEVICE_H_
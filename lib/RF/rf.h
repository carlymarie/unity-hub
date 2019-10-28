#ifndef RF_H_
#define RF_H_
#include <RF24.h>
#include <pin_mapping.h>
#include <configuration.h>
#include <structs.h>
#include <externs.h>
#include <helpers.h>
#include <ArduinoJson.h>
#include <Hash.h>
class RF
{
public:
  RF();
  void setup();
  void run();

private:
  Packet payload;
  unsigned long last_broadcast;
  RF24 radio;
  bool configureDevice(uint8_t, bool);
  void willCheck(unsigned long);
  Sensor sensor[SENSORS_MAX];
};
#endif // RF_H_
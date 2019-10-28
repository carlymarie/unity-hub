#ifndef STRUCTS_H_
#define STRUCTS_H_
#include <Arduino.h>
struct __attribute((__packed__)) Packet
{
    uint8_t uuid = 0;          // Device Unique ID
    float temperature = 0;     // Temperature
    int8_t soil_moisture = 0;  // Soil Sensor
    uint8_t battery_level = 0; // Battery Level
};

struct Sensor
{
  public:
    String bssid;
    bool configured = false;
    bool connected = false;
    String state_topic;
    String json_attributes_topic;
    String battery_topic;
    String config_topic;
    String availability_topic;
    unsigned long last_message;
    Packet data;
};
#endif // STRUCTS_H_
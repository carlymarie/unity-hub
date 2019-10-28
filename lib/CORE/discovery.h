#ifndef DISCOVERY_H_
#define DISCOVERY_H_
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <configuration.h>

static const String uniqueID()
{
    String callback = WiFi.macAddress();
    callback.replace(":", "");
    return callback;
}

const String MQTT_DEVICE_NAME = uniqueID(); // MAC ADDRESS
const String MQTT_BASE = MQTT_DISCOVERY_PREFIX "/light/" + MQTT_DEVICE_NAME;
const String MQTT_AVAILABILITY_TOPIC = MQTT_BASE + "/status";
const String MQTT_CONFIG_TOPIC = MQTT_BASE + "/config";
const String MQTT_COMMAND_TOPIC = MQTT_BASE + "/command";
const String MQTT_STATE_TOPIC = MQTT_BASE + "/state";
#endif // DISCOVERY_H_
#include <Arduino.h>
#include <device.h>
#include <externs.h>
#ifdef DEBUG_ENABLED
#include <debugger.h>
#endif

// Setup global variables
#ifdef DEBUG_ENABLED
DEBUGGER *dbg = new DEBUGGER();
#endif

MQTT *mqtt = new MQTT();
Effects *effect = new Effects();
DEVICE device;

void setup()
{
  device.setup();
}

void loop()
{
  device.run();
}
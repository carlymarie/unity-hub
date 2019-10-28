#include "rf.h"
RF::RF() : radio(CE_PIN, CSN_PIN)
{
}
void RF::setup()
{
    radio.begin();
    radio.openReadingPipe(0, RF_PIPE);
    radio.setDataRate(RF_DATA_RATE);
    radio.setPALevel(RF_PA_LEVEL);
    radio.enableDynamicPayloads();
    radio.setChannel(RF_CHANNEL);
    radio.startListening();
}

bool RF::configureDevice(uint8_t _id, bool _soil)
{
    // TODO: Add config for soil sensor
    if (!sensor[_id].configured)
    {
        sensor[_id].bssid = sha1(String(_id)).substring(0, 12);
        String base = "HomeAssistant/sensor/" + sensor[_id].bssid;
        sensor[_id].state_topic = base + "/state";
        sensor[_id].json_attributes_topic = base + "/attributes";
        sensor[_id].configured = true;
        sensor[_id].connected = true;
    }
    else
    {
        return true;
    }
    return false;
}
void RF::run()
{
    unsigned long ms = millis();
    if (radio.available())
    {
        radio.read(&payload, sizeof(payload));
        uint8_t id = payload.uuid;
        float temp = payload.temperature;
        bool soil = payload.soil_moisture > -1;
        if (configureDevice(id, soil))
        {
            sensor[id].data = payload;
            DEBUG_PRINT(F("[INFO] RF Data Received("), false);
            DEBUG_PRINT(sensor[id].bssid, false);
            DEBUG_PRINT(F("): T: "), false);
            DEBUG_PRINT(temp, false);
            DEBUG_PRINT(F("F | B: "), false);
            DEBUG_PRINT(payload.battery_level, false);
            DEBUG_PRINT(F("%"), true);

            DynamicJsonDocument data(1024);
            data["temperature"] = temp;
            String json;
            serializeJson(data, json);
            if (mqtt->client.publish(sensor[id].state_topic.c_str(), json.c_str(), MQTT_RETAIN))
            {
                DynamicJsonDocument data(1024);
                data["battery_level"] = payload.battery_level;
                String json;
                serializeJson(data, json);

                mqtt->client.publish(sensor[id].json_attributes_topic.c_str(), json.c_str(), MQTT_RETAIN);

                DEBUG_PRINT(F("[OK] Published data to: "), false);
                DEBUG_PRINT(sensor[id].state_topic, true);
            }
            else
            {
                DEBUG_PRINT(F("[ERROR] Unable to publish to: "), false);
                DEBUG_PRINT(sensor[id].state_topic, true);
            }
            sensor[id].last_message = ms;
        }
    }
}
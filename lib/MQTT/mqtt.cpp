#include <mqtt.h>
MQTT::MQTT() : client(espClient)
{
}
void MQTT::setup()
{
    client.setServer(MQTT_HOST, MQTT_PORT);
}
bool MQTT::run()
{
    bool connected = client.connected();
    if (!connected)
    {
        if (client.connect(MQTT_DEVICE_NAME.c_str(), MQTT_USER, MQTT_PASS, MQTT_AVAILABILITY_TOPIC.c_str(), MQTT_QOS, MQTT_RETAIN, MQTT_WILL_OFFLINE, MQTT_CLEAN_SESSION))
        {
            DEBUG_PRINT(F("[OK] Connected to MQTT Server"), true);
            if (!client.publish(MQTT_AVAILABILITY_TOPIC.c_str(), MQTT_WILL_ONLINE, MQTT_RETAIN))
            {
                DEBUG_PRINT(F("[ERROR] Unable to publish to: "), false);
                DEBUG_PRINT(MQTT_AVAILABILITY_TOPIC, true);
            }
            DynamicJsonDocument data(1024);

            data["name"] = MQTT_DEVICE_NAME;
            data["schema"] = "json";
            data["state_topic"] = MQTT_STATE_TOPIC;
            data["command_topic"] = MQTT_COMMAND_TOPIC;
            data["availability_topic"] = MQTT_AVAILABILITY_TOPIC;
            data["payload_available"] = MQTT_WILL_ONLINE;
            data["payload_not_available"] = MQTT_WILL_OFFLINE;
            data["retain"] = MQTT_RETAIN;
            data["qos"] = MQTT_QOS;
            data["effect"] = true;
            JsonArray effects = data.createNestedArray("effect_list");
            effects.add("Pulse");
            effects.add("Blink");
            effects.add("Rainbow Fade");
            effects.add("Rainbow Cycle");
            effects.add("Wipe");
            effects.add("Walk");
            effects.add("Cylon");
            effects.add("Whirl");
            effects.add("Sparkle");
            effects.add("Solid");
            effects.add("Ripple");
            effects.add("Fire");
            effects.add("Bounce");
            effects.add("Meteor Rain");
            effects.add("Color Cycle");
            data["rgb"] = true;
            data["brightness"] = true;
            data["brightness_scale"] = 255;
            String config;
            serializeJson(data, config);
            if (client.publish(MQTT_CONFIG_TOPIC.c_str(), config.c_str(), MQTT_RETAIN))
            {
                DEBUG_PRINT(F("[OK] Published to: "), false);
                DEBUG_PRINT(MQTT_CONFIG_TOPIC, true);
            }
            else
            {
                DEBUG_PRINT(F("[ERROR] Unable to publish to: "), false);
                DEBUG_PRINT(MQTT_CONFIG_TOPIC, true);
            }
            if (client.subscribe(MQTT_COMMAND_TOPIC.c_str(), MQTT_QOS))
            {
                DEBUG_PRINT(F("[OK] Subscribed to: "), false);
                DEBUG_PRINT(MQTT_COMMAND_TOPIC, true);
            }
            else
            {
                DEBUG_PRINT(F("[ERROR] Unable to subscribe to: "), false);
                DEBUG_PRINT(MQTT_COMMAND_TOPIC, true);
            }
        }
        else
        {
            DEBUG_PRINT(F("[ERROR] Unable to connect to MQTT Server"), true);
        }
    }
    client.loop();
    return connected;
}
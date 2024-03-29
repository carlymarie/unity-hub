#include "wifi.h"
void WIFI::setup()
{
    delay(5000);
    #ifdef DEBUG_ENABLED
        dbg->setup();
    #endif
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    WiFi.setAutoReconnect(true);
}
bool WIFI::run()
{
    uint8_t status = WiFi.status();
    if (last_status != status)
    {
        switch (status)
        {
        case WL_CONNECT_FAILED:
            DEBUG_PRINT(F("[ERROR] WIFI Connection Failed"), true);
            break;
        case WL_CONNECTED:
            DEBUG_PRINT(F("[OK] WIFI Connected: "), false);
            DEBUG_PRINT(WiFi.localIP(), true);
#ifdef OTA_ENABLED
            ota.setup();
#endif
            break;
        case WL_DISCONNECTED:
            // Occurs when the router is unplugged
            DEBUG_PRINT(F("[ERROR] WIFI Disconnected"), true);
            if(!isBooting){
                isBooting = true;
            }else{
                WiFi.reconnect();
            }
            break;
        case WL_CONNECTION_LOST:
            // Occurs when device is out of range of the router
            DEBUG_PRINT(F("[ERROR] WIFI signal lost"), true);
            WiFi.reconnect();
            break;
        }
        last_status = status;
    }
#ifdef DEBUG_ENABLED
    ota.run();
#endif
    return status == WL_CONNECTED;
}
#include "wled.h"
/*
 * This v1 usermod file allows you to add own functionality to WLED more easily
 * See: https://github.com/Aircoookie/WLED/wiki/Add-own-functionality
 * EEPROM bytes 2750+ are reserved for your custom use case. (if you extend #define EEPSIZE in const.h)
 * If you just need 8 bytes, use 2551-2559 (you do not need to increase EEPSIZE)
 * 
 * Consider the v2 usermod API if you need a more advanced feature set!
 */

//Use userVar0 and userVar1 (API calls &U0=,&U1=, uint16_t)

#include <DHT.h>

#define humidity_topic "wled/shelf/hum"
#define temperature_topic "wled/shelf/temp"

#define DHTTYPE DHT22
#define DHTPIN  4

DHT dht(DHTPIN, DHTTYPE);

//gets called once at boot. Do all initialization that doesn't depend on network here
void userSetup()
{
  dht.begin();
}

//gets called every time WiFi is (re-)connected. Initialize own network interfaces here
void userConnected()
{
}

long lastMsg = 0;
float temp = 0.0;
float hum = 0.0;

//loop. You can use "if (WLED_CONNECTED)" to check for successful connection
void userLoop()
{
    if (mqtt != nullptr && mqtt->connected())
    {
        long now = millis();
        if (now - lastMsg > 300000) {
            lastMsg = now;

            float temp = dht.readTemperature();
            float hum = dht.readHumidity();

            mqtt->publish(temperature_topic, 0, true, String(temp).c_str());
            mqtt->publish(humidity_topic, 0, true, String(hum).c_str());
        }
    }
}

#include <WiFi_Client.h>
#include <ESP8266WiFi.h>

WiFi_Client::WiFi_Client(const char* ssid, const char *passphrase)
{
    this->ssid = ssid;
    this->passphrase = passphrase;
}

bool WiFi_Client::connect()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(this->ssid, this->passphrase);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.printf("Connected to WiFi, IP: %s", WiFi.localIP().toString().c_str());

    return true;
}

IPAddress WiFi_Client::localIP()
{
    return WiFi.localIP();
}

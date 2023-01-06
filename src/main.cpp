#if !defined(ESP8266)
  #error This code is intended to run only on the ESP8266 boards ! Please check your Tools->Board setting.
#endif

#include <Arduino.h>
#include <I2C.h>
// #include <IPAddress.h>

#define WIFI_SSID "BS-GUESTS"
#define WIFI_PASSWORD "mugawislup93"

// WiFi_Client wifi(WIFI_SSID, WIFI_PASSWORD);
I2C i2c;

// IPAddress serverIP(192,168,68,157);
// uint16_t  serverPort = 41221;

void setup() 
{
  Serial.begin(115200);
  while (!Serial);

  Serial.println("Starting...");
  Wire.begin(2, 14);

  i2c.attach(Wire);
  Serial.println("Inited I2C bus, starting scan...");
  i2c.scan();

  i2c.selectChannel(0);
  for (uint8_t ch = 0; ch < 4; ch++) { 
    i2c.selectChannel(ch);
    for (uint8_t i = 0; i < 8; i++) { 
      i2c.configure_port(i);
    }
  }

  for (uint8_t ch = 0; ch < 4; ch++) { 
    i2c.selectChannel(ch);
    for (uint8_t i = 0; i < 8; i++) { 
      i2c.led_set(i, 0, 1, 0);
    }
  }

  // wifi.connect();
  // SocketIO.connect();
}

void loop() 
{

}

// void not_in_use()
// {
//   Serial.print("WebSockets Client started @ IP address: ");
//   // Serial.println(wifi.localIP());

//   Serial.print("Connecting to WebSockets Server @ IP address: ");
//   Serial.print(serverIP);
//   Serial.print(", port: ");
//   Serial.println(serverPort);
// }

void handleEvent(char * payload, size_t length)
{
// #include <ArduinoJson.h>
  // Serial.printf("handleEvent: %s", payload);

  // StaticJsonDocument<200> doc;
  // DeserializationError error = deserializeJson(doc, payload);
  // if (error) {
  //   Serial.print(F("deserializeJson() failed: "));
  //   Serial.println(error.f_str());
  //   return;
  // }

  // // ["pick",{"led": "green"}]
  // String event = doc[0];
  // String led = doc[1]["led"];
}

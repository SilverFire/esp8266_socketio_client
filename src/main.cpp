#if !defined(ESP8266)
  #error This code is intended to run only on the ESP8266 boards ! Please check your Tools->Board setting.
#endif
#define _WEBSOCKETS_LOGLEVEL_ 1

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <WebSocketsClient_Generic.h>
#include <SocketIOclient_Generic.h>
#include <Hash.h>
#include <EventButton.h>

#define WIFI_SSID "Button Net"
#define WIFI_PASSWORD "12345678"

IPAddress serverIP(192,168,17,127);
uint16_t  serverPort = 41221;


#define PIN_LED_YELLOW  D0
#define PIN_LED_GREEN   D1
#define PIN_BUTTON      D2

SocketIOclient socketIO;
EventButton button(PIN_BUTTON);

void onButtonClicked(EventButton& eb) {
  DynamicJsonDocument doc(1024);
  JsonArray array = doc.to<JsonArray>();
  array.add("confirmed");
  String output;
  serializeJson(doc, output);
  socketIO.sendEVENT(output);
  
  digitalWrite(PIN_LED_YELLOW, LOW);
  digitalWrite(PIN_LED_GREEN, LOW);
}

void handleEvent(char * payload, size_t length)
{
  Serial.printf("handleEvent: %s", payload);

  // Deserialize the JSON document
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  // ["pick",{"led": "green"}]
  String event = doc[0];
  String led = doc[1]["led"];

  if (led == "green") {
    digitalWrite(PIN_LED_GREEN, HIGH);
  } else {
    digitalWrite(PIN_LED_YELLOW, HIGH);
  }
}

void socketIOEvent(const socketIOmessageType_t& type, uint8_t * payload, const size_t& length)
{
  switch (type) 
  {
    case sIOtype_DISCONNECT:
      Serial.println("[IOc] Disconnected");
      break;
    case sIOtype_CONNECT:
      Serial.print("[IOc] Connected to url: ");
      Serial.println((char*) payload);
      socketIO.send(sIOtype_CONNECT, "/");
      
      break;
    case sIOtype_EVENT:
      Serial.print("[IOc] Get event: ");
      Serial.println((char*) payload);
      handleEvent((char*) payload, length);
      
      break;
    case sIOtype_ACK:
      Serial.print("[IOc] Get ack: ");
      Serial.println(length);
      
      hexdump(payload, length);
      break;
    case sIOtype_ERROR:
      Serial.print("[IOc] Get error: ");
      Serial.println(length);
      
      hexdump(payload, length);
      break;
    case sIOtype_BINARY_EVENT:
      Serial.print("[IOc] Get binary: ");
      Serial.println(length);
      
      hexdump(payload, length);
      break;
    case sIOtype_BINARY_ACK:
       Serial.print("[IOc] Get binary ack: ");
      Serial.println(length);
      
      hexdump(payload, length);
      break;
      
    case sIOtype_PING:
      Serial.println("[IOc] Get PING");

      break;

   case sIOtype_PONG:
      Serial.println("[IOc] Get PONG");

      break;   
      
    default:
      break;
  }
}

void setup() 
{
  Serial.begin(115200);
  while (!Serial);

  pinMode(PIN_LED_YELLOW, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("Connected to WiFi, IP: %s", WiFi.localIP().toString().c_str());

  Serial.setDebugOutput(true);

  Serial.print("WebSockets Client started @ IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Connecting to WebSockets Server @ IP address: ");
  Serial.print(serverIP);
  Serial.print(", port: ");
  Serial.println(serverPort);

  socketIO.setReconnectInterval(10000);
  socketIO.begin(serverIP, serverPort);
  socketIO.onEvent(socketIOEvent);

  button.setPressedHandler(onButtonClicked);
}

void loop() 
{
  socketIO.loop();
  button.update();
}
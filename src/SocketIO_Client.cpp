#include <WebSocketsClient_Generic.h>
#include <SocketIOclient_Generic.h>
#include <IPAddress.h>
#define _WEBSOCKETS_LOGLEVEL_ 1

SocketIOclient socketIO;

void foo_setup(IPAddress serverIP, uint16_t serverPort)
{
  socketIO.setReconnectInterval(10000);
  socketIO.begin(serverIP, serverPort);
  // socketIO.onEvent(socketIOEvent);
}

void foo_loop()
{
    socketIO.loop();
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
      // TODO: implement
      // handleEvent((char*) payload, length);
      
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

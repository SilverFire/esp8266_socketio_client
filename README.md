# ESP8266 with Socket.IO client

## Prerequisites

1. [Platform.IO](https://docs.platformio.org/en/latest/) must be installed
2. [Node.JS](https://nodejs.org/) must be installed
3. ESP8266 module as WEMOS D1 mini module

## Flashing

```
platformio run --target upload
```

## Running NodeJS server

```
cd SocketIO\ Sample\ Server
npm install
node src/index.js
```
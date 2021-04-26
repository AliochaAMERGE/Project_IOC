## connection au wifi et installation des librairies

- installation de la libraire PubSub
> pio lib install 89

https://appcodelabs.com/introduction-to-iot-how-to-build-a-simple-mqtt-subscriber-using-esp8266

```cpp
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
```

NB : nécéssité de créer manuellement un fichier WifiConfig.h dans /esp32/include/WifiConfig.h
afin d'y renseigner ses parametre wifi:

```cpp
const char *SSID = "Your SSID";
const char *WiFiPassword = "Your Password";
```

Global Constants and Variables

The following global constants and variables should go at the top of the file.

You’ll need the following includes:-

```cpp
#include <WiFi.h> 
#include <PubSubClient.h>
```

Add a couple of macro definitions for convenience:-
```cpp
// use onboard LED for convenience 
#define LED (2)
// maximum received message length 
#define MAX_MSG_LEN (128)
```

Now you need the WiFi config:-
```cpp
// Wifi configuration
const char* ssid = "your wifi network name";
const char* password = "your wifi password";
```
And now the MQTT configuration:-
```cpp
// MQTT Configuration
// if you have a hostname set for the MQTT server, you can use it here
const char *serverHostname = "your MQTT server hostname";
// otherwise you can use an IP address like this
//const IPAddress serverIPAddress(192, 168, 0, 11);
// the topic we want to use
const char *topic = "test/message";
```
Create the MQTT and WiFi stacks:-
```cpp
WiFiClient espClient;
PubSubClient client(espClient);
```

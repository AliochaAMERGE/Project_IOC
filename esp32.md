## connection au wifi et installation des librairies

- installation de la libraire PubSub
> pio lib install 89

https://appcodelabs.com/introduction-to-iot-how-to-build-a-simple-mqtt-subscriber-using-esp8266

```cpp
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
```

NB : nécéssité de créer manuellement un fichier WifiConfig.h dans /esp32/include/WiFiCredentials.h
afin d'y renseigner ses parametre wifi:

```cpp
const char *SSID = "SSID";
const char *WiFiPassword = "passwd";
```
WiFi.status();
    0: WL_IDLE_STATUS when Wi-Fi is changing state
    1: WL_NO_SSID_AVAIL if the configured SSID cannot be reached
    3: WL_CONNECTED after connection is established
    4: WL_CONNECT_FAILED if the password is incorrect
    6: WL_DISCONNECTED if the module is not configured in station mode

- In `MQTTConfig.h`
topic = "esp32/output"
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include "WifiConfig.h"

// use onboard LED for convenience
#define LED (2)
// maximum received message length
#define MAX_MSG_LEN (128)

// *** MQTT Configuration
// if you have a hostname set for the MQTT server, you can use it here
// ! verifier

// todo : change
const char *mqttServer = "localhost";
// the topic we want to use
const int mqttPort = 12595;
const char *mqttUser = "alio";
const char *mqttPassword = "raspberry";

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char *msgTopic, byte *msgPayload, unsigned int msgLength) {
  // copy payload to a static string
  unsigned int length = msgLength;
  byte *payload = msgPayload;
  static char message[MAX_MSG_LEN + 1];
  if (length > MAX_MSG_LEN) {
    length = MAX_MSG_LEN;
  }
  strncpy(message, (char *)payload, length);
  message[length] = '\0';

  Serial.printf("topic %s, message received: %s\n", topic, message);

  // decode message
  if (strcmp(message, "off") == 0) {
    digitalWrite(LED, LOW);
  } else if (strcmp(message, "on") == 0) {
    digitalWrite(LED, HIGH);
  }
}

void ConnectToWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, WiFiPassword);
  Serial.print("Connecting to ");
  Serial.println(SSID);

  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);

    if ((++i % 16) == 0) {
      Serial.println(F(" still trying to connect"));
    }
  }

  Serial.print(F("Connected. My IP address is: "));
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqttUser, mqttPassword)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("test/message", "hello world");
      // ... and resubscribe
      // client.subscribe(MQTT_SERIAL_RECEIVER_CH);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }

  /*****************************
   ********** SETUP & LOOP *****
   * ***************************/

  void setup() {
    // LED pin as output
    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH);
    // Configure serial port for debugging
    Serial.begin(115200);
    // Initialise wifi connection - this will wait until connected
    ConnectToWiFi();
    // connect to MQTT server
    client.setServer(serverHostname, 1883);
    client.setCallback(callback);
  }

  void loop() {
    if (!client.connected()) {
      connectMQTT();
    }
    // this is ESSENTIAL!
    client.loop();
    // idle
    delay(500);
  }
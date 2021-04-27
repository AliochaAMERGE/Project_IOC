#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <WiFiCredentials.h>

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;

void callback(char* topic, byte* message, unsigned int length);

void connectMQTT(void) {
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(LED_BUILTIN, OUTPUT);
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // TODO : Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the
  // message is either "on" or "off". Changes the output state according to the
  // message
  if (String(topic) == "esp32/output") {
    Serial.print("Changing output to ");
    if (messageTemp == "on") {
      Serial.println("on");
      digitalWrite(LED_BUILTIN, HIGH);
    } else if (messageTemp == "off") {
      Serial.println("off");
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // ? may need to change
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
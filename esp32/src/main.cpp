#include <Arduino.h>

#include "MQTTConfig.h"
#include "WifiConfig.h"

// use onboard LED for convenience
#define PHOTORESISTANCE 36
// maximum received message length
#define MAX_MSG_LEN (128)

unsigned int lumos = 0;

/*****************************
 ********** SETUP & LOOP *****
 * ***************************/

void setup() {
  Serial.begin(9600);
  connectWifi();
  connectMQTT();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    lumos = analogRead(PHOTORESISTANCE);

    // Convert the value to a char array
    char tempString[8];
    dtostrf(lumos, 1, 2, tempString);
    Serial.print("value = ");
    Serial.println(tempString);

    client.publish("esp32/output", tempString);
  }
}
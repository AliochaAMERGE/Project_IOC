/* Arduino base */
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
/* librairie MQTT */
#include "MQTTConfig.h"
#include "WifiConfig.h"


// use onboard LED for convenience
#define PHOTORESISTANCE_PIN 36
// maximum received message length
#define MAX_MSG_LEN (128)

unsigned int lumos = 0;


/*****************************
 ********** SETUP & LOOP *****
 * ***************************/

void setup() {
  Serial.begin(9600);
  /****  Connection au WiFi  ****/
  connectWifi();
  /**** Connection au broker MQTT & subscribe aux topics désirés ****/
  connectMQTT();
}



void loop() {

  /* Reconnection au broker au besoin */
  if (!client.connected()) {
    reconnect();
  }
  /* loop pour la réception de message */
  client.loop();
  /* envoie d'un message toutes les 1 seconds */
  long now = millis();
  if (now - lastMsg > 1000) {
    lastMsg = now;
    lumos = analogRead(PHOTORESISTANCE_PIN);

    /* Convert the value to a char array */
    char tempString[8];
    dtostrf(lumos, 1, 2, tempString);
    Serial.print("value = ");
    Serial.println(tempString);

    client.publish("esp32/output", tempString);
  }
}
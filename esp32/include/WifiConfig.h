#include "WiFi.h"
#include "WiFiCredentials.h"

void connectWifi(void) {
  WiFi.begin(SSID, WiFiPassword);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println(WiFi.status());
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}
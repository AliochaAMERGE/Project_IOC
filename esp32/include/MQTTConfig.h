#include <Arduino.h>
#include <MQTTCredentials.h>
#include <PubSubClient.h>
#include <WiFi.h>
/* librarire Oled */
#include <Adafruit_GFX.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_SSD1306.h>

WiFiClient espClient;
PubSubClient client(espClient);

/********** POUR L'ECRAN **********/
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 32  // OLED display height, in pixels

#define OLED_RESET 16  // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS \
  0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

TwoWire twi = TwoWire(1);  // our own TwoWire instance on bus 1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &twi, OLED_RESET);

#define NUMFLAKES 10  // Number of snowflakes in the animation example

#define LOGO_HEIGHT 16
#define LOGO_WIDTH 16

static const unsigned char PROGMEM logo_bmp[] = {
    B00000000, B11000000, B00000001, B11000000, B00000001, B11000000, B00000011,
    B11100000, B11110011, B11100000, B11111110, B11111000, B01111110, B11111111,
    B00110011, B10011111, B00011111, B11111100, B00001101, B01110000, B00011011,
    B10100000, B00111111, B11100000, B00111111, B11110000, B01111100, B11110000,
    B01110000, B01110000, B00000000, B00110000};

/********** FIN POUR L'ECRAN **********/

long lastMsg = 0;
char msg[50];
int value = 0;

void callback(char* topic, byte* message, unsigned int length);

void connectMQTT(void) {
  // Connection au serveur
  client.setServer(mqtt_server, mqtt_port);
  // Gestion du callback
  client.setCallback(callback);
  // abonnnement au topic esp32/input
  client.subscribe("esp32/input");

  pinMode(LED_BUILTIN, OUTPUT);
  /****  amorçage de l'écran OLED  ****/
  twi.begin(4, 15);  // Needs to come before display.begin is used
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
  display.setTextSize(1);  // Draw 1 X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp = "";

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
  pinMode(LED_BUILTIN, OUTPUT);

  // Si nous recevons un message du topic esp32/input
  if (String(topic) == "esp32/input") {
    // si message est la commande "LedOn"
    if (String(messageTemp) == "LedOn") {
      // on allume la LED
      digitalWrite(LED_BUILTIN, HIGH);
    } else  // si message est la commande "LedOff"
        if (String(messageTemp) == "LedOff") {
      // on éteint la LED
      digitalWrite(LED_BUILTIN, LOW);
    } else {
      // on affiche le contenu du message sur l'écran Oled
      display.clearDisplay();        // efface le display
      display.setCursor(0, 0);     // place le curseur
      display.println(messageTemp);  // écrit le message dans le buffer
      display.display();             // affiche le contenu du buffer
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
      client.subscribe("esp32/input");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
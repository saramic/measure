/*
 *  Fetch the number of processed jobs from sidekiq stats and display 
 *  the number on a 7-segment display.
 */

#include "secrets.h"
#include <WiFi.h>

#include <HTTPClient.h>
#include <WiFiClientSecure.h>

#include <ArduinoJson.h>

//#define DEBUG // turn on debugging statements

#include "SPI.h" // SPI Serial Peripheral Interface

/*
 * pin out
 *   7-seg board  function  ESP func   VSPI pin  HSPI pin  Meaning
 *   DIN          MOSI      HSPI MOSI  GPIO23    GPIO13    Master Output Slave Input (data output from master)
 *   CS           SS        HSPI CS    GPIO5     GPIO15    Slave Select / Chip Select
 *   CLK          SCLK      HSPI CLK   GPIO18    GPIO14    Serial CLocK
 */
// Define the number of 7-seg digits
// in the display. Valid range [1..8]
const uint8_t NUM_DIGITS = 8;

// Define the SS pin
const uint8_t pinSS = 5; // GPIO5

// Opcodes for the MAX7221 and MAX7219
const uint16_t OP_NOOP = 0;
const uint16_t OP_DIGIT0 = 1;
// note: all OP_DIGITn are +n offsets from OP_DIGIT0
const uint16_t OP_DIGIT1 = 2;
const uint16_t OP_DIGIT2 = 3;
const uint16_t OP_DIGIT3 = 4;
const uint16_t OP_DIGIT4 = 5;
const uint16_t OP_DIGIT5 = 6;
const uint16_t OP_DIGIT6 = 7;
const uint16_t OP_DIGIT7 = 8;
const uint16_t OP_DECODEMODE = 9;
const uint16_t OP_INTENSITY = 10;
const uint16_t OP_SCANLIMIT = 11;
const uint16_t OP_SHUTDOWN = 12;
const uint16_t OP_DISPLAYTEST = 15;

void sendData(uint16_t cmd, uint8_t data)
// Send a simple command to the MAX7219
// using the hardware SPI interface
{
  uint16_t x = (cmd << 8) | data;
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
  digitalWrite(pinSS, LOW);
  SPI.transfer16(x);
  digitalWrite(pinSS, HIGH);
  SPI.endTransaction();
}

void update(uint32_t n)
// Work out the individual digits to
// be displayed and show them.
{
  for (uint8_t i = 0; i < NUM_DIGITS; i++)
  {
    uint8_t digit = n % 10;
    sendData(OP_DIGIT0 + i, digit);
    n /= 10;
  }
}

void setup()
{
  Serial.begin(115200);
  delay(10);

  // initialize the 7-segment display
  Serial.println("\n");
  Serial.print("Setting up 7-setment display... ");

  pinMode(pinSS, OUTPUT);
  SPI.begin();
  // Set the MAX7219 parameters
  sendData(OP_SHUTDOWN, 1);
  sendData(OP_DECODEMODE, 0xff);
  sendData(OP_SCANLIMIT, NUM_DIGITS-1);
  sendData(OP_INTENSITY, 3);

  // turn on all LED for short time
  sendData(OP_DISPLAYTEST, 1);
  delay(500);
  sendData(OP_DISPLAYTEST, 0);

  // initialize the display to 0
  update(0);
  Serial.println("done");

  // connect to a WiFi network
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("\n");
  Serial.print("Waiting for WiFi... ");

  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  WiFiClientSecure *client = new WiFiClientSecure;
  if(client) {
    client -> setCACert(AWS_CA_CERTIFICATE);

    if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
      HTTPClient https;

      https.begin(*client, SIDEKIQ_STATS_URL);
      https.addHeader("Cookie", SIDEKIQ_SESSION_COOKIE);
      int httpsCode = https.GET();
 
      if (httpsCode > 0) { //Check for the returning code
        String payload = https.getString();

#ifdef DEBUG
        Serial.println(httpsCode);
        Serial.println(payload);
#endif

        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, payload);
        if(error) {
          Serial.print("deserializeJson() failed: ");
          Serial.print(error.c_str());
          Serial.println(payload);
          return;
        }
        long processed = doc["sidekiq"]["processed"];
        Serial.println(processed);
        update(processed);
      } 
      else {
        Serial.println("Error on HTTP request");
      }
      https.end(); //Free the resources
    }
  delay(5000); // wait 5 seconds between next poll
  }
}

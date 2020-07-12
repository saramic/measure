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

void setup()
{
  Serial.begin(115200);
  delay(10);

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
      } 
      else {
        Serial.println("Error on HTTP request");
      }
      https.end(); //Free the resources
    }
  delay(5000); // wait 5 seconds between next poll
  }
}

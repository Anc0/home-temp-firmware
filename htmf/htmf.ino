#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <MQTT.h>
#include "config.h"

// Data wire is plugged into port D2 on Wemos
#define ONE_WIRE_BUS 4

// Setup a oneWire and Dallas instance
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float temperature;
char buf [16];

WiFiClient net;
MQTTClient client;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.print("\nconnecting...");
  while (!client.connect("test")) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nconnected!");
}

void setup(void)
{
  // Initialize serial communication
  Serial.begin(115200);
  // Start up the Dallas library
  sensors.begin();
  // Initialize wifi
  WiFi.begin(SSID, PASS);
  // Initialize mqtt client
  client.begin(MQTT_IP, MQTT_PORT, net);
  client.setOptions(90, true, 1000);
  // Connect to wifi and mqtt broker
  connect();
}

void loop(void)
{ 
  // Read the temperature from the sensor
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);  
  // Connect to the mqtt broker
  client.loop();
  delay(10);
  if (!client.connected()) {
    connect();
  }
  // Convert temperature to char*
  dtostrf(temperature, 5, 2, buf);
  Serial.println(temperature);
  // Publish the temperature
  client.publish("temperature/h41lkj23", buf);
  // Wait for n seconds TODO: change to deep sleep to conserve energy
  delay(60000);
}

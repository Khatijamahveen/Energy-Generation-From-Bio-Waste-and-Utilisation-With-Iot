/*
 * Project: Energy Generation from Bio-Waste and Utilisation with IoT
 * Module:   Appendix B - IoT Control (ESP8266 & SinricPro)
 * 
 * Authorship Disclaimer: 
 * This code is part of a collaborative project. The core design and 
 * primary development were led by Rafath Fatima.
 * I, Khatija Mahveen, acted as a Contributing Team Member. 
 * I do not claim sole ownership of this project.
 * 
 * SECURITY NOTE: Replace the placeholders below with your own credentials.
 * NEVER upload your real passwords, App Keys, or Device IDs to a public repository.
 */

#include <ESP8266WiFi.h>
#include <SinricPro.h>
#include <SinricProSwitch.h>

/* WiFi Credentials (Replace with your own) */
#define WIFI_SSID         "YOUR_WIFI_SSID"     
#define WIFI_PASS         "YOUR_WIFI_PASSWORD"

/* SinricPro Credentials (Replace with your own) */
#define APP_KEY           "YOUR_APP_KEY"      
#define APP_SECRET        "YOUR_APP_SECRET"  

/* Device IDs (Replace with your own) */
#define DEVICE_ID_1   "YOUR_DEVICE_ID_1"
#define DEVICE_ID_2   "YOUR_DEVICE_ID_2"

/* Relay Pins (D3 and D4 on NodeMCU/ESP8266) */
#define RELAY1 D3
#define RELAY2 D4

// Callback for Switch 1
bool onPowerState1(const String &deviceId, bool &state) {
  // Relay is Active LOW, so LOW = ON, HIGH = OFF
  digitalWrite(RELAY1, state ? LOW : HIGH);
  return true;
}

// Callback for Switch 2
bool onPowerState2(const String &deviceId, bool &state) {
  // Relay is Active LOW, so LOW = ON, HIGH = OFF
  digitalWrite(RELAY2, state ? LOW : HIGH);
  return true;
}

void setupWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void setupSinricPro() {
  // Register the switches with the SinricPro library
  SinricProSwitch &switch1 = SinricPro[DEVICE_ID_1];
  SinricProSwitch &switch2 = SinricPro[DEVICE_ID_2];

  // Attach the callback functions
  switch1.onPowerState(onPowerState1);
  switch2.onPowerState(onPowerState2);

  // Start the SinricPro connection
  SinricPro.begin(APP_KEY, APP_SECRET);
}

void setup() {
  // Initialize Relay Pins as Outputs
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);

  // Turn OFF relays initially (HIGH = OFF for Active LOW relays)
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);

  // Connect to Wi-Fi and Cloud
  setupWiFi();
  setupSinricPro();
}

void loop() {
  // Handle incoming commands from the mobile app
  SinricPro.handle();
}

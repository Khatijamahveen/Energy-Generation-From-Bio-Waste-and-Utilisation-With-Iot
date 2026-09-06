#include <Wire.h>
#include <Adafruit_INA219.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_INA219 ina219;


void setup(void) 
{
  Serial.begin(115200);
   lcd.begin();
 	lcd.backlight();
    
  Serial.println("Hello!");
    while (!Serial) 
  {
    delay(1);
  }
  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }
   ina219.setCalibration_32V_1A();
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();

 lcd.setCursor(4, 0); // set cursor to first row
  lcd.print("Energy"); // print out to LCD
  lcd.setCursor(0, 1); // set cursor to first row
  lcd.print(" generanion "); // print out to LCD
delay(3000);
lcd.clear();
}

void loop(void) 
{
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
  Serial.println("");
lcd.setCursor(0,0); 
  lcd.print("V:");
  lcd.print(loadvoltage,1 );
  //lcd.print("V");
  
  lcd.setCursor(7,0); 
  lcd.print("P:");
  lcd.print(power_mW );
  lcd.print("mW");
  lcd.setCursor(0,1);
  lcd.print("C:");
  lcd.print(current_mA);
  lcd.print("ma");
  delay(1000);
 lcd.clear();
  
}




#include <ESP8266WiFi.h>
#include <SinricPro.h>
#include <SinricProSwitch.h>

/* WiFi */
 #define WIFI_SSID         "ACTFIBERNET"     
 #define WIFI_PASS         "act12345"
 #define APP_KEY           "666b6a02-98f5-4c7a-b1ae-fa76ee6cefc8"      // Should look like "de0bxxxx-1x3x-4x3x-ax2x-5dabxxxxxxxx" 
 #define APP_SECRET        "257ebe67-0f15-4d43-ab44-bd0607216aef-b335b300-66e4-4932-87dd-091dd7f5d6c6" 

/* Device IDs */
#define DEVICE_ID_1   "69d29631dafb005af4e4ffbc"
#define DEVICE_ID_2   "69d29669dafb005af4e4ffe8"

/* Relay Pins */
#define RELAY1 D3
#define RELAY2 D4

bool onPowerState1(const String &deviceId, bool &state) {
  digitalWrite(RELAY1, state ? LOW : HIGH);
  return true;
}

bool onPowerState2(const String &deviceId, bool &state) {
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
  SinricProSwitch &switch1 = SinricPro[DEVICE_ID_1];
  SinricProSwitch &switch2 = SinricPro[DEVICE_ID_2];

  switch1.onPowerState(onPowerState1);
  switch2.onPowerState(onPowerState2);

  SinricPro.begin(APP_KEY, APP_SECRET);
}

void setup() {
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);

  digitalWrite(RELAY1, HIGH); // OFF initially
  digitalWrite(RELAY2, HIGH);

  setupWiFi();
  setupSinricPro();
}

void loop() {
  SinricPro.handle();
}

/*
 * Project: Energy Generation from Bio-Waste and Utilisation with IoT
 * Module:   Appendix A - Monitoring (INA219 Sensor & LCD Display)
 * 
 * Authorship Disclaimer: 
 * This code is part of a collaborative project. The core design and 
 * primary development were led by Rafath Fatima.
 * I, Khatija Mahveen, acted as a Contributing Team Member. 
 * I do not claim sole ownership of this project.
 */

#include <Wire.h>
#include <Adafruit_INA219.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Address for 16x2 LCD
Adafruit_INA219 ina219;

void setup(void) 
{
  Serial.begin(115200);
  lcd.begin();
  lcd.backlight();
  
  Serial.println("Initializing INA219...");

  // Initialize the INA219 (uses largest range by default: 32V, 2A)
  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }
  
  // Set calibration for higher precision on current (32V, 1A range)
  ina219.setCalibration_32V_1A();

  // Display startup message
  lcd.setCursor(4, 0);
  lcd.print("Energy");
  lcd.setCursor(0, 1);
  lcd.print("generation");
  delay(3000);
  lcd.clear();
}

void loop(void) 
{
  // 1. Read Sensor Values
  float shuntvoltage = ina219.getShuntVoltage_mV();
  float busvoltage   = ina219.getBusVoltage_V();
  float current_mA   = ina219.getCurrent_mA();
  float power_mW     = ina219.getPower_mW();
  
  // Calculate actual load voltage
  float loadvoltage = busvoltage + (shuntvoltage / 1000);

  // 2. Print to Serial Monitor for Debugging
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
  Serial.println("");

  // 3. Update LCD Display (Without clearing every loop to prevent flickering)
  lcd.setCursor(0, 0);
  lcd.print("V:");
  lcd.print(loadvoltage, 1);
  lcd.print("  P:");
  lcd.print(power_mW);
  lcd.print("mW   "); // Add spaces to overwrite old characters

  lcd.setCursor(0, 1);
  lcd.print("C:");
  lcd.print(current_mA);
  lcd.print("mA    "); 

  delay(1000); // Update every 1 second
}

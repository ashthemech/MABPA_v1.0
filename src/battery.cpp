/* 
 * File:   battery.cpp
 * Author: Ashton Coons
 * Brief: Myoware 2.0 muscle sensor source file for MAPBA system
 * Created on 2 19, 2025, 3:15pm
 * Modified on 2 19, 2025, 4:04pm
 */
#ifndef BATTERY
 /******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/
  #include <Arduino.h>
  #include "battery.h"

  #include <SPI.h>
  #include <ILI9341_t3.h>
  #include <font_DroidSans_Bold.h>
  #include <font_AwesomeF200.h>

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
  #define BATTERY_PIN     17
  #define BATTERY_TOGGLE  40 //pin to toggle battery measurement on/off
/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/

    const int numSamples = 200;  // size of moving average window
    float samples[numSamples] = {0};  // circular buffer for samples
    int sampleIndex = 0;
    int prevPercent = -1;

    float deadValue = 2.68;     //dead baterry voltage on ADC
    float fullValue = 3.05;   //full charge battery voltage on ADC
    float batteryDivisor = fullValue - deadValue;

    float lastPercent = 0.0;
    float lastDisplayPercent = 0.0;
    const float hysteresisThreshold = 0.5; // update percent only if change > 1%

    extern ILI9341_t3 tft;
/*******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES                                                *
 ******************************************************************************/
//filtering
/*******************************************************************************
 * PUBLIC FUNCTION IMPLEMENTATIONS                                             *
 ******************************************************************************/
bool batteryInit()
{
  Serial.begin(115200);
  pinMode(BATTERY_PIN, INPUT);
  analogReadResolution(10);
  pinMode(BATTERY_TOGGLE, OUTPUT);
  digitalWrite(BATTERY_TOGGLE, HIGH); //enable battery measurement
  delay(100); //wait for battery to stabilize

  // Initialize samples array with initial readings
  float initVoltage = analogRead(BATTERY_PIN) * (3.3f / 1023.0f);
  for (int i = 0; i < numSamples; i++) samples[i] = initVoltage;

  if (initVoltage <= deadValue || initVoltage >= fullValue) {
    return false;
  } else {
    return true;
  }
}

float readBatteryVoltage() {
    // Read and scale ADC
    int raw = analogRead(BATTERY_PIN);
    float voltage = raw * (3.3f / 1023.0f);  // Teensy 10-bit ADC

    // Add to samples circular buffer
    samples[sampleIndex] = voltage;
    sampleIndex = (sampleIndex + 1) % numSamples;

    // Compute moving average
    float sum = 0;
    for (int i = 0; i < numSamples; i++) {
        sum += samples[i];
    }
    float avgVoltage = sum / numSamples;

    return avgVoltage;
}

float getBatteryPercent(float voltage) {
    float percent = (voltage - deadValue) / batteryDivisor * 100.0f;
    percent = constrain(percent, 0, 100);

    // Clamp very low percentages to 0 to avoid flicker
    //if (percent < 2.0f) percent = 0.0f;

    // Hysteresis: update only if change is significant
    if (lastPercent < 0 || abs(percent - lastPercent) >= hysteresisThreshold) {
        lastPercent = percent;
    }

    return lastPercent;
}

void updateBatteryDisplay(ILI9341_t3 &tft) {
  if (prevPercent >= 80 && prevPercent <= 100) {
    tft.fillRect(10, 10, 150, 40, ILI9341_BLACK);
    tft.setCursor(10, 10);
    tft.setFont(AwesomeF200_20);
    tft.print((char)64);

    // Re-print only the updated percentage
    tft.setCursor(60, 10);
    tft.setFont(DroidSans_20_Bold);
    tft.print(prevPercent);
    tft.print("%");
  }
  if (prevPercent >= 60 && prevPercent <= 79) {
    tft.fillRect(10, 10, 150, 40, ILI9341_BLACK);
    tft.setCursor(10, 10);
    tft.setFont(AwesomeF200_20);
    tft.print((char)65);

    // Re-print only the updated percentage
    tft.setCursor(60, 10);
    tft.setFont(DroidSans_20_Bold);
    tft.print(prevPercent);
    tft.print("%");
  }

  if (prevPercent >= 40 && prevPercent <= 59) {
    tft.fillRect(10, 10, 150, 40, ILI9341_BLACK);
    tft.setCursor(10, 10);
    tft.setFont(AwesomeF200_20);
    tft.print((char)66);

    // Re-print only the updated percentage
    tft.setCursor(60, 10);
    tft.setFont(DroidSans_20_Bold);
    tft.print(prevPercent);
    tft.print("%");
  }

  if (prevPercent >= 20 && prevPercent <= 39) {
    tft.fillRect(10, 10, 150, 40, ILI9341_BLACK);
    tft.setCursor(10, 10);
    tft.setFont(AwesomeF200_20);
    tft.print((char)67);

    // Re-print only the updated percentage
    tft.setCursor(60, 10);
    tft.setFont(DroidSans_20_Bold);
    tft.print(prevPercent);
    tft.print("%");
  }

  if (prevPercent >= 0 && prevPercent <= 19) {
    tft.fillRect(10, 10, 150, 40, ILI9341_BLACK);
    tft.setCursor(10, 10);
    tft.setFont(AwesomeF200_20);
    tft.print((char)68);

    // Re-print only the updated percentage
    tft.setCursor(60, 10);
    tft.setFont(DroidSans_20_Bold);
    tft.print(prevPercent);
    tft.print("%");
  }
}

bool updateBatteryReading() {
    float voltage = readBatteryVoltage();
    float percent = (voltage - deadValue) / batteryDivisor * 100.0f;
    percent = constrain(percent, 0, 100);

    if (lastPercent < 0 || abs(percent - lastPercent) >= hysteresisThreshold) {
        lastPercent = percent;
        int newPercent = (int)percent;
        if (newPercent != prevPercent) {
            prevPercent = newPercent;
            return true;  // Need to update display
        }
    }
    return false; // No update needed
}

void updateBatterySamples() {
    readBatteryVoltage(); // Just updates the circular buffer
}

/*******************************************************************************
 * PRIVATE FUNCTION IMPLEMENTATIONS                                            *
 ******************************************************************************/

/*******************************************************************************
 * MAIN                                                                        *
 ******************************************************************************/
#endif
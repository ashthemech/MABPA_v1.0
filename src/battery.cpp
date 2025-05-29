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

/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
    static int batteryPin = 17;
    static int batteryTogglePin = 40; // pin to toggle battery measurement on/off

    const int numSamples = 20;  // size of moving average window
    float samples[numSamples] = {0};  // circular buffer for samples
    int sampleIndex = 0;
    int prevPercent = -1;

    float deadValue = 2.654;     // your dead battery voltage
    float fullValue = 3.008;   //3.002 on multimeter, 3.008 on ADC
    float batteryDivisor = fullValue - deadValue;

    float lastPercent = 0.0;
    const float hysteresisThreshold = 0.5; // update percent only if change > 1%

    extern ILI9341_t3 tft;
/*******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES                                                *
 ******************************************************************************/
//filtering
/*******************************************************************************
 * PUBLIC FUNCTION IMPLEMENTATIONS                                             *
 ******************************************************************************/
void batteryInit()
{
    Serial.begin(115200);
    pinMode(batteryPin, INPUT);
    pinMode(batteryTogglePin, OUTPUT);
    digitalWrite(batteryTogglePin, HIGH); //enable battery measurement

    // Initialize samples array with initial readings
    float initVoltage = analogRead(batteryPin) * (3.3f / 1023.0f);
    for (int i = 0; i < numSamples; i++) samples[i] = initVoltage;
}

float readBatteryVoltage() {
    // Read and scale ADC
    int raw = analogRead(batteryPin);
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
    if (abs(percent - lastPercent) >= hysteresisThreshold) {
        lastPercent = percent;
    }

    return lastPercent;
}

void updateBatteryDisplay(ILI9341_t3 &tft, int percent) {
  if (percent >= 80 && percent <= 100) {
    if (percent != prevPercent) {
      // Clear only the area where the percentage is displayed
      tft.fillRect(60, 10, 100, 30, ILI9341_BLACK);  // Adjust width/height as needed

      // Re-print only the updated percentage
      tft.setCursor(60, 10);
      tft.setFont(DroidSans_20_Bold);
      tft.print(percent);
      tft.print("%");

      // Draw the battery icon only once, or skip if unchanged
      if (prevPercent == -1) {
        tft.setCursor(10, 10);
        tft.setFont(AwesomeF200_20);
        tft.print((char)64);
      }

      prevPercent = percent;  // Update stored value
    }
  }
}
/*******************************************************************************
 * PRIVATE FUNCTION IMPLEMENTATIONS                                            *
 ******************************************************************************/

/*******************************************************************************
 * MAIN                                                                        *
 ******************************************************************************/
#endif
/* 
 * File:   myowareSensor.cpp
 * Author: Ashton Coons
 * Brief: Myoware 2.0 muscle sensor source file for MAPBA system
 * Created on 2 19, 2025, 3:15pm
 * Modified on 2 19, 2025, 4:04pm
 */
#ifndef PERIPHERAL_TEST
 /******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/
  #include <Arduino.h>
  #include <Servo.h> 

  #include "servoB.h"

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
//define the servo pin and battery pin
  #define SERVO_PIN    4
  #define BATTERY_PIN 17
/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/
  //define servo object for the brake servo (arduino class)
  Servo brakeServo;
/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
//define the servo positions for brake release and engage
  const int brakeReleasePos = 115;
  const int brakeEngagePos = 70;

/*******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES                                                *
 ******************************************************************************/

/*******************************************************************************
 * PUBLIC FUNCTION IMPLEMENTATIONS                                             *
 ******************************************************************************/
bool servoInit(){
    //set servo pin mode and attach the servo
    pinMode(SERVO_PIN, OUTPUT);
    brakeServo.attach(SERVO_PIN);
    //write servo to initial brake position
    brakeServo.write(brakeReleasePos);
    delay(300);  // Let servo settle

    //set up the battery pin sampling variables to determine if the servo movement causes a voltage dip
    const int numSamples = 10;
    const float thresholdDipVolts = 0.005f; // ~5mV dip
    const float ADC_SCALE = 3.3f / 1023.0f;
    int rawBefore = 0, rawDuring = 0;
    float vBefore = 0.0f, vDuring = 0.0f;

    // 1. Sample battery voltage before movement
    for (int i = 0; i < numSamples; i++) {
        rawBefore += analogRead(BATTERY_PIN);
        delayMicroseconds(500);
    }
    rawBefore /= numSamples;
    vBefore = rawBefore * ADC_SCALE;

    // 2. Trigger servo to engage brake
    brakeServo.write(brakeEngagePos);
    delay(200); // Let movement begin

    // 3. Sample battery voltage during movement
    for (int i = 0; i < numSamples; i++) {
        rawDuring += analogRead(BATTERY_PIN);
        delayMicroseconds(500);
    }
    rawDuring /= numSamples;
    vDuring = rawDuring * ADC_SCALE;

    // 4. Reset servo
    brakeServo.write(brakeReleasePos);
    //calculate the dip in voltage
    float dip = vBefore - vDuring;

    // 6. Return results based on dip
    if (dip > thresholdDipVolts) {
        return true;
    } else {
        return false;
    }
}

void servoBrake()
{
  brakeServo.write(brakeEngagePos);
}

void servoRelease()
{
  brakeServo.write(brakeReleasePos);
}

/*******************************************************************************
 * PRIVATE FUNCTION IMPLEMENTATIONS                                            *
 ******************************************************************************/

/*******************************************************************************
 * MAIN                                                                        *
 ******************************************************************************/
#endif
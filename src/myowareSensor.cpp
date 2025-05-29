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
  #include "myowareSensor.h"

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
    static int analogPinSensor = 14; //myoware pin number
    const float divisor = 310.303; //get V from the ADC value

    static volatile int sensorVal = 0;
    static volatile bool sampleReady = false;



    float voltage = 0.00;
    unsigned long currTime = 0;
    unsigned long prevTime = 0;
    const long interval = 500; //us interval (2000Hz)

    float filteredVal = 0;
    const float alpha = 0.3;  // between 0.01 (very smooth) and 0.3 (fast reacting)

    //                 sensorVal = analogRead(analogPinSensor);
    //             voltage = (float)sensorVal / divisor;
    //             Serial.print(">");
    //             Serial.print("sEMG1:");
    //             Serial.print(voltage, 2);
    //             Serial.println();
    //             delay(100);
/*******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES                                                *
 ******************************************************************************/
//filtering
/*******************************************************************************
 * PUBLIC FUNCTION IMPLEMENTATIONS                                             *
 ******************************************************************************/
bool muscleSensorInit()
{
    //initialization code for sensor
    pinMode(analogPinSensor, INPUT);
    analogReadResolution(10);

    //sensor initialization check
    const int numSamples = 1000;
    int minThreshold = 900;
    int maxThreshold = 20;

    const int tolerance = 10;

    //for the number of samples, read the sensor value and add it to the total
    for (int i = 0; i < numSamples; i++) {
        sensorVal = analogRead(analogPinSensor);
        if(minThreshold > sensorVal){
            minThreshold = sensorVal;
        }
        if(maxThreshold < sensorVal){
            maxThreshold = sensorVal;
        }
        delay(10);
    }

    //and not disconnected (1.65) -> sensor is working
    bool notDisconnected = fabs(maxThreshold-minThreshold) > tolerance; 

    return (notDisconnected);
}

void readMuscleSensor(){
    sampleReady = true;
}

int getSensorVal(){
    sensorVal = analogRead(analogPinSensor);
    filteredVal = alpha * sensorVal + (1 - alpha) * filteredVal;
    return filteredVal;
}

bool isSampleReady() {
    return sampleReady;
}

void clearSampleFlag() {
    sampleReady = false;
}
/*******************************************************************************
 * PRIVATE FUNCTION IMPLEMENTATIONS                                            *
 ******************************************************************************/

/*******************************************************************************
 * MAIN                                                                        *
 ******************************************************************************/
#endif
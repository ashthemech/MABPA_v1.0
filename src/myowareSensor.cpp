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
  //interrupt timer for muscle sensor sampling
  IntervalTimer muscleTimer;
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
    const long interval = 666; //us interval (1500Hz)

    float filteredVal = 0;
    const float alpha = 0.2;  // between 0.01 (very smooth) and 0.3 (fast reacting)

    const int SAMPLE_COUNT = 1500 * 15;  // 22500 samples
    uint16_t emgData[SAMPLE_COUNT];
    int dataIndex = 0;

/*******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES                                                *
 ******************************************************************************/

/*******************************************************************************
 * PUBLIC FUNCTION IMPLEMENTATIONS                                             *
 ******************************************************************************/
void muscleSensorInit()
{
    //initialization code for sensor
    pinMode(analogPinSensor, INPUT);
    analogReadResolution(10);
    muscleTimer.begin(readMuscleSensor, interval);
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

void collectSamples(int flexVal)
{
    if (dataIndex < SAMPLE_COUNT) {
        emgData[dataIndex] = flexVal;
        dataIndex++; 
    }
}

void resetEMGData() {
    dataIndex = 0;
    memset(emgData, 0, sizeof(emgData));  // optional: zero out old data
}

int findMaxEMG()
{
    uint16_t maxEMG = 0;
    for (int i = 0; i < dataIndex; i++) {
        uint16_t val = emgData[i];
        if (val > maxEMG) maxEMG = val;
    }
    return maxEMG;
}

int findMinEMG()
{
    uint16_t minEMG = 1023;
    for (int i = 0; i < dataIndex; i++) {
        uint16_t val = emgData[i];
        if (val < minEMG) minEMG = val;
    }
    return minEMG;
}

/*******************************************************************************
 * PRIVATE FUNCTION IMPLEMENTATIONS                                            *
 ******************************************************************************/

/*******************************************************************************
 * MAIN                                                                        *
 ******************************************************************************/
#endif
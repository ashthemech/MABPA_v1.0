/* 
 * File:   myowareSensor.cpp
 * Author: Ashton Coons
 * Brief: Myoware 2.0 muscle sensor source file for MAPBA system
 * Created on 2 19, 2025, 3:15pm
 * Modified on 2 19, 2025, 4:04pm
 */

 /******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/
  #include <Arduino.h>
  #include <Servo.h>

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/

    // pinMode(analogPinSensor, INPUT);
    // analogReadResolution(10);
    // Serial.begin(115200);

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
    int analogPinSensor = 14; //myoware pin number
    int sensorVal = 0;
    float divisor = 310.303; //get V from the ADC value
    float voltage = 0.00;
    unsigned long currTime = 0;
    unsigned long prevTime = 0;
    const long interval = 500; //us interval (2000Hz)
}
/*******************************************************************************
 * PRIVATE FUNCTION IMPLEMENTATIONS                                            *
 ******************************************************************************/

/*******************************************************************************
 * MAIN                                                                        *
 ******************************************************************************/
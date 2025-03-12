/* 
 * File:   peripheralTest.cpp
 * Author: Ashton Coons
 * Brief: Source for test harness for the required MABPA project peripherals on the Teensy 4.1
 * Created on 02.20.2025, 1:53pm
 * Modified on 02.20.2025, 2:04pm
 */

/*******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/
    #include <Arduino.h>   
    #include "peripheralTest.h" 

 /*******************************************************************************
  * PRIVATE #DEFINES                                                            *
  ******************************************************************************/
 
 
 /*******************************************************************************
  * PRIVATE TYPEDEFS                                                            *
  ******************************************************************************/
    #ifdef SLOW_BLINK_TEST
        int ledPin = 13;
    #endif

    #ifdef ANALOG_READ_TEST
        int analogPin = A0;
        int val = 0;
    #endif

    #ifdef EMG_READ_TEST
        int analogPinSensor = A0;
        int sensorVal = 0;
    #endif
 /*******************************************************************************
  * PRIVATE FUNCTIONS PROTOTYPES                                                *
  ******************************************************************************/

 /*******************************************************************************
  * PUBLIC FUNCTION IMPLEMENTATIONS                                             *
  ******************************************************************************/

 /*******************************************************************************
  * PRIVATE FUNCTION IMPLEMENTATIONS                                            *
  ******************************************************************************/
 
  /*******************************************************************************
   * MAIN                                                                        *
   ******************************************************************************/
  #ifdef PERIPHERAL_TEST
    void setup(){
        #ifdef SLOW_BLINK_TEST
            pinMode(ledPin, OUTPUT);
        #endif

        #ifdef ANALOG_READ_TEST
            pinMode(analogPin, INPUT);
            analogReadResolution(10);
            Serial.begin(38400);
        #endif

        #ifdef EMG_READ_TEST
            pinMode(analogPinSensor, INPUT);
            analogReadResolution(10);
            Serial.begin(115200);
        #endif
    }

    void loop() {
        #ifdef SLOW_BLINK_TEST
            digitalWrite(ledPin, HIGH);
            delay(1000);
            digitalWrite(ledPin, LOW);
            delay(1000);
        #endif

        #ifdef ANALOG_READ_TEST
            val = analogRead(analogPin);
            Serial.println(val);
            delay(250);
        #endif

        #ifdef EMG_READ_TEST
            sensorVal = analogRead(analogPinSensor);
            Serial.println(sensorVal);
            delay(1000); //delay 1 second so we can see flex and no flex values
        #endif
    }
  #endif
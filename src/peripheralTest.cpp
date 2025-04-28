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
    #include <Servo.h>   
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

    #ifdef SERVO_TEST
        Servo brakeServo;
        int servoPin = 4;
        int servoPos = 0;
    #endif

    #ifdef EMG_GRAPH_TEST
        int analogPinSensor = A0;
        int sensorVal = 0;
        float divisor = 310.303; //get V from the ADC value
        float voltage = 0.00;
    #endif

    #ifdef EMG_EXTRACT_DATA
        int analogPinSensor = A0;
        int sensorVal = 0;
        unsigned long currentTime = 0;
        unsigned long previousTime = 0;
        const long interval = 1; //1ms interval (1000Hz)
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

        #ifdef SERVO_TEST
            pinMode(servoPin, OUTPUT);
            brakeServo.attach(servoPin);
            brakeServo.write(0); //set servo to 0 degrees
            delay(1000); //wait for servo to move
        #endif

        #ifdef EMG_GRAPH_TEST
            pinMode(analogPinSensor, INPUT);
            analogReadResolution(10);
            Serial.begin(115200);
        #endif

        #ifdef EMG_EXTRACT_DATA
            pinMode(analogPinSensor, INPUT);
            analogReadResolution(10);
            Serial.begin(115200);
            currentTime = 0;
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

        #ifdef SERVO_TEST
            for(servoPos = 10; servoPos < 170; servoPos += 1){
                brakeServo.write(servoPos);              
                delay(15);                       
            } 
            for(servoPos = 180; servoPos>=1; servoPos-=1){                                
                brakeServo.write(servoPos);              
                delay(15);                       
            }
        #endif

        #ifdef EMG_GRAPH_TEST
            sensorVal = analogRead(analogPinSensor);
            voltage = (float)sensorVal / divisor;
            Serial.print(">");
            Serial.print("sEMG1:");
            Serial.print(voltage, 2);
            Serial.println();
            delay(100);
        #endif

        #ifdef EMG_EXTRACT_DATA
            currentTime = millis();
            
            if (currentTime - previousTime >= interval) {
                previousTime = currentTime;
                sensorVal = analogRead(analogPinSensor);
                //voltage = (float)sensorVal / divisor;

                Serial.print(currentTime);
                Serial.print(",");
                //Serial.print(voltage, 2);
                Serial.println();
            }
        #endif
    }
  #endif
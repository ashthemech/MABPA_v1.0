/* 
 * File:   main.cpp
 * Author: Ashton Coons
 * Brief: Main source file for MABPA System
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
  int bicepSensor = A0; //Bicep flex sensor pin
  int flexValue = 0; //Bicep flex sensor value, intialized to zero
  
  //sampling rate set up
    unsigned long currentTime = 0;
    unsigned long previousTime = 0;
    const long interval = 1; //1ms interval (1000Hz)

  Servo brakeServo; //set up servo object for the brake servo
  int brakePin = 4; //brake servo pin
  int brakePos = 0; //intialize position to zero

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
#ifndef PERIPHERAL_TEST
  void setup(){
    /* Myoware 2.0 Mucsle Sensor Setup */

    
    pinMode(bicepSensor, INPUT); //set the bicep flex sensor pin as input
    analogReadResolution(10); //set the analog read resolution to 10 bits (0-1023)
    Serial.begin(115200); //set the serial baud rate to 115200 for communication
    Serial.println("Teensy 4.1: Bicep Flex Sensor Test Initialized");

    pinMode(brakePin, OUTPUT); //set the brake servo pin as output
    brakeServo.attach(brakePin); //attach the servo to the pin
    brakeServo.write(0); //set servo to 0 degrees
    delay(1000); //wait for 1 second to allow servo to initialize
  }

  void loop() {
    currentTime = millis(); //get the current time in milliseconds

    if (currentTime - previousTime >= interval) { //check if the interval has passed
        previousTime = currentTime; //update the previous time
        flexValue = analogRead(bicepSensor); //read the bicep flex sensor value
        
        //Control servo based on flex value
        if (flexValue > 512) { //if flex value is greater than 512, move servo to 90 degrees
            brakePos = 90;
        } else { //otherwise, move servo to 0 degrees
            brakePos = 0;
        }
        
        brakeServo.write(brakePos); //write the position to the servo
    }
  }
#endif

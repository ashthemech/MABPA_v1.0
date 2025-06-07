/* 
 * File:   main.cpp
 * Author: Ashton Coons
 * Brief: Main source file for MABPA System
 * Created on 2 19, 2025, 3:15pm
 * Modified on 2 19, 2025, 4:04pm
 */
#ifndef PERIPHERAL_TEST
 /******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/
  #include <Arduino.h>
  #include <imxrt.h>
  #include <IntervalTimer.h>

  #include "tftScreen.h"
  #include "myowareSensor.h"
  #include "battery.h"
  #include "servoB.h"

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/

  //enum for the braking state machine
  enum BrakeState {
  RELEASE_BRAKE, 
  ENGAGE_BRAKE};
  BrakeState brakeState = RELEASE_BRAKE;

  // //enum for the system state machine (initialization and error)
  // enum SystemState {INIITIALIZING, READY, ERROR};
  // SystemState currentState = INIITIALIZING;

  enum StartupState {
  DRAW_LOGO,
  INIT_MUSCLE,
  INIT_SERVO,
  INIT_BATTERY,
  SYSTEM_READY,
  SYSTEM_ERROR
};
  StartupState StartUpState = DRAW_LOGO;

enum EMGState {
  RUN_PLACEMENT_PROMPT,
  RUN_TIMER_PROMPT,
  COLLECT_EMG_DATA,
  EMG_DATA_ERROR,
  EMG_DATA_VALID
};
  EMGState collectionState = RUN_PLACEMENT_PROMPT;
/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
//avoid multiple prints
  bool promptRan = false;
  bool collecting = false;

  //hysteresis bounds for a flex
  int flexValue = 0;
  const int engageThreshold = 250; //400 prev
  const int relaxThreshold  = 100; //350 prev

  int maxEMG = 0;
  int minEMG = 0;
  int validEMGData = 200;

  unsigned long lastBatteryRead = 0;      // stores the last time battery was read, initialized to 0
  const unsigned long batteryInterval = 2000;  //2 seconds

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
  void setup(){
    // //ensure 10 bit resolution
    analogReadResolution(10);
    screenInit();
    collectionState = RUN_PLACEMENT_PROMPT;
    StartUpState = DRAW_LOGO;
    brakeState = RELEASE_BRAKE;
  }

  void loop() {
    switch(StartUpState){
      case DRAW_LOGO:
      //function to draw the teams logo on the screen
        drawLogo();
        //testFont();
        StartUpState = INIT_BATTERY;
      break; //DRAW_LOGO

      case INIT_BATTERY:
        //initalize the battery level reading system
        if (batteryInit())
        {
          updateBatteryReading();
          StartUpState = INIT_SERVO;
        }
        else if(!batteryInit())
        {
          batteryFail();
          StartUpState = SYSTEM_ERROR;
        }
      break; //INIT_BATTERY

      case SYSTEM_ERROR:
        ; //display failure message, brick system
      break; //SYSTEM_ERROR

      case INIT_SERVO:
        if(servoInit())
        {
          StartUpState = INIT_MUSCLE;
        }else if(!servoInit())
        {
          servoFail();
          StartUpState = SYSTEM_ERROR;
        }
      break; //INIT_SERVO

      case INIT_MUSCLE:
        switch(collectionState)
        {
          case RUN_PLACEMENT_PROMPT:
            if(!promptRan){
              muscleSensorInit();
              musclePlacementPrompt();
              promptRan = true;
            }
            else if(userReady()){
              promptRan = false;
              collectionState = RUN_TIMER_PROMPT;
            }
          break;

          case RUN_TIMER_PROMPT:
            if(!promptRan)
            {
              muscleFlexPrompt();
              promptRan = true;
            }
            else if(userReady())
            {
              promptRan = false;
              startFlexTimer();
              resetEMGData();
              collectionState = COLLECT_EMG_DATA;
            }
          break;

          case COLLECT_EMG_DATA:
            if(updateFlexTimer()){
              if (isSampleReady()) {
                clearSampleFlag();
                flexValue = getSensorVal();
                collectSamples(flexValue);
              }
            }
            else{
              minEMG = findMinEMG();
              maxEMG = findMaxEMG();
              Serial.println(minEMG);
              Serial.println(maxEMG);

              if(maxEMG - minEMG <= validEMGData)
              {
                collectionState = EMG_DATA_ERROR;
              }
              else if(maxEMG - minEMG > validEMGData)
              {
                collectionState = EMG_DATA_VALID;
              }
            } 
          break;

          case EMG_DATA_ERROR:
            if(!promptRan)
            {
              EMGDataError();
              promptRan = true;
            }
            else if(userReady())
            {
              promptRan = false;
              collectionState = RUN_TIMER_PROMPT;
            }
          break;

          case EMG_DATA_VALID:
            if(!promptRan)
            {
              EMGDataValid();
              promptRan = true;
              //set threshold values based on min/max data
            }
            else if(userReady())
            {
              promptRan = false;
              runScreenInit();
              batteryUpdate();
              StartUpState = SYSTEM_READY;
              break;
            }
          break;
        }
      break; //INIT_MUSCLE

      case SYSTEM_READY:
        updateBatterySamples();
          if (millis() - lastBatteryRead >= batteryInterval){
              lastBatteryRead = millis();
              if (updateBatteryReading()) {
                batteryUpdate();
              }
          }
        
    if (isSampleReady()) {
        clearSampleFlag();
        int muscle = getSensorVal();
        printMuscle(muscle);

        switch(brakeState) {
          case RELEASE_BRAKE:
            if (muscle > engageThreshold) {
              servoBrake();
              brakeEngaged();
              brakeState = ENGAGE_BRAKE;
            }
          break;

          case ENGAGE_BRAKE:
            if(muscle < relaxThreshold) { //was release threshold
              servoRelease();
              brakeReleased();
              brakeState = RELEASE_BRAKE; //was BRAKE_RELEASE
            }
          break;
        }//switch
      }// muscle sensor if
        
      break; //INIT_SERVO
    }//init switch




    //   
    }//loop
#endif

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

  #include "muscleFSM.h"
  #include "tftScreen.h"
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
  bool promptRan = false;
  bool collecting = false;

  int flexValue = 0;
  int maxEMG = 0;
  int minEMG = 0;
  int validEMGData = 200;
/*******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES                                                *
 ******************************************************************************/

/*******************************************************************************
 * PUBLIC FUNCTION IMPLEMENTATIONS                                             *
 ******************************************************************************/
MuscleState runMuscleFSM(){
    static MuscleState state = RUN_PLACEMENT_PROMPT;
    switch(state){
        case RUN_PLACEMENT_PROMPT:
            if(!promptRan){
              muscleSensorInit();
              musclePlacementPrompt();
              promptRan = true;
            }
            else if(userReady()){
              promptRan = false;
              state = RUN_TIMER_PROMPT;
            }
        break; //RUN_PLACEMENT_PROMPT

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
              state = COLLECT_EMG_DATA;
            }
        break; //RUN_TIMER_PROMPT

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
                state = EMG_DATA_ERROR;
              }
              else if(maxEMG - minEMG > validEMGData)
              {
                state = EMG_DATA_VALID;
              }
            } 
        break; //COLLECT_EMG_DATA

        case EMG_DATA_ERROR:
            if(!promptRan)
            {
              EMGDataError();
              promptRan = true;
            }
            else if(userReady())
            {
              promptRan = false;
              state = RUN_TIMER_PROMPT;
            }
        break; //EMG_DATA_ERROR

        case EMG_DATA_VALID:
            if(!promptRan)
            {
              EMGDataValid();
              promptRan = true;
              //set threshold values based on min/max data? (FUTURE)
            }
            else if(userReady())
            {
              promptRan = false;
              runScreenInit();
              batteryUpdate();
              state = EMG_DONE;
            }
        break; //EMG_DATA_VALID
    }//state switch handler

    return state;
}//function end
/*******************************************************************************
 * PRIVATE FUNCTION IMPLEMENTATIONS                                            *
 ******************************************************************************/

/*******************************************************************************
 * MAIN                                                                        *
 ******************************************************************************/
#endif
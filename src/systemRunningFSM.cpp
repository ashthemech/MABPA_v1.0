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

  #include "systemRunningFSM.h"
  #include "myowareSensor.h"
  #include "battery.h"
  #include "tftScreen.h"
  #include "servoB.h"

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
  unsigned long lastBatteryRead = 0;      // stores the last time battery was read, initialized to 0
  const unsigned long batteryInterval = 5000;  //5 seconds

//hysteresis bounds for a flex
  int muscle = 0;
  const int engageThreshold = 250; //400 prev
  const int relaxThreshold  = 100; //350 prev

const unsigned long BAR_REFRESH_INTERVAL = 33; // ~30Hz update
unsigned long lastBarUpdate = 0;
/*******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES                                                *
 ******************************************************************************/

/*******************************************************************************
 * PUBLIC FUNCTION IMPLEMENTATIONS                                             *
 ******************************************************************************/
RunState runSystemRunningFSM(){
    static RunState state = INIT;

    if(isSampleReady())
    {
        clearSampleFlag();
        muscle = getSensorVal();
        if (millis() - lastBarUpdate >= BAR_REFRESH_INTERVAL){
            drawHorizontalEMGBar(muscle);
            lastBarUpdate = millis();
        }
        //printMuscle(muscle);

        switch(state){
            case INIT:
                updateBatterySamples();
                if (millis() - lastBatteryRead >= batteryInterval){
                    lastBatteryRead = millis();
                    if (updateBatteryReading()){
                        batteryUpdate();
                    }
                }
                brakeReleased();
                state = RELEASE_BRAKE;
            break;

            case RELEASE_BRAKE:
                if(muscle > engageThreshold){
                    servoBrake();
                    brakeEngaged();
                    state = ENGAGE_BRAKE;
                }
                else{
                    updateBatterySamples();
                    if (millis() - lastBatteryRead >= batteryInterval){
                        lastBatteryRead = millis();
                        if (updateBatteryReading()){
                            batteryUpdate();
                        }
                    }
                }
            break;

            case ENGAGE_BRAKE:
                if(muscle < relaxThreshold){
                    servoRelease();
                    brakeReleased();
                    state = RELEASE_BRAKE;
                }
            break;

        }//muscle if end
    } //switch state end
    return state;
}//loop end
/*******************************************************************************
 * PRIVATE FUNCTION IMPLEMENTATIONS                                            *
 ******************************************************************************/

/*******************************************************************************
 * MAIN                                                                        *
 ******************************************************************************/
#endif
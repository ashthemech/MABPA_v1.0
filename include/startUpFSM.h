/* 

 * File:   peripheralTest.h
 * Author: Ashton Coons
 * Brief: Header for test harness for the required MABPA project peripherals on the Teensy 4.1
 * Created on 02.20.2025, 1:53pm
 * Modified on 05.28.2025, 1:24pm
 */

 #ifndef STARTUP_FSM_H //header guard
 #define STARTUP_FSM_H
 
 /*******************************************************************************
  * PUBLIC #INCLUDES                                                            *
  ******************************************************************************/
 
 /*******************************************************************************
  * PUBLIC #DEFINES                                                             *
  ******************************************************************************/

 /*******************************************************************************
  * PUBLIC TYPEDEFS                                                             *
  ******************************************************************************/
enum StartupState {
  DRAW_LOGO,
  INIT_MUSCLE,
  INIT_SERVO,
  INIT_BATTERY,
  LOW_BATT,
  SYSTEM_READY,
  SYSTEM_ERROR
};
 /*******************************************************************************
  * PUBLIC FUNCTION PROTOTYPES                                                  *
  ******************************************************************************/

/*******************************************************************************
 Function
   runSTartupFSM
 Parameters
   none
 Returns
   StartupState (state of the FSM)
 Description
   runs the start up finite state machine 
 Notes

 Author
   Ashton Coons, 5/12/25,
 ****************************************************************************/
StartupState runStartupFSM();

#endif
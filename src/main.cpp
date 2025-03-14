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
  #include "peripheralTest.h"

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/

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
    Serial.begin (9600);
  }

  void loop() {
    Serial.println ("Teensy 4.1: Hello world!");
    delay(5000);
  }
#endif

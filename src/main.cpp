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

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
  #define SLOW_BLINK_TEST
/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
#ifdef SLOW_BLINK_TEST
  int ledPin = 13;
#endif
/*******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES                                                *
 ******************************************************************************/
 /**
  * @Function slowBlinkTest(void)
  * @param none
  * @return none
  * @brief Test function for slow blinking LED on Teensy 4.1
  * @note
  * @author Ashton Coons
  * @modified AshtonCoons, 2025.2.19 3:54pm */
 void slowBlinkTest(void);

/*******************************************************************************
 * PUBLIC FUNCTION IMPLEMENTATIONS                                             *
 ******************************************************************************/


/*******************************************************************************
 * PRIVATE FUNCTION IMPLEMENTATIONS                                            *
 ******************************************************************************/
 /**
  * @Function slowBlinkTest(void)
  * @param none
  * @return none
  * @brief Test function for slow blinking LED on Teensy 4.1
  * @note
  * @author Ashton Coons
  * @modified AshtonCoons, 2025.2.19 3:54pm */
 void slowBlinkTest(void){
    digitalWrite(ledPin, HIGH);
    delay(1000);
    digitalWrite(ledPin, LOW);
    delay(1000);
 }

 /**
  * @Function setup(void)
  * @param none
  * @return none
  * @brief Arduino setup function
  * @note Placed in private function implementation to take out of MAIN section
  * @author Ashton Coons
  * @modified AshtonCoons, 2025.2.19 4:20pm */
 void setup(){
  #ifdef SLOW_BLINK_TEST
    pinMode(ledPin, OUTPUT);
  #endif
 }
/*******************************************************************************
 * MAIN                                                                        *
 ******************************************************************************/

void loop() {

  #ifdef SLOW_BLINK_TEST
    slowBlinkTest();
  #endif
  
}

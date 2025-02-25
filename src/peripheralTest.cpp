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
 
  /*******************************************************************************
   * MAIN                                                                        *
   ******************************************************************************/
  #ifdef PERIPHERAL_TEST
    void setup(){
        #ifdef SLOW_BLINK_TEST
            pinMode(ledPin, OUTPUT);
        #endif
    }

    void loop() {
        #ifdef SLOW_BLINK_TEST
            slowBlinkTest();
        #endif
    }
  #endif
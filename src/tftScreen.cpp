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
  #include "tftScreen.h"

  #include <SPI.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SharpMem.h>
  #include <ILI9341_t3.h>
  #include <font_DroidSans_Bold.h>
  #include <font_AwesomeF200.h>

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
    //define the SPI pins for the tft screen
    #define TFT_DC      9
    #define TFT_CS      10
    #define TFT_RST    255  // 255 = unused, connect to 3.3V
    #define TFT_MOSI    11
    #define TFT_SCLK    13
    #define TFT_MISO    12
    // ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);

/*******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES                                                *
 ******************************************************************************/
//filtering
/*******************************************************************************
 * PUBLIC FUNCTION IMPLEMENTATIONS                                             *
 ******************************************************************************/
// bool screenInit()
//{
    // uint16_t testColor = ILI9341_RED;
    // int16_t x = 5, y = 5;

    // //initialization code for screen
    // tft.begin();
    // tft.setRotation(1); //check with ben on screen orientation

    // tft.drawPixel(x, y, testColor);
    // delay(5);  //allow time to update

    // //read back the pixel value
    // uint16_t readColor = tft.readPixel(x, y);

    // if (readColor == testColor) {
    //     return true;  //screen responded correctly
    // } else {
    //     return false; //mismatch suggests screen failed to initialize
    // }
//}
/*******************************************************************************
 * PRIVATE FUNCTION IMPLEMENTATIONS                                            *
 ******************************************************************************/

/*******************************************************************************
 * MAIN                                                                        *
 ******************************************************************************/
#endif
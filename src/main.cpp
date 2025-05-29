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
  #include <Servo.h> 
  #include <imxrt.h>
  #include <IntervalTimer.h>


  #include <SPI.h>
  #include <ILI9341_t3.h>
  #include <font_DroidSans_Bold.h>
  #include <font_AwesomeF200.h>

  #include "tftScreen.h"
  #include "myowareSensor.h"
  #include "battery.h"

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
    #define TFT_DC      9
    #define TFT_CS      10
    #define TFT_RST    255  // 255 = unused, connect to 3.3V
    #define TFT_MOSI    11
    #define TFT_SCLK    13
    #define TFT_MISO    12
    ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);

    Servo brakeServo;
    int servoPin = 4;
    int servoPos = 0;
/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/
  enum BrakeState { BRAKE_RELEASED, BRAKE_ENGAGED };
  BrakeState brakeState = BRAKE_RELEASED;
/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
  //reboot failure counter in case of a component failure on boot
  __attribute__((section(".noinit"))) static uint8_t retryCount;
  const uint8_t MAX_RETRIES = 3;

  //interrupt timer for muscle sensor sampling
  IntervalTimer muscleTimer;

  //hysteresis bounds for a flex
  const int engageThreshold = 500;
  const int servoThreshold  = 750;
  const int relaxThreshold  = 200;

  bool flexDetected = false;

  unsigned long flexReleaseStart = 0;
  const unsigned long releaseDebounce = 100;  // milliseconds
  
  //sampling rate set up
    // unsigned long currentTime = 0;
    // unsigned long previousTime = 0;
    // const long interval = 667; //1ms interval (1500Hz)

  // Servo brakeServo; //set up servo object for the brake servo
  // int brakePin = 4; //brake servo pin
  // int brakePos = 0; //intialize position to zero

/*******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES                                                *
 ******************************************************************************/

/*******************************************************************************
 * PUBLIC FUNCTION IMPLEMENTATIONS                                             *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE FUNCTION IMPLEMENTATIONS                                            *
 ******************************************************************************/

bool systemReset(){
  retryCount++;
  if (retryCount >= MAX_RETRIES){
    while (1);
    return false;
  }else{
    delay(2000); //wait 2 seconds before retrying
    SCB_AIRCR = 0x05FA0004;
    return true;
  }
}
/*******************************************************************************
 * MAIN                                                                        *
 ******************************************************************************/
  void setup(){

    ///////////REMOVE AFTER TESTING
    Serial.begin(115200);
    // while (!Serial) {
    // ; // Wait here until the Serial connection is established
    // }
    //////////

    /* ILI9341 Screen Setup */
    tft.begin();
    tft.setRotation(-1); //check with ben on screen orientation
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_YELLOW);
    tft.setFont(DroidSans_20_Bold);

    pinMode(servoPin, OUTPUT);
    brakeServo.attach(servoPin);
    brakeServo.write(0); //set servo to 0 degrees

    delay(1000);
    // if (screenInit()) {
    //   Serial.println("Screen initialized successfully.");
    //   tft.fillScreen(ILI9341_BLACK);
    //   tft.setTextColor(ILI9341_YELLOW);
    //   tft.setFont(DroidSans_40_Bold);

    // } else {
    //   Serial.println("Screen not detected.");
      // Serial.println("Trying reboot #");
      // Serial.print(retryCount);
      // Serial.flush();

      // if(!systemReset()){
      //   Serial.println("Terminal screen failure.");
      // }
      // else{
      //   retryCount = 0;
      //   Serial.println("Trying reset.");
      // }

    //if screen initialized successfully, run the team logo while hardware initializes







    /* Myoware 2.0 Muscle Sensor Setup */

    if (muscleSensorInit()) {
      Serial.println("Myoware sensor initialized successfully.");

      //start the timer for the muscle sensor (1500Hz)
      muscleTimer.begin(readMuscleSensor, 666);
    
      // COME BACK TO THIS
    } 

    batteryInit();
    //else {
    //   Serial.println("Sensor not detected.");
    //   while (1); // halt or retry
    // }

    // pinMode(brakePin, OUTPUT); //set the brake servo pin as output
    // brakeServo.attach(brakePin); //attach the servo to the pin
    // brakeServo.write(0); //set servo to 0 degrees
    // delay(1000); //wait for 1 second to allow servo to initialize
}

  void loop() {

    // float voltage = readBatteryVoltage();
    // if (voltage < 0.01f) voltage = 0.0f;  // clamp very small noise

    // float percent = getBatteryPercent(voltage);

    // updateBatteryDisplay(tft, percent);
    // delay(500);

    if (isSampleReady()) {
        clearSampleFlag();
        int muscle = getSensorVal();
        // tft.setFont(DroidSans_20_Bold);
        // tft.fillScreen(ILI9341_BLACK);
        // tft.setCursor(10, 50);
        // tft.print("No Change Detected");
        // tft.setCursor(10, 100);
        // tft.print(muscle);
        tft.fillScreen(ILI9341_BLACK);
        tft.setCursor(50, 50);
        tft.print(muscle);

        if (brakeState == BRAKE_RELEASED){
          //only allow engagement if clearly above the engage threshold
          if (muscle > engageThreshold && muscle < servoThreshold) {
            brakeState = BRAKE_ENGAGED;
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 50);
            tft.print("ENGAGED BRAKE");
            brakeServo.write(70);
            delay(10);
            flexReleaseStart = false; // reset release timer
          }
        }
        else if (brakeState == BRAKE_ENGAGED) {
          if(!flexReleaseStart && muscle < servoThreshold){
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(10, 50);
            tft.print("RELEASED BRAKE");
            brakeServo.write(115);
            delay(10);
            flexReleaseStart = true; 
          }
          else if(flexReleaseStart && muscle < relaxThreshold){
            brakeState = BRAKE_RELEASED;
            flexReleaseStart = false;
          }
        }
      }
    }
#endif

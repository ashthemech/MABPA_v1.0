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
  //define the screen pins for ILI9341 screen
    #define TFT_DC       9
    #define TFT_CS      10
    #define TFT_RST    255  //255 = unused, connected to 3.3V
    #define TFT_MOSI    11
    #define TFT_SCLK    13
    #define TFT_MISO    12
    ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);

  //initialize the servo pins and angles for the brake servo
    #define SERVO_PIN    4

    #define BATTERY_PIN 17
/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/
  //define servo object for the brake servo (arduino class)
  Servo brakeServo;

  // //enum for the braking state machine
  // enum BrakeState {WAITING, BRAKE_RELEASE, ENGAGE_BRAKE};
  // BrakeState brakeState = WAITING;

  // //enum for the system state machine (initialization and error)
  // enum SystemState {INIITIALIZING, READY, ERROR};
  // SystemState currentState = INIITIALIZING;

  enum StartupState {
  DRAW_LOGO,
  INIT_MUSCLE,
  INIT_SERVO,
  SERVO_FAIL,
  INIT_BATTERY,
  BATT_FAIL,
  SYSTEM_READY,
  SYSTEM_ERROR
};
  StartupState StartUpState = DRAW_LOGO;
/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
  //reboot failure counter in case of a component failure on boot
  __attribute__((section(".noinit"))) static uint8_t retryCount;
  const uint8_t MAX_RETRIES = 3;

  //screen initialization timers
  unsigned long initStartTime;

  //interrupt timer for muscle sensor sampling
  IntervalTimer muscleTimer;

  //hysteresis bounds for a flex
  const int engageThreshold = 400;
  const int releaseThreshold  = 500;
  const int relaxThreshold  = 350;

  unsigned long lastBatteryRead = 0;      // stores the last time battery was read, initialized to 0
  const unsigned long batteryInterval = 2000;  //2 seconds

  const int brakeReleasePos = 115;
  const int brakeEngagePos = 70;

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
    //ensure 10 bit resolution
    analogReadResolution(10);
    //begin communication for the tft screen
    tft.begin();
    //set the rotation to display the correct orientation to the user
    tft.setRotation(-1);
    //fill the screen white upon startup
    tft.fillScreen(ILI9341_WHITE);
    //set the text color and font for the screen
    tft.setTextColor(ILI9341_YELLOW);
    tft.setFont(DroidSans_20_Bold);

}

  void loop() {

    switch(StartUpState){
      case DRAW_LOGO:
        //draw the team logo on the screen
        //add screen initialization check
        tft.fillScreen(ILI9341_WHITE);
        for (int i = 1; i <= 140; i++) {
          tft.fillRectVGradient(50, 50, 220, i, ILI9341_YELLOW, ILI9341_BLACK);
          delay(15);
        }

        delay(100);
        tft.fillScreen(ILI9341_BLACK);
        tft.fillRectVGradient(50, 50, 220, 140, ILI9341_YELLOW, ILI9341_BLACK);
        delay(500);
        tft.setTextColor(ILI9341_WHITE);  // White contrasts well with black/yellow
        tft.setFont(DroidSans_40_Bold);
        tft.setCursor(50 + 10, 60 + 20);  // Position centered-ish inside box
        tft.print("MABPA");

        tft.setTextColor(ILI9341_YELLOW);
        tft.setFont(AwesomeF200_72);
        tft.setCursor(100, 120);  // Position centered-ish inside box
        tft.print((char)6); //bike logo

        StartUpState = INIT_BATTERY;
      break; //DRAW_LOGO

      case INIT_BATTERY:
        //initalize the battery level reading system
        if (batteryInit())
        {
          updateBatteryReading();
          updateBatteryDisplay(tft); //MOVE THIS
          StartUpState = INIT_SERVO;
        }
        else if(!batteryInit())
        {
          tft.fillScreen(ILI9341_BLACK);
          tft.setTextColor(ILI9341_YELLOW);
          tft.setFont(DroidSans_16_Bold);
          tft.setCursor(20, 50);
          tft.print("CRITICAL BATTERY ERROR");
          tft.setCursor(20, 75);
          tft.print("PLEASE REPLACE BATTERY");
          tft.setCursor(20, 150);
          tft.print("USE A 2S 7.4V LIPO ONLY");
          StartUpState = BATT_FAIL;
        }
      break; //INIT_BATTERY

      case BATT_FAIL:
        ; //display battery failure message
      break; //INIT_BATTERY

      case INIT_MUSCLE:
        //initalize the muscle sensor -> true if successful, false if not
        // muscleSensorInit();
        // muscleTimer.begin(readMuscleSensor, 666);
        // StartUpState = INIT_SERVO;
        ;
      break; //INIT_MUSCLE

      case INIT_SERVO:
        Serial.begin(115200);
        pinMode(SERVO_PIN, OUTPUT);
        brakeServo.attach(SERVO_PIN);
        brakeServo.write(brakeReleasePos);
        delay(300);  // Let servo settle

        const int numSamples = 10;
        const float thresholdDipVolts = 0.005f; // ~20mV dip
        const float ADC_SCALE = 3.3f / 1023.0f;

        int rawBefore = 0, rawDuring = 0;
        float vBefore = 0.0f, vDuring = 0.0f;

        // 1. Sample battery voltage before movement
        for (int i = 0; i < numSamples; i++) {
            rawBefore += analogRead(BATTERY_PIN);
            delayMicroseconds(500);
        }
        rawBefore /= numSamples;
        vBefore = rawBefore * ADC_SCALE;

        // 2. Trigger servo to engage brake
        brakeServo.write(brakeEngagePos);
        delay(200); // Let movement begin

        // 3. Sample battery voltage during movement
        for (int i = 0; i < numSamples; i++) {
            rawDuring += analogRead(BATTERY_PIN);
            delayMicroseconds(500);
        }
        rawDuring /= numSamples;
        vDuring = rawDuring * ADC_SCALE;

        // 4. Reset servo
        brakeServo.write(brakeReleasePos);

        float dip = vBefore - vDuring;

        // 5. Debug prints (Serial, invisible to user)
        Serial.print("Raw Before: "); Serial.println(rawBefore);
        Serial.print("Raw During: "); Serial.println(rawDuring);
        Serial.print("Voltage Dip: "); Serial.println(dip, 4);
        Serial.print("Raw Analog Now: "); Serial.println(analogRead(BATTERY_PIN));

        // 6. Show result on screen
        tft.fillScreen(ILI9341_BLACK);
        tft.setTextColor(ILI9341_YELLOW);
        tft.setFont(DroidSans_16_Bold);
        tft.setCursor(20, 50);
        if (dip > thresholdDipVolts) {
            tft.print("SERVO MOVED");
        } else {
            tft.print("SERVO DID NOT MOVE");
        }

        StartUpState = INIT_MUSCLE;
      break; //INIT_SERVO

      // if (isSampleReady()) {
        //   clearSampleFlag();
        //   int muscle = getSensorVal();
        //   tft.fillRect(10, 175, 200, 60, ILI9341_BLACK);
        //   tft.setCursor(10, 175);
        //   tft.setFont(DroidSans_20_Bold);
        //   tft.print(muscle);
        // }

    }//init switch




    //   updateBatterySamples();
    //     if (millis() - lastBatteryRead >= batteryInterval) {
    //         lastBatteryRead = millis();
    //         if (updateBatteryReading()) {
    //             updateBatteryDisplay(tft);
    //         }
    //     }
        
    // if (isSampleReady()) {
    //     clearSampleFlag();
    //     int muscle = getSensorVal();
    //     // tft.setFont(DroidSans_20_Bold);
    //     // tft.fillScreen(ILI9341_BLACK);
    //     // tft.setCursor(10, 50);
    //     // tft.print("No Change Detected");
    //     // tft.setCursor(10, 100);
    //     // tft.print(muscle);
    //     tft.fillRect(10, 175, 200, 60, ILI9341_BLACK);
    //     tft.setCursor(10, 175);
    //     tft.print(muscle);

    //     switch(brakeState) {
    //       case WAITING:
    //         if (muscle > engageThreshold) {
    //           brakeState = ENGAGE_BRAKE;
    //           tft.fillRect(10, 75, 200, 60, ILI9341_BLACK);
    //           tft.setCursor(10, 75);
    //           tft.print("BRAKE ENGAGED");
    //           delay(10);
    //         }
    //       break;

    //       case ENGAGE_BRAKE:
    //         brakeServo.write(brakeEngagePos); // Engage brake
    //         if(muscle < releaseThreshold) {
    //           brakeState = BRAKE_RELEASE;
    //         }
    //       break;

    //       case BRAKE_RELEASE:
    //         brakeServo.write(brakeReleasePos); // Release brake
    //         tft.fillRect(10, 75, 200, 60, ILI9341_BLACK);
    //         tft.setCursor(10, 75);
    //         tft.print("BRAKE RELEASED");
    //         if(muscle < relaxThreshold) {
    //           brakeState = WAITING; // Go back to waiting state
    //         }
    //         delay(10);
    //       break;

    //     }//switch
    //   }// muscle sensor if
    }//loop
#endif

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
  #include "battery.h"

  #include <SPI.h>
  #include <ILI9341_t3.h>
  #include <XPT2046_Touchscreen.h>
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
    #define TFT_DC       9
    #define TFT_CS      10
    #define TFT_RST    255  // 255 = unused, connect to 3.3V
    #define TFT_MOSI    11
    #define TFT_SCLK    13
    #define TFT_MISO    12

    #define T_CLK       13
    #define T_CS         8  
    #define T_DIN       11 
    #define T_DO        12 
    #define T_IRQ        2 

    ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);
    XPT2046_Touchscreen ts(T_CS, T_IRQ);

    int flexTimer = 15;
    bool flexTimerActive = false;
    unsigned long lastFlexUpdate = 0;

/*******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES                                                *
 ******************************************************************************/

/*******************************************************************************
 * PUBLIC FUNCTION IMPLEMENTATIONS                                             *
 ******************************************************************************/
bool screenInit(){

    //configure CS pins as toggleable outputs so TFT and touchscreen can be selected
    pinMode(TFT_CS, OUTPUT);
    digitalWrite(TFT_CS, HIGH);
    pinMode(T_CS, OUTPUT);
    digitalWrite(T_CS, HIGH);


    //begin communication for the tft screen
    tft.begin();
    tft.setRotation(-1);
    tft.fillScreen(ILI9341_WHITE);

    // Initialize touchscreen
    ts.begin();
    ts.setRotation(-1);

    return true;
}

void drawLogo(){
    //set the text color and font for the screen
    tft.setTextColor(ILI9341_YELLOW);
    tft.setFont(DroidSans_20_Bold);
    //fill the screen with white and fade in a black and yellow rectangle
    tft.fillScreen(ILI9341_WHITE);
    for (int i = 1; i <= 140; i++) {
        tft.fillRectVGradient(50, 50, 220, i, ILI9341_YELLOW, ILI9341_BLACK);
        delay(15);
    }

    delay(100);
    //fill the screen with black, keep the gradient rectangle and print the team name and bike logo
    tft.fillScreen(ILI9341_BLACK);
    tft.fillRectVGradient(50, 50, 220, 140, ILI9341_YELLOW, ILI9341_BLACK);
    delay(500);
    tft.setTextColor(ILI9341_WHITE);  // White contrasts well with black/yellow
    tft.setFont(DroidSans_40_Bold);
    tft.setCursor(50 + 10, 60 + 10);  // Position centered-ish inside box
    tft.print("MABPA");

    tft.setTextColor(ILI9341_YELLOW);
    tft.setFont(AwesomeF200_96);
    tft.setCursor(80, 100);  // Position centered-ish inside box
    tft.write(char(6));
}

void testFont()
{
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_YELLOW);
  tft.setFont(AwesomeF200_72); // <-- crash here on cold boot
  tft.setCursor(10, 100);
  tft.write(char(6)); // bike logo
}

void batteryFail(){
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_YELLOW);
    tft.setFont(DroidSans_12_Bold);
    tft.setCursor(50, 80);
    tft.print("PLEASE REPLACE BATTERY");
    tft.setCursor(55, 100);
    tft.print("USE A 2S 7.4V LIPO ONLY");
    tft.setCursor(65, 150);
    tft.print("IF PROBLEM PERSISTS");
    tft.setCursor(75, 170);
    tft.print("DO NOT USE SYSTEM");

    tft.setFont(DroidSans_16_Bold);
    while(1)
    {
        tft.setTextColor(ILI9341_RED);
        tft.setCursor(20, 50);
        tft.print("CRITICAL BATTERY ERROR");
        delay(500);
        tft.setTextColor(ILI9341_BLACK);
        tft.setCursor(20, 50);
        tft.print("CRITICAL BATTERY ERROR");
        delay(500);
    }
}

void servoFail()
{
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_YELLOW);
    tft.setFont(DroidSans_12_Bold);
    tft.setCursor(30, 80);
    tft.print("ENSURE BRAKE ATTACHMENT IS");
    tft.setCursor(40, 100);
    tft.print("CONNECTED TO BRAKE LEVER");
    tft.setCursor(70, 150);
    tft.print("IF PROBLEM PERSISTS");
    tft.setCursor(80, 170);
    tft.print("DO NOT USE SYSTEM");

    tft.setFont(DroidSans_18_Bold);

    while(1)
    {
        tft.setTextColor(ILI9341_RED);
        tft.setCursor(20, 50);
        tft.print("CRITICAL BRAKE ERROR");
        delay(500);
        tft.setTextColor(ILI9341_BLACK);
        tft.setCursor(20, 50);
        tft.print("CRITICAL BRAKE ERROR");
        delay(500);
    }
}

void touchScreenTest()
{
    Serial.begin(115200);
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_YELLOW);
    tft.setFont(DroidSans_20_Bold);
    tft.setCursor(10, 10);
    tft.print("Touch the screen to test");

    digitalWrite(TFT_CS, HIGH);  // Disable TFT
    digitalWrite(T_CS, LOW);     // Enable touch
    
    while (true) {
        if (ts.touched()) {
        TS_Point p = ts.getPoint();
        if(p.x > 2100)
        {
            Serial.print("YES");
        }
        else if(p.x < 1900)
        {
            Serial.print("NO");
        }
        else
        {
            Serial.print("UNKNOWN");
        }
        delay(500); 
    }
}
}

void musclePlacementPrompt()
{
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_YELLOW);
    tft.setFont(DroidSans_16_Bold);
    tft.setCursor(30, 10);
    tft.print("Place the muscle sensor");
    tft.setCursor(30, 30);
    tft.print("on your bicep and secure");
    tft.setCursor(30, 50);
    tft.print("the armband tightly.");
    tft.setCursor(30, 90);
    tft.print("Press the screen when");
    tft.setCursor(30, 110);
    tft.print("ready to test.");

    digitalWrite(TFT_CS, HIGH);  //Disable TFT
    digitalWrite(T_CS, LOW);     //Enable touch
}

bool userReady()
{
    while (!ts.touched()) {
        return false;  // Wait for touch
    }
    if (ts.touched()){
        delay(100); //debouncing
        return true;  // User is ready
    }
    digitalWrite(TFT_CS, LOW);  //Enable TFT
    digitalWrite(T_CS, HIGH);     //disable touch
}

void muscleFlexPrompt()
{
    flexTimer = 15;
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_YELLOW);
    tft.setFont(DroidSans_16_Bold);
    tft.setCursor(20, 10);
    tft.print("Flex and unflex your bicep");
    tft.setCursor(20, 30);
    tft.print("at a comfortable pace and"); 
    tft.setCursor(20, 50);
    tft.print("intensity for 15 seconds.");
    tft.setCursor(20, 90);
    tft.print("Press the screen when");
    tft.setCursor(20, 110);
    tft.print("ready to start the timer:");

    tft.setFont(DroidSans_40_Bold);
    tft.setCursor(125, 175);
    tft.print(flexTimer);

    digitalWrite(TFT_CS, HIGH);  //Disable TFT
    digitalWrite(T_CS, LOW);     //Enable touch
}

void startFlexTimer()
{
    lastFlexUpdate = millis();
    flexTimerActive = true;
    tft.setCursor(125, 175);
    tft.print(flexTimer);
}

bool updateFlexTimer()
{
    if ((millis() - lastFlexUpdate >= 1000) && flexTimerActive){   
        lastFlexUpdate += 1000; // stable stepping
        flexTimer--;

        if (flexTimer >= 0)
        {
            tft.fillRect(125, 170, 100, 60, ILI9341_BLACK);
            tft.setCursor(125, 175);
            tft.print(flexTimer);
            return true;
        }
        else
        {
            flexTimerActive = false;
            return false;
        }
    }

    return true;
}

void EMGDataError()
{
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_RED);
    tft.setFont(DroidSans_16_Bold);
    tft.setCursor(30, 10);
    tft.print("EMG PLACEMENT ERROR");
    tft.setTextColor(ILI9341_YELLOW);
    tft.setCursor(30, 40);
    tft.print("Reposition the sensor.");
    tft.setCursor(30, 90);
    tft.print("Press the screen when");
    tft.setCursor(30, 110);
    tft.print("ready to try again.");

    digitalWrite(TFT_CS, HIGH);  //Disable TFT
    digitalWrite(T_CS, LOW);     //Enable touch
}

void EMGDataValid()
{
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_GREEN);
    tft.setFont(DroidSans_16_Bold);
    tft.setCursor(30, 10);
    tft.print("EMG Collection Done");
    tft.setTextColor(ILI9341_YELLOW);
    tft.setCursor(30, 90);
    tft.print("Press the screen when");
    tft.setCursor(30, 110);
    tft.print("ready to ride!");

    digitalWrite(TFT_CS, HIGH);  //Disable TFT
    digitalWrite(T_CS, LOW);     //Enable touch
}

void batteryUpdate()
{
    updateBatteryDisplay(tft);
}

void brakeEngaged()
{
    tft.fillRect(10, 75, 200, 60, ILI9341_BLACK);
    tft.setCursor(10, 75);
    tft.print("BRAKE ENGAGED");
    delay(10);
}

void brakeReleased()
{ 
    tft.fillRect(10, 75, 200, 60, ILI9341_BLACK);
    tft.setCursor(10, 75);
    tft.print("BRAKE RELEASED");
}

void runScreenInit()
{
    tft.setFont(DroidSans_20_Bold);
    tft.fillScreen(ILI9341_BLACK);
}

void printMuscle(int muscle)
{ 
    tft.fillRect(10, 175, 200, 60, ILI9341_BLACK);
    tft.setCursor(10, 175);
    tft.print(muscle);
}


/*******************************************************************************
 * PRIVATE FUNCTION IMPLEMENTATIONS                                            *
 ******************************************************************************/

/*******************************************************************************
 * MAIN                                                                        *
 ******************************************************************************/
#endif
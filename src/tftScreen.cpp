/* 
 * File:   tftScreen.cpp
 * Author: Ashton Coons
 * Brief: tft screen source code file for screen functions used in MABPA system
 * Created on 2 19, 2025, 3:15pm
 * Modified on 5 25, 2025, 6:04pm
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
/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/
//MOSI screen type declaration
    ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);
//MISO screen stype declatration
    XPT2046_Touchscreen ts(T_CS, T_IRQ);
/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
    int flexTimer = 15;
    bool flexTimerActive = false;
    unsigned long lastFlexUpdate = 0;
    int prevBarWidth = 0;

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

void batteryFailPrompt(){
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

void servoFailPrompt()
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
    tft.setTextColor(ILI9341_GREEN);
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
    tft.setFont(DroidSans_40_Bold);
    tft.setTextColor(ILI9341_MAGENTA);
    tft.fillRect(10, 70, 300, 60, ILI9341_BLACK);
    tft.setCursor(10, 75);
    tft.print("ENGAGED");
    delay(10);
}

void brakeReleased()
{ 
    tft.setFont(DroidSans_40_Bold);
    tft.setTextColor(ILI9341_OLIVE);
    tft.fillRect(10, 70, 300, 60, ILI9341_BLACK);
    tft.setCursor(10, 75);
    tft.print("RELEASED");
}

void runScreenInit()
{
    tft.setFont(DroidSans_20_Bold);
    tft.fillScreen(ILI9341_BLACK);
}

void printStrengthText()
{ 
    tft.fillRect(10, 150, 200, 40, ILI9341_BLACK);
    tft.setCursor(10, 150);
    tft.setFont(DroidSans_14_Bold);
    tft.setTextColor(ILI9341_YELLOW);
    tft.print("MUSCLE SIGNAL STRENGTH:");
}

void drawHorizontalEMGBar(int val) {

  const int barY = 175;            // vertical position of the bar
  const int barX = 10;             // left edge
  const int maxBarWidth = 300;     // max width of bar (screen width - margin)
  const int barHeight = 60;

  int barWidth = map(val, 0, 1023, 0, maxBarWidth); // EMG to width

  // Only erase the "shrinking" portion
  if (barWidth < prevBarWidth) {
    tft.fillRect(barX + barWidth, barY, prevBarWidth - barWidth, barHeight, ILI9341_BLACK);
  }

  // Draw new bar section (only new pixels if increasing)
  tft.fillRect(barX, barY, barWidth, barHeight, ILI9341_YELLOW);

  prevBarWidth = barWidth;
}

void batteryLowPrompt(float volt){
    tft.fillScreen(ILI9341_BLACK);
    tft.setFont(DroidSans_16_Bold);
    tft.setTextColor(ILI9341_RED);
    tft.setCursor(20, 50);
    tft.print("WARNING: BATTERY LOW");
    tft.setTextColor(ILI9341_YELLOW);
    tft.setFont(DroidSans_16_Bold);

    tft.setCursor(50, 100);
    tft.print("BATTERY LEVEL: ");
    tft.print(volt);
    tft.print("%");
    tft.setFont(DroidSans_12_Bold);
    tft.setCursor(10, 120);
    tft.print("Tap on screen if you wish to proceed");

    digitalWrite(TFT_CS, HIGH);  //Disable TFT
    digitalWrite(T_CS, LOW);     //Enable touch
}

void batteryLowAccepted()
{
    tft.fillScreen(ILI9341_BLACK);
}
/*******************************************************************************
 * PRIVATE FUNCTION IMPLEMENTATIONS                                            *
 ******************************************************************************/

/*******************************************************************************
 * MAIN                                                                        *
 ******************************************************************************/
#endif
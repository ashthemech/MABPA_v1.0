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
    #include <Servo.h>   
    #include "peripheralTest.h"

    #include "imxrt.h"
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
    #ifdef SLOW_BLINK_TEST
        int ledPin = 13;
    #endif

    #ifdef ANALOG_READ_TEST
        int analogPin = A0;
        int val = 0;
    #endif

    #ifdef EMG_READ_TEST
        int analogPinSensor = A0;
        int sensorVal = 0;
    #endif

    #ifdef SERVO_TEST
        Servo brakeServo;
        int servoPin = 4;
        int servoPos = 0;
    #endif

    #ifdef EMG_GRAPH_TEST
        int analogPinSensor = A0;
        int sensorVal = 0;
        float divisor = 310.303; //get V from the ADC value
        float voltage = 0.00;
        unsigned long currTime = 0;
        unsigned long prevTime = 0;
        const long inte = 500; //us interval (2000Hz)
    #endif

    // #ifdef EMG_EXTRACT_DATA
    //     int analogPinSensor = A0;
    //     int sensorVal = 0;
    //     unsigned long currentTime = 0;
    //     unsigned long previousTime = 0;
    //     const long interval = 1; //1ms interval (1000Hz)
    // #endif

    #ifdef SPI_TEST
        #define CS 10
    #endif

    #ifdef SCREEN_TEST
        //define the SPI pins for the tft screen
        #define TFT_DC      9
        #define TFT_CS      10
        #define TFT_RST    255  // 255 = unused, connect to 3.3V
        #define TFT_MOSI    11
        #define TFT_SCLK    13
        #define TFT_MISO    12
        ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);

    #endif

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
  #ifdef PERIPHERAL_TEST
    void setup(){
        #ifdef SLOW_BLINK_TEST
            pinMode(ledPin, OUTPUT);
        #endif

        #ifdef ANALOG_READ_TEST
            pinMode(analogPin, INPUT);
            analogReadResolution(10);
            Serial.begin(38400);
        #endif

        #ifdef EMG_READ_TEST
            pinMode(analogPinSensor, INPUT);
            analogReadResolution(10);
            Serial.begin(115200);
        #endif

        #ifdef SERVO_TEST
            pinMode(servoPin, OUTPUT);
            brakeServo.attach(servoPin);
            brakeServo.write(0); //set servo to 0 degrees
            delay(1000); //wait for servo to move
        #endif

        #ifdef EMG_GRAPH_TEST
            pinMode(analogPinSensor, INPUT);
            analogReadResolution(10);
            Serial.begin(115200);
        #endif

        // #ifdef EMG_EXTRACT_DATA
        //     pinMode(analogPinSensor, INPUT);
        //     analogReadResolution(10);
        //     Serial.begin(115200);
        // #endif

        #ifdef SPI_TEST
            pinMode(CS, OUTPUT);

            digitalWriteFast(CS, HIGH); //set CS high to deselect the device

            // Manually set the register bits:
            // SRE (bit 0) = 0 (slow slew rate)
            // DSE (bits 3:1) = 2 (drive strength ~2.5mA)
            // SPEED (bits 7:6) = 1 (medium frequency)
            
            IOMUXC_SW_PAD_CTL_PAD_GPIO_B0_02 = 
            (0 << 0) |      // SRE: slow slew rate
            (2 << 3) |      // DSE: ~2.5 mA drive strength
            (1 << 6);       // SPEED: medium (0 = low, 1 = medium, 2 = max)

            // SPI.begin(); //initialize SPI
            // SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE0));
            Serial.begin(115200);
        #endif

        #ifdef SCREEN_TEST
            #define ICON_BIKE 0xF206

            tft.begin();
            tft.setRotation(1); //check with ben on screen orientation

            tft.fillScreen(ILI9341_WHITE);
            delay(500); //for fade in of screen
            for (int i = 1; i <= 140; i++) {
                tft.fillRectVGradient(50, 50, 220, i, ILI9341_YELLOW, ILI9341_BLACK);
                delay(15);  // Adjust for speed of fade-in
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
            tft.print((char)6);

            delay(2000);
            tft.fillScreen(ILI9341_BLACK);

            //actual user screen stuff lol
            tft.setFont(AwesomeF200_20);
            tft.setCursor(10, 10);  // Position centered-ish inside box
            tft.print((char)64); //full battery
            tft.setCursor(10, 30);  // Position centered-ish inside box
            tft.print((char)65); 
            tft.setCursor(10, 50);  // Position centered-ish inside box
            tft.print((char)66); //half full battery
            tft.setCursor(10, 70);  // Position centered-ish inside box
            tft.print((char)67); 
            tft.setCursor(10, 90);  // Position centered-ish inside box
            tft.print((char)68); //empty battery

            //build battery test circuit and scale to 0-3.3V for input into ADC, scale with battery icone and send flashing alert when battery dips
            



            

        #endif
    }

    void loop() {
        #ifdef SLOW_BLINK_TEST
            digitalWrite(ledPin, HIGH);
            delay(1000);
            digitalWrite(ledPin, LOW);
            delay(1000);
        #endif

        #ifdef ANALOG_READ_TEST
            val = analogRead(analogPin);
            Serial.println(val);
            delay(250);
        #endif

        #ifdef EMG_READ_TEST
            sensorVal = analogRead(analogPinSensor);
            Serial.println(sensorVal);
            delay(1000); //delay 1 second so we can see flex and no flex values
        #endif

        #ifdef SERVO_TEST
            for(servoPos = 10; servoPos < 170; servoPos += 1){
                brakeServo.write(servoPos);              
                delay(5);                       
            } 
            for(servoPos = 180; servoPos>=1; servoPos-=1){                                
                brakeServo.write(servoPos);              
                delay(5);                       
            }
        #endif

        #ifdef EMG_GRAPH_TEST
                sensorVal = analogRead(analogPinSensor);
                voltage = (float)sensorVal / divisor;
                Serial.print(">");
                Serial.print("sEMG1:");
                Serial.print(voltage, 2);
                Serial.println();
                delay(100);
        #endif

        // #ifdef EMG_EXTRACT_DATA
        //     currentTime = millis();
            
        //     if (currentTime - previousTime >= interval) {
        //         previousTime = currentTime;
        //         sensorVal = analogRead(analogPinSensor);
        //         //voltage = (float)sensorVal / divisor;

        //         Serial.print(currentTime);
        //         Serial.print(",");
        //         //Serial.print(voltage, 2);
        //         Serial.println();
        //     }
        // #endif

        #ifdef SPI_TEST
            digitalWriteFast(CS, LOW);         // Assert CS (normally LOW)
            delay(100);
            // SPI.transfer(0xAA);                // Send test byte
            // SPI.transfer(0x55);                // Send another test byte
            digitalWriteFast(CS, HIGH);        // Deassert CS
            delay(1000); 

        #endif 

        #ifdef SCREEN_TEST
            
        #endif
    }
  #endif
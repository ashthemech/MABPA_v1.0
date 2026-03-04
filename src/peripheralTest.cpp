
/* 
 * File:   peripheralTest.cpp
 * Author: Ashton Coons
 * Brief: Source for test harness for the required MABPA project peripherals on the Teensy 4.1
 * Created on 02.20.2025, 1:53pm
 * Modified on 02.20.2025, 2:04pm
 */

 #ifdef PERIPHERAL_TEST
/*******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/
    #include <Arduino.h>
    #include <Servo.h>   
    #include "peripheralTest.h"
    #include "myowareSensor.h"

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
        int count = 0;
    #endif

    #ifdef EMG_GRAPH_TEST
        float filteredVal = 0;
        const float alpha = 0.3;  // between 0.01 (very smooth) and 0.3 (fast reacting)


        int analogPinSensor = A0;
        int sensorVal = 0;
        float divisor = 310.303; //get V from the ADC value
        float voltage = 0.00;
        unsigned long currTime = 0;
        unsigned long prevTime = 0;
        const long inte = 500; //us interval (2000Hz)
    #endif

    #ifdef EMG_EXTRACT_DATA
        int analogPinSensor = A0;
        int sensorVal = 0;
        float divisor = 310.303; //get V from the ADC value
        float voltage = 0.00;
        unsigned long currentTime = 0;
        unsigned long previousTime = 0;
        const long interval = 667; //(1500Hz)
    #endif

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

    #ifdef BATTERY_TEST
        int batteryPin = A3;
        int batteryTogglePin = 40; // pin to toggle battery measurement on/off
        const int numSamples = 20;  // size of moving average window
        float samples[numSamples] = {0};  // circular buffer for samples
        int sampleIndex = 0;

        float deadValue = 2.654;     // your dead battery voltage
        float fullValue = 3.008;   //3.002 on multimeter, 3.008 on ADC
        float batteryDivisor = fullValue - deadValue;

        float lastPercent = 0.0;
        const float hysteresisThreshold = 0.5; // update percent only if change > 1%


        //define the SPI pins for the tft screen
        #define TFT_DC      9
        #define TFT_CS      10
        #define TFT_RST    255  // 255 = unused, connect to 3.3V
        #define TFT_MOSI    11
        #define TFT_SCLK    13
        #define TFT_MISO    12
        ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);
    #endif

    #ifdef EMG_SERVO_INTEGRATION
        Servo brakeServo;
        int analogPinSensor = A0;
        int servoPin = 4;
        const int flexThreshold = 512; // flex detection
        const int servoRestAngle = 0;  // rest
        const int servoFlexAngle = 90; // flexed
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
#ifdef BATTERY_TEST
  float readBatteryVoltage() {
    // Read and scale ADC
    int raw = analogRead(batteryPin);
    float voltage = raw * (3.3f / 1023.0f);  // Teensy 10-bit ADC

    // Add to samples circular buffer
    samples[sampleIndex] = voltage;
    sampleIndex = (sampleIndex + 1) % numSamples;

    // Compute moving average
    float sum = 0;
    for (int i = 0; i < numSamples; i++) {
        sum += samples[i];
    }
    float avgVoltage = sum / numSamples;

    return avgVoltage;
}

float getBatteryPercent(float voltage) {
    float percent = (voltage - deadValue) / batteryDivisor * 100.0f;
    percent = constrain(percent, 0, 100);

    // Clamp very low percentages to 0 to avoid flicker
    //if (percent < 2.0f) percent = 0.0f;

    // Hysteresis: update only if change is significant
    if (abs(percent - lastPercent) >= hysteresisThreshold) {
        lastPercent = percent;
    }

    return lastPercent;
}

#endif
  /*******************************************************************************
   * MAIN                                                                        *
   ******************************************************************************/
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

        #ifdef EMG_EXTRACT_DATA
            pinMode(analogPinSensor, INPUT);
            analogReadResolution(10);
            Serial.begin(115200);
        #endif

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

        #ifdef BATTERY_TEST
            Serial.begin(115200);
            pinMode(batteryPin, INPUT);
            pinMode(batteryTogglePin, OUTPUT);
            digitalWrite(batteryTogglePin, HIGH); //enable battery measurement

            // Initialize samples array with initial readings
            float initVoltage = analogRead(batteryPin) * (3.3f / 1023.0f);
            for (int i = 0; i < numSamples; i++) samples[i] = initVoltage;

            tft.begin();
            tft.setRotation(-1);
            tft.fillScreen(ILI9341_BLACK);
            tft.setTextColor(ILI9341_YELLOW);
            tft.setFont(AwesomeF200_20);
    
        #endif

        #ifdef EMG_SERVO_INTEGRATION
            Serial.begin(115200);
            analogReadResolution(10);
            pinMode(analogPinSensor,INPUT);

            pinMode(servoPin, OUTPUT);
            brakeServo.attach(servoPin);
            brakeServo.write(servoRestAngle);
            delay(1000);
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
            if (count % 2 == 0) { //every other second
                brakeServo.write(115); //UNBRAKED
                delay(10);
            } else {
                brakeServo.write(70); //BRAKED
                delay(10);
            }
            delay(4000);
            count++; 
        #endif

        #ifdef EMG_GRAPH_TEST
            sensorVal = analogRead(analogPinSensor);
            voltage = (float)sensorVal / divisor;

            //filter test
            filteredVal = alpha * voltage + (1 - alpha) * filteredVal;
            Serial.print(">");
            Serial.print("sEMG raw:");
            Serial.print(voltage, 2);
            Serial.println();

            Serial.print(">");
            Serial.print("sEMG filtered:");
            Serial.print(filteredVal, 2);
            Serial.println();
            delay(100);
        #endif

        #ifdef EMG_EXTRACT_DATA
            currentTime = micros();
            
            if (currentTime - previousTime >= interval) {
                previousTime = currentTime;
                sensorVal = analogRead(analogPinSensor);
                voltage = (float)sensorVal / divisor;

                Serial.print(currentTime);
                Serial.print(",");
                Serial.print(voltage, 2);
                Serial.println();
            }
        #endif

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

        #ifdef BATTERY_TEST

            //batteryTogglePin = HIGH; //enable battery measurement

            float voltage = readBatteryVoltage();
            if (voltage < 0.01f) voltage = 0.0f;  // clamp very small noise

            float percent = getBatteryPercent(voltage);

            Serial.print("Battery Voltage: ");
            Serial.println(voltage, 3);

            Serial.print("Battery Percent: ");
            Serial.println(percent, 1);

            delay(500);
        #ifdef EMG_SERVO_INTEGRATION
            int EMGval = analogRead(analogPinSensor);
            
            if (EMGval > flexThreshold) {
                brakeServo.write(servoFlexAngle);
            } else {
                brakeServo.write(servoRestAngle);                
            }

                delay(10);

        #endif
    }
#endif
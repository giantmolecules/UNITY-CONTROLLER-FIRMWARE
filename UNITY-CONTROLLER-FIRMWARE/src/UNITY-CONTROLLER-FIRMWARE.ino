/*
 * Project UNITY-CONTROLLER-FIRMWARE
 * Description: UDP controller for Unity
 * Author: Brett Ian Balogh
 * Email: brettbalogh@gmail.com
 * Date: April 11, 2017
 * License: CC-BY-SA-NC
 */

// Include Libraries

 #include <Particle.h>
 #include <neopixel.h>
 #include <simple-OSC.h>
 #include <Wire.h>
 #include <Adafruit_MMA8451.h>
 #include <Adafruit_Sensor.h>
 #include <Adafruit_DRV2605.h>

 #define PIXEL_PIN D6
 #define PIXEL_COUNT 5
 #define PIXEL_TYPE WS2812B

 #define TEMP_PIN A0
 #define LIGHT_PIN A1
 #define PIR_PIN D2
 #define PIEZO_PIN D3

 Adafruit_DRV2605 drv;
 Adafruit_MMA8451 mma = Adafruit_MMA8451();
 Adafruit_NeoPixel neo(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.

}

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

 // Define some stuff

 #define PIXEL_PIN D6
 #define PIXEL_COUNT 5
 #define PIXEL_TYPE WS2812B

 #define TEMP_PIN A0
 #define LIGHT_PIN A1
 #define PIR_PIN D2
 #define PIEZO_PIN D3
 #define STATUS_PIN 7

// Create an instance of the haptic driver
 Adafruit_DRV2605 drv;

 // Create an instance of the accelerometer
 Adafruit_MMA8451 mma = Adafruit_MMA8451();

 // Creat an instance of the neopixels
 Adafruit_NeoPixel pixels(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

// Create some variable to hold accelerometer data
double accelX = 0;
double accelY = 0;
double accelZ = 0;

// Variables for other sensors
int tempVal = 0;
int lightVal = 0;
int pirVal = 0;

// Create a variable to hold the time
int theTime;

// Create an instance of UDP
UDP Udp;

// Variable for local IP address
String localIpString;

// !!IMPORTANT!!
// Change IP address to match that of the computer running Unity
IPAddress outIp(10, 73, 24, 136);//your computer IP
unsigned int outPort = 8000; //computer incoming port

//
// Setup
//

void setup() {

    // Start the serial port for debugging
    Serial.begin(9600);

    // Set up the Accelerometer
    Serial.println("Adafruit MMA8451 test!");
    if (! mma.begin()) {
        Serial.println("Couldnt start");
        // next line blocks if accelerometer isn't found. Not desirable.
        // while (1);
    }
    Serial.println("MMA8451 found!");
    mma.setRange(MMA8451_RANGE_2_G);
    Serial.print("Range = "); Serial.print(2 << mma.getRange());
    Serial.println("G");

    // Set up cloud variables for accelerometer data
    Particle.variable("X", accelX);
    Particle.variable("Y", accelY);
    Particle.variable("Z", accelZ);

    // Set the local IP address
    localIpString = WiFi.localIP();

    // Start UDP
    Udp.begin(0);

    // Set up the onboard led
    pinMode(STATUS_PIN, OUTPUT);

    // Set up cloud variables for time and IP address
    Particle.variable("time", theTime);
    Particle.variable("localIP", localIpString);

    // Take contol of the RGB status LED
    RGB.control(true);
    RGB.brightness(255);

    // Start the Neopixels
    pixels.begin();
    pixels.show();
}

//
// Main loop
//

void loop() {

    // Turn all neopixels to WHITE
    for(int i = 0; i < PIXEL_COUNT; i++){
      pixels.setPixelColor(i, 255,255,255);
    }
    pixels.show();

    // Send time to Unity
    theTime = millis();
    // test
    theTime = 1000;
    //
    sendTimeToUnity(theTime);

    // Read temperature sensor & send
    tempVal = analogRead(TEMP_PIN);
    // test
    tempVal = 4095;
    //
    sendTempToUnity(tempVal);

    // Read photo sensor & send
    lightVal = analogRead(LIGHT_PIN);
    // test
    lightVal = random(255);
    //
    sendLightToUnity(lightVal);

    // Read PIR & send
    pirVal = digitalRead(PIR_PIN);
    // test
    pirVal = 1;
    //
    sendPirToUnity(pirVal);

    /* Get a new sensor event */
    sensors_event_t event;
    mma.getEvent(&event);
    // store accelerometer values
    accelX = event.acceleration.x;
    accelY = event.acceleration.y;
    accelZ = event.acceleration.z;
    // test
    accelX = 90.1;
    accelY = 180.2;
    accelZ = 270.3;
    // send accelerometer data to Unity
    sendAccelToUnity(accelX, accelY, accelZ);

    /* Get the orientation of the sensor */
    uint8_t orientation = mma.getOrientation();
    sendOrientationToUnity(orientation);

    Serial.println();
    delay(500);

}

//
//    functions
//

void sendTimeToUnity(int t){
    OSCMessage outMessage("/time");
    outMessage.addInt(t);
    outMessage.send(Udp,outIp,outPort);
    status();
}

void sendTempToUnity(int t){
    OSCMessage outMessage("/temp");
    outMessage.addInt(t);
    outMessage.send(Udp,outIp,outPort);
    status();
}

void sendLightToUnity(int l){
    OSCMessage outMessage("/light");
    outMessage.addInt(l);
    outMessage.send(Udp,outIp,outPort);
    status();
}

void sendPirToUnity(int p){
    OSCMessage outMessage("/motion");
    outMessage.addInt(p);
    outMessage.send(Udp,outIp,outPort);
    status();
}

void sendAccelToUnity(double x, double y, double z){
    OSCMessage outMessage("/accel");
    outMessage.addFloat(x);
    outMessage.addFloat(y);
    outMessage.addFloat(z);
    outMessage.send(Udp,outIp,outPort);
    status();
}

void sendOrientationToUnity(uint8_t o){
    int orientOut=8;
    switch (o) {
    case MMA8451_PL_PUF:
    Serial.println("Portrait Up Front");
    orientOut = 1;
    // RED
    RGB.color(255,0,0);
    break;
    case MMA8451_PL_PUB:
    Serial.println("Portrait Up Back");
    orientOut = 2;
    // GREEN
    RGB.color(0,255,0);
    break;
    case MMA8451_PL_PDF:
    Serial.println("Portrait Down Front");
    orientOut = 3;
    // BLUE
    RGB.color(0,0,255);
    break;
    case MMA8451_PL_PDB:
    Serial.println("Portrait Down Back");
    orientOut = 4;
    // YELLOW
    RGB.color(255,255,0);
    break;
    case MMA8451_PL_LRF:
    Serial.println("Landscape Right Front");
    orientOut = 5;
    // CYAN
    RGB.color(0,255,255);
    break;
    case MMA8451_PL_LRB:
    Serial.println("Landscape Right Back");
    orientOut = 6;
    // MAGENTA
    RGB.color(255,0,255);
    break;
    case MMA8451_PL_LLF:
    Serial.println("Landscape Left Front");
    orientOut = 7;
    // WHITE
    RGB.color(255,255,255);
    break;
    case MMA8451_PL_LLB:
    Serial.println("Landscape Left Back");
    orientOut = 8;
    // OFF
    RGB.color(0,0,0);
    break;
    }
    OSCMessage outMessage("/orient");
    outMessage.addInt(orientOut);
    outMessage.send(Udp,outIp,outPort);
    status();
}

void status(){
  digitalWrite(STATUS_PIN, HIGH);
  delay(100);
  digitalWrite(STATUS_PIN, LOW);
}

//
// End of Program
//

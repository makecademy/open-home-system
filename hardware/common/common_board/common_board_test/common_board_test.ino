/*************************************************** 
  This is a sketch to test all sensors/actuators of Open Home System
  
  Written by Marco Schwartz for Open Home System.
  BSD license, all text above must be included in any redistribution
 ****************************************************/
 
#include <ccspi.h>
#include <string.h>
#include <SPI.h>
#include "utility/debug.h"
#include <ohs.h>
#include "DHT.h"
 
// Define pins
int switchPin = 2;
int relayPin = A2;
int tempPin = 0;
int motionPin = 7;

// DHT11 sensor pins
#define DHTPIN 6 
#define DHTTYPE DHT11

// Create DHT11 instance
DHT dht(DHTPIN, DHTTYPE);

// Include camera & SD libraries
#include <Adafruit_VC0706.h>
#include <SdFat.h>
#include <SoftwareSerial.h>  

// Software serial
SoftwareSerial cameraconnection = SoftwareSerial(A0, A1);
Adafruit_VC0706 cam = Adafruit_VC0706(&cameraconnection);

// Setup
void setup() {
    // Init serial
    Serial.begin(115200);
   
    // Initialize DHT sensor
    dht.begin();
    
    // Set pins
    pinMode(relayPin,OUTPUT);
    
    // Try to locate the camera
  if (cam.begin()) {
    Serial.println("Camera found:");
  } else {
    Serial.println("Camera not found !");
    return;
  }
  
  // Set picture size
  cam.setImageSize(VC0706_640x480);
}

// Main loop
void loop() {
    
    /*** Contact sensor test ***/
    
    Serial.println("Starting contact sensor test ...");
    
    // Get contact sensor data
    int sensorValue = digitalRead(switchPin);
    
    // Put temperature on the serial port
    Serial.println(sensorValue,DEC);
    
    // Wait for 1 sec
    delay(1000);
    
    /*** Relay test ***/
    
    Serial.println("Starting relay test ...");
    
    // Change relay state
    Serial.println("Switching relay ON ...");
    digitalWrite(relayPin, HIGH);
    
    // Wait for 1 sec
    delay(1000);
    
    // Change relay state
    Serial.println("Switching relay OFF ...");
    digitalWrite(relayPin, LOW);
    
    // Wait for 1 sec
    delay(1000);
    
    /*** Temperature & Humidity test ***/
    
    Serial.println("Starting temp & humidity test ...");
    
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    
    Serial.print("Temperature: ");
    Serial.println(t);
    Serial.print("Humidity: ");
    Serial.println(h);
    Serial.println("");
    
     // Wait for 1 sec
    delay(1000);
    
     /*** Motion sensor test ***/
    
    Serial.println("Starting motion test ...");
    
    // Get contact sensor data
    int motionValue = digitalRead(motionPin);
    
    // Put temperature on the serial port
    Serial.println(motionValue,DEC);
    
    // Wait for 1 sec
    delay(1000);
    
    /*** Motion sensor test ***/
    
    Serial.println("Starting camera test ...");
    
    if (! cam.takePicture()) 
    Serial.println("Failed to snap!");
  else 
    Serial.println("Picture taken!");
  
  // Get the size of the image (frame) taken  
  uint16_t jpglen = cam.frameLength();
  Serial.print("Storing ");
  Serial.print(jpglen, DEC);
  Serial.print(" byte image.");
}

/*************************************************** 
  This is the sketch to test the contact sensor with
  the CC3000 WiFi chip
  
  Copyright Open Home Automation 2013.
  Licensed under CERN OHL v.1.2
  
  Written by Marco Schwartz for Open Home System.
 ****************************************************/
 
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <string.h>
#include <SPI.h>
#include "utility/debug.h"
#include <ohs.h>
 
// Sensor pin
const int sensor_pin = 2; 

// Registration
boolean detected = false;
boolean registered = false;
String sensor_id = "contact1";

// Sensor status
int old_sensor_value;
int sensor_value;

// Create CC3000 instances
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT, SPI_CLOCK_DIV2);
uint32_t ip = cc3000.IP2U32(IP1,IP2,IP3,IP4);

void setup() {
  Serial.begin(115200);
  
  // Initialise the module
  cc3000Init(cc3000);
  
  // Check registration status
  Serial.println("Already registered ?");
  String request = "device="+sensor_id+"&phase=registration&end";
  String result = sendRequest(request, cc3000, ip);
  
   if(result.startsWith("Device registered"))
    {
      Serial.println("Device already registered");
      registered = true;
      detected = true;
    }
        
  // Check detection status
  if (detected == false) {
    
    Serial.println("Already detected ?");
    String request = "device="+ sensor_id +"&phase=detection&end";
    String result = sendRequest(request, cc3000, ip);
    
    if(result.startsWith("Device already detected"))
    {
      Serial.println("Device already detected");
      detected = true;
    }
  }
  
   // First readout
  int sensor_value = digitalRead(sensor_pin);
  old_sensor_value = sensor_value;
  
  // End of setup
  Serial.println("Detected:");
  Serial.println(detected);
  Serial.println("Registered:");
  Serial.println(registered);
}

void loop() {
  
  // Detection phase 
  if (registered == false && detected == false)
  {
    Serial.println("Waiting for detection");
    
    String request = "device="+ sensor_id +"&phase=detection&end";
    String result = sendRequest(request, cc3000, ip);
    
    // Sensor detected ?
    if(result.startsWith("Device detected"))
    {
      Serial.println("Device detected");
      detected = true;
    }
  
  // Retry every second
  delay(1000);

  }
  
  // Registration phase 
  if (registered == false && detected == true)
  {
    Serial.println("Waiting for registration");
    String request = "device="+sensor_id+"&phase=registration&end";
    String result = sendRequest(request, cc3000, ip);
  
  // Request received ?  
  if(result.startsWith("Device registered"))
    {
      Serial.println("Sensor Registered");
      registered = true;
    }
  
  // Retry every second
  delay(1000);

  }
  
  // Normal operation
  if (registered == true) {
 
    // Read sensor
    int sensor_value = digitalRead(sensor_pin);
    
    // If the value didn't change, transmit nothing
    if (sensor_value == old_sensor_value) {
      delay(1000);
    }
    
    // Otherwise, connect to server to transmit data
    else {
       if (sensor_value == 1) {
          String request = "device="+sensor_id+"&data=On&phase=receive&end";
          String result = sendRequest(request, cc3000, ip);
       }
       else {
          String request = "device="+sensor_id+"&data=Off&phase=receive&end";
          String result = sendRequest(request, cc3000, ip);
       }
    
    old_sensor_value = sensor_value;
    
    }
  }
  
}

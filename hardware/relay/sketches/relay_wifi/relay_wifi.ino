/*************************************************** 
  This is the sketch to test the relay controller 
  with the CC3000 WiFi chip
  
  Written by Marco Schwartz for Open Home System.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

//#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <string.h>
#include <SPI.h>
#include "utility/debug.h"
#include <ohs.h>

// Relay pin
int relay_pin = 8;

// Registration
boolean detected = false;
boolean registered = false;
String sensor_id = "led1";

// Create CC3000 instances
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT, SPI_CLOCK_DIV2);                                                        
uint32_t ip = cc3000.IP2U32(IP1,IP2,IP3,IP4);

void setup(void)
{

  pinMode(relay_pin,OUTPUT);
  
  Serial.begin(115200);
  
  // Initialise the module  
  cc3000Init(cc3000);
  
  // Check registration status
  Serial.println("Already registered ?");
  String request = "device="+ sensor_id +"&phase=registration&end=a"; 
  
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
    String request = "device="+ sensor_id +"&phase=detection&end=a";
    
    String result = sendRequest(request, cc3000, ip);
    if(result.startsWith("Device already detected"))
    {
      Serial.println("Device already detected");
      detected = true;
    }
  }
  
  // End of setup
  Serial.println("Detected:");
  Serial.println(detected);
  Serial.println("Registered:");
  Serial.println(registered);
}

void loop(void)
{
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
    
    String request = "device="+sensor_id+"&phase=command&end";
    String result = sendRequest(request, cc3000, ip);
  
      // Request received ?  
      if(result.startsWith("Off"))
      {
         digitalWrite(relay_pin,LOW);
      }
      
      if(result.startsWith("On"))
      {
         digitalWrite(relay_pin,HIGH);
      }
     }
   
   delay(100);  
}

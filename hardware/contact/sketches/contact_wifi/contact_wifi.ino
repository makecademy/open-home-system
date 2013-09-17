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
const int sensor_pin = 7; 

// Registration
boolean detected = false;
boolean registered = false;
String sensor_id = "contact1";

int old_sensor_value;
int sensor_value;

char outBuf[64];
char inBuf[64];

// Create CC3000 instances
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT, SPI_CLOCK_DIV2);
Adafruit_CC3000_Client client;                                                         
uint32_t ip = cc3000.IP2U32(IP1,IP2,IP3,IP4);

void setup() {
  Serial.begin(115200);
  
  // Initialise the module
  Serial.println(F("\nInitializing..."));
  if (!cc3000.begin())
  {
    Serial.println(F("Couldn't begin()! Check your wiring?"));
    while(1);
  }

  // Connect to  WiFi network
  cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY);
  Serial.println(F("Connected!"));
    
  // Display connection details
  Serial.println(F("Request DHCP"));
  while (!cc3000.checkDHCP())
  {
    delay(100); // ToDo: Insert a DHCP timeout!
  }
  
  cc3000.printIPdotsRev(ip);
  
  // Check registration status
  Serial.println("Already registered ?");
  String request = "device="+ sensor_id +"&phase=registration&end=a";
  request.toCharArray(outBuf,request.length()); 
  
  String result = send_request();
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
    request.toCharArray(outBuf,request.length()); 
    
    String result = send_request();
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
    request.toCharArray(outBuf,request.length()); 
    
    String result = send_request();
  
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
    request.toCharArray(outBuf,request.length()); 
    
    String result = send_request();
  
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
          String request = "device="+sensor_id+"&data=On&end";
          request.toCharArray(outBuf,request.length()); 
          String result = send_request();
       }
       else {
          String request = "device="+sensor_id+"&data=Off&end";
          request.toCharArray(outBuf,request.length()); 
          String result = send_request();
       }
    
    old_sensor_value = sensor_value;
    
    }
  }
  
}

String send_request () {
    
  unsigned long startTime;  
    
  Serial.println(F("\r\nAttempting connection..."));
    startTime = millis();
    do {
      client = cc3000.connectUDP(ip, SERVER_PORT);
    } while((!client.connected()) &&
            ((millis() - startTime) < connectTimeout));

    if(client.connected()) {
      Serial.print(F("connected!\r\nIssuing request..."));
      
      // Assemble and issue request packet
      client.write(outBuf, sizeof(outBuf));
      
      Serial.print(F("\r\nAwaiting response..."));
      startTime = millis();
      while((!client.available()) &&
            ((millis() - startTime) < responseTimeout));
      if(client.available()) {
        client.read(inBuf, sizeof(inBuf));
        Serial.println(inBuf);
      }

      client.close();
    }
    
    Serial.println("Time to connect, send & read:");
    Serial.println(millis() - startTime);
    
    return String(inBuf);
    
}

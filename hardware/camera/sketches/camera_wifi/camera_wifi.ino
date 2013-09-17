/*************************************************** 
  This is a sketch to test the camera module with the CC3000 WiFi chip
  
  Written by Marco Schwartz for Open Home System.
  Code inspired by the work done on the Adafruit_VC0706 & CC3000 libraries
  BSD license, all text above must be included in any redistribution
 ****************************************************/

// Include camera & SD libraries
#include <Adafruit_VC0706.h>
#include <SdFat.h>
#include <SoftwareSerial.h>
#include <ccspi.h>
#include <string.h>
#include <SPI.h>
#include "utility/debug.h"
#include <ohs.h>

// Software serial
SoftwareSerial cameraconnection = SoftwareSerial(2, 4);
Adafruit_VC0706 cam = Adafruit_VC0706(&cameraconnection);

// Create CC3000 instances
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT, SPI_CLOCK_DIV4);                                                        
uint32_t ip = cc3000.IP2U32(IP1,IP2,IP3,IP4);

void setup() {
 
  Serial.begin(115200);
  Serial.println("Camera test");
  
  // Try to locate the camera
  if (cam.begin()) {
    Serial.println("Camera found:");
  } else {
    Serial.println("Camera not found !");
    return;
  }
  
  // Set picture size
  cam.setImageSize(VC0706_640x480);
  
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

  if (! cam.takePicture()) 
    Serial.println("Failed to snap!");
  else 
    Serial.println("Picture taken!");
  
  // Get the size of the image (frame) taken  
  uint16_t jpglen = cam.frameLength();
  Serial.print("Storing ");
  Serial.print(jpglen, DEC);
  Serial.print(" byte image.");
  
  // Prepare request
  String start_request = "";
  String end_request = "";
  start_request = start_request + "\n" + "--AaB03x" + "\n" + "Content-Disposition: form-data; name=\"picture\"; filename=\"CAM.JPG\"" + "\n" + "Content-Type: image/jpeg" + "\n" + "Content-Transfer-Encoding: binary" + "\n" + "\n";  
  end_request = end_request + "\n" + "--AaB03x--" + "\n";
  
  uint16_t extra_length;
  extra_length = start_request.length() + end_request.length();
  Serial.println("Extra length:");
  Serial.println(extra_length);
  
  uint16_t len = jpglen + extra_length;
  
  Serial.println("Full request:");
   Serial.println(F("POST /ohs/camera.php HTTP/1.1"));
   Serial.println(F("Host: 169.254.251.235:8887"));
   Serial.println(F("Content-Type: multipart/form-data; boundary=AaB03x"));
   Serial.print(F("Content-Length: "));
   Serial.println(len);
   Serial.print(start_request);
   Serial.print("binary data");
   Serial.print(end_request);
 
  Serial.println("Starting connection to server...");
  Adafruit_CC3000_Client client = cc3000.connectTCP(ip, 8887);
  
  if (client.connected()) {
      Serial.println("Connected !");
      client.println(F("POST /ohs/camera.php HTTP/1.1"));
      client.println(F("Host: 169.254.251.235:8887"));
      client.println(F("Content-Type: multipart/form-data; boundary=AaB03x"));
      client.print(F("Content-Length: "));
      client.println(len);
      client.print(start_request);
            
      // Read all the data up to # bytes!
      byte wCount = 0; // For counting # of writes
      while (jpglen > 0) {
        
        uint8_t *buffer;
        uint8_t bytesToRead = min(16, jpglen); // change 32 to 64 for a speedup but may not work with all setups!
        
        buffer = cam.readPicture(bytesToRead);
        client.write(buffer, bytesToRead);
    
        if(++wCount >= 64) { // Every 2K, give a little feedback so it doesn't appear locked up
          Serial.print('.');
          wCount = 0;
        }
        jpglen -= bytesToRead; 
        delay(10);
      }
      
      client.print(end_request);
      client.println();
      
  Serial.println("Transmission over");
  } 
  
  else {
      Serial.println(F("Connection failed"));    
    }
    
     while (client.connected()) {

      while (client.available()) {

    // Read answer
        char c = client.read();
        Serial.print(c);
        //result = result + c;
      }
    }
    client.close();
  
}

void loop() {
}


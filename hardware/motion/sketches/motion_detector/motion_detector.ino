/*************************************************** 
  This is the sketch to test the wireless motion detector 
  using the Parallax PIR sensor with the CC3000 WiFi chip
  
  Written by Marco Schwartz for Open Home System.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"

// Define CC3000 chip pins
#define ADAFRUIT_CC3000_IRQ   3
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10

// WiFi network
#define WLAN_SSID       "QL2"           // cannot be longer than 32 characters!
#define WLAN_PASS       "5145246333"
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2

// Sensor pin
const int sensor_pin = 8; 

// Sensor readout
int old_sensor_value;
int sensor_value;

// Registration
boolean detected = false;
boolean registered = false;
String sensor_id = "motion1";

// Server IP
char server[] = "192.168.1.133";
int port = 8888;

// Create CC3000 object
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIV2);
                                         
uint32_t ip = cc3000.IP2U32(192,168,1,179);

/**************************************************************************/
/*!
    @brief  Sets up the HW and the CC3000 module (called automatically
            on startup)
*/
/**************************************************************************/

void setup(void)
{
  Serial.begin(115200);
  //displayFreeRam();
  
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

  while (! displayConnectionDetails()) {
    delay(1000);
  }
  
  cc3000.printIPdotsRev(ip);
  
  // Check registration status
  Serial.println("Already registered ?");
  String request = "GET /ohs/server.php?device="+ sensor_id +"&phase=registration HTTP/1.0";
  char charBuf[request.length()];
  request.toCharArray(charBuf,request.length()); 
  
  String result = send_request(charBuf);
   if(result.endsWith("Device registered"))
    {
      Serial.println("Device already registered");
      registered = true;
      detected = true;
    }
    
  // Check detection status
  if (detected == false) {
    
    Serial.println("Already detected ?");
    String request = "GET /ohs/server.php?device="+ sensor_id +"&phase=detection HTTP/1.0";
    char charBuf[request.length()];
    request.toCharArray(charBuf,request.length()); 
    
    String result = send_request(charBuf);
    if(result.endsWith("Device already detected"))
    {
      Serial.println("Device already detected");
      detected = true;
    }
  }
  
  // First readout
  int sensor_value = digitalRead(8);
  old_sensor_value = sensor_value;
  
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
    String request = "GET /ohs/server.php?device="+ sensor_id +"&phase=detection HTTP/1.0";
    char charBuf[request.length()];
    request.toCharArray(charBuf,request.length()); 
    
    String result = send_request(charBuf);
  
    // Sensor detected ?
    if(result.endsWith("Device detected"))
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
    String request = "GET /ohs/server.php?device="+sensor_id+"&phase=registration HTTP/1.0";
    char charBuf[request.length()];
    request.toCharArray(charBuf,request.length()); 
    
    String result = send_request(charBuf);
  
  // Request received ?  
  if(result.endsWith("Device registered"))
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
    int sensor_value = digitalRead(8);
    
    // If the value didn't change, transmit nothing
    if (sensor_value == old_sensor_value) {
      delay(1000);
    }
    
    // Otherwise, connect to server to transmit data
    else {
       if (sensor_value == 1) {
          String request = "GET /ohs/server.php?device="+sensor_id+"&data=On HTTP/1.0";
          char charBuf[request.length()];
          request.toCharArray(charBuf,request.length()); 
         String result = send_request(charBuf);
       }
       else {
          String request = "GET /ohs/server.php?device="+sensor_id+"&data=Off HTTP/1.0";
          char charBuf[request.length()];
          request.toCharArray(charBuf,request.length()); 
         String result = send_request(charBuf);
       }
    
    old_sensor_value = sensor_value;
    
    }
  }
}

/**************************************************************************/
/*!
    @brief  Tries to read the IP address and other connection details
*/
/**************************************************************************/
bool displayConnectionDetails(void)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;
  
  if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
    Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  }
  else
  {
    Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}

String send_request (const char *request) {
     
    // Connect
    String result;
    
    unsigned long connect_time;
    Serial.println("\nStarting connection to server...");
    connect_time = millis();
    Adafruit_CC3000_Client www = cc3000.connectTCP(ip, 8888);
    connect_time = millis() - connect_time;
    Serial.println("Time to connect:");
    Serial.println(connect_time);
    
    // Send request
    unsigned long request_time;
    request_time = millis();
    if (www.connected()) {
      www.fastrprintln(request);      
      Serial.println("Time for one GET:");
      Serial.println(millis() - request_time);
      www.fastrprintln(F(""));
    } 
    else {
      Serial.println(F("Connection failed"));    
      return "no answer";
    }
    request_time = millis() - request_time; 
    Serial.println("Time to send request:");
    Serial.println(request_time);
    
    unsigned long answer_time;
    answer_time = millis();
    
    while (www.connected()) {
      while (www.available()) {

    // Read answer
   //uint8_t buf[64];
   // memset(buf, 0, sizeof(buf));
        char c = www.read();
       // Serial.print(c);
        result = result + c;
      }
    }
    www.close();
    answer_time = millis() - answer_time; 
    Serial.println("Time to read answer:");
    Serial.println(answer_time);
    
    // Disconnect
    // Serial.println(F("\n\nDisconnecting"));
    //cc3000.disconnect();
    
    return result;
}

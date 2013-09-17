/*************************************************** 
  This is the sketch to test the DHT11 temperature & humidity sensor
  with the CC3000 WiFi chip
  
  Written by Marco Schwartz for Open Home System.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <ccspi.h>
#include <string.h>
#include <SPI.h>
#include "utility/debug.h"
#include <ohs.h>
#include "DHT.h"

// DHT11 sensor pins
#define DHTPIN 7 
#define DHTTYPE DHT11

// Registration
boolean detected = false;
boolean registered = false;
String sensor_id = "temp1";

// Create CC3000 instances
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT, SPI_CLOCK_DIV2);                                                        
uint32_t ip = cc3000.IP2U32(IP1,IP2,IP3,IP4);

// Create DHT11 instance
DHT dht(DHTPIN, DHTTYPE);

void setup(void)
{

  // Initialize DHT sensor
  dht.begin();
  
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
    
    // Measure the humidity & temperature
    //float h = dht.readHumidity();
    float temp = dht.readTemperature();
    
    String temperature = String((int) temp);

    String request = "device="+sensor_id+"&data="+ temperature +"&end";
    String result = sendRequest(request, cc3000, ip);
    
    delay(1000);
    
  }

}

#include "DHT.h"
#include<stdlib.h>

#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11 

DHT dht(DHTPIN, DHTTYPE);

char tempBuf[5];
char humBuf[5];

// Setup
void setup(void)
{
 
   dht.begin();
  
  Serial.begin(115200);
  
}

void loop(void)
{
  
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    
    Serial.println(t);
    Serial.println(h);
    
    dtostrf(t,5,2,tempBuf);
    
    String temperature = String(tempBuf);
   
    dtostrf(h,5,2,humBuf);
    
    String humidity = String(humBuf);
    
    Serial.println(temperature);
    Serial.println(humidity);
  
}

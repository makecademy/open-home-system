/*************************************************** 
  This is a sketch to test the contact sensor
  
  Written by Marco Schwartz for Open Home System.
  BSD license, all text above must be included in any redistribution
 ****************************************************/
 
const int sensor_pin = 7; // Sensor pin

void setup() {
  Serial.begin(115200);
}

void loop() {
  // Get sensor value
  int sensor_value = digitalRead(sensor_pin);
  
  // Print on serial
  Serial.println(sensor_value);
  delay(10);
}

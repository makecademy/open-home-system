/*************************************************** 
  This is a sketch to test the temperature measurement using the TMP36 sensor.
  
  Written by Marco Schwartz for Open Home System.
  BSD license, all text above must be included in any redistribution
 ****************************************************/
 
const int sensor_pin = 0; // Sensor on analog 0 pin

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Get sensor value
  int sensor_value = analogRead(sensor_pin);
  
  // Print on serial
  Serial.println(sensor_value);
  delay(10);
}

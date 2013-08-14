/*************************************************** 
  This is a sketch to test the relay controller board
  
  Written by Marco Schwartz for Open Home System.
  BSD license, all text above must be included in any redistribution
 ****************************************************/
 
const int relay_pin = 8; // Relay pin

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Activate relay
  digitalWrite(relay_pin, HIGH);
  
  // Wait for 1 second
  delay(1000);
  
   // Deactivate relay
  digitalWrite(relay_pin, LOW);
  
  // Wait for 1 second
  delay(1000);
}

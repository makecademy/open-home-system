const int sensor_pin = 8;

void setup() {
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  // Get sensor value
  int sensor_value = digitalRead(sensor_pin);
  
  // Print on serial
  Serial.println(sensor_value);
  delay(1000);
}

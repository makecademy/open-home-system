const int sensor_pin = 8;
int audio_pin = 7;

void setup() {
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  // Get sensor value
  int sensor_value = digitalRead(sensor_pin);
  
  // Make a sound if something is detected
    if (sensor_value){
        tone(audio_pin, 440, 10);
    }
  
  // Print on serial
  Serial.println(sensor_value);
  delay(10);
}

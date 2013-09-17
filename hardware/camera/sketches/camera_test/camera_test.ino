/*************************************************** 
  This is a sketch to test the camera module
  
  Written by Marco Schwartz for Open Home System.
  Code inspired by the work done on the Adafruit_VC0706 & SdFat libraries 
  BSD license, all text above must be included in any redistribution
 ****************************************************/

// Include camera & SD libraries
#include <Adafruit_VC0706.h>
#include <SdFat.h>
#include <SoftwareSerial.h>      

// Define SPI speed
const uint8_t spiSpeed = SPI_QUARTER_SPEED;

// SD Create instances
SdFat sd;
SdFile myFile;

// CS pin & software serial
#define chipSelect 10
SoftwareSerial cameraconnection = SoftwareSerial(2, 3);
Adafruit_VC0706 cam = Adafruit_VC0706(&cameraconnection);

void setup() {
  
  Serial.begin(9600);
  Serial.println("Camera test");
  
  // Init SD card
  if (!sd.begin(chipSelect,spiSpeed)) {
    Serial.println("SD card init failed");
    return;
  }

  else {
    Serial.println("SD card OK");
  }  
  
  // Try to locate the camera
  if (cam.begin()) {
    Serial.println("Camera found:");
  } else {
    Serial.println("Camera not found !");
    return;
  }
  
  // Set picture size
  cam.setImageSize(VC0706_640x480);        // biggest
 
  // Wait 3 seconds before taking a picture
  Serial.println("Picture in 3 seconds ...");
  delay(3000);

  if (! cam.takePicture()) 
    Serial.println("Failed to snap!");
  else 
    Serial.println("Picture taken!");
  
  // Create an image with the name IMAGExx.JPG
  char filename[13];
  strcpy(filename, "IMAGE00.JPG");
  for (int i = 0; i < 100; i++) {
    filename[5] = '0' + i/10;
    filename[6] = '0' + i%10;
    // create if does not exist, do not open existing, write, sync after write
    if (! sd.exists(filename)) {
      break;
    }
  }
  
  // Open the file for writing
  myFile.open(filename, O_CREAT | O_EXCL | O_WRITE);

  // Get the size of the image (frame) taken  
  uint16_t jpglen = cam.frameLength();
  Serial.print("Storing ");
  Serial.print(jpglen, DEC);
  Serial.print(" byte image.");

  int32_t time = millis();
  pinMode(8, OUTPUT);
  // Read all the data up to # bytes!
  byte wCount = 0; // For counting # of writes
  while (jpglen > 0) {
    // read 32 bytes at a time;
    uint8_t *buffer;
    uint8_t bytesToRead = min(32, jpglen); // change 32 to 64 for a speedup but may not work with all setups!
    buffer = cam.readPicture(bytesToRead);
    myFile.write(buffer, bytesToRead);
    if(++wCount >= 64) { // Every 2K, give a little feedback so it doesn't appear locked up
      Serial.print('.');
      wCount = 0;
    }
    //Serial.print("Read ");  Serial.print(bytesToRead, DEC); Serial.println(" bytes");
    jpglen -= bytesToRead;
  }
  myFile.close();

  time = millis() - time;
  Serial.println("done!");
  Serial.print(time); Serial.println(" ms elapsed");
}

void loop() {
}


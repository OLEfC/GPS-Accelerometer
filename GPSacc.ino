#include <stdio.h>
#include "MPU9250.h"
#include "Math.h"
#include "BluetoothSerial.h"
#include <TinyGPS++.h>

#define GPS_BAUDRATE 9600  // The default baudrate of NEO-6M is 9600
#define time_offset 7200  // define a clock offset of 7200 seconds (2 hour) ==> UTC + 2
TinyGPSPlus gps;  // the TinyGPS++ object



MPU9250 IMU(Wire,0x68);
int status;
const int ledPin = 32;





void setup() {
  pinMode (ledPin, OUTPUT);
  Serial.begin(9600);
  Serial2.begin(GPS_BAUDRATE);

  Serial.println(F("ESP32 - GPS module"));

  while(!Serial) {}
  status = IMU.begin();
  if (status < 0) {
    
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.println("Status: ");
    Serial.println(status);
    while(1) {}
  }


int get_acc() {
  float accX = IMU.getAccelX_mss();
  float accY = IMU.getAccelY_mss();
  float accZ = IMU.getAccelZ_mss();
  // Serial.print("accX:");
  // Serial.println(accX);
  // Serial.print("accY:");
  // Serial.println(accY);
  // Serial.print("accZ:");
  // Serial.println(accZ);
  int acc = (int)abs(9.9-sqrt(accX*accX + accY*accY + accZ*accZ));
  return acc;
}
void get_gps(){
    if (Serial2.available() > 0) {
    if (gps.encode(Serial2.read())) {
      if (gps.location.isValid()) {
        Serial.print(F("- latitude: "));
        Serial.println(gps.location.lat(), 7);

        Serial.print(F("- longitude: "));
        Serial.println(gps.location.lng(), 7);

        Serial.print(F("- altitude: "));
        if (gps.altitude.isValid())
          Serial.println(gps.altitude.meters(), 7);
        else
          Serial.println(F("INVALID"));
      } else {
        Serial.println(F("- location: INVALID"));
      }

      Serial.print(F("- speed: "));
      if (gps.speed.isValid()) {
        Serial.print(gps.speed.kmph());
        Serial.println(F(" km/h"));
      } else {
        Serial.println(F("INVALID"));
      }

      Serial.print(F("- GPS date&time: "));
      if (gps.date.isValid() && gps.time.isValid()) {
        Serial.print(gps.date.year());
        Serial.print(F("-"));
        Serial.print(gps.date.month());
        Serial.print(F("-"));
        Serial.print(gps.date.day());
        Serial.print(F(" "));
        Serial.print(gps.time.hour()+3);
        Serial.print(F(":"));
        Serial.print(gps.time.minute());
        Serial.print(F(":"));
        Serial.println(gps.time.second());
      } else {
        Serial.println(F("INVALID"));
      }

      Serial.println();
    }
  }

  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));
}


void loop() {
IMU.readSensor();
  
  int acc = get_acc();

  // Serial.print("Variable_2:");
  // Serial.println(acc);
  get_gps();

  if(acc>=20){
    digitalWrite (ledPin, HIGH);  // turn on the LED
    // Serial.print('1');
    // Serial.print('/n');
    // delay(20000);      
  }
  else{
    // Serial.print('0');
    // Serial.print('\n');
  }
  delay(10);
}

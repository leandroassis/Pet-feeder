/*
 * Project name:
     ESP-12F Routine
 * Revision History:
     - initial release
 * Description:
     This is a main code file of Pet Feeder control using an ESP-12F.
 * Made by:
      Leandro Assis dos Santos

 * Note:
      In this version there isn't server comunication and app integration.
      These features can be developed someday. 
*/

#include "defines.h"
#include "utils.h"
#include <LiquidCrystal.h>
#include <Wire.h>
#include "RTClib.h"
#include <Servo.h>


// RTC and Servo class inicialization
RTC_DS1307 rtc;
Servo servo1;

void setup() {
  Serial.begin(9600);
  Serial.print(STB);

  if(!Wire.begin(SDA, SCL)){
    Serial.print(ERR);
    LCDPrint("I2C bus inicialization error.\nPlease restart.");
    Serial.print(ETB);
  }

  if(!rtc.begin()){
    Serial.print(ERR);

    LCDPrint("Couldn't find RTC module.\nTake your device into maintenance.");

    Serial.print(ETB);
    abort();
  }

  pinMode(BALANCE, INPUT);
  pinMode(AXIS_MOTORS, OUTPUT);
  pinMode(SERVO_MOTORS, OUTPUT);
  digitalWrite(AXIS_MOTORS, LOW);
  servo1.attach(SERVO_MOTORS);
  servo1.write(90);
  servo1.detach();

  if(!rtc.isrunning()) rtc.adjust(DateTime(F(__DATE__), F(__TIME__));

  if(ReadEEPROM(0x00) == NULL){ // read the first memory byte
    Serial.print(ADD);
    AddFeedConfigs();
  }

  Serial.print(DONE);
  Serial.print(ETB);
}

void loop() {
  DateTime now = rtc.now();
  hour = now.hour();
  minutes = now.minute();

  amount = FoodTime(hour, minutes);
  if(amount){
    SpinAxis(amount);
  }

  if(Serial.read() == STB){
    do{
      incoming_data = Serial.read();

      switch (incoming_data)
      {
        case ADD:
          AddFeedConfigs();
          break;
        case REM:
          RemoveFeedConfigs()
          break;
        case GET:
          GetMemoryData(hour, minutes);
          break;
        case CLR:
          EraseMemory();
          break;
        default:
          break;
      }

    }while(incoming_data != ETB);
  }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
// Project:   DataLogger
// Version:   0.1
// Date:      17 February 2018
// Author:    Greg Howell <gjhmac@gmail.com>
// ------------------------------------------------------------------------------------------------------------------------------------------------
// Version    Comments
// 0.1        Initial Development
//
// ------------------------------------------------------------------------------------------------------------------------------------------------
// Description:
//  - Logs analog0 value to a text file on the SD card along with date/time stamp in CSV format
//  - Maintains date/time via DS1302 Real Time Clock
//  - Builds with Arduino 1.8.5
// ------------------------------------------------------------------------------------------------------------------------------------------------

// #includes
#include <SPI.h>    // Serial Peripheral Interface
#include <SD.h>     // SD Cards
#include <DS1302.h> // DS1302 RTC

const int chipSelect = 4;

// Init the DS1302
DS1302 rtc(2, 3, 4);
// ------------------------------------------------------------------------------------------------------------------------------------------------
// setup()
// ------------------------------------------------------------------------------------------------------------------------------------------------
void setup() {
  // Set the clock to run-mode, and disable the write protection
  rtc.halt(false);
  rtc.writeProtect(false);

  Serial.begin(9600);

  // Use following lines once to set clock if battery fails (modify to suit)
  //rtc.setDOW(SATURDAY); // Set Day-of-Week to FRIDAY
  //rtc.setTime(22, 0, 0); // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(24, 2, 2018); // Set the date to August 6th, 2010

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
// loop()
// ------------------------------------------------------------------------------------------------------------------------------------------------
void loop() {
  // make a string for assembling the data to log:
  String dataString = "";

  int sensor = analogRead(0);
  dataString += String(sensor);

  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(String(rtc.getDateStr())+","+String(rtc.getTimeStr())+","+dataString);
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
  // Wait ten seconds before repeating :)
  delay (10000);
}

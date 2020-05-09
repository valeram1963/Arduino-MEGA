//
//  DHT_SD_RTC_LCD_Test_VR.h
//  For Arduino Mega with Old version Datalogger
//  Thermo sensor DHT11
//  Created by Valerio Raminelli  on 6/5/2020.
//  Copyright (c) 2020 Valerio Raminelli All rights reserved.
//  valeram1919@gmail.com



// Done temperature, humidity reading from DHT11 on Arduino Mega
// Done read the data and show on LCD Display 2 lines 16 chars
// DONE store data in a log file using DataLogger
// Done set time stamp using RTC on LCD

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SD.h>
#include "DHT.h"
#include "RTClib.h"
#include <DataLoggerSD.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

#define DHTPIN 9     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
float t,h,f,hic;

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;

// Log file base name.  Must be six characters or less.
#define FILE_BASE_NAME "Data.txt"

const int chipSelect = 10;    
File dataFile;

RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup() {
  Serial.begin(9600);

  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("LCD Display");
  lcd.setCursor(0,1);
  lcd.print("on Mega by Valy");
  delay(2000);
  lcd.clear();
                                  // initialize the DHT11 temperature&Humidity
  dht.begin();


                                  // initialize the RTC
    if (! rtc.begin()) {
      Serial.println("Couldn't find RTC");
      while (1);
    }
  
    if (! rtc.isrunning()) {
      Serial.println("RTC is NOT running!");
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
     if (!SD.begin(10, 11, 12, 13)) {
  
      Serial.print("Card Present");
    }
    if (! rtc.isrunning()) {
      Serial.println("RTC is NOT running!");
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
                                                  // initialize the SD card loader on shield old
  
    Serial.print("\nInitializing SD card...");
   
    int SS=53;
    pinMode(SS, OUTPUT);
    
    while (!card.init(SPI_HALF_SPEED, 10, 11, 12, 13)){
      Serial.println("initialization failed. Things to check:");
      Serial.println("* is a card is inserted?");
      Serial.println("* Is your wiring correct?");
      Serial.println("* did you change the chipSelect pin to match your shield or module?");
    } 
  
    if (SD.begin(10, 11, 12, 13)) {
  
      Serial.print("Card Present!!!!!!!");
    }
    // print the type of card
    Serial.print("\nCard type: ");
    switch(card.type()) {
      case SD_CARD_TYPE_SD1:
        Serial.println("SD1");
        break;
      case SD_CARD_TYPE_SD2:
        Serial.println("SD2");
        break;
      case SD_CARD_TYPE_SDHC:
        Serial.println("SDHC");
        break;
      default:
        Serial.println("Unknown");
    }
  
    // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
    if (!volume.init(card)) {
      Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
      return;
    }
  /*
    // print the type and size of the first FAT-type volume
    uint32_t volumesize;
    Serial.print("\nVolume type is FAT");
    Serial.println(volume.fatType(), DEC);
    Serial.println();
    
    volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
    volumesize *= volume.clusterCount();       // we'll have a lot of clusters
    volumesize *= 512;                            // SD card blocks are always 512 bytes
    Serial.print("Volume size (bytes): ");
    Serial.println(volumesize);
    Serial.print("Volume size (Kbytes): ");
    volumesize /= 1024;
    Serial.println(volumesize);
    Serial.print("Volume size (Mbytes): ");
    volumesize /= 1024;
    Serial.println(volumesize);
  */
    
    Serial.println("\nFiles found on the card (name, date and size in bytes): ");
                                              //Initialize and Open File
    
    
  //  root.openRoot(volume);
    
                                          // list all files in the card with date and size
   // root.ls(LS_R | LS_DATE | LS_SIZE);

    
                                        // open the file. note that only one file can be open at a time,
 /*                                       // so you have to close this one before opening another.
  dataFile = SD.open(FILE_BASE_NAME, FILE_WRITE);
  
                                        // if the file opened okay, write to it:
  if (dataFile) {
    Serial.print("Writing header to ");
    Serial.print(dataFile);
    Serial.println("Test Day,Month,Year,Hour,Min,SecTemperature,Humidity,HeatIndex,");
    String dataString = ("Day,Month,Year,Hour,Min,SecTemperature,Humidity,HeatIndex,");
    dataFile.print(dataString);                                  // close the file:
    dataFile.close();
    Serial.println("done.");
  } else {
                                          // if the file didn't open, print an error:
    Serial.println("error opening ");
    Serial.print(dataFile);
    
  }
 */                                             
      //String dataString;
      //pin 4, 15 minutes between readings, and pin 3 as an interrupt pin
      DataLoggerSD logger(chipSelect, 15, 3); 
                                        // Open up the file we're going to log to!
      dataFile = SD.open(FILE_BASE_NAME, FILE_WRITE);
      String dataString = ("Day/Month/Year,Hour:Min:Sec,Temperature,Humidity,HeatIndex");
       Serial.println(dataString);
       dataFile.println(dataString);
       dataFile.flush();  
       
      if (! dataFile) {
        Serial.println("error opening datalog.txt");
                                      // Wait forever since we cant write data
        while (1) ;
        }
     
                                     
                                         //Initialize date and time
      
      DateTime now = rtc.now();
  
      Serial.print(now.year(), DEC);
      Serial.print('/');
      Serial.print(now.month(), DEC);
      Serial.print('/');
      Serial.print(now.day(), DEC);
      Serial.print(" (");
      Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
      Serial.print(") ");
      Serial.print(now.hour(), DEC);
      Serial.print(':');
      Serial.print(now.minute(), DEC);
      Serial.print(':');
      Serial.print(now.second(), DEC);
      Serial.println();


 
}


void dataLoggerSD(){
      String dataString;
       int sensor =0;                                   // make a string for assembling the data to log:
  
    DateTime now = rtc.now();
  
    dataString  =now.day();
     dataString += "/"; 
    dataString  +=now.month();
     dataString += "/"; 
    dataString  +=now.year();
     dataString += ","; 
     dataString +=now.hour();
     dataString += ":"; 
    dataString  +=now.minute();
     dataString += ":"; 
    dataString  +=now.second();
     dataString += ","; 
    sensor=t;
    dataString += String(sensor);
    dataString += ","; 
    sensor=h;
    dataString += String(sensor);
    dataString += ","; 
    sensor=hic;
    dataString += String(sensor);
    dataFile.println(dataString);

  // print to the serial port too:
  Serial.println(dataString);
  
  // The following line will 'save' the file to the SD card after every
  // line of data - this will use more power and slow down how much data
  // you can read but it's safer! 
  // If you want to speed up the system, remove the call to flush() and it
  // will save the file only every 512 bytes - every time a sector on the 
  // SD card is filled with data.
  dataFile.flush();
  
  // Take 1 measurement every 500 milliseconds
  delay(1000);
  
}


void dateTimeLCD(){ //Send date and time on LCD
 DateTime now = rtc.now();
 lcd.clear();
 lcd.backlight(); 
   /*
      Serial.print(now.year(), DEC);
      Serial.print('/');
      Serial.print(now.month(), DEC);
      Serial.print('/');
      Serial.print(now.day(), DEC);
      Serial.print(" (");
      Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
      Serial.print(") ");
      Serial.print(now.hour(), DEC);
      Serial.print(':');
      Serial.print(now.minute(), DEC);
      Serial.print(':');
      Serial.print(now.second(), DEC);
      Serial.println();
    */
   

  lcd.setCursor(0,0);
  lcd.print(now.hour());
  lcd.setCursor(2,0);
  lcd.print(":");
  lcd.setCursor(3,0);
  lcd.print(now.minute(),DEC);

  lcd.setCursor(0,1);
  lcd.print(now.day(),DEC);
  lcd.setCursor(2,1);
  lcd.print("/");
  lcd.setCursor(3,1);
  lcd.print(now.month(),DEC);
  lcd.setCursor(5,1);
  lcd.print("/");
  lcd.setCursor(6,1);
  lcd.print(now.year(),DEC);

  delay(5000);
  lcd.clear();
  lcd.noBacklight();

}

void tempHumLCD(){


  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
 
   h = dht.readHumidity();
  // Read temperature as Celsius (the default)
   t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
   f = dht.readTemperature(true);
 
 
 lcd.backlight();
 
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    
    lcd.setCursor(1,0);
    lcd.print("Failed to read DHT");
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
   

  
    // Compute heat index in Fahrenheit (the default)
   //float hif = dht.computeHeatIndex(f, h);
  
  // Compute heat index in Celsius (isFahreheit = false)
  hic = dht.computeHeatIndex(t, h, false);
 
  lcd.setCursor(0,0);
  lcd.print("Hum%|");
  lcd.setCursor(1,1);
  lcd.print(int(h));
  lcd.setCursor(5,0);
  lcd.print("Temp.|");
  lcd.setCursor(6,1);
  lcd.print(int(t)); 
  lcd.setCursor(11,0);
  lcd.print("HIdx");
  lcd.setCursor(12,1);
  lcd.print(int(hic)); 
  delay(5000);
  lcd.clear();
  lcd.noBacklight();

 /* 
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.println(F("°C "));
  //Serial.print(hif);
  //Serial.println(F("°F"));


*/
  
}

void loop() {
  
  dateTimeLCD();
  tempHumLCD();
  dataLoggerSD();

}

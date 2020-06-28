// temperature, humidity reading from DHT11 on Arduino Mega
// read data from MPU 6050
// read the data and show on LCD Display 2 lines 16 chars
// DONE set time stamp using RTC

// DONE store data in a log file using DataLogger dataDHT.txt file store all information on Date,time,Temp.,Humidity, HIndex.

#include <SPI.h>
#include<SD.h>
File myFile;

// change this to match your SD shield or module;
//     Arduino Ethernet shield: pin 4
//     Adafruit SD shields and modules: pin 10
//     Sparkfun SD shield: pin 8
const int chipSelect = 10;


#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h> 
Adafruit_MPU6050 mpu;


#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//********************TEMPERATURE AND HUMIDITY TEST
#include "DHT.h"
#define DHTPIN 9     // Digital pin connected to the DHT sensor


// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!

#define DHTTYPE DHT11   // DHT 11


//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);
float t, h, f, hic;
 
// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
// On MEGA Datashield SDA and SCL must be connected to pin 21 & 20 
#include "RTClib.h"
RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

  // following line sets the RTC to the date & time this sketch was compiled
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
 
 String dataString;


void setup() {
  Serial.begin(9600);
// initialize SD data logger SD card R/W
Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
  int SS=53;
  pinMode(SS, OUTPUT);

  if (!SD.begin(chipSelect, 11, 12, 13)) {
    Serial.println("initialization failed! VR");
    return;
  }
  Serial.println("initialization done.");

if (SD.exists("dataDHT.txt")) {
    Serial.println("dataDHT.txt exists.");
  }
  else {
    Serial.println("dataDHT.txt doesn't exist.");

  // open a new file and immediately close it:
  Serial.println("Creating dataDHT.txt...");
  myFile = SD.open("dataDHT.txt", FILE_WRITE);
  myFile.close();
  }



  
   // initialize the lcd !!!!!!!!!!!!!!!!!!!!!
  lcd.init();   
  // Print a message to the LCD.
  lcd.backlight(); 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("LCD Display");
  lcd.setCursor(0,1);
  lcd.println("on Mega by Valy ");
  delay(4000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("DHT_SD_RTC_Test");
  lcd.setCursor(0,1);
  lcd.print("MPU6050_VR_v3.0");
  delay(4000);
  lcd.clear();
 

 
  // Try to initialize DHT Sensor for Temp, Hum and Headindex!!!
 dht.begin();

  
   // Try to initialize MPU-5060!!!
   mpu.begin();
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }
  Serial.println("");




    // Try to initialize RTC for time and date!!!!!!!!!!!!!!!

  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    if (!rtc.begin()) {
      Serial.println("Couldn't find RTC");
      while (1);
    }
  
          if (! rtc.isrunning()) {
            Serial.println("RTC is NOT running!");
            rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
          }
     // following line sets the RTC to the date & time this sketch was compiled
     // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    //  This line sets the RTC with an explicit date & time, for example to set
    //  January 21, 2014 at 3am you would call:
    //  rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));


}

void dateTimeLCD(){
 DateTime now = rtc.now();
 lcd.clear();

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
      delay(10000);
      lcd.clear();

      
  
}

void tempHumLCD(){


  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
 
 h = dht.readHumidity();
  // Read temperature as Celsius (the default)
 t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
 f = dht.readTemperature(true);
 
 

 
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
  //float hic = dht.computeHeatIndex(t, h, false);
  hic = dht.computeHeatIndex(t, h, false);
  
  lcd.setCursor(0,0);
  lcd.print("Temp. ");
  lcd.setCursor(0,1);
  lcd.print(String(t));
  lcd.setCursor(6,0);
  lcd.print("Hum% ");
  lcd.setCursor(6,1);
  lcd.print(h); 
  lcd.setCursor(11,0);
  lcd.print("HIdx");
  lcd.setCursor(12,1);
  lcd.print(hic); 
  delay(10000);
  
  lcd.clear();
  lcd.setCursor(0,0);
  
  
/* The sprintf function will write the formatting string and the variables into the "data" character array.
 * You provide a formatting string template, that contains placeholders for variables that you plan to insert.
 * These placeholders have a percentage sign (%) prefix.  Eg.  %s, %d, %f etc.
 * The number of placeholders must match the number of variables.
 * The variables are inserted at the placeholder position, and formatted based on the type of placeholder used.
 * %d = signed integer               %f = floating point number  
 * %s = string                     %.1f = float to 1 decimal place
 * %c = character                  %.3f = float to 3 decimal places
 * %e = scientific notation          %g = shortest representation of %e or %f                
 * %u = unsigned integer             %o = unsigned octal
 * %x = unsigned hex (lowercase)     %X = unsigned hex (uppercase)
 * %hd = short int                  %ld = long int
 * %lld = long long int
 * =============================================================================  */
/*
char str[16];
int t2=t;
int h2=h;
int hic2=hic;
  
  //sprintf(str,"T=%hd H=%hd Ix=%hd",t2,h2,hic2); // use sprintf() to compose the string str
 sprintf(str,"T=%dH=%dIdx=%d",t2,h2,hic2); // use sprintf() to compose the string str
  lcd.setCursor(0,0);
  lcd.print(str);
  lcd.setCursor(0,1);
  lcd.print("    C    %     %");
  delay(4000);
  Serial.print(F("Hum. dht: "));
  Serial.print(h);
  Serial.print(F("%  Temp. dht: "));
  Serial.print(t);
  //Serial.print(F("°C "));
  //Serial.print(f);
  Serial.print(F("Heat index dht: "));
  Serial.print(hic);
 // Serial.println(F("°C "));
  //Serial.print(hif);
  //Serial.println(F("°F"));

*/

  
}
 void mpuDataLCD(){
  sensors_event_t a, g, temp;
   for (int i=0; i<=50; i+=1){
   
   /* Get new sensor events with the readings */
  
  mpu.getEvent(&a, &g, &temp);
  
  //lcd.backlight(); 
  /* Print out the values */
  //lcd.scrollDisplayLeft();
  //lcd.autoscroll();
  delay(100);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Acc.X: ");
  lcd.setCursor(0,1);
  lcd.print(a.acceleration.x);
  lcd.setCursor(7,0);
  lcd.print("Y: ");
  lcd.setCursor(7,1);
  lcd.print(a.acceleration.y);
  lcd.setCursor(11,0);
  lcd.print(" Z:");
  lcd.setCursor(12,1);
  lcd.print(a.acceleration.z);
  //lcd.println(" m/s^2");

   }
 for (int i=0; i<=50; i+=1){
 
  mpu.getEvent(&a, &g, &temp);
 delay(100);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Rot.X:");
  lcd.setCursor(0,1);
  lcd.print(g.gyro.x);
  lcd.setCursor(7,0);
  lcd.print("Y: ");
  lcd.setCursor(7,1);
  lcd.print(g.gyro.y);
  lcd.setCursor(11,0);
  lcd.print(" Z: ");
  lcd.setCursor(12,1);
  lcd.print(g.gyro.z);

 }
  
  mpu.getEvent(&a, &g, &temp);
  //Serial.println(" rad/s");
   delay(1000);
    lcd.clear();
 lcd.setCursor(0,0);
  lcd.print("Temp. MPU6050: ");
   lcd.setCursor(0,1);
  lcd.print(temp.temperature);
  lcd.print(" C");

  Serial.println("");
  delay(1000);
  

 }
 
void dataLoggerSD() { //Store data in csv on sd card
   
    int sensor = 0;                                  // make a string for assembling the data to log:


  
    DateTime now = rtc.now();
  
    dataString  = now.day();
    dataString += "/";
    dataString  += now.month();
    dataString += "/";
    dataString  += now.year();
    dataString += ",";
    dataString += now.hour();
    dataString += ":";
    dataString  += now.minute();
    dataString += ":";
    dataString  += now.second();
    dataString += ",";
    sensor = t;// temperature from DHT11 sensor
    dataString += String(sensor);
    dataString += ",";
    sensor = h;// humidity from DHT11 sensor
    dataString += String(sensor);
    dataString += ",";
    sensor = hic;// heatindex from DHT11 sensor
    dataString += String(sensor);
//    Serial.println();
//    Serial.print("Hic: ");
//    Serial.print(sensor);
//    Serial.println();
  
  // print to the serial port too:
  Serial.println("Datastring to be record in SD card: Date,Time,Temperature,Humidity,Heat indx");
  Serial.println(dataString);// the sring will be write in a file on SD card ( still to be done)
 myFile = SD.open("dataDHT.txt", FILE_WRITE);
  
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to dataDHT.txt...");
    myFile.println(dataString);
  // close the file:
    myFile.close();
    Serial.println("Writing data done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening dataDHT.txt");
  }
 
  // Take 1 measurement every 500 milliseconds
  delay(10000);

}


void loop() {
  
  dateTimeLCD();//dysply data and time on LCD
 // mpuDataLCD(); //display accellerometer and XYZ rotation angular accelleration on LCD
  tempHumLCD(); //Display Temperature, Humidity and Healt Temprature Index
  dataLoggerSD();//Store Data,Time,temperature,Humidity and Health Temperature Index on dataDHT.txt file on SD Card.

  

}

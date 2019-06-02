#include <Wire.h>  
#include <LiquidCrystal_I2C.h> 
#include "dht.h"
#include <SPI.h>
#include <SD.h>

#define dht_apin A0
const int chipSelect = 10;
dht DHT;

LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE); // Initialize LCD Display at address 0x27 


void setup()
{
 lcd.begin (16,2);
 lcd.backlight();

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }



  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    while (1);
  }
  Serial.println("card initialized.");

  
    
}
 
void loop()
{
  DHT.read11(dht_apin);
  File dataFile = SD.open("temp.txt", FILE_WRITE);
  String dataString = "";
  
  lcd.setCursor(0,0); // Move cursor to 0
  lcd.print(" Humidity : ");
  lcd.print( DHT.humidity);
  lcd.print("% ");
  lcd.setCursor(0,1);
  
  lcd.print(" temperature : ");
  lcd.print( DHT.temperature);
  lcd.println("C ");
  
  dataString += "Humidity : ";
  dataString += String(DHT.humidity);
  dataString += "%  - and - ";
  dataString += " Temperature: ";
  dataString += String(DHT.temperature);
  dataString += "C ";
  
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
   
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening temp.txt");
  }
  
  delay(5000); 
 
}

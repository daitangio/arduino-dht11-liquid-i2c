// -*- mode:c++; mode: flymake -*-
#include <Wire.h>

#include <LiquidCrystal_I2C.h>
#include <SimpleDHT.h>
#include <Ultrasonic.h>
/**
 * See README.md for wiring information parts and so on
 * 
 * HC RS04 ultrasonic sensor is used to detect presence and turn on oww backlite
 */
Ultrasonic ultrasonic(11 /*TRIG*/ , 10 /*ECHO*/, 40000UL /*TIMEOUT*/);


LiquidCrystal_I2C lcd(0x3F,20,4); // set the LCD address to 0x27 for a 20 chars and 4 line display

// for DHT11, 
//      VCC: 5V or 3V
//      GND: GND

SimpleDHT11 dht11;

void readTemperature(){
  static int readCounter=0;
  int pinDHT11 = 13; // pin for dht sensor
    // start working...
  Serial.println("=================================");
  Serial.println("Sample DHT11...");
  
  // read without samples.
  byte temperature = 0;
  byte humidity = 0;
  int err = dht11.read(pinDHT11, &temperature, &humidity, NULL);
  if (err  != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.println(err);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Sensor err="); lcd.println((int)err);
    delay(900);
    return;
  }
  

  // Push it to lcd
  lcd.clear();
  // set the cursor to column 0, line 0
  lcd.setCursor(0, 0);
  lcd.print((int)temperature);
  lcd.print(" C -- "); 
  //lcd.setCursor(0, 1);
  lcd.print((int)humidity);
  lcd.print(" % Humyd");

  lcd.setCursor(0,3);
  lcd.print("Samples so far:");
  lcd.print((int)++readCounter);
  
}



void setup()

{

lcd.init(); 

lcd.backlight();

}

void loop(){
  delay(1000); // Sensor init take some time
  lcd.backlight();
  lcd.setCursor(0,2);
  lcd.print(" Scan...");
  readTemperature();

  unsigned long startTime=millis();
  /** To count time it is tricky:
   * we do stuff until 70 sec are passed
   */
  while( millis() < (startTime+70000) ) {
    lcd.setCursor(0, 1);
    lcd.print("Time:");
    lcd.print(startTime);
    
    unsigned int distCM=ultrasonic.distanceRead();  
    lcd.setCursor(0,2);
    lcd.print("... Dist:");
    lcd.print((unsigned int)distCM);
    lcd.print(" cm ");
    if(distCM <=26 ) {
      lcd.backlight();
      delay(35000); // Let it look for a while
    }else{
      lcd.noBacklight();
      delay(1300);
    }

  }

}


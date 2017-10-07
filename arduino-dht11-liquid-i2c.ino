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

byte tempHistory[3], humHistory[3];

/**
 * Store last three measure and show it on display
 *
 */
void readTemperature(){
  static int historyCount=0;
  static int readCounter=0;
  //static unsigned long startTimeShift=millis();
  
  
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

  lcd.clear();
  
  // shift stored data
  tempHistory[2]=tempHistory[1];
  tempHistory[1]=tempHistory[0];
  humHistory[2]=humHistory[1];
  humHistory[1]=humHistory[0];
  
  tempHistory[0]=temperature;
  humHistory[0]=humidity;

  // Show all the data in two lines
  

  // Push it to lcd  
  // set the cursor to column 0, line 0
  lcd.setCursor(0, 0);

  for(int i=0; i<=2; i++){
    if (i==2) {
      lcd.setCursor(0, 1);
    }
    lcd.print((int) tempHistory[i] );
    lcd.print(" C ");    
    lcd.print((int) humHistory[i]);
    lcd.print("% ");
  }  
  
  lcd.setCursor(0,3);
  lcd.print("Samp:");
  lcd.print((int)++readCounter);
  

  // Update history management
  if(++historyCount > (3-1) ) { historyCount=0; };
}



void setup()

{

lcd.init(); 

lcd.backlight();

}

void loop(){
  delay(900); // Sensor init take some time
  lcd.backlight();
  unsigned long backlightStart=millis();
  while(true) {
      
    readTemperature();

    unsigned long startTime=millis();
    /** To count time it is tricky:
     * we do stuff until 80 sec are passed then we issue a temperature rescan
     */
    while( millis() < (startTime+120000) ) {
      // Register passed time
      lcd.setCursor(11,3);
      lcd.print(" m:");
      float m= (((float)millis())/(float)60000);
      lcd.print((float) m);

      delay(900);
      
      // Trun off after some seconds please
      long countDownLight=millis()-1*(backlightStart+15000);
      //lcd.setCursor(0, 1);
      //lcd.print("Turn off:");
      //lcd.print(countDownLight);
      if(countDownLight < 0) {
        // lcd.print("     ");
      }else{
        // We measure only when light is off
        //lcd.print(" off?");
        lcd.noBacklight();
        
        unsigned int distCM=ultrasonic.distanceRead();  
        lcd.setCursor(0,2);
        lcd.print("Dist:");
        lcd.print((unsigned int)distCM);
        lcd.print(" cm ");
        if(distCM !=0) {
          if(distCM <=20 ) {
            lcd.backlight();
            lcd.print("ON   ");
            backlightStart=millis();
          }else{
            lcd.print(" OFF ");
          }
        }else {
          // BAD Measure
        }       
      }
    } // while 
  } // while(true)
}


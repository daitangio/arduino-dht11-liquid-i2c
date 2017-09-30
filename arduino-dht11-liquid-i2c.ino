#include <Wire.h>

#include <LiquidCrystal_I2C.h>
#include <SimpleDHT.h>

LiquidCrystal_I2C lcd(0x3F,20,4); // set the LCD address to 0x27 for a 20 chars and 4 line display

// for DHT11, 
//      VCC: 5V or 3V
//      GND: GND

SimpleDHT11 dht11;

void readTemperature(){
  static int readCounter=0;
  int pinDHT11 = 2; // pin for dht sensor
    // start working...
  Serial.println("=================================");
  Serial.println("Sample DHT11...");
  
  // read without samples.
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.println(err);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("err="); lcd.println((int)err);
    delay(900);
    return;
  }
  
  Serial.print("Sample OK: ");
  Serial.print((int)temperature); Serial.print(" *C, "); 
  Serial.print((int)humidity); Serial.println(" H");

  // Push it to lcd
  lcd.clear();
  // set the cursor to column 0, line 0
  lcd.setCursor(0, 0);
  lcd.print((int)temperature);
  lcd.print(" C ");
 
  lcd.setCursor(0, 1);
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
  lcd.setCursor(0,2);
  lcd.print(" Scan...");
  readTemperature();
  lcd.setCursor(0,2);
  lcd.print(" by Giovanni Giorgi");
  delay(70000);   // 70 sec
}


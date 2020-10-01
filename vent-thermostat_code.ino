// Vent thermostat project 

// Author: Holden Meador
// Produced 9-30-2020 
// Check out my githup page @kerbalpro  also, HI MOM!

//This code should be used with a micro processor of your 
//choice (I used a nano), a lcd display, POT, a DHT11 sensor, and some form of 
//power supply etc. anker battery bank. 

//  **** NOTE: I am not a pro at coding as you will see bellow, good thing this is completly open source! *****


#include "DHT.h"                                     //just checked these out at the library ;)
#include "LiquidCrystal.h"
#include <Servo.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
Servo vent_control;

LiquidCrystal lcd(3, 4, 5, 6, 7, 8);                  //lcd pinnout

int set_temp = 70;                                     //This is the default set temp on start, set it to what u like 
const int up_button = 9;                               //pin  of button to increase set temp
const int down_button = 10;                            //pin of button to decrease set temp
const int low_temp_range = 1;                          //set to prevent over correction of temp adjustment
const int high_temp_range = 1;                         //set to prevent under correction of temp adjustment
const int BUILTIN = 13;                                //on board LED status 
const int lcd_backlight = 12;                          //pin of backlight for lcd
const int lcd_contrast = 19;                           //pin of contrast for lcd

void setup() {                                         //on start...
  
  pinMode(BUILTIN, OUTPUT);
  pinMode(up_button, INPUT_PULLUP);
  pinMode(down_button, INPUT_PULLUP);
  pinMode(lcd_backlight, OUTPUT);
  pinMode(lcd_contrast, OUTPUT);
  digitalWrite(lcd_contrast, LOW);
  digitalWrite(lcd_backlight, HIGH);
  dht.begin();
  lcd.begin(0, 2);
  lcd.print("Loading...");

  vent_control.attach(11);
  vent_control.write(90);

  delay(3000);
  lcd.clear();


}

void loop() {                                           //prepair to feel the cringe all software engineers


  int f = dht.readTemperature(true);
  digitalWrite(BUILTIN, HIGH);
  lcd.setCursor(0, 0);
  lcd.print("room temp: ");
  lcd.setCursor(11, 0);
  lcd.print(f);

  lcd.setCursor(0, 1);
  lcd.print("set temp: ");
  lcd.setCursor(10, 1);
  lcd.print(set_temp);
  

  if (f + high_temp_range > set_temp) {
    
    vent_control.attach(11);
    vent_control.write(90);                              //adjust to needed value for OPEN angle
  }

  else if (f - low_temp_range < set_temp) {              //adjust to needed value for CLOSED angle
    vent_control.attach(11);
    vent_control.write(130);
  }
  delay(400);                                            //adjust to give relay time to move to angle
  digitalWrite(BUILTIN, LOW);
  digitalWrite(lcd_backlight, LOW);
  digitalWrite(lcd_contrast, HIGH);

  for (int count = 0; count < 300; count++) {            //Where the magic happens 

    if (digitalRead(up_button) == LOW) {
      set_temp = set_temp + 1;
      digitalWrite(lcd_backlight, HIGH);
      digitalWrite(lcd_contrast, LOW);
      delay(200);
    }

    if (digitalRead(down_button) == LOW) {
      set_temp = set_temp - 1;
      digitalWrite(lcd_backlight, HIGH);
      digitalWrite(lcd_contrast, LOW);
      delay(200);
    }
    else {
      lcd.setCursor(11, 0);
      lcd.print(f);
      lcd.setCursor(10, 1);
      lcd.print(set_temp);
      delay(100);
      vent_control.detach();
    }






  }
}

//long live the kerbals!

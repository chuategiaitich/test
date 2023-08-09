#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
String x ;
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16, 2);
  
}

void loop() {
  // put your main code here, to run repeatedly:
 while(Serial.available()>0){
      x = Serial.readString();
      
      if(x=="a\n"){
        lcd.clear();
        lcd.println("Hello");
        Serial.println("Hello");
        
      }
      if(x=="b\n"){
        lcd.clear();
        lcd.println("Bye");
        Serial.println("Bye");
        
      }
 }
  
}

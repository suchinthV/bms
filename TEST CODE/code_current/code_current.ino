#include "LiquidCrystal.h"
int current_sense  = A0;
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
void setup() {
  lcd.begin(16,2);

  lcd.print(" DC CURRENT"); 
  lcd.setCursor(0, 1);
  lcd.print(" USING OPAMP "); 

  delay(2000);
  lcd.clear();

}

void loop() {
 
 float omp_value= analogRead(current_sense); // in volts
 float current= (omp_value * (5.0/1023.0))*0.54;// this line converts the readings into actual value
 lcd.setCursor(0, 0);
 lcd.print(" Current_Value"); 
lcd.setCursor(5, 1);
lcd.print("I="); 
  lcd.print(current,1); 
lcd.print("A");
}

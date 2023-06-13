#define b1_pin A0
#define b2_pin A1
#define b3_pin A2
#define b4_pin A3
#define c_pin A7
#define temp_pin A5
#define bl3_pin 12
#define bl2_pin 11
#define bl1_pin 10
#define CCtrl_pin 5
#define DCtrl_pin 7
#define SCCheck_pin 4
float x, y, z, b1, b2, b3, v, i, t, icheck;
bool charging_flag = 0, discharging_flag = 0, bl1_flag = 0, bl2_flag = 0, bl3_flag = 0, charging_stop = 0, discharge_stop = 0;
float ofset_voltage = 2.5;
float current_factor = 0.183;
int count = 0;
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 6
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

LiquidCrystal_I2C lcd(0x3F, 16, 2);
unsigned long int pre, pre2;
void setup() {
  Serial.begin(9600);// put your setup code here, to run once:
  sensors.begin();

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);

  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(7, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  // initialize the lcd
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  pre = millis();
  pre2 = millis();
}

void loop() {
  x = get_battery_voltage(b1_pin, 10);
  y = get_battery_voltage(b2_pin, 10);
  z = get_battery_voltage(b3_pin, 10);
  v = (get_battery_voltage(b4_pin, 10)+0.5);
  i = get_load_current(c_pin , 10);
  t = get_temperature();
  b1 = x;
  b1 = b1 + 0.2;
  b2 = y - x;
  b3 = z - y;
  Serial.print("voltage ");
  Serial.print(b1);
  Serial.print("  , ");
  Serial.print(b2);
  Serial.print("  , ");
  Serial.print(b3);
  Serial.print(" , ");
  Serial.print(v);
  Serial.print(" , ");
  Serial.print(i);
  Serial.print(" , ");
  Serial.println(t);

  if ((millis() - pre) > 2000)
  {
    lcd.clear();
    pre=millis();
    if (count == 1) {

      pre = millis();
      lcd.setCursor(1, 0);
      lcd.print("b1");
      if (bl1_flag == 1) {
        lcd.setCursor(4, 0);
        lcd.print("*");
      }
      lcd.setCursor(6, 0);
      lcd.print("b2");
      if (bl2_flag == 1) {
        lcd.setCursor(9, 0);
        lcd.print("*");
      }

      lcd.setCursor(12, 0);
      lcd.print("b3");
      if (bl3_flag == 1) {
        lcd.setCursor(15, 0);
        lcd.print("*");
      }


     

    }
    if (count == 2)
    {
      lcd.setCursor(2, 0);
      lcd.print("V i/o");
      lcd.setCursor(10, 0);
      lcd.print("I i/o");
   
      lcd.setCursor(2, 1);
      lcd.print(v);
   
      lcd.setCursor(10, 1);
      lcd.print(i);
     
    }
    if (count == 3)
    {
      lcd.setCursor(2, 0);
      lcd.print("temperature");
      lcd.setCursor(4, 1);
      lcd.println(t,2);
      lcd.print("\337C");

      count = 0;

    }
    count++;

  }



  if (v > 16 ) {
    while(1){
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("over voltage");
    digitalWrite(CCtrl_pin, LOW);
    digitalWrite(DCtrl_pin, LOW);
    delay(1000);
    v = (get_battery_voltage(b4_pin, 10)+0.5);
    if(v<14)
    {
      break;
    }
    }
    
  }
  if (charging_stop == 1)
  {
    digitalWrite(CCtrl_pin, LOW);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("warning !!");
        lcd.setCursor(0, 1);
        lcd.print("battary full");
        delay(2000);
    
  }
  else
  {
    digitalWrite(CCtrl_pin, HIGH);
  }
  if (discharge_stop == 1)
  {
        digitalWrite(DCtrl_pin, LOW);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("warning !!");
        lcd.setCursor(0, 1);
        lcd.print("low battery");
        delay(2000);
    
  }
  else
  {
    digitalWrite(DCtrl_pin, HIGH);
  }


  if (bl1_flag == 1) {
    digitalWrite(bl1_pin, HIGH);
  }
  else
  {
    digitalWrite(bl1_pin, LOW);
  }
  if (bl2_flag == 1) {
    digitalWrite(bl2_pin, HIGH);
  }
  else
  {
    digitalWrite(bl2_pin, LOW);
  }
  if (bl3_flag == 1) {
    digitalWrite(bl3_pin, HIGH);
  }
  else
  {
    digitalWrite(bl3_pin, LOW);
  }


  if ((millis() - pre2) > 5000) {

    digitalWrite(CCtrl_pin, LOW);
    x = get_battery_voltage(b1_pin, 10);
    y = get_battery_voltage(b2_pin, 10);
    z = get_battery_voltage(b3_pin, 10);
    b1 = x;
    b1 = b1 + 0.2;
    b2 = y - x;
    b3 = z - y;

    if (b1 > 4.15) {                 // cell balancing
      bl1_flag = 1; 
    }
    else
    {
      bl1_flag = 0;
    }
    if (b2 > 4.15)
    {
      bl2_flag = 1;

    }
    else
    {
      bl2_flag = 0;

    }
    if (b3 > 4.15)
    {
      bl3_flag = 1;

    }
    else
    {
      bl3_flag = 0;

    }
    if ((b3 >= 4.2) && (b2 >= 4.2) && (b1 >= 4.2))   // full charge
    {
      charging_stop = 1;
    }
    else
    {
      charging_stop = 0;
    }
    if (b3 < 2.5 || b2 < 2.5 || b1 < 2.5)    // low battery
    
    {
      discharge_stop = 1;
      
    }
    else
    {
      discharge_stop = 0;
    }
      lcd.setCursor(0, 1);
      lcd.print(b1);
      lcd.print("  ");
      lcd.print(b2);
      lcd.print("  ");
      lcd.print(b3);
    digitalWrite(CCtrl_pin, HIGH);
   

  }



  if (i > 0) {
    
    if (i > 5 )
    {
     
      delay(1000);
      while (1) {
      digitalWrite(DCtrl_pin, LOW);
      digitalWrite(CCtrl_pin, LOW);
      digitalWrite(SCCheck_pin, HIGH);

        delay(10);
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("short circuit");
        Serial.print("short circuit  :");
        delay(100);
        v = get_battery_voltage(b4_pin, 10);
        Serial.println(v);
        if (v > 0.5) {
          break;
        }
      }

    }
  }
  else {
    

    if ((-i) > 5 )
    {
      digitalWrite(DCtrl_pin, LOW);
      digitalWrite(CCtrl_pin, LOW);
      digitalWrite(SCCheck_pin, HIGH);
      delay(1000);
      while (1) {

        delay(10);
        Serial.print("over current");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("warning !!");
        lcd.setCursor(0, 1);
        lcd.print("over current");
       delay(100);
        
        v = get_battery_voltage(b4_pin, 10);
        Serial.println(v);
        if (v > 0.5) {
          break;
        }
      }
    }

  }
  if (t > 60)    /// temperature
  {
    while (1) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Warning !!");
      lcd.setCursor(0, 1);
      lcd.print("high temperature");

      digitalWrite(DCtrl_pin, LOW);
      digitalWrite(CCtrl_pin, LOW);
      t = get_temperature();
      if (t < 50) {
        break;
      }

    }
  }



}
float get_battery_voltage(int pin, int n_samples)
{
  float voltage = 0;
  for (int i = 0; i < n_samples; i++)
  {
    voltage += (analogRead(pin) * (5.0 / 1024.0) * 11.0);
  }
  voltage = voltage / n_samples;

  //  Serial.print(" bat volt :");
  //  Serial.print(voltage);
  return (voltage);
}
float get_load_current(int pin , int n_samples)
{
  float Sensor_voltage;
  float current = 0;
  for (int i = 0; i < n_samples; i++)
  {
    Sensor_voltage = analogRead(pin) * (5.0 / 1024.0);
    // Serial.println(Sensor_voltage);
    current = current + (Sensor_voltage - ofset_voltage) / current_factor;
  }
  current = current / n_samples;
  //  Serial.print(" load current : ");
  //  Serial.print(current);
  return (current);
}
float get_temperature()
{
  float temperature = 0;
  sensors.requestTemperatures();

  temperature = sensors.getTempCByIndex(0);
  return (temperature);
}
void clearLCDLine(int line)
{
  lcd.setCursor(0, line);
  for (int n = 0; n < 20; n++) // 20 indicates symbols in line. For 2x16 LCD write - 16
  {
    lcd.print(" ");
  }
}

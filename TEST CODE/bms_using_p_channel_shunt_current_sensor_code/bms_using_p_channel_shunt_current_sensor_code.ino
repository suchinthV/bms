#define b1_pin A0
#define b2_pin A1
#define b3_pin A2
#define b4_pin A3
#define c_pin A4
#define temp_pin A5
#define bl3_pin 5
#define bl2_pin 6
#define bl1_pin 7
#define CCtrl_pin 8
#define DCtrl_pin 9
#define SCCheck_pin 10
float x, y, z, b1, b2, b3, v, i, t,icheck;
bool charging = 0, discharging = 0;
float ofset_voltage = 2.5;
float current_factor = 0.1;
void setup() {
  Serial.begin(9600);// put your setup code here, to run once:
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
}

void loop() {
  x = get_battery_voltage(b1_pin, 10);
  y = get_battery_voltage(b2_pin, 10);
  z = get_battery_voltage(b3_pin, 10);
  v = get_battery_voltage(b4_pin, 10);
  i = get_load_current(c_pin , 10);
  t = get_temperature(temp_pin, 10);
  b1 = x;
  b2 = y - x;
  b3 = z - y;
  Serial.print("voltage ");
  Serial.print(b1);
  Serial.print(" , ");
  Serial.print(b2);
  Serial.print(" , ");
  Serial.print(b3);
  Serial.print(" , ");
  Serial.print(v);
  Serial.print(" , ");
  Serial.print(i);
  Serial.print(" , ");
  Serial.println(t);
  if (v > 12.6 ) {
    digitalWrite(CCtrl_pin, LOW);
    digitalWrite(DCtrl_pin, HIGH);
  }
  else {
    digitalWrite(CCtrl_pin, HIGH);
    digitalWrite(DCtrl_pin, LOW);
  }
  if (b1 > 4.1) {
    digitalWrite(bl1_pin, LOW);

  }
  else
  {
    digitalWrite(bl1_pin, HIGH);
  }
  if (b2 > 4.1)
  {
    digitalWrite(bl2_pin, LOW);
  }
  else
  {
    digitalWrite(bl2_pin, HIGH);
  }
  if (b3 > 4.1)
  {
    digitalWrite(bl3_pin, LOW);
  }
  else
  {
    digitalWrite(bl3_pin, HIGH);
  }
  if (b3 < 3.5 || b2 < 3.5 || b2 < 3.5)
  {
    digitalWrite(DCtrl_pin, HIGH);
  }
    if (b3 >= 4.1 && b2 >= 4.1 && b2 >= 4.1)
  {
    digitalWrite(CCtrl_pin, HIGH);
  }
  if ((-i) > 5)
  {
    digitalWrite(DCtrl_pin, HIGH);
    digitalWrite(CCtrl_pin, HIGH);
    digitalWrite(SCCheck_pin, LOW);
    delay(1000);
    while(1){
      
      delay(10);
      Serial.println("short circuit");
    }
//    digitalWrite(DCtrl_pin, LOW);
//    digitalWrite(SCCheck_pin, HIGH);
  }
  if (t > 60)
  {
    digitalWrite(DCtrl_pin, HIGH);
    digitalWrite(CCtrl_pin, HIGH);
  }



}
float get_battery_voltage(int pin, int n_samples)
{
  float voltage = 0;
  for (int i = 0; i < n_samples; i++)
  {
    voltage += (analogRead(pin) * (5.0 / 1023.0) * 11.0);
  }
  voltage = voltage / n_samples;
  if (voltage < 0) {
    voltage = 0;
  }
  //  Serial.print(" bat volt :");
  //  Serial.print(voltage);
  return (voltage);
}
float get_load_current(int pin , int n_samples)
{
  float Sensor_voltage;
  float current = 0;
  float omp_value =0;
  for (int i = 0; i < n_samples; i++)
  {
    omp_value= analogRead(pin); // in volts
    current += (omp_value * (5.0/1023.0))*0.54;
    
  }
  current = current / n_samples;
  //  Serial.print(" load current : ");
  //  Serial.print(current);
  return (current);
}
float get_temperature(int pin, int n_samples)
{
  float temperature = 0;
  float total = 0;
  int val = 0, mv = 0, cel = 0, farh = 0;
  for (int i = 0; i < n_samples; i++)
  {
    val = analogRead(pin);
    mv = ( val / 1024.0) * 5000;
    cel = mv / 10;
    farh = (cel * 9) / 5 + 32;
    temperature = temperature + cel;
  }
  temperature = temperature / n_samples;
  return (temperature);
}

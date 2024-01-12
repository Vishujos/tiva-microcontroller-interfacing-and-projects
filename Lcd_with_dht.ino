void setup() {#include<DHT.h>
#include <LiquidCrystal.h>
#define DHTPIN A0
#define DHTTYPE DHT11
DHT dht (DHTPIN,DHTTYPE);
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");
lcd.begin(16, 2);
 lcd.print("DHT11 Sensor");
  lcd.setCursor(0, 1);
  lcd.print("Interfacing");
  pinMode(PB_7,OUTPUT);
  delay(2000);
  lcd.clear();
  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) ) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

 
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Humid: ");
  lcd.print(h);
  if(t>=30){
    analogWrite(PB_7,t);
  }
  else{
    analogWrite(PB_7,0);
  }
  

  delay(2000);
  lcd.clear();
}
  

  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly: 
  
}

#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>>
#include <DHT.h>
#include <Blynk.h>
#include <Wire.h>
#include <SPI.h>
#include <SimpleTimer.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C LCD(0x27, 16, 2); //lcd board

DHT dht(D5, DHT11); // sensor pin, sensor type

int led = 2; //pin 2 pada nodemcupin 4
int pump =0; //pin 0 pada nodemcupin 3


// Auth token pada Blynk App dari email
char auth[] = "f-h22_8cyGpj2hc_f5seSoOWTGocnIjx";

// username dan password pada Wifi
char ssid[] = "IPHONE 13";          // nama hotspot
char pass[] = "Suksesluarbiasa";    // passord hotspot

BlynkTimer timer;
WidgetLCD lcd(V1); //lcd android

// Pengiriman data ke virtual PIN (5)
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); 

  if (isnan(h) || isnan(t)) {
  Serial.println("Gagal dalam membaca sensor DHT-11");
  return;
  }

  // temperature
  Serial.print(t);
  lcd.print(0, 0, "TEMP");  
  LCD.setCursor(0, 0);  LCD.print("TEMP");  
  lcd.print(5, 0, t); lcd.print(9, 0, "C      ");
  LCD.setCursor(5, 0);  LCD.print(t); LCD.setCursor(9, 0); LCD.print((char)223); LCD.print("C      ");
  //delay(500);
  //lcd.clear();  LCD.clear();
 
  // humidity
  Serial.print(h);
  lcd.print(0, 1, "HUMI");
  LCD.setCursor(0, 1); LCD.print("HUMI"); 
  lcd.print(5, 1, h); lcd.print(7, 1, "%      ");
  LCD.setCursor(5, 1); LCD.print(h); LCD.setCursor(7,1); LCD.print("%     ");
  delay(500);
  lcd.clear();  LCD.clear();
  
  Blynk.virtualWrite(V5, t); //inisialisasi V5 di Blynk
  Blynk.virtualWrite(V6, h); //inisialisasi V6 di Blynk

  //lcd to android | lcd to board lcd
  int POT = analogRead(A0); 
  Serial.print(POT);
  lcd.print(0,0,"KEADAAN");   LCD.setCursor(0,0); LCD.print("MOIS"); LCD.setCursor(5,0); LCD.print(POT); LCD.print(" ");
  lcd.print(0,1,"PUMP");      LCD.setCursor(0,1); LCD.print("PUMP");
  Blynk.virtualWrite(V0, POT);
  
  if (POT>500){
  Serial.println("KERING");//ke serial monitor
  lcd.print(9,0,"KERING");      LCD.setCursor(9,0);LCD.print("KERING");
  lcd.print(5,1,"ON ");         LCD.setCursor(5,1);LCD.print("ON ");
  digitalWrite(pump,LOW);
  for(int x=0; x<=10; x++){     LCD.setCursor(9,1);LCD.print(x);
  lcd.print(9,1,x);delay(500);}
  lcd.clear();                  LCD.clear();
  digitalWrite(pump,HIGH);
  lcd.print(0,0,"AIR MERESAP"); LCD.setCursor(0,0);LCD.print("AIR MERESAP");
  lcd.print(0,1,"    WAIT");    LCD.setCursor(0,1);LCD.print("    WAIT");
  for(int x=9; x>0; x--){       LCD.setCursor(9,1);LCD.print(x);
  lcd.print(9,1,x);delay(500);}
  lcd.clear();                  LCD.clear();
  }
  
  else if (POT>400&&POT<500){
  Serial.println("NORMAL");
  lcd.print(8,0,"NORMAL");      LCD.setCursor(9,0);LCD.print("NORMAL");
  lcd.print(5,1,"OFF");         LCD.setCursor(5,1);LCD.print("OFF");
  digitalWrite(pump,HIGH);
  }
  
  else if (POT<400){
  Serial.println("BASAH");
  lcd.print(8,0,"BASAH ");      LCD.setCursor(9,0);LCD.print("BASAH ");
  lcd.print(5,1,"OFF");         LCD.setCursor(5,1);LCD.print("OFF");
  delay(500);
  lcd.clear(); LCD.clear();
  digitalWrite(pump,HIGH);
  }
}

void setup()
{
  // Debug console
  Wire.begin(D2, D1);
  LCD.init();
  LCD.backlight();
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  dht.begin();

  // Setiap 1 detik data dikirim
  timer.setInterval(1000L, sendSensor);
  pinMode(pump,OUTPUT);
  lcd.clear();

}

void loop()
{ 
  Blynk.run();
  timer.run();
  delay(500);
}

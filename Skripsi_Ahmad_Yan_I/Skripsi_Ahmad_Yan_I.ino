#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLukBLDcg4"
#define BLYNK_DEVICE_NAME "Monitoring Ph dan Suhu Air"
#define BLYNK_AUTH_TOKEN "_uQoU2acQEyHqssoPgjeLRwpVov8uPlX"
#include<SoftwareSerial.h>
#include <DFRobot_PH.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h> // include library
#include <BlynkSimpleEsp8266.h> // include library
char auth[] = BLYNK_AUTH_TOKEN ; //Auth Token
char ssid[] = "Tetangga sebelah"; //nama hotspot yang digunakan
char pass[] = "jandabolong123"; //password hotspot yang digunakan
//BlynkTimer timer;

// Data wire is plugged into digital pin 2 on the Arduino
#define sensor_ds18b20 D7 //pada pin 2 untuk sensor ds18b20

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(sensor_ds18b20);  
LiquidCrystal_I2C lcd(0x27,16,2);

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);
const int ph_pin = A0;
float Po = 0;
float PH_step;

double TeganganPh;

float PH4 = 2.95;
float PH7 = 2.6;

byte LCD [8] =
{
  B00100,
  B01010,
  B01010,
  B01110,
  B11111,
  B11111,
  B01110,
  B00000
};

void setup() {
Serial.begin(115200);
pinMode(ph_pin, INPUT);
sensors.begin();  // Start up the library
Blynk.begin(auth, ssid, pass); //memulai Blynk 
//timer.setInterval(1000L, sendSensor);

  lcd.begin();
  lcd.backlight();
  lcd.createChar(0, LCD );
  lcd.setCursor(0,0); //diatas LCD
  lcd.print(" Monitoring ");
  lcd.setCursor(0,1); //Dibawah LCD
  lcd.print("  Kualitas Air ");
  delay(5000);
  lcd.clear();
}
  void loop() {
 Blynk.run(); 
// timer.run();

  float nilai_analog_PH = analogRead(ph_pin); //membaca sensor PH
  TeganganPh = 3.3 / 1023.0 * nilai_analog_PH; // rumus tegangan PH
  Serial.print(nilai_analog_PH);
  Serial.print(" || ");
  Serial.print("TeganganPh: "); // menampilkan komen diserial monitor
  Serial.print(TeganganPh, 3); // menampilkan hasil Tegangan ph
  Serial.print(" || ");

  PH_step = (PH4 - PH7) / 3;
  float Po = 7.00 + ((PH7 - TeganganPh) / PH_step);
  Serial.print("Nilai PH cairan: "); //menampilkan komen diserial monitor
  Serial.println(Po, 2); // menampilkan hasil Ph cairan
  delay (1000);

  sensors.requestTemperatures(); 
  static unsigned long timepoint = millis();
    if(millis()-timepoint>1000U){                  //time interval: 1s
        timepoint = millis();}
  float suhu = sensors.getTempCByIndex(00);
  //print the temperature in Celsius
  Serial.print("Temperature: ");
  Serial.print(suhu);
  Serial.println("C  |  ");
  delay(1000);
    
  Blynk.virtualWrite(V0, Po); //mengirimkan data temperatur ke Virtual pin VO di Blynk Cloud
  Blynk.virtualWrite(V1, suhu); //mengirimkan data temperatur ke Virtual pin VO di Blynk Cloud
  
  lcd.setCursor(0,0);
  lcd.print("Tegangan = ");
  lcd.print(TeganganPh);
  lcd.write(0);
  lcd.print("V");
  lcd.setCursor(0,1);
  lcd.print("Ph = ");
  lcd.print(Po);
  delay(2000);
  
  lcd.setCursor(0,1);
  lcd.print("Suhu =  ");
  lcd.print(suhu);
  delay(1000);
}  


#define BLYNK_PRINT Serial
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>
#include <Wire.h>
#include <NewPing.h> //Library untuk HC-SR04
#define TRIGGER_PIN  D5  //Pin Trigger HC-SR04 pada NodeMCU
#define ECHO_PIN     D6  //Pin Echo HC-SR04 pada NodeMCU
#define MAX_DISTANCE 250 //Maksimum Pembacaan Jarak (cm)ss
#define BLYNK_TEMPLATE_ID "TMPL6Fevl1B43"
#define BLYNK_TEMPLATE_NAME "Servo"
#define BLYNK_AUTH_TOKEN "OIWj1G4-XnGLa6PGmPI9BbV1nxEWLP1w"

RTC_DS3231 rtc; //SCL D1 SDA D2 Bisa diseri dengan LCD
char daysOfTheWeek[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jum'at", "Sabtu"};
LiquidCrystal_I2C lcd(0x27,16,2);

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); //Setup Pin HC-SR04 dan Jarak Pembacaan dengan Fungsi Library

Servo myservo ;
int StatusPakan;
float jarak = sonar.ping_cm();
void setup(){
  lcd.init();
  lcd.begin(16,2);
  lcd.backlight();
  delay(3000);
  if (! rtc.begin()) {
    Serial.println("RTC tidak terbaca");
    while (1);
  }
  if (rtc.lostPower()) {
    //atur waktu sesuai waktu pada komputer
   // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    //atur waktu secara manual
    // January 21, 2019 jam 10:30:00
    // rtc.adjust(DateTime(2019, 1, 25, 10, 30, 0));
    //rtc.adjust(DateTime(2023, 6, 30, 9, 50, 0));
  }

  myservo.attach(2);
  myservo.write(0);

  Serial.begin(115200);
  // koneksi ke wifi
  WiFi.begin("BDS Caffeinery", "kopisemangat");
  // uji wifi
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }

  Serial.println("Wifi Connected");

  //Koneksi wifi ke blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, "BDS Caffeinery", "kopisemangat");
  Serial.println("Blynk Connected");
  
}
void BeriPakan()
{
    for (int posisi=0; posisi<=180; posisi++)
  {
    myservo.write(posisi);
    
  }
  for (int posisi=180; posisi>=0; posisi--)
  {
    myservo.write(posisi);
    delay(20);
  }
}
void loop()
{
// run blink from node mcu
 Blynk.run();
  
   DateTime now = rtc.now();
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);//hari
  Serial.print(", ");
  Serial.print(now.day(), DEC); //tanggal
  Serial.print('/');
  Serial.print(now.month(), DEC); //bulan
  Serial.print('/');
  Serial.print(now.year(), DEC); //tahun
  Serial.print(' ');
  Serial.print(now.hour(), DEC); //jam
  Serial.print(':');
  Serial.print(now.minute(), DEC); //tanggal
  Serial.print(':');
  Serial.print(now.second(), DEC); //detik
  Serial.println();
  //Instruksi untuk Menampilkan Data RTC pada LCD
  lcd.setCursor(0,0);
  lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
   lcd.setCursor(5,0);
  lcd.print(",");
  lcd.setCursor(6,0);
  lcd.print(now.day(), DEC);
  lcd.setCursor(8,0);
  lcd.print("/");
  lcd.setCursor(9,0);
  lcd.print(now.month(), DEC);
  lcd.setCursor(11,0);
  lcd.print("/");
  lcd.setCursor(12,0);
  lcd.print(now.year(), DEC);
  lcd.setCursor(0,1);
  lcd.print("Jam: ");
  lcd.setCursor(4,1);
  lcd.print(now.hour(), DEC);
  lcd.setCursor(6,1);
  lcd.print(":");
  lcd.setCursor(7,1);
  lcd.print(now.minute(), DEC);
  lcd.setCursor(9,1);
  lcd.print(":");
  lcd.setCursor(10,1);
  lcd.print(now.second(), DEC);
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Status Pakan:");
  lcd.print(StatusPakan);
  lcd.setCursor(0,1);
  lcd.print("Jarak:");
  lcd.print(jarak);
  lcd.print("cm");
  // delay(1000);
// tampilakn statsupakan di serial monitor
 Serial.println("Setatus Pakan :" + String(StatusPakan));
 if(StatusPakan == 1){
  BeriPakan();

  Blynk.virtualWrite(V1,0);
  StatusPakan = 0;
  delay(2000);

 
}
 

  //Melakukan Pembacaan Jarak dalam bentuk cm
  //Mencetak Hasil Pembacaan pada Serial Monitor          
  Serial.println("Monitoring Jarak");
  Serial.print("Jarak: ");
  Serial.print(jarak);
  Serial.println(" cm");
  delay(1000); //Delay 1 Detik
  

  // if (now.hour() == 8 && now.minute == 0) {
  //   for(int posisi=0; posisi<=180; posisi++)
  // {
  //   myservo.write(posisi);
  //   delay(2);
  //   }
  //   delay(5000);
  //   for(int posisi=180; posisi>=0; posisi--)
  // {
  //   myservo.write(posisi);
  //   delay(10);
  // }

  // }
  //  else if (now.hour() == 12 && now.minute == 00){
  //   for(angle = 0; angle < 180; angle++) {
  //       servo.write(angle);
  //       delay(15);
  //   }
  //   delay(5000);
  //   for(angle = 180; angle > 0; angle--) {
  //       servo.write(angle);
  //       delay(15);
  //   }
  // }
  // else if (now.hour() == 16 && now.minute == 00){
  //   for(angle = 0; angle < 180; angle++) {
  //       servo.write(angle);
  //       delay(15);
  //   }
  //   delay(5000);
  //   for(angle = 180; angle > 0; angle--) {
  //       servo.write(angle);
  //       delay(15);
  //   }
  }
// Jika variabel status = 180 Beri pakan

// }
// Baca Data stream status pakan
BLYNK_WRITE(V1)
{
  //baca datastream dan tampung ke dalam variable status pakan
  StatusPakan = param.asInt();
}

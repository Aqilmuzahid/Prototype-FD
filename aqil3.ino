#include <ESP8266WiFi.h>
#include <CTBot.h>
#include <WiFiClientSecure.h>

CTBot bot;

//Masukan Nama WiFi dan Password
String ssid         = "Kyaaa";
String pass         = "aqil1234";
String token        = "1562796322:AAGf75NBVvp8APFi7SIueTWrKQv-gvbQfpo";
const int id        = -488555918;

int triggerPin = D1;
int echoPin    = D2; 
int buzzer     = D3;
int LEDmerah   = D4;
int LEDK       = D7;
int LEDhijau   = D6;

long duration, distance;

void connectwifi(){
  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {  //Pengaturan Pin dan Variabel
  Serial.begin(115200);

  connectwifi();

  //koneksi dengan telegram  
  bot.wifiConnect(ssid,pass);
  bot.setTelegramToken(token);
  
  if(bot.testConnection()) {
    Serial.println("Telegram Terhubung");
  } 
  else  {
    Serial.println("Telegram Terputus");
  }
  Serial.println("Alat siap digunakan...");
  
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(LEDmerah, OUTPUT);
  pinMode(LEDK, OUTPUT);
  pinMode(LEDhijau, OUTPUT);
}
 
void loop() {  //Perulangan Program

  TBMessage msg;
  bot.getNewMessage(msg);
  
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(8);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(8);
  digitalWrite(triggerPin, LOW);
    
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
//  Serial.print("Jarak Air: ");
//  Serial.print(distance);
//  Serial.println("Cm");

  String dis = "Jarak Air: ";
  dis += int(distance);
  dis += "cm \n";

  if (distance <= 30 && distance >= 20){
    Serial.println("Siaga 3, Air telah memasuki Batas siaga");
    digitalWrite(buzzer,    LOW);
    digitalWrite(LEDmerah,  LOW);
    digitalWrite(LEDK,      LOW);
    digitalWrite(LEDhijau,  HIGH);
    bot.sendMessage(id,dis, "");
    bot.sendMessage(id, "Siaga 3, Ketinggian Air Kurang Dari 30 cm");
  } 
  
  if (distance <=19 && distance >= 10) {
    Serial.println("Siaga 2, Air telah memasuki Batas siaga");
    digitalWrite(buzzer,    LOW);
    digitalWrite(LEDmerah,  LOW);
    digitalWrite(LEDK,      HIGH);
    digitalWrite(LEDhijau,  LOW);  
    bot.sendMessage(id, dis, "");
    bot.sendMessage(id, "Siaga 2, Ketinggian Air Kurang Dari 19 cm");
  }
  
  if (distance <=9 ) {
    Serial.println("Siaga 1, Air telah memasuki Batas siaga");
    digitalWrite(buzzer,    HIGH);
    digitalWrite(LEDmerah,  HIGH);
    digitalWrite(LEDK,      LOW);
    digitalWrite(LEDhijau,  LOW); 
    bot.sendMessage(id, dis, "");
    bot.sendMessage(id, "Siaga 1, Ketinggian Air Kurang Dari 9 cm");
    }

  if (distance > 30 ) {
    digitalWrite(buzzer,    LOW);
    digitalWrite(LEDmerah,  LOW);
    digitalWrite(LEDK,      LOW);
    digitalWrite(LEDhijau,  LOW); 
    }
}

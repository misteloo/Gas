#include <DHT.h>
#include <Wire.h>
#include <SSD1306.h>
#include <ESP8266WiFi.h>
#include <ThingerESP8266.h>

#define USERNAME "mistelo"
#define DEVICE_ID "nodemcu"
#define DEVICE_CREDENTIAL "123456789"
#define SSID "mamadi"
#define SSID_PASSWORD "mamad888"
#define DHTPIN D3
#define DHTTYPE DHT22

#define bazzer D8

int q = 0;

String f1(25);
String f2(30);
int count = 5;
int seda [] = {100,0,100,0};

int Alarmvalue = 450;

DHT dht (DHTPIN,DHTTYPE);

ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

SSD1306 display(0x3c,4,5);



int GASvalue = 0;
int localTemp = 0;
int localHum = 0;
int thingtemp = 0;
int thinghum = 0;

void setup() {
  pinMode(bazzer,OUTPUT);
  thing.add_wifi(SSID,SSID_PASSWORD);
  delay(2000);
  display.init();
  display.flipScreenVertically();

  thing["Temperature"] >> outputValue(localTemp);
  thing["Humidity"] >> outputValue(localHum);
  thing["Gas Level"] >> outputValue(GASvalue);
  thing ["Alarm"] << inputValue (Alarmvalue);
  

}

  void loop() {

  if (GASvalue > Alarmvalue)
  {
    GAS_Alarm_notification();
  }
  display.clear();
  getDHT();
  drawDHT();
  drawGAS_sensor();
  display.display();
  thing.handle();

 
  if (GASvalue > 450)
    {
      for (int q = 0; q<count ;q++)
      {
        tone (bazzer,seda[q]);
        delay(500);
      }
       
    }
    else 
    {
      noTone(bazzer);
    }
}
 

void getDHT()
{
 thingtemp = localTemp;
 thinghum = localHum;
 localTemp = dht.readTemperature();
 localHum = dht.readHumidity();
 
 if (isnan(localHum) || isnan(localTemp))
 {
  localTemp = thingtemp;
  localHum = thinghum;
  return;
 }
 
}
void drawDHT()
{
  int x=0;
  int y=0;
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0+x,5+y,"Hum");
  
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(43+x,y,"DHT22");
  
  display.setFont(ArialMT_Plain_16);
  String humdht22 = String(localHum);
  display.drawString(0 + x , 15 + y , f2 + "%" );
  
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(95 + x , 5 + y , "Temp");

  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  String tempdht22 = String(localTemp);
  display.drawString(70 + x , 15 + y , f1 + "C°");
  
}
void drawGAS_sensor ()
{
  int x=0;
  int y=0;
  

  

  for (int i = 0;i<10;i++)
  {
    GASvalue += analogRead(A0);
    delay(50);
  }
  
  GASvalue = GASvalue / 10;
  //GAScount = GASvalue;


   

  String GAS(GASvalue);

  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(32 + x , 37 + y , "Gas level");
  display.setFont(ArialMT_Plain_16);
  display.drawString(46,48 , GAS + " (ppm)" );
  
}
void GAS_Alarm_notification()
{
  unsigned long currentTs = millis();
  unsigned long lastcheck;
  if (currentTs - lastcheck >= 60 * 1000)
  {
    lastcheck = currentTs;
    thing.call_endpoint("GmailServer");
  }
}

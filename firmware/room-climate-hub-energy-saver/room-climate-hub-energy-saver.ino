#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "policy.h"
constexpr uint8_t DOOR_PIN=13, SDA_PIN=4, SCL_PIN=5, BLE_RX=14, BLE_TX=12;
SoftwareSerial ble(BLE_RX,BLE_TX);
Adafruit_SSD1306 oled(128,64,&Wire,-1);
EnergyPolicy policy;
LineParser parser;
bool oledPresent=false,lastDisplay=true;
uint32_t lastReport=0;
void report(){
 char out[160];
 snprintf(out,sizeof(out),"{\"id\":4,\"door_open\":%s,\"display_on\":%s,\"oled_present\":%s,\"override_active\":%s}",
 policy.doorOpen?"true":"false",policy.displayOn?"true":"false",oledPresent?"true":"false",policy.overrideActive?"true":"false");
 Serial.println(out);ble.println(out);
}
void setup(){
 Serial.begin(115200);WiFi.mode(WIFI_OFF);
 pinMode(DOOR_PIN,INPUT_PULLUP);Wire.begin(SDA_PIN,SCL_PIN);
 ble.begin(9600);
 oledPresent=oled.begin(SSD1306_SWITCHCAPVCC,0x3C);
 if(oledPresent){oled.clearDisplay();oled.setTextSize(1);oled.setTextColor(SSD1306_WHITE);}
 report();
}
void loop(){
 uint32_t now=millis();policy.update(digitalRead(DOOR_PIN)==HIGH,now);
 while(ble.available()){
  int result=parser.feed(char(ble.read()));
  if(result==1){if(policy.command(parser.buf,now))report();else ble.println("{\"error\":\"unknown_command\"}");}
  if(result==-1)ble.println("{\"error\":\"line_too_long\"}");
 }
 if(oledPresent){
  if(policy.displayOn!=lastDisplay){oled.ssd1306_command(policy.displayOn?SSD1306_DISPLAYON:SSD1306_DISPLAYOFF);lastDisplay=policy.displayOn;}
  if(policy.displayOn && uint32_t(now-lastReport)>=1000){
   oled.clearDisplay();oled.setCursor(0,0);oled.println("ENERGY SAVER");oled.println(policy.doorOpen?"DOOR OPEN":"DOOR CLOSED");oled.println(policy.overrideActive?"WAKE OVERRIDE":"AUTO");oled.display();
  }
 }
 if(uint32_t(now-lastReport)>=1000){lastReport=now;report();}
 delay(2);
}

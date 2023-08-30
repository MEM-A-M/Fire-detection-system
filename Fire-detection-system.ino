#include "ThingSpeak.h" //library for thing speak
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>

#define greenled 12
#define redled 14
#define buzzer 13
bool x = false ;

const char* serverName = "http://maker.ifttt.com/trigger/GAS_EMAIL/json/with/key/nTt5xIiGEMTOmoLOyYcSt";
char ssid[] = "Huawei-957D";   // our network SSID (name) 
char pass[] = "68120065";      // our network password
int keyIndex = 0;              // our network key Index number
WiFiClient  client;

unsigned long myChannelNumber =  2183963;
const char * myWriteAPIKey = "WI4W5LPTXTUCXYW6";
String myStatus = "";

void setup() {
  Serial.begin(9600);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  pinMode(greenled,OUTPUT);
  pinMode(redled,OUTPUT);
  digitalWrite(greenled,HIGH);
  digitalWrite(redled,LOW);
}
void loop() {
   if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
  HTTPClient http;
  int gas = analogRead(A0);
  if (gas >=300 && x==true) {
  digitalWrite(greenled,LOW);
  digitalWrite(redled,HIGH); 
  tone(buzzer,550);
  }
  else if(gas>=300 && x==false){
  digitalWrite(greenled,LOW);
  digitalWrite(redled,HIGH); 
  tone(buzzer,550);
  http.begin(client, serverName);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String httpRequestData = "value1=" + String(random(40)) + "&value2=" + String(random(40))+ "&value3=" + String(random(40));           
  int httpResponseCode = http.POST(httpRequestData);
  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);
  http.end();
  x=true;}
  
  else {
  digitalWrite(greenled,HIGH);
  digitalWrite(redled,LOW);
  noTone(buzzer);
  x=false;}

  Serial.println(gas);
  ThingSpeak.setField(1, gas);
  ThingSpeak.setStatus(myStatus);
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{ Serial.println("Problem updating channel. HTTP error code " + String(x)); }
  delay(1000); }
#include<Wire.h>
#include<LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
WiFiClient wifiClient;
int redPin = 16;
int greenPin = 15;
int buzzer = 13; // declare a variable buzzer and assign pin number 10 to it
LiquidCrystal_I2C lcd(0x27, 16, 2);
const char* host = "192.168.0.1";
void setup() {
    WiFi.begin("dlink", "");
  // put your setup code here, to run once:
    Serial.begin(9600);
    pinMode(redPin, OUTPUT);
    pinMode(buzzer, OUTPUT);
    pinMode(greenPin, OUTPUT);
    lcd.init();
    //lcd.init();
    lcd.backlight();
}
void loop() {
  // put your main code here, to run repeatedly:
   int rawData = analogRead(A0);
    float vcc = 5.0;
    float voltage = rawData * (vcc / 1024.0);
    float temperature = voltage * 100;
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" *C");
    delay(500);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(temperature);
    if(temperature >= 47){
        digitalWrite(greenPin,LOW);
        digitalWrite(redPin,HIGH);
        digitalWrite(buzzer, HIGH);   // turn the buzzer on (HIGH is the voltage level)
        delay(100);
        String mData="";
        String device_name = "Front001-Device";
        mData = "device="+device_name+"&temperature="+(String) temperature;
        //uncomment this only if you're using Gaby's server
        // sendData(80, "insecure.benax.rw", "/iot" , mData);
     //when using your own server use this and comment it when using Gaby's server
    String request = "POST /iot?device="+device_name+"&temperature="+(String)temperature+" HTTP/1.1";
    wifiClient.connect(host, 8000);
    wifiClient.println(request);
    wifiClient.println("Host: 192.168.0.1");
    wifiClient.println("User-Agent: ESP8266/1.0");
    wifiClient.println("C: ESP8266/1.0");
    wifiClient.println();
    Serial.println("Response: "+wifiClient.readStringUntil('\n'));
    delay(1000);
    //end
   }
    else{
      digitalWrite(greenPin,HIGH);
      digitalWrite(redPin,LOW);
      digitalWrite(buzzer, LOW);    // turn the buzzer off by making the voltage LOW
      delay(1000);
        }
   delay(500);
}
void sendData(const int Port, const char* host,const char* filepath , String data){
  wifiClient.connect(host, Port);
  wifiClient.println("POST "+(String)filepath+data+" HTTP/1.1");
  wifiClient.println("Host: " + (String)host);
  wifiClient.println("User-Agent: ESP8266/1.0");
  wifiClient.print(data)





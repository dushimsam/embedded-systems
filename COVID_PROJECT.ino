#include <LiquidCrystal_I2C.h>

#include <ESP8266WiFi.h>


#define REDPIN 14
#define GREENPIN 16
#define BUZZERPIN 12
#define TEMPVOUT A0

int lcdColumns = 16;
int lcdRows = 2;
WiFiClient wifiClient;
const char* host = "192.168.1.208";

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

void setup() {

    Serial.begin(9600);

    Serial.println("================================");
    Serial.println("===== DOBY's TEMPO LIMITED =====");
    Serial.println("================================");
    
    // LEDs
    pinMode(REDPIN, OUTPUT);
    pinMode(GREENPIN, OUTPUT);
    pinMode(BUZZERPIN, OUTPUT);

    // LCD
//    lcd.begin(5,4);
//    lcd.init();
//    lcd.backlight();    

    // SETUP WiFi
//     WiFi.begin("RCA-WiFi", "rca@2019");
 connectToWiFi("RCA-WiFi", "rca@2019");     
}

void loop() {

  // Read Temperature 
  int rawData = analogRead(TEMPVOUT);
  float celsius = rawData*(5/1024.0)*100;

  Serial.print("Degrees READ=   ");
  Serial.println(celsius);

  String mData="";
  String device_name = "340722SPE0082022";
  mData = "device="+device_name+"&temperature="+(String)celsius;
    
  if(celsius <= 38 ){
    // Turn on GREEN LED
    digitalWrite(GREENPIN, HIGH);
    // Turn off GREEN LED
    digitalWrite(REDPIN, LOW);
    // Turn off BUZZER
    digitalWrite(BUZZERPIN, LOW);

//    lcd.clear();
//    lcd.setCursor(0,0);
//    lcd.print("NORMAL");   

    
  }else{
    // Turn on GREEN LED
    digitalWrite(GREENPIN, LOW);
    // Turn off GREEN LED
    digitalWrite(REDPIN, HIGH);
    // Turn off BUZZER
    digitalWrite(BUZZERPIN, HIGH);
//
//    lcd.clear();
//    lcd.setCursor(0,0);
//    lcd.print("ABNORMAL");  

//    sendData(80, "http://192.168.1.150", "/api/data/" , mData); 
    connectToHost(8000);
    transferData(mData, "/iot/"); 
  }

  delay(2000);

}


void sendData(const int httpPort, const char* host,const char* filepath , String data){
  
  wifiClient.connect(host, httpPort); 

  wifiClient.println("POST "+(String)filepath+" HTTP/1.1");
  wifiClient.println("Host: " + (String)host);
  wifiClient.println("User-Agent: ESP8266/1.0");
  wifiClient.println("Content-Type: application/x-www-form-urlencoded");
  wifiClient.println("Content-Length: " +(String)data.length());
  wifiClient.println();
  wifiClient.print(data);

  Serial.println("Response: " + wifiClient.readStringUntil('\n'));

}

void connectToWiFi(const char* ssid, const char* passwd){
    WiFi.mode(WIFI_OFF); //This prevents reconnection issue
    delay(10);
    WiFi.mode(WIFI_STA); //This hides the viewing of ESP as wifi hotspot
    WiFi.begin(ssid, passwd); //Connect to your WiFi router
    while (WiFi.status() != WL_CONNECTED){
        delay(1000);
        Serial.print(".");
    }
    Serial.println();  
}
void connectToHost(const int httpPort){
    int retry_counter=0; //To be used while retrying to get connected
    wifiClient.setTimeout(15000); // 15 Seconds
    delay(1000);
    Serial.printf("Connecting to \"%s\"\n", host);
  
    while((!wifiClient.connect(host, httpPort)) && (retry_counter <= 30)){
      delay(100);
      Serial.print(".");
      retry_counter++;
    }
  
    if(retry_counter==31){
      Serial.println("\nConnection failed.");
      return;
    }
    else{
      Serial.printf("Connected to \"%s\"\n", host);
    }   
}
void transferData(String data, const char* filepath){
    Serial.println("Transferring data... ");
    wifiClient.println("POST "+(String)filepath+" HTTP/1.1");
    wifiClient.println("Host: " + (String)host);
    wifiClient.println("User-Agent: ESP8266/1.0");
    wifiClient.println("Content-Type: application/x-www-form-urlencoded");
    wifiClient.println("Content-Length: " +(String)data.length());
    wifiClient.println();
    wifiClient.print(data); 
    getFeedback("Success");
}
/*
 * GET FEEDBACK
*/
void getFeedback(String success_msg){
    String datarx;
    while (wifiClient.connected()){
        String line = wifiClient.readStringUntil('\n');
        if (line == "\r") {
        break;
        }
    }
    while (wifiClient.available()){
        datarx += wifiClient.readStringUntil('\n');
    }
    if(datarx.indexOf(success_msg) >= 0){
        Serial.println("Data Transferred.\n");  
    }
    else{
        Serial.println("Data Transfer Failed.\n"); 
    }
    datarx = "";  
}

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include<EEPROM.h>
#include <Ticker.h> 

//Object Declaration
Ticker thread;
WiFiClientSecure client;

//To connect externally
const char* ssid = "Fingerpointengg";
const char* password = "onestopshop";
const char* host = "fpelabs.com";
const int httpsPort = 443;

//Important String to find important params
static String bulb_1;
static String changedb = "0";

//Flags - To be renamed as per its use
int flag1=0;
int flag2=0;
int flag3=0;

//Helper Variables
int powerCycleCounter=0;
int firstStateLock=1;

#define powerFailure A0
#define input1 D1 
#define output1  D2

#define EEPROM_WRITE_TIMEOUT 600

void setup() {
  Serial.begin(115200); 
  EEPROM.begin(1);
  pinMode(input1, INPUT);
  pinMode(output1, OUTPUT);
  thread.attach(0.5,EEPROM_THREAD);// every 0.5second call thread
  // set the LED to the last stored state
  Serial.println("First State");
  if(EEPROM.read(0)==1){
    Serial.print(EEPROM.read(0));
    digitalWrite(output1, HIGH);
    bulb_1="1";
  }else{
    Serial.print(EEPROM.read(0));
    digitalWrite(output1, LOW);
    bulb_1="0";
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
}

void loop() {
  
  if((digitalRead(input1)==HIGH)&&flag1==0)
  {
    if(firstStateLock==0){
      Serial.println("high firstStateLock==0");
      doToggle();
      changedb="1";
      flag1=1;
      flag2=0;
    }else{
      Serial.println("high firstStateLock==1");
      firstStateLock=0;
      changedb="1";
      flag1=1;
      flag2=0;
    }
    
  }else if ((digitalRead(input1)==LOW)&&flag2==0){
    if(firstStateLock==0){
    Serial.println(" low firstStateLock==0");
    doToggle();
    changedb="1";
    flag1=0;
    flag2=1;
    }else{
      Serial.println("low firstStateLock==1");
      firstStateLock=0;
      changedb="1";
      flag1=0;
      flag2=1;
    }
    
  }
   URLping();
}

void doToggle(){
  Serial.println("Toggling");
  if(bulb_1.equals("1")){
    digitalWrite(output1, LOW);
    bulb_1="0";
  }
  else if (bulb_1.equals("0")){
    digitalWrite(output1, HIGH);
    bulb_1="1";
  }
}


void URLping(){
client.connect(host, httpsPort);
  client.setTimeout(4000);
  if(client.connected()){
  client.print(String("POST /Android_App/Hardware/read_esp.php HTTP/1.0\r\n") +
               "Host: " + host + 
               "\r\nCache-Control: no-cache\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 54\r\n\r\nsecret=n1mcyexjgp&changeDb="+changedb+"&bulb_1="+bulb_1+"&fan=0&tube_light=0&socket=0\r\n");

    String line = client.readStringUntil('}')+"}";
    if(!(line.equals("}"))){

    DynamicJsonBuffer jsonBuffer;
    //String line = "{\"status\":\"readsuccess\",\"data\":\"state\",\"bulb_1\":\"1\",\"fan\":\"0\",\"tube_light\":\"0\",\"socket\":\"0\"}";
    JsonObject& root = jsonBuffer.parseObject(line.substring(line.lastIndexOf("{")));
    Serial.println("line.substring(line.lastIndexOf(\"{\"))");
    Serial.println(line.substring(line.lastIndexOf("{")));
    
    if(changedb.equals("1"))
    {
      String isdbchanged = root["dbChange"];   
      if(isdbchanged.equals("success")){
        changedb="0";
      }
      
    }else if(changedb.equals("0")&&!(root["bulb_1"].as<String>().equals(bulb_1))) {    
        doToggle();
      }
      jsonBuffer.clear();
      client.stop();
    }
  }
}

void EEPROM_THREAD(){
  if(powerCycleCounter>=EEPROM_WRITE_TIMEOUT&&flag3==0){
    flag3=1;
    Serial.println("Counter Timeout Reached");
    Serial.print(powerCycleCounter);
  }else if(flag3==0){
    powerCycleCounter++;
  }
  if(analogRead(powerFailure)<100&&flag3==1){
    Serial.println(analogRead(powerFailure));
    if(bulb_1.equals("1")){
      EEPROM.write(0, 1);
      EEPROM.commit();
      Serial.println("State 1 saved in flash memory");
    }
    else if(bulb_1.equals("0")){
      EEPROM.write(0, 0);
      EEPROM.commit();
      Serial.println("State 0 saved in flash memory");
    }
    flag3=0;
    powerCycleCounter=0;
  }
}



 

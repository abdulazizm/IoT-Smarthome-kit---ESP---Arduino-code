#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <EEPROM.h>
#include <Ticker.h> 
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         // https://github.com/tzapu/WiFiManager
#include <ArduinoJson.h>


//Object Declaration

WiFiManager wifiManager;
//To connect externally
//const char* ssid = "Fingerpointengg";
//const char* password = "onestopshop";
const char* host = "fpelabs.com";
const int httpsPort = 443;

//Important String to find important params
static String bulb_1 ="0";
static String fan="0";
static String tube_light="0";
static String socket="0";

static String changedb = "0";
//static String parentalLock ="0";

//Flags - To be renamed as per its use
//for bulb_1
int flag1=0;
int flag2=0;


//for fan
int flag11=0;
int flag12=0;

//for tub_light
int flag21=0;
int flag22=0;

//for socket
int flag31=0;
int flag32=0;

//Helper Variables
//int powerCycleCounter=0;
int firstStateLock=1;
int secondStateLock=1;
int thirdStateLock=1;
int fourthStateLock=1;
//parental_lock
int setFlag=0;

//#define powerFailure A0 //2
//declaring input and output variables
#define input1 D0
#define output1 D1
#define input2 D4
#define output2 D2
#define input3 D5
#define output3 D3
#define input4 D6
#define output4 D7

Ticker thread;
//#define EEPROM_WRITE_TIMEOUT 1000
//300000 for  5 min
void setup() {
  Serial.begin(115200); 
  EEPROM.begin(4096);
  pinMode(input1, INPUT);
  pinMode(input2, INPUT);
  pinMode(input3, INPUT);
  pinMode(input4, INPUT);
  //pinMode(powerFailure, INPUT);
  pinMode(output1, OUTPUT);
  pinMode(output2, OUTPUT);
  pinMode(output3, OUTPUT);
  pinMode(output4, OUTPUT);
  thread.attach(0.1,SWITCH_THREAD);
//  pinMode(test, INPUT);

 
  // set the LED to the last stored state
//  Serial.println("First State");
  //bulb_1
  if(EEPROM.read(0)==1){
    Serial.print(EEPROM.read(0));
    digitalWrite(output1, LOW);
    bulb_1="0";
  }else{
    Serial.print(EEPROM.read(0));
    digitalWrite(output1, HIGH);
    bulb_1="1";
  }
  //fan
   if(EEPROM.read(1)==1){
    Serial.print(EEPROM.read(1));
    digitalWrite(output2, LOW);
    fan="0";
  }else{
    Serial.print(EEPROM.read(1));
    digitalWrite(output2, HIGH);
    fan="1";
  }
  //tube_light
   if(EEPROM.read(2)==1){
    Serial.print(EEPROM.read(2));
    digitalWrite(output3, LOW);
    tube_light="0";
  }else{
    Serial.print(EEPROM.read(2));
    digitalWrite(output3, HIGH);
    tube_light="1";
  }
  //socket
   if(EEPROM.read(3)==1){
    Serial.print(EEPROM.read(3));
    digitalWrite(output4, LOW);
    socket="0";
  }else{
    Serial.print(EEPROM.read(3));
    digitalWrite(output4, HIGH);
    socket="1";
  }
    // every 0.5second call thread
 // WiFi.mode(WIFI_AP_STA);
 // WiFi.begin(ssid, password);

 
 wifiManager.autoConnect("iSmart A4","12345678");
  
}

void loop() {

    URLping();
}


void doToggle_1(){
  Serial.println("Toggling");
  //bulb_1
  if(bulb_1.equals("1")){
    digitalWrite(output1, LOW);
    EEPROM.write(0,1);
    EEPROM.commit();
    bulb_1="0";
  }
  else if (bulb_1.equals("0")){
    digitalWrite(output1, HIGH);
     EEPROM.write(0,0);
     EEPROM.commit();
    bulb_1="1";
  }
}
  //fan
void doToggle_2(){
   if(fan.equals("1")){
    digitalWrite(output2, LOW);
    EEPROM.write(1,1);
    EEPROM.commit();
    fan="0";
  }
  else if (fan.equals("0")){
    digitalWrite(output2, HIGH);
    EEPROM.write(1,0);
    EEPROM.commit();
    fan="1";
  }
}
  //tube_light
void doToggle_3(){
   if(tube_light.equals("1")){
    digitalWrite(output3, LOW);
    EEPROM.write(2,1);
    EEPROM.commit();
    tube_light="0";
  }
  else if (tube_light.equals("0")){
    digitalWrite(output3, HIGH);
    EEPROM.write(2,0);
    EEPROM.commit();
    tube_light="1";
  }
} 
  //socket
void doToggle_4(){
   if(socket.equals("1")){
    digitalWrite(output4, LOW);
    EEPROM.write(3,1);
    EEPROM.commit();
    socket="0";
  }
  else if (socket.equals("0")){
    digitalWrite(output4, HIGH);
    EEPROM.write(3,0);
    EEPROM.commit();
    socket="1";
  }
}


void URLping(){
  WiFiClientSecure client;
  client.connect(host, httpsPort);
  client.setTimeout(4000);
  if(client.connected()){
    Serial.println("Client connected");
    client.print(String("POST /Android_App/Hardware/read_esp.php HTTP/1.1\r\n") +
               "Host: " + host + 
               "\r\nCache-Control: no-cache\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 65\r\n\r\nsecret=64lak85ore&changeDb="+changedb+"&bulb_1="+bulb_1+"&fan="+fan+"&tube_light="+tube_light+"&socket="+socket+"\r\n");

    String line = client.readStringUntil('}')+"}";
    String parsedJson = line.substring(line.lastIndexOf("{"));
    Serial.println(parsedJson);
    if(!(line.equals("}"))&&!(parsedJson.equals("{\"status\":\"failedToQuery\"}"))){

    DynamicJsonBuffer jsonBuffer;
    //String line = "{\"status\":\"readsuccess\",\"data\":\"state\",\"bulb_1\":\"1\",\"fan\":\"0\",\"tube_light\":\"0\",\"socket\":\"0\"}";
    JsonObject& root = jsonBuffer.parseObject(parsedJson);
    
    if(changedb.equals("1"))
    {
      String isdbchanged = root["dbChange"];   
      if(isdbchanged.equals("success")){
        changedb="0";
      }
      
    }else {
      if(changedb.equals("0")&&!(root["bulb_1"].as<String>().equals(bulb_1))&&((root["bulb_1"].as<String>().equals("0"))||(root["bulb_1"].as<String>().equals("1")))) {        
        doToggle_1();
      }
     if(changedb.equals("0")&&!(root["fan"].as<String>().equals(fan))&&((root["fan"].as<String>().equals("0"))||(root["fan"].as<String>().equals("1")))) {    
        doToggle_2();
      }
     if(changedb.equals("0")&&!(root["tube_light"].as<String>().equals(tube_light))&&((root["tube_light"].as<String>().equals("0"))||(root["tube_light"].as<String>().equals("1")))) {    
        doToggle_3();
      }
     if(changedb.equals("0")&&!(root["socket"].as<String>().equals(socket))&&((root["socket"].as<String>().equals("0"))||(root["socket"].as<String>().equals("1")))) {    
        doToggle_4();
      }
     if(changedb.equals("0")&&(root["reset"].as<String>().equals("true"))){
       wifiManager.resetSettings();
       Serial.println("wifi reset");
       ESP.restart();
     }
     if(changedb.equals("0")&&(root["parental_lock"].as<String>().equals("true"))){
      setFlag=1;
     }
     else{
      setFlag=0;
     }
    }
      jsonBuffer.clear();
    } 
      client.stop();
      delay(2000);
    }
}
void SWITCH_THREAD()
{
  if(setFlag==0){
   //bulb_1
  if((digitalRead(input1)==HIGH)&&flag1==0)
  {
    if(firstStateLock==0){
      Serial.println("high firstStateLock==0");
      doToggle_1();
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
    doToggle_1();
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
//fan
  if((digitalRead(input2)==HIGH)&&flag11==0)
  {
    if(secondStateLock==0){
      Serial.println("high secondStateLock==0");
      doToggle_2();
      changedb="1";
      flag11=1;
      flag12=0;
    }else{
      Serial.println("high secondStateLock==1");
      secondStateLock=0;
      changedb="1";
      flag11=1;
      flag12=0;
    }
    
  }else if ((digitalRead(input2)==LOW)&&flag12==0){
    if(secondStateLock==0){
    Serial.println(" low secondStateLock==0");
    doToggle_2();
    changedb="1";
    flag11=0;
    flag12=1;
    }else{
      Serial.println("low secondStateLock==1");
      secondStateLock=0;
      changedb="1";
      flag11=0;
      flag12=1;
    } 
  }
 //tube_light
    if((digitalRead(input3)==HIGH)&&flag21==0)
  {
    if(thirdStateLock==0){
      Serial.println("high thirdStateLock==0");
      doToggle_3();
      changedb="1";
      flag21=1;
      flag22=0;
    }else{
      Serial.println("high thirdStateLock==1");
      thirdStateLock=0;
      changedb="1";
      flag21=1;
      flag22=0;
    }
    
  }else if ((digitalRead(input3)==LOW)&&flag22==0){
    if(thirdStateLock==0){
    Serial.println(" low thirdStateLock==0");
    doToggle_3();
    changedb="1";
    flag21=0;
    flag22=1;
    }else{
      Serial.println("low thirdStateLock==1");
      thirdStateLock=0;
      changedb="1";
      flag21=0;
      flag22=1;
    } 
  }
 //socket
   if((digitalRead(input4)==HIGH)&&flag31==0)
  {
    if(fourthStateLock==0){
      Serial.println("high fourthStateLock==0");
      doToggle_4();
      changedb="1";
      flag31=1;
      flag32=0;
    }else{
      Serial.println("high fourthStateLock==1");
      fourthStateLock=0;
      changedb="1";
      flag31=1;
      flag32=0;
    }
    
  }else if ((digitalRead(input4)==LOW)&&flag32==0){
    if(fourthStateLock==0){
    Serial.println(" low fourthStateLock==0");
    doToggle_4();
    changedb="1";
    flag31=0;
    flag32=1;
    }else{
      Serial.println("low fourthStateLock==1");
      fourthStateLock=0;
      changedb="1";
      flag31=0;
      flag32=1;
    }
   } 
  }
}

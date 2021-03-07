#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include<EEPROM.h>

IPAddress ip(192, 168, 43, 50); // where xx is the desired IP Address
IPAddress gateway(192, 168, 43, 1); // set gateway to match your network
//Serial.print(F("Setting static ip to : "));
//Serial.println(ip);
IPAddress subnet(255, 255, 255, 0); // set subnet mask to match yournetwork

IPAddress ip2(192,168,4,50);         // choose IP address
IPAddress gateway2(192,168,4,1); 
IPAddress subnet2(255,255,255,0);
//Our Wi-Fi ssdid and password
char* ssid = "fpe";
char* password = "12345678";
String bulb_1 = "0";
String tube_light = "0";
String fan = "0";
String socket = "0";
String manualSwitch = "0";//bulb
String onlineSwitch = "0";
int flag1=0;
int flag2=0;
#define input1  D0//D0 //D4
#define output1  D1//D4 //D0
String manualSwitch2 = "0";//fan
String onlineSwitch2= "0";
int flag11=0;
int flag12=0;
#define input2  D4 //D5
#define output2  D2 //D1
String manualSwitch3 = "0";//tube_light
String onlineSwitch3 = "0";
int flag21=0;
int flag22=0;
#define input3 D5 //D6
#define output3 D3 //D2
String manualSwitch4 = "0";//socket
String onlineSwitch4 = "0";
int flag31=0;
int flag32=0;
#define input4 D6  //D7
#define output4 D7 //D3
int firstStateLock = 1;
int secondStateLock = 1;
int thirdStateLock = 1;
int fourthStateLock = 1;
//parental_lock
int setFlag=1;
String parental_lock ="0";
int pFlag=0;


ESP8266WebServer server(80); //server variable

void setup() {
 // Serial.begin(115200);
  EEPROM.begin(4096);
  pinMode(input1, INPUT);
  pinMode(input2, INPUT);
  pinMode(input3, INPUT);
  pinMode(input4, INPUT);
  pinMode(output1, OUTPUT);
  pinMode(output2, OUTPUT);
  pinMode(output3, OUTPUT);
  pinMode(output4, OUTPUT);

  digitalWrite(output1,LOW );
  digitalWrite(output2, LOW);
  digitalWrite(output3, LOW);
  digitalWrite(output4, LOW);
  //EEPROM READ
    if(EEPROM.read(0)==1){
//    Serial.print("EEPROM.read(0)");
//    Serial.print(EEPROM.read(0));
    digitalWrite(output1, LOW);
    bulb_1="1";
  }else{
//    Serial.print("EEPROM.read(0)");
//    Serial.print(EEPROM.read(0));
    digitalWrite(output1, HIGH);
    bulb_1="0";
  }
  //fan
   if(EEPROM.read(1)==1){
//    Serial.print(EEPROM.read(1));
    digitalWrite(output2, LOW);
    fan="1";
  }else{
//    Serial.print(EEPROM.read(1));
    digitalWrite(output2, HIGH);
    fan="0";
  }
  //tube_light
   if(EEPROM.read(2)==1){
//    Serial.print(EEPROM.read(2));
    digitalWrite(output3, LOW);
    tube_light="1";
  }else{
//    Serial.print(EEPROM.read(2));
    digitalWrite(output3, HIGH);
    tube_light="0";
  }
  //socket
   if(EEPROM.read(3)==1){
//    Serial.print(EEPROM.read(3));
    digitalWrite(output4, LOW);
    socket="1";
  }else{
//    Serial.print(EEPROM.read(3));
    digitalWrite(output4, HIGH);
    socket="0";
  }
  //Making Connection With netword
  WiFi.mode(WIFI_STA);//WiFi.mode(WIFI_STA);
 // WiFi.softAP("fpelabs","12345678");
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);
//  WiFi.softAPConfig(ip2, gateway2, subnet2); 
  serverSection();
 
}

void loop() {
  
  server.handleClient();
  if(parental_lock.equals("0")){
  
  if((digitalRead(input1)==HIGH)&&flag1==0)
  {
    if(firstStateLock==0){
      manualSwitch="0";
      bulb_1_state();
      flag1=1;
      flag2=0;
    } else{
      //manualSwitch="0";
      //bulb_1_state();
      firstStateLock=0;
      flag1=1;
      flag2=0;
    }
  }else if((digitalRead(input1)==LOW)&&flag2==0)
  {
    if(firstStateLock==0){
      manualSwitch="1";
      bulb_1_state();
      flag1=0;
      flag2=1;
    }
    else{
//      manualSwitch="1";
//      bulb_1_state();
      firstStateLock=0;
      flag1=0;
      flag2=1;
    }
  }  

  if((digitalRead(input2)==HIGH)&&flag11==0)
  {
    if(secondStateLock==0){
    manualSwitch2="0";
    fan_state();
    flag11=1;
    flag12=0;
    }
    else {
//      manualSwitch2="0";
//    fan_state();
    secondStateLock=0;
    flag11=1;
    flag12=0;
    }
  }else if((digitalRead(input2)==LOW)&&flag12==0)
  {
     if(secondStateLock==0){
    manualSwitch2="1";
    fan_state();
    flag11=0;
    flag12=1;
     }
     else{
      secondStateLock =0;
      flag11=0;
      flag12=1;
     }
  }  

  if((digitalRead(input3)==HIGH)&&flag21==0)
  {
   if(thirdStateLock == 0){ 
    manualSwitch3="0";
    tube_light_state();
    flag21=1;
    flag22=0;
   }
   else
   {
    thirdStateLock = 0;
    flag21=1;
    flag22=0;
   }
  }else if((digitalRead(input3)==LOW)&&flag22==0)
  {
   if(thirdStateLock == 0){
    manualSwitch3="1";
    tube_light_state();
    flag21=0;
    flag22=1;
   }
   else{
    thirdStateLock = 0;
    flag21=0;
    flag22=1;
   }
   }
    

  if((digitalRead(input4)==HIGH)&&flag31==0)
  {
   if(fourthStateLock == 0){ 
    manualSwitch4="0";
    socket_state();
    flag31=1;
    flag32=0;
   }
   else{
    fourthStateLock = 0;
    flag31=1;
    flag32=0;
   }
  }else if((digitalRead(input4)==LOW)&&flag32==0)
  {
   if(fourthStateLock == 0){
    manualSwitch4="1";
    socket_state();
    flag31=0;
    flag32=1;
   }
   else{
    fourthStateLock = 0;
    flag31=0;
    flag32=1;
   }
  } 
 } 
}

void serverSection(){
  server.on("/", []() {
    server.send(200, "text/html", "<!DOCTYPE html><html><meta charset='UTF-8'><head></head><body><h2>FPELABS</h2><h3><a href='/bulb_1'>bulb1</a></h3><br><h3><a href='/fan'>fan</a></h3><br><h3><a href='/tube_light'>tube_Light</a></h3><br><h3><a href='/socket'>socket</a></h3><br><h4><a href='/parental_lock'>parental_lock</a></h4><br></body></html>");
  });

  server.on("/parental_lock",parental_lock_state);
  
  server.on("/bulb_1", bulb_1_state);
  server.on("/fan", fan_state);
  server.on("/tube_light", tube_light_state);
  server.on("/socket",socket_state);

  server.on("/status", all_state);
  
  server.begin();
}
void parental_lock_state(){
  if(pFlag==0 ){
    
      parental_lock ="0";
     // Serial.println("condition-1");
      server.send(200, "text/html", parental_lock);
      pFlag=1;
  }
  else{
      parental_lock ="1";
     // Serial.println("condition-2");
      server.send(200, "text/html", parental_lock);
      pFlag=0;
     }
}

void bulb_1_state(){
 
  if(manualSwitch=="0"&& onlineSwitch=="1")
  {
    
    bulb_1 = "1";
    digitalWrite(output1, LOW);
    EEPROM.write(0, 1);
    EEPROM.commit();
//    Serial.println("bulb_1 = 1");
//    Serial.print("EEPROM.read(0)");
//    Serial.println(EEPROM.read(0));
    server.send(200, "text/html", bulb_1);
    onlineSwitch="0";
    
  }else if(manualSwitch=="1"&& onlineSwitch=="0")
  {
    bulb_1 = "0";
    digitalWrite(output1, HIGH);
    EEPROM.write(0, 0);
    EEPROM.commit();
//    Serial.println("bulb_1 = 0");
//    Serial.print("EEPROM.read(0)");
//    Serial.println(EEPROM.read(0));
    server.send(200, "text/html", bulb_1);
    onlineSwitch="1";

  }else if(manualSwitch=="1"&& onlineSwitch=="1")
  {
    bulb_1 = "1";
    digitalWrite(output1, LOW);
    EEPROM.write(0, 1);
    EEPROM.commit();
//    Serial.println("bulb_1 = 1");
//    Serial.print("EEPROM.read(0)");
//    Serial.println(EEPROM.read(0));
    server.send(200, "text/html", bulb_1);
    onlineSwitch="0";
 
  }else if(manualSwitch=="0"&& onlineSwitch=="0")
  {
    //working
    //at first toggle online
    bulb_1 = "0";
    digitalWrite(output1, HIGH);
    EEPROM.write(0, 0);
    EEPROM.commit();
//    Serial.println("bulb_1 = 0");
//    Serial.print("EEPROM.read(0)");
//    Serial.println(EEPROM.read(0));
    server.send(200, "text/html", bulb_1);
    onlineSwitch="1";

  }
}

void fan_state(){
  if(manualSwitch2=="0"&& onlineSwitch2=="1")
  {
    
    fan = "1";
    digitalWrite(output2, LOW);
    EEPROM.write(1, 1);
    EEPROM.commit();
    server.send(200, "text/html", fan);
    onlineSwitch2="0";
    
  }else if(manualSwitch2=="1"&& onlineSwitch2=="0")
  {
    fan = "0";
    digitalWrite(output2, HIGH);
    EEPROM.write(1, 0);
    EEPROM.commit();
    server.send(200, "text/html", fan);
    onlineSwitch2="1";

  }else if(manualSwitch2=="1"&& onlineSwitch2=="1")
  {
    fan = "1";
    digitalWrite(output2, LOW);
    EEPROM.write(1, 1);
    EEPROM.commit();
    server.send(200, "text/html", fan);
    onlineSwitch2="0";
 
  }else if(manualSwitch2=="0"&& onlineSwitch2=="0")
  {
    fan = "0";
    digitalWrite(output2, HIGH);
    EEPROM.write(1, 0);
    EEPROM.commit();
    server.send(200, "text/html", fan);
    onlineSwitch2="1";

  }
}


void tube_light_state(){
  if(manualSwitch3=="0"&& onlineSwitch3=="1")
  {
    
   tube_light = "1";
    digitalWrite(output3, LOW);
    EEPROM.write(2, 1);
    EEPROM.commit();
    server.send(200, "text/html",tube_light);
    onlineSwitch3="0";
    
  }else if(manualSwitch3=="1"&& onlineSwitch3=="0")
  {
    tube_light = "0";
    digitalWrite(output3, HIGH);
    EEPROM.write(2, 0);
    EEPROM.commit();
    server.send(200, "text/html",tube_light);
    onlineSwitch3="1";

  }else if(manualSwitch3=="1"&& onlineSwitch3=="1")
  {
   tube_light = "1";
    digitalWrite(output3, LOW);
    EEPROM.write(2, 1);
    EEPROM.commit();
    server.send(200, "text/html",tube_light);
    onlineSwitch3="0";
 
  }else if(manualSwitch3=="0"&& onlineSwitch3=="0")
  {
   tube_light = "0";
    digitalWrite(output3, HIGH);
    EEPROM.write(2, 0);
    EEPROM.commit();
    server.send(200, "text/html",tube_light);
    onlineSwitch3="1";

  }
}

void socket_state(){
  if(manualSwitch4=="0"&& onlineSwitch4=="1")
  {
    
    socket = "1";
    digitalWrite(output4, LOW);
    EEPROM.write(3, 1);
    EEPROM.commit();
    server.send(200, "text/html", socket);
    onlineSwitch4="0";
    
  }else if(manualSwitch4=="1"&& onlineSwitch4=="0")
  {
    socket = "0";
    digitalWrite(output4, HIGH);
    EEPROM.write(3, 0);
    EEPROM.commit();
    server.send(200, "text/html", socket);
    onlineSwitch4="1";

  }else if(manualSwitch4=="1"&& onlineSwitch4=="1")
  {
    socket = "1";
    digitalWrite(output4, LOW);
    EEPROM.write(3, 1);
    EEPROM.commit();
    server.send(200, "text/html", socket);
    onlineSwitch4="0";
 
  }else if(manualSwitch4=="0"&& onlineSwitch4=="0")
  {
    socket = "0";
    digitalWrite(output4, HIGH);
    EEPROM.write(3, 0);
    EEPROM.commit();
    server.send(200, "text/html", socket);
    onlineSwitch4="1";

  }
}

void all_state(){
  
  server.send(200, "text/html", "{'bl':'"+bulb_1+"','fl':'"+tube_light+"','tl':'"+fan+"','s1':'"+socket+"','p1':'"+parental_lock+"'}");
}

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

//Our Wi-Fi ssdid and password
char* ssid = "fpe";
char* password = "12345678";
String bulb_1 = "0";
String tube_light = "0";
String fan = "0";
String socket = "0";
String manualSwitch = "0";
String onlineSwitch = "0";
int flag1=0;
int flag2=0;
#define input1  1
#define output1  2
//#define input3  2
//#define input4  3
ESP8266WebServer server; //server variable

void setup() {
  PINS(); //call function
  String tube_light = "1";
  String fan = "1";
  String socket = "1";
  //Making Connection With netword
  WiFi.begin(ssid, password);
 
  serverSection();
 
}

void loop() {
  server.handleClient();
  
  if((digitalRead(input1)==HIGH)&&flag1==0)
  {
    manualSwitch="0";
    bulb_1_state();
    flag1=1;
    flag2=0;
  }else if((digitalRead(input1)==LOW)&&flag2==0)
  {
    //blue light on
    manualSwitch="1";
    bulb_1_state();
    flag1=0;
    flag2=1;
  }  
}

void PINS(){
  pinMode(1,FUNCTION_3);
  pinMode(input1, INPUT);
  pinMode(output1, OUTPUT);
//  pinMode(input3, OUTPUT);
//  pinMode(input4, OUTPUT);

  
//  digitalWrite(3,FUNCTION_3);
  digitalWrite(output1, LOW);
//  digitalWrite(input2, LOW);
//  digitalWrite(input3, LOW);
//  digitalWrite(input4, LOW);
 
}

void serverSection(){
  server.on("/", []() {
    server.send(200, "text/html", "<!DOCTYPE html><html><meta charset='UTF-8'><head></head><body><h2>FPELABS</h2><h3><a href='/bulb_1'>bulb1</a></h3><br><h3><a href='/fan'>fan</a></h3><br><h3><a href='/tube_light'>tube_Light</a></h3><br><h3><a href='/socket'>socket</a></h3><br></body></html>");
  });


  server.on("/bulb_1", bulb_1_state);
  server.on("/tube_light", tube_light_state);
  server.on("/fan", fan_state);
  server.on("/socket",socket_state);

  server.on("/status", all_state);
  
  server.begin();
}

void bulb_1_state(){
 
  if(manualSwitch=="0"&& onlineSwitch=="1")
  {
    
    bulb_1 = "1";
    digitalWrite(output1, LOW);
    server.send(200, "text/html", bulb_1);
    onlineSwitch="0";
    
  }else if(manualSwitch=="1"&& onlineSwitch=="0")
  {
    bulb_1 = "0";
    digitalWrite(output1, HIGH);
    server.send(200, "text/html", bulb_1);
    onlineSwitch="1";

  }else if(manualSwitch=="1"&& onlineSwitch=="1")
  {
    bulb_1 = "1";
    digitalWrite(output1, LOW);
    server.send(200, "text/html", bulb_1);
    onlineSwitch="0";
 
  }else if(manualSwitch=="0"&& onlineSwitch=="0")
  {
    //working
    //at first toggle online
    bulb_1 = "0";
    digitalWrite(output1, HIGH);
    server.send(200, "text/html", bulb_1);
    onlineSwitch="1";

  }
}

void fan_state(){
  if(fan == "0"){
    fan = "1";
    //digitalWrite(1, HIGH);
    server.send(200, "text/html", fan);
  }else{
    fan = "0";
    //digitalWrite(1, LOW);
    server.send(200, "text/html", fan);
  }
}

void tube_light_state(){
  if(tube_light == "0"){
    tube_light = "1";
    //digitalWrite(2, HIGH);
    server.send(200, "text/html", tube_light);
  }else{
    tube_light = "0";
    //digitalWrite(2, LOW);
    server.send(200, "text/html", tube_light);
  }
}

void socket_state(){
  if(socket == "0"){
    socket = "1";
    //digitalWrite(3, HIGH);
    server.send(200, "text/html", socket);
  }else{
    socket = "0";
    //digitalWrite(3, LOW);
    server.send(200, "text/html", socket);
  }
}

void all_state(){
  
  server.send(200, "text/html", "{'bl':'"+bulb_1+"','fl':'"+tube_light+"','tl':'"+fan+"','s1':'"+socket+"'}");

}

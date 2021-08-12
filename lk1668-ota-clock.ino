#include <ESPWiFiManager.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>


#define STB 0
#define CLK 2
#define DIO 1
#define BTN 3
long int stat;
unsigned char disp_buffer[14];
int dot=0;
int h=0, m=0;
long timer;
boolean setupMode = false;

const long utcOffsetInSeconds = 19800;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", utcOffsetInSeconds);

ESPWiFiManager wifi;

ESP8266WebServer server(80);

void getNtpTime(){
  timeClient.update();
  h = timeClient.getHours();
  if(h==0){
    h=12;
  } else if(h>12){
    h = h - 12;
  }
  m = timeClient.getMinutes();
}

void displayTime(){
  disp_buffer[12]=c2h(m%10);
  disp_buffer[10]=c2h((m/10)%10);
  if(h>9)
  {
    disp_buffer[8]=c2h(h%10);
    disp_buffer[6]=c2h((h/10)%10);
    if(dot==1){
      disp_buffer[8] += 0x01;
    }
  }
  else
  {
    disp_buffer[8]=c2h(h%10);
    disp_buffer[6]=0x00;
    if(dot==1){
      disp_buffer[8] += 0x01;
    }
  }
}

void setup() {
  pinMode(STB, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(DIO, OUTPUT);
  pinMode(BTN, INPUT_PULLUP);
  digitalWrite(BTN, HIGH);
//  Serial.begin(115200); 
  clear_display();
  
  //display --:-- while fetching time
  disp_buffer[12]=0x02;
  disp_buffer[10]=0x02;
  disp_buffer[8]=0x03;
  disp_buffer[6]=0x02;
  display_data();
      
  stat=millis();
  timer=millis();
  //delay(20000);
  wifi.initWifiManager();
  initOta();
  server.on("/", handleRoot);
  server.begin();
  timeClient.begin();
  getNtpTime();
  
}

void loop() {
  server.handleClient();
  otaHandler();
  wifi.handleHttpRequest();
  handleButton();
  if(!setupMode){
    displayTime();
  }
  long int curr=millis();
  if((curr-stat)>=500)
  {
    stat=curr;
    if(dot==0)
    {
      dot=1;
    }
    else
    {
      dot=0;
    }
  }
  
  long int timenow=millis();
  if(timenow-timer>=60000)
  {
    timer=millis();
    m++;
    if(m>59)
    {
      h++;
      m=0;
    }
    if(h>12)
    {
      h=1;
    }
  }
  
  display_data();
}

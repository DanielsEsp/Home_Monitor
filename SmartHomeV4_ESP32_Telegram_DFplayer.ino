



#include <WiFi.h>
const char* ssid = "SSID";
const char* password = "MDP";
       int  wifitime, esprestart;
       
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#define BOTtoken "TOKEN"  // your Bot Token (Get from Botfather)
#define CHAT_ID "CHATID6"
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
#include <ArduinoJson.h>

// Set GPIOs and Flags
#define    garage 25 //  D2 vient de base 16
   bool    garageFlag;
   bool oldGarageFlag = true;
#define    fosse  26 //  D6 vient de base 17
   bool    fosseFlag;
   bool oldFosseFlag = true;
#define    celier 27 //  D7 vient de base 18
   bool    celierFlag;
   bool oldCelierFlag = true;
   
#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
#define FPSerial Serial1
DFRobotDFPlayerMini myDFPlayer;

void printDetail(uint8_t type, int value);



void setup() {

     Serial.begin(115200); Serial.println("\n \n");

FPSerial.begin(9600, SERIAL_8N1, /*rx =*/16, /*tx =*/17);

     // Connect to Wi-Fi
     WiFi.mode (WIFI_STA);
     WiFi.begin (ssid, password);
     while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
            wifitime ++;
            Serial.print("."); delay(100);
            if (wifitime > 5) { ESP.restart(); }
           }
     // Print ESP32 Local IP Address
     Serial.println(WiFi.localIP());

  if (!myDFPlayer.begin(FPSerial, /*isACK = */true, /*doReset = */true)) {  //Use serial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }

     client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  
     pinMode(garage, INPUT);
     pinMode(fosse, INPUT);
     pinMode(celier, INPUT);

myDFPlayer.volume(10);

    bot.sendMessage(CHAT_ID, "SmartHome V4", "");
    bot.sendMessage(CHAT_ID, "Bot is ready", "");
    
}


void loop() {
  
        garageFlag = !digitalRead(garage); delay(50); // Serial.print("Garage "); Serial.println(garageFlag);                       
        if (garageFlag != oldGarageFlag) {
            oldGarageFlag = garageFlag;
            if (garageFlag == 0) {
                myDFPlayer.play(1);
                bot.sendMessage(CHAT_ID, "Garage porte FERMEE", "");
               } else {
                myDFPlayer.play(2);
                bot.sendMessage(CHAT_ID, "Garage porte OUVERTE", ""); 
               }
           }
 
        fosseFlag = !digitalRead(fosse); delay(50); // Serial.print("Fosse  "); Serial.println(fosseFlag);                        
        if (fosseFlag != oldFosseFlag) {
            oldFosseFlag = fosseFlag;
            if (fosseFlag == 0) {
                myDFPlayer.play(5);
                bot.sendMessage(CHAT_ID, "Fosse niveau BAS", "");
               } else {
                myDFPlayer.play(6);
                bot.sendMessage(CHAT_ID, "Fosse niveau HAUT", ""); 
               }
           }        

       celierFlag = !digitalRead(celier); delay(50); // Serial.print("Celier "); Serial.println(celierFlag);                       
        if (celierFlag != oldCelierFlag) {
            oldCelierFlag = celierFlag;
            if (celierFlag == 0) {
                myDFPlayer.play(4);
                bot.sendMessage(CHAT_ID, "Celier R.à.S", "");
               } else {
                myDFPlayer.play(5);
                bot.sendMessage(CHAT_ID, "Celier FUITE", "");
               }
           }                

}


//

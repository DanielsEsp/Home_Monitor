



#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "SSID";
const char* password = "PASS";
        int wifitime, esprestart;
        
// Initialize Telegram BOT
#define BOTtoken "TOKENk"  // your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "ID"

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

// Set GPIOs and Flags
#define    garage 12 //  D6 vient de base 18
   bool    garageFlag;
   bool oldGarageFlag = false;
#define    fosse  13 //  D7 vient de base 19
   bool    fosseFlag;
   bool oldFosseFlag = false;
#define    celier 14 //  D5 vient de base 21
   bool    celierFlag;
   bool oldCelierFlag = false;



void setup() {
  Serial.begin(115200);

  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
    client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  #endif
  
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

     pinMode(garage, INPUT);
     pinMode(fosse, INPUT);
     pinMode(celier, INPUT);

    bot.sendMessage(CHAT_ID, "SmartHome V4", "");
    bot.sendMessage(CHAT_ID, "Bot is ready", "");
}

void loop() {
        garageFlag = !digitalRead(garage); delay(50); // Serial.print("Garage "); Serial.println(garageFlag);                       
        if (garageFlag != oldGarageFlag) {
            oldGarageFlag = garageFlag;
            if (garageFlag == 0) {
                bot.sendMessage(CHAT_ID, "Garage porte FERMEE", "");
               } else {
                bot.sendMessage(CHAT_ID, "Garage porte OUVERTE", ""); 
               }
           }
 
        fosseFlag = !digitalRead(fosse); delay(50); // Serial.print("Fosse  "); Serial.println(fosseFlag);                        
        if (fosseFlag != oldFosseFlag) {
            oldFosseFlag = fosseFlag;
            if (fosseFlag == 0) {
                bot.sendMessage(CHAT_ID, "Fosse niveau BAS", "");
               } else {
                bot.sendMessage(CHAT_ID, "Fosse niveau HAUT", ""); 
               }
           }        

       celierFlag = !digitalRead(celier); delay(50); // Serial.print("Celier "); Serial.println(celierFlag);                       
        if (celierFlag != oldCelierFlag) {
            oldCelierFlag = celierFlag;
            if (celierFlag == 0) {
                bot.sendMessage(CHAT_ID, "Celier R.à.S", "");
               } else {
                bot.sendMessage(CHAT_ID, "Celier FUITE", "");
               }
           }                


}



//      SmartHome_V3_Connect_base

/*
#define ILI9488_DRIVER
        marron Vcc
        rouge  Gnd
#define bleu   TFT_MOSI 27
#define violet TFT_SCLK 14
#define orange TFT_CS   25
#define vert   TFT_DC   26
#define jaune  TFT_RST  -1//  RST 3,3vcc
#define N/C    TFT_MISO 12

   NEW  MAC Address:  0x10, 0x52, 0x1C, 0x64, 0x76, 0x64
*/


#include "FS.h"
#include "SPIFFS.h"  // For ESP32 only
#define   FS_NO_GLOBALS

//____________________________________________________________________________ 
#include <SPI.h>
#include <TFT_eSPI.h>
          TFT_eSPI tft = TFT_eSPI();
#include "TFT_eSPI.h"
#include "Free_Fonts.h" // Include the header file attached to this sketch

//____________________________________________________________________________  
#include <WiFi.h>
#include <esp_now.h>

#include <esp_wifi.h>
// Set your new MAC Address
uint8_t newMACAddress[] = {0x10, 0x52, 0x1C, 0x64, 0x76, 0x64};

int id;
int analogValue;
int batterie;
bool displayData = false;

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
    int id;
    int temp;
    int humi;
    bool flag;
    int analogValue;
    int batterie;
} struct_message;

// Create a struct_message called test to store variables to be sent
struct_message myDataReceve;

//____________________________________________________________________________  

     unsigned long delayTime_05 = (5*60)*1000;// pour 5 minutes en millisecondes
     unsigned long delayTime_60 = (60*60)*1000;// pour 1 heure en millisecondes
//     unsigned long delayTime = (30)*1000;// pour Time en millisecondes
     unsigned long currentTime = 0;

//____________________________________________________________________________  


#define Garage 18
   bool garageFlag, oldGarageFlag;
#define Fosse  19
   bool fosseFlag, oldFosseFlag;
#define Celier 21
   bool celierFlag, oldCelierFlag;


#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
#define FPSerial Serial1
DFRobotDFPlayerMini myDFPlayer;

void printDetail(uint8_t type, int value);



// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {

     memcpy(&myDataReceve, incomingData, sizeof(myDataReceve));
     Serial.print("Bytes received: ");
     Serial.println(len); delay(50);
     Serial.print("idModule: ");
     Serial.println(myDataReceve.id); delay(50);
     Serial.print("Temperature: ");
     Serial.println(myDataReceve.temp); delay(50);
     Serial.print("Humidite: ");
     Serial.println(myDataReceve.humi); delay(50);
     Serial.print("Etat Flag: ");
     Serial.println(myDataReceve.flag); delay(50);
     Serial.print("Lecture analogValue: ");
     Serial.println(myDataReceve.analogValue);
     Serial.print("Batterie: ");
     Serial.println(myDataReceve.batterie); delay(50);
     Serial.println();     
     displayData = true;


     if (myDataReceve.id == 101 && displayData == true) {
         drawBmp("/chambre_100.bmp", 0, 220);
         drawBmp("/chambre.bmp", 0, 0);
         tft.drawString("chambre", 100, 9, FONT4);   
         tft.fillRect(80, 30, 155, 45, TFT_BLACK);
         tft.drawRightString(String(myDataReceve.temp), 150, 35, FONT6); tft.drawString("o", 155, 30, FONT4);
         drawBmp("/humid_20.bmp", 155, 50);
         tft.drawString(String(myDataReceve.humi), 180, 50, FONT4); tft.drawString("%", 210, 50, FONT4);
         if (myDataReceve.batterie <= 330) {
             drawBmp("/signal.bmp", 0, 0);        
            } else if (myDataReceve.batterie > 330 && myDataReceve.batterie <= 350) {
             tft.drawRoundRect(0, 0, 80, 80, 5, TFT_RED);
            } else if (myDataReceve.batterie > 350 && myDataReceve.batterie <= 360) {
             tft.drawRoundRect(0, 0, 80, 80, 5, TFT_ORANGE);
            } else {
             tft.drawRoundRect(0, 0, 80, 80, 5, TFT_GREEN);
            } 
         displayData = false;     
        }

     if (myDataReceve.id == 202 && displayData == true) {
         drawBmp("/salon.bmp", 240, 0);
         tft.drawString("salon", 340, 9, FONT4);          
         tft.fillRect(320, 30, 155, 45, TFT_BLACK);
         tft.drawRightString(String(myDataReceve.temp), 390, 35, FONT6); tft.drawString("o", 395, 30, FONT4);
         drawBmp("/humid_20.bmp", 395, 50);
         tft.drawString(String(myDataReceve.humi), 420, 50, FONT4); tft.drawString("%", 450, 50, FONT4);     
         if (myDataReceve.batterie <= 330) {
         drawBmp("/signal.bmp", 240, 0);        
            } else if (myDataReceve.batterie > 330 && myDataReceve.batterie <= 350) {
             tft.drawRoundRect(240, 0, 80, 80, 5, TFT_RED);
            } else if (myDataReceve.batterie > 350 && myDataReceve.batterie <= 360) {
             tft.drawRoundRect(240, 0, 80, 80, 5, TFT_ORANGE);
            } else {
             tft.drawRoundRect(240, 0, 80, 80, 5, TFT_GREEN);
            }
         displayData = false;      
        }

     if (myDataReceve.id == 303 && displayData == true) {
         drawBmp("/bain.bmp", 0, 80);
         tft.drawString("bain", 100, 89, FONT4);          
         tft.fillRect(80, 110, 155, 45, TFT_BLACK);
         tft.drawRightString(String(myDataReceve.temp), 150, 115, FONT6); tft.drawString("o", 155, 110, FONT4);
         drawBmp("/humid_20.bmp", 155, 130);
         tft.drawString(String(myDataReceve.humi), 180, 130, FONT4); tft.drawString("%", 210, 130, FONT4);
         if (myDataReceve.batterie <= 330) {
         drawBmp("/signal.bmp", 0, 80);        
            } else if (myDataReceve.batterie > 330 && myDataReceve.batterie <= 350) {
             tft.drawRoundRect(0, 80, 80,80, 5, TFT_RED);
            } else if (myDataReceve.batterie > 350 && myDataReceve.batterie <= 360) {
             tft.drawRoundRect(0, 80, 80, 80, 5, TFT_ORANGE);
            } else {
             tft.drawRoundRect(0, 80, 80, 80, 5, TFT_GREEN);
            }
         displayData = false;     
        }

     if (myDataReceve.id == 404 && displayData == true) {
         drawBmp("/bureau.bmp", 240, 80);
         tft.drawString("bureau", 340, 89, FONT4);          
         tft.fillRect(320, 110, 155, 45, TFT_BLACK);
         tft.drawRightString(String(myDataReceve.temp), 390, 115, FONT6); tft.drawString("o", 395, 110, FONT4);
         drawBmp("/humid_20.bmp", 395, 130);
         tft.drawString(String(myDataReceve.humi), 420, 130, FONT4); tft.drawString("%", 450, 130, FONT4);
         if (myDataReceve.batterie <= 330) {
         drawBmp("/signal.bmp", 240, 80);       
            } else if (myDataReceve.batterie > 330 && myDataReceve.batterie <= 350) {
             tft.drawRoundRect(240, 80, 80, 80, 5, TFT_RED);
            } else if (myDataReceve.batterie > 350 && myDataReceve.batterie <= 360) {
             tft.drawRoundRect(240, 80, 80, 80, 5, TFT_ORANGE);
            } else {
             tft.drawRoundRect(240, 80, 80, 80, 5, TFT_GREEN);
            } 
         displayData = false;   
        }

     if (myDataReceve.id == 505 && displayData == true) {

         garageFlag = myDataReceve.flag;         
         
         if (myDataReceve.flag == false) {         
             drawBmp("/garageFerme.bmp", 0, 160);

             if (garageFlag != oldGarageFlag) {
                 oldGarageFlag =  garageFlag;               
                 myDFPlayer.play(1);
                }
             
             digitalWrite(Garage, HIGH);
             tft.setTextColor(TFT_LIGHTGREY);             
             tft.drawString("garage", 100, 169, FONT4);                         
           } else {             
             drawBmp("/garageOuvert.bmp", 0, 160);

             if (garageFlag != oldGarageFlag) {
                 oldGarageFlag = garageFlag;               
                 myDFPlayer.play(2);
                }

             digitalWrite(Garage, LOW);
             tft.setTextColor(TFT_RED);
             tft.drawString("garage", 100, 169, FONT4);
             tft.setTextColor(TFT_LIGHTGREY);  
           }
         tft.fillRect(80, 190, 155, 45, TFT_BLACK);
         tft.drawRightString(String(myDataReceve.temp), 150, 195, FONT6); tft.drawString("o", 155, 190, FONT4);
         drawBmp("/humid_20.bmp", 155, 210);
         tft.drawString(String(myDataReceve.humi), 180, 210, FONT4); tft.drawString("%", 210, 210, FONT4);
         tft.drawRoundRect(0, 160, 80, 80, 5, TFT_GREEN);
         if (myDataReceve.batterie <= 330) {
         drawBmp("/signal.bmp", 0, 160);        
            } else if (myDataReceve.batterie > 330 && myDataReceve.batterie <= 350) {
             tft.drawRoundRect(0, 160, 80,80, 5, TFT_RED);
            } else if (myDataReceve.batterie > 350 && myDataReceve.batterie <= 360) {
             tft.drawRoundRect(0, 160, 80, 80, 5, TFT_ORANGE);
            } else {
             tft.drawRoundRect(0, 160, 80, 80, 5, TFT_GREEN);
            }
         displayData = false;               
        }

     if (myDataReceve.id == 606 && displayData == true) {
         drawBmp("/pergola.bmp", 240, 160);
         tft.drawString("pergola", 340, 169, FONT4);          
         tft.fillRect(320, 190, 155, 45, TFT_BLACK);
         tft.drawString(String(myDataReceve.temp), 320, 195, FONT6); tft.drawString("o", 395, 190, FONT4);
         drawBmp("/humid_20.bmp", 395, 210);
         tft.drawString(String(myDataReceve.humi), 420, 210, FONT4); tft.drawString("%", 450, 210, FONT4);
         if (myDataReceve.batterie >= 600) {
             tft.drawRoundRect(240, 160, 80, 80, 5, TFT_GREEN);          
            }
         if (myDataReceve.batterie >= 550 && myDataReceve.batterie < 600) {          
             tft.drawRoundRect(240, 160, 80, 80, 5, TFT_ORANGE);          
            }          
         if (myDataReceve.batterie < 550) {          
             tft.drawRoundRect(240, 160, 80, 80, 5, TFT_RED);          
            }       
         displayData = false;      
        }

     if (myDataReceve.id == 707 && displayData == true) {
         drawBmp("/piscine.bmp", 0, 240);
         tft.drawString("piscine", 100, 249, FONT4);          
         tft.fillRect(80, 270, 155, 45, TFT_BLACK);
         tft.drawRightString(String(myDataReceve.temp), 150, 275, FONT6); tft.drawString("o", 155, 270, FONT4);
         if (myDataReceve.batterie >= 600) {
             tft.drawRoundRect(0, 240, 80, 80, 5, TFT_GREEN);          
            }
         if (myDataReceve.batterie >= 550 && myDataReceve.batterie < 600) {          
             tft.drawRoundRect(0, 240, 80, 80, 5, TFT_ORANGE);          
            }          
         if (myDataReceve.batterie < 550) {          
             tft.drawRoundRect(0, 240, 80, 80, 5, TFT_RED);          
            }       
         displayData = false;      
        }

     if (myDataReceve.id == 808 && displayData == true) {

         fosseFlag = myDataReceve.flag;         
         
         if (myDataReceve.flag == false) {         
             drawBmp("/fosseAlarme.bmp", 240, 240);

             if (fosseFlag != oldFosseFlag) {
                 oldFosseFlag = fosseFlag;               
                 myDFPlayer.play(6);
                }

             digitalWrite(Fosse, LOW);      
           } else {             
             drawBmp("/fosseOk.bmp", 240, 240);

             if (fosseFlag != oldFosseFlag) {
                 oldFosseFlag = fosseFlag;               
                 myDFPlayer.play(5);
                }

             digitalWrite(Fosse, HIGH);
           }
         displayData = false;               
        }

     if (myDataReceve.id == 909 && displayData == true) {

         celierFlag = myDataReceve.flag;        
         
         if (myDataReceve.flag == false) {         
             drawBmp("/celierAlarme.bmp", 320, 240);

             if (celierFlag != oldCelierFlag) {
                 oldCelierFlag = celierFlag;               
                 myDFPlayer.play(2);
                }

             digitalWrite(Celier, LOW);      
           } else {             
             drawBmp("/celierOk.bmp", 320, 240);

             if (celierFlag != oldCelierFlag) {
                 oldCelierFlag = celierFlag;               
                 myDFPlayer.play(3);
                }

             digitalWrite(Celier, HIGH);
           }
         displayData = false;               
        }

/*  

     drawBmp("/carportFerme.bmp", 400, 240);
//     tft.drawRoundRect(400, 240, 80, 80, 5, TFT_ORANGE);
     delay(3000);

*/ 
       
}

//____________________________________________________________________________  


void setup() {

     Serial.begin(115200); Serial.println(); Serial.println();

FPSerial.begin(9600, SERIAL_8N1, /*rx =*/16, /*tx =*/17);
     
     pinMode(Garage, OUTPUT); digitalWrite(Garage, LOW);
     pinMode(Fosse, OUTPUT); digitalWrite(Fosse, LOW);
     pinMode(Celier, OUTPUT); digitalWrite(Celier, LOW);

  WiFi.mode(WIFI_STA);
  
  Serial.print("[OLD] ESP32 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  
  // ESP32 Board add-on before version < 1.0.5
  // esp_wifi_set_mac(ESP_IF_WIFI_STA, &newMACAddress[0]);
  
  // ESP32 Board add-on after version > 1.0.5
  esp_wifi_set_mac(WIFI_IF_STA, &newMACAddress[0]);
  
  Serial.print("[NEW] ESP32 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());

     if (!SPIFFS.begin()) {
         Serial.println("SPIFFS initialisation failed!");
         tft.drawString("Erreur SPIFFS !", 290, 30, 4);
         while (1) yield();
        }
     Serial.println("\r\nSPIFFS Ok");

     //Set device as a Wi-Fi Station
     WiFi.mode(WIFI_STA);

  if (!myDFPlayer.begin(FPSerial, /*isACK = */true, /*doReset = */true)) {  //Use serial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
myDFPlayer.volume(20);
     
     //Init ESP-NOW
     if (esp_now_init() != ESP_OK) {
         Serial.println("Error initializing ESP-NOW");
         return;
        } else { 
          Serial.println("ESP-NOW initialise");
        }

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
//***     esp_now_register_recv_cb(OnDataRecv);
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

     tft.init();
     tft.setRotation(3);
     tft.fillScreen(TFT_BLACK);
     tft.setTextColor(TFT_LIGHTGREY); 

     drawBmp("/smart_home.bmp", 0, 0);
     delay(5000);
     tft.fillScreen(TFT_BLACK);     
     
     drawBmp("/chambre.bmp", 0, 0);
     tft.drawString("chambre", 100, 9, FONT4);          
     
     drawBmp("/salon.bmp", 240, 0);
     tft.drawString("salon", 340, 9, FONT4);          

     drawBmp("/bain.bmp", 0, 80);
     tft.drawString("bain", 100, 89, FONT4);          

     drawBmp("/bureau.bmp", 240, 80);
     tft.drawString("bureau", 340, 89, FONT4);          

     drawBmp("/garageOuvert.bmp", 0, 160);
     tft.drawString("garage", 100, 169, FONT4);          

     drawBmp("/pergola.bmp", 240, 160);
     tft.drawString("pergola", 340, 169, FONT4);          
/*     tft.fillRect(320, 190, 155, 45, TFT_BLACK);
     tft.drawString(" 23", 320, 195, FONT6); tft.drawString("o", 395, 190, FONT4);
     drawBmp("/humid_20.bmp", 395, 210);
     tft.drawString("55", 420, 210, FONT4); tft.drawString("%", 450, 210, FONT4);
     tft.drawRoundRect(240, 160, 80, 80, 5, TFT_GREEN);
*/
     drawBmp("/piscine.bmp", 0, 240);
     tft.drawString("piscine", 100, 249, FONT4);          
/*     tft.fillRect(80, 270, 155, 45, TFT_BLACK);
     tft.drawString(" 29", 80, 275, FONT6); tft.drawString("o", 155, 270, FONT4);
     drawBmp("/humid_20.bmp", 155, 290);
     tft.drawString("89", 180, 290, FONT4); tft.drawString("%", 210, 290, FONT4);
     tft.drawRoundRect(0, 240, 80, 80, 5, TFT_GREEN);
*/
     drawBmp("/fosseAlarme.bmp", 240, 240);
//     tft.drawRoundRect(240, 240, 80, 80, 5, TFT_GREEN);

     drawBmp("/celierAlarme.bmp", 320, 240);
//     tft.drawRoundRect(320, 240, 80, 80, 5, TFT_ORANGE);

     drawBmp("/carportOuvert.bmp", 400, 240);
//     tft.drawRoundRect(400, 240, 80, 80, 5, TFT_RED);
            
}


void loop() {
           
}


//



//        SmartHome DatasLog
//      ! orientation display

/*
  #define ILI9488_DRIVER  //3,5"
  
  #define TFT_MISO 12 //  T_OUT
  #define TFT_MOSI 13 //  T_DIN
  #define TFT_SCLK 14 //  T_SCLK
  #define TFT_CS   26 //  
  #define TFT_DC   25 //  
  #define TFT_LED  33 // 
  #define TFT_RST  -1 //  ESP32 board EN
  #define TOUCH_CS 27 //  T_CS
*/


#include   <SPI.h>
#include   <TFT_eSPI.h> //             ! version 2.5.34
            TFT_eSPI tft = TFT_eSPI();
#include   "U8g2_for_TFT_eSPI.h"
            U8g2_for_TFT_eSPI u8f; //  U8g2 font instance

#define     FS_NO_GLOBALS
#include   <FS.h>
#include   "SPIFFS.h"

#include   <TJpg_Decoder.h>
            uint16_t w = 0, h = 0;
            int x, y = 0;
            bool tft_output (int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
                           if (y >= tft.height() ) return 0;
                               tft.pushImage(x, y, w, h, bitmap);
                               return 1;    
                              }
            String image;

#define NOMBRE_BOUTONS 3
        TFT_eSPI_Button bouton[NOMBRE_BOUTONS];
        uint16_t nt_x;//  pour correction inversion axe X de touch ILI avec callibrage software

         int meteoCall = (15)*1000;
         int lastMeteoCall;
        bool firstCall = true; 
       

void image_display() {

     TJpgDec.getFsJpgSize(&w, &h, "/" + image + ".jpg");
     TJpgDec.drawFsJpg(x, y, "/" + image + ".jpg");
  
}


#include <WiFi.h>
          const char * ssid="Bbox-E295749E-2,4GHz";
          const char * wifipw="Mb&2427242711";

#include "time.h"
//   Alléatoire Source        Garage, Fosse, Celier
//              Time code
//              Evènement     Porte ouverte/fermée, Niveau haut/normal, Fuite/RaS
int    Source;     // 1 = Garage, 2 = Fosse, 3 = Celier
String SourceTxt[4] = { "", "Garage", "Fosse ", "Celier" };
String Horodatage;
int    Evenement;  // 0 = Normal, 1 = Alarme
String Garage[2] = { "Porte fermee", "Porte ouverte !" };
String  Fosse[2] = { "Niveau normal", "Niveau haut !" };
String Celier[2] = { "R a S", "Fuite !" };
String EvenementTxt;
  bool flag = false;
  bool fileFlag = false;

#define    garage 34
   int    garageFlag;
   int oldGarageFlag;
#define    fosse  35
   int    fosseFlag;
   int oldFosseFlag;
#define    celier 36
   int    celierFlag;
   int oldCelierFlag;
   
#include "FS.h"
#include "SD.h"
#include "SPI.h"
          String dataMessage;
             int dataRead, ligne, posLigne;



void setTimezone(String timezone) {
  
     Serial.printf("  Setting Timezone to %s\n",timezone.c_str());
     setenv("TZ",timezone.c_str(),1);  //  Now adjust the TZ.  Clock settings are adjusted to show the new local time
     tzset();
     
}


void initTime(String timezone) {
  
     struct tm timeinfo;
     Serial.println("  Setting up time");
     configTime(0, 0, "pool.ntp.org");    // First connect to NTP server, with 0 TZ offset
     if (!getLocalTime(&timeinfo)) {
         Serial.println("  Failed to obtain time");
         return;
        }
     Serial.println("  Got the time from NTP");
     // Now we can set the real timezone
     setTimezone(timezone);
     
}


void printLocalTime() {
 
     struct tm timeinfo;
     if (!getLocalTime(&timeinfo)) {
         Serial.println("Failed to obtain time 1");
         return;
        }

     char timeHour[3];
     strftime(timeHour,3, "%H", &timeinfo);
     String heures = timeHour;
     char timeMinute[3];
     strftime(timeMinute,3, "%M", &timeinfo);
     String minutes = timeMinute;
     char timeInfoJour[3];
     strftime(timeInfoJour,3, "%e", &timeinfo);    
     String timeJour = timeInfoJour;    

     Horodatage = timeJour + "-" + heures + ":" + minutes;

}


void initSDCard() {
  
   if (!SD.begin()) {
       Serial.println("Card Mount Failed");
       return;
      }
}


void appendFile(fs::FS &fs, const char * path, const char * message) {
  
     Serial.printf("Appending to file: %s\n", path);
     File file = fs.open(path, FILE_APPEND);
     if (!file) {
         Serial.println("Failed to open file for appending");
         return;
        }
     if (file.print(message)) {
         Serial.println("Message appended");
        } else {
         Serial.println("Append failed");
        }
     file.close();
     
}


void deleteFile(fs::FS &fs, const char * path) {
  
     Serial.printf("Deleting file: %s\n", path);
  if (fs.remove(path)) {
      Serial.println("File deleted");
     } else {
      Serial.println("Delete failed");
     }
     
}


void startWifi() {
 
     WiFi.begin(ssid, wifipw);
     Serial.println("Connecting Wifi");
     while (WiFi.status() != WL_CONNECTED) {
            Serial.print(".");
            delay(500);
           }
     Serial.print("Wifi RSSI=");
     Serial.println(WiFi.RSSI());
     Serial.println();
     
}


void setTime(int yr, int month, int mday, int hr, int minute, int sec, int isDst) {
  
     struct tm tm;
            tm.tm_year = yr - 1900;   // Set date
            tm.tm_mon = month-1;
            tm.tm_mday = mday;
            tm.tm_hour = hr;      // Set time
            tm.tm_min = minute;
            tm.tm_sec = sec;
            tm.tm_isdst = isDst;  // 1 or 0
            time_t t = mktime(&tm);
            Serial.printf("Setting time: %s", asctime(&tm));

     struct timeval now = { .tv_sec = t };
     settimeofday(&now, NULL);
     
}


void affiche_evenement() {

     if (ligne == 12) {
         tft.fillRect(0, 0, 480, 270, TFT_BLACK);
         ligne = 0; posLigne = 0;              
        }
     posLigne = posLigne + 20;
     u8f.setCursor(5, posLigne);
     u8f.print(dataMessage);
     ligne ++;
  
}


void setup() {

     Serial.begin(115200); Serial.println("\n \n");

     startWifi();
     initTime("CET-1CEST,M3.5.0,M10.5.0/3");   // Set for Berlin
     WiFi.disconnect();

     initSDCard(); 

     if (!SPIFFS.begin()) {
         Serial.println("SPIFFS initialisation failed!");
         while (1) yield();
     } else { Serial.println("SPIFFS OK !"); }
     
     tft.init();
     tft.setRotation(3); // Erreur Touch si rotation 1
     tft.begin();
     tft.fillScreen(TFT_BLACK);     
     u8f.begin(tft); 
 
     tft.setSwapBytes(true);
     TJpgDec.setJpgScale(1);
     TJpgDec.setCallback(tft_output);

//     x =  50; y = 270; image = "file_create"; image_display();
     x = 190; y = 270; image = "file_create"; image_display();
     x = 300; y = 270; image = "file_delete"; image_display();
     x = 410; y = 270; image = "file_read";   image_display();

     uint16_t calData[5];
     uint8_t calDataOK = 0;
     calData[0] = 306; calData[1] = 3613; calData[2] = 263; calData[3] = 3487; calData[4] = 1; //   ILI9488
     bouton[0].initButton(&tft, 215, 295, 50, 50, TFT_TRANSPARENT, TFT_TRANSPARENT, TFT_TRANSPARENT, "", 1);
     bouton[1].initButton(&tft, 325, 295, 50, 50, TFT_TRANSPARENT, TFT_TRANSPARENT, TFT_TRANSPARENT, "", 1);
     bouton[2].initButton(&tft, 435, 295, 50, 50, TFT_TRANSPARENT, TFT_TRANSPARENT, TFT_TRANSPARENT, "", 1);

     pinMode(garage, INPUT);
     pinMode(fosse, INPUT);
     pinMode(celier, INPUT);

}


void loop() {

     if (flag == true) {
 
        garageFlag = !digitalRead(garage); delay(50); // Serial.print("Garage "); Serial.println(garageFlag);                       
        if (garageFlag != oldGarageFlag) {
            oldGarageFlag = garageFlag; printLocalTime(); fileFlag = true;
            if (garageFlag == 0) {
                dataMessage = String ("Garage " + Horodatage + " porte fermee" + "\n");
               } else {
                dataMessage = String ("Garage " + Horodatage + " porte ouverte !" + "\n"); 
               }
           }
        fosseFlag = !digitalRead(fosse); delay(50); // Serial.print("Fosse  "); Serial.println(fosseFlag);                       
        if (fosseFlag != oldFosseFlag) {
            oldFosseFlag = fosseFlag; printLocalTime(); fileFlag = true;
            if (fosseFlag == 0) {
                dataMessage = String ("Fosse  " + Horodatage + " niveau bas" + "\n");
               } else {
                dataMessage = String ("Fosse  " + Horodatage + " niveau haut !" + "\n"); 
               }
           }        
        celierFlag = !digitalRead(celier); delay(50); // Serial.print("Celier "); Serial.println(celierFlag);                       
        if (celierFlag != oldCelierFlag) {
            oldCelierFlag = celierFlag; printLocalTime(); fileFlag = true;
            if (celierFlag == 0) {
                dataMessage = String ("Celier " + Horodatage + " R a S" + "\n");
               } else {
                dataMessage = String ("Celier " + Horodatage + " Fuite !" + "\n"); 
               }
           }                

        if (fileFlag == true) {           
            File file = SD.open("/data.txt");
            Serial.println(dataMessage);
            appendFile(SD, "/data.txt", dataMessage.c_str());
            file.close();
            fileFlag = false;
            affiche_evenement();
           }
        
        }     
         
     uint16_t t_x = 0, t_y = 0; // coordonnées touchées par l'utilisateur
     boolean pressed = tft.getTouch(&t_x, &t_y); // vrai si contact avec l'écran
     nt_x = 480 - t_x; // compense inversion de x de touch ILI9488 3,5" et 4"

  // On vérifie si la position du contact correspond à celle d'un bouton
     for (uint8_t numero = 0; numero < NOMBRE_BOUTONS; numero++) {
          if (pressed && bouton[numero].contains(nt_x, t_y)) {
              bouton[numero].press(true);  
             } else {
              bouton[numero].press(false);  
             }
         }

  // Vérifions maintenant si l'état d'un des boutons a changé
     for (uint8_t numero = 0; numero < NOMBRE_BOUTONS; numero++) {
          if (bouton[numero].justPressed()) {
//        ... puis on fait ce que l'utilisateur a demandé:
              switch (numero) {
                      case 0:    // deuxième bouton
                      flag = true;
                      Serial.println("Aquisition");
                      tft.fillRect(0, 270, 180, 50, TFT_BLACK);
                      u8f.setFont(u8g2_font_profont22_mf);
                      u8f.setBackgroundColor(TFT_BLACK);  
                      u8f.setForegroundColor(TFT_WHITE);
                      u8f.setCursor(30, 300);
                      u8f.print("Aquisition");
                      tft.fillRect(0, 0, 480, 270, TFT_BLACK);
                      ligne = 0; posLigne = 0;
                      break;
                      case 1:    // troisième bouton
                      flag = false;
                      Serial.println("Delete file");
                      tft.fillRect(0, 270, 180, 50, TFT_BLACK);
                      u8f.setFont(u8g2_font_profont22_mf);
                      u8f.setBackgroundColor(TFT_BLACK);  
                      u8f.setForegroundColor(TFT_WHITE);
                      u8f.setCursor(10, 300);
                      u8f.print("Efface datas !");
                      deleteFile (SD, "/data.txt"); delay(500);
                      tft.fillRect(0, 0, 480, 270, TFT_BLACK);
                      break; 
                      case 2:    // troisième bouton
                      flag = false;
                      Serial.println("Read file");
                      tft.fillRect(0, 270, 180, 50, TFT_BLACK);
                      u8f.setFont(u8g2_font_profont22_mf);
                      u8f.setBackgroundColor(TFT_BLACK);  
                      u8f.setForegroundColor(TFT_WHITE);
                      u8f.setCursor(10, 300);
                      u8f.print("Lecture datas");
                      File file = SD.open("/data.txt");
                      Serial.println("Read from file: "); 
                      tft.fillRect(0, 0, 480, 270, TFT_BLACK);
                      u8f.setCursor(0, 20); int ligne = 0;
                      while (file.available()) {                             
                             dataRead = (file.read());
                             u8f.write(dataRead);
                             if (dataRead == 10) { ligne ++; delay(100); }
                             if (ligne == 14) {
                                 delay(3000);
                                 tft.fillRect(0, 0, 480, 270, TFT_BLACK);
                                 u8f.setCursor(0, 20); ligne = 0;
                                }                                                                                     
                             }
                      file.close();
                      break; 
                     }

             delay(10); // anti-rebond
            }
         }

}


//

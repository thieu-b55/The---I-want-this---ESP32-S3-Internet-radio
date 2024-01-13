/*
* MIT License
*
* Copyright (c) 2024 thieu-b55
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/


/*
 * ESP32-S3 WROOM1  N16R8 Module
 * 
 * Partition Scheme:
 * Partition Scheme   Huge APP(3MB No OTA/1MB SPISS)
 * Flash Mode         QIO 80Mhz
 * Flash Size         16MB(128Mb)
 * PSRAM              OPI PSRAM 
 * 
 * VERBINDINGEN
 * 
 * PCM5102A 
 * FLT                    GND
 * DMP                    3.3V
 * SCL                    GND
 * BCK                    GPIO16 
 * DIN                    GPIO15
 * LCK                    GPIO17  
 * FMT                    GND
 * XMT                    3.3V
 * VCC                    5V
 * GND                    GND
 * 
 * W5500
 * INT_GPIO               GPIO4
 * MISO_GPIO              GPIO13
 * MOSI_GPIO              GPIO11
 * SCLK_GPIO              GPIO12
 * CS_GPIO                GPIO10
 * 5V                     5V
 * GND                    GND
 * 
 * SD
 * SD_MISO                GPIO5
 * SD_MOSI                GPIO6
 * SD_SCLK                GPIO7
 * SD_CS                  GPIO18 
 * 
 * TFT / TOUCH
 * 
 * Type : 4" 480x320  Driver  ST7796
 * 
 * TFT_MISO               GPIO5
 * TFT_MOSI               GPIO6
 * TFT_SCLK               GPIO7
 * TFT_CS                 GPIO8
 * TFT_DC                 GPIO39
 * TFT_RESET              -1  // verbinden met ESP32 RESET
 * TOUCH_CS               GPIO40
 * TOUCH_INT              GPIO41
 * LED                    +3.3V ESP32
 * VCC                    5V
 * GND                    GND
 * 
 * 
 * Rotary switch station
 * STATION                GPIO9
 *
 * Rotary switch volume
 * VOLUME                 GPIO14
 * 
 * Audio librarie
 * https://github.com/schreibfaul1/ESP32-audioI2S
 * 
 * zender_data.csv
 * geen header
 * kolom 1  >>  zendernaam
 * kolom2   >>  zender url
*/

#include "Arduino.h"
#include "WiFi.h"
#include <SPI.h>
#include <Audio.h>
#include <AsyncWebServer_ESP32_SC_W5500.h>
#include <AsyncTCP.h>   
#include <Preferences.h>
#include <TFT_eSPI.h>
#include "FS.h"
#include "SD.h"
#include <CSV_Parser.h>
#include "time.h"

Audio                                   audio;
AsyncWebServer                          server(80);
TFT_eSPI                                tft = TFT_eSPI();
Preferences                             pref;

struct tm timeinfo;

#define CALIBRATION_FILE                "/TouchCalData1"
#define REPEAT_CAL                      false

#define TOUCH_TRESHOLD                  50
#define MAX_LOOP                        10

//PCM5102A
#define I2S_DOUT                        15
#define I2S_BCLK                        16
#define I2S_LRC                         17

//SD kaart
#define SD_MISO                          5
#define SD_MOSI                          6
#define SD_SCLK                          7
#define SD_CS                           18 

//TFT scherm en touch
#define TFT_MISO                         5
#define TFT_MOSI                         6
#define TFT_SCLK                         7
#define TFT_CS                           8
#define TFT_DC                          39
#define TFT_RESET                       -1  // verbinden met ESP32 RESET
#define TOUCH_CS                        40
#define TOUCH_INT                       41

//Rotary switch station
#define STATION_ENCODER                  9

//Rotary switch volume
#define VOLUME_ENCODER                  14

#define MAX_AANTAL_KANALEN              75

int gekozen = 1;
int keuze = 1;
int keuze_vorig = 1;
int gn_keuze_vorig = 1;
int volgend;
int totaalmp3;
int eerste;
int tweede;
int songindex;
int row;
int volume_keuze;
int volume_gekozen;
int laag_keuze;
int laag_gekozen;
int midden_keuze;
int midden_gekozen;
int hoog_gekozen;
int hoog_keuze;
int mp3_per_songlijst;
int array_index = MAX_AANTAL_KANALEN - 1;
int songlijst_index_vorig;
int songlijst_index;
int mp3_folder_teller;
int teller = 0;
int mp3_aantal;
int gn_keuze = 0;
int ip_1_int = 192;
int ip_2_int = 168;
int ip_3_int = 1;
int ip_4_int = 222;
int tft_positie_int = 10;
int tft_positie_plus_int = 10;
int netwerk_keuze_int = 0;
int tft_pos_int = 20;
uint16_t t_x;
uint16_t t_y; 
unsigned long wacht_op_netwerk;
unsigned long inlezen_begin;
unsigned long inlezen_nu;
unsigned long wachttijd;
unsigned long tft_update_long;
unsigned long touch_update_long;
unsigned long tft_bericht_long;
unsigned long int_wachttijd_long = 111;
unsigned long int_begin_long;
unsigned long isr_volume_begin_long;
unsigned long isr_station_begin_long;
unsigned long verschil_volume_millis;
unsigned long verschil_station_millis;
bool eerste_bool = false;
bool kiezen = false;
bool web_kiezen = false;
bool lijst_maken = false;
bool speel_mp3 = false;
bool webradio = false;
bool schrijf_csv = false;
bool netwerk;
bool nog_mp3;
bool mp3_ok;
bool mp3_lijst_maken = false;
bool songlijsten = false;
bool dir_bestaat_bool;
bool songlijst0_bestaat_bool;
bool mp3_0_bestaat_bool;
bool touch_int_bool = false;
bool int_bool = false;
bool gekozen_bool = false;
bool pressed;
bool station_plus_bool = false;
bool station_min_bool = false;
bool station_switch_bool = false;
bool tijd_station_gestart_bool = false;
bool volume_plus_bool = false;
bool volume_min_bool = false;
bool volume_switch_bool = false;
bool tijd_volume_gestart_bool = false;
bool volume_bewaren_bool = false;
bool eth_bool = false;
bool wifi_bool = false;
bool eerste_loop_bool = true;
bool eth_verbonden_bool = false;
bool tijd_bool = false;
bool tijdzone_bool = false;
bool ntp_bool = false;
char ip_char[20];
char songfile[200];
char mp3file[200];
char song[200];
char datastring[200];
char password[40];
char ssid[40];
char speler[20];
char gn_actie[20];
char gn_selectie[20];
char zendernaam[60];
char charUrlFile[12];
char url[120];
char mp3_dir[10];
char folder_mp3[10];
char aantal_mp3[10];
char songlijst_dir[12];
char totaal_mp3[15];
char mp3_lijst_folder[10];
char mp3_lijst_aantal[5];
char leeg[0];
char zenderarray[MAX_AANTAL_KANALEN][60];
char urlarray[MAX_AANTAL_KANALEN][120];
char hoeveel_mp3[5];
char tijd[5];
char tijdzone_char[50];   
char ntp_char[30];
const char* IP_1_KEUZE = "ip_1_keuze";
const char* IP_2_KEUZE = "ip_2_keuze";
const char* IP_3_KEUZE = "ip_3_keuze";
const char* IP_4_KEUZE = "ip_4_keuze";
const char* KEUZEMIN_INPUT = "minKeuze";
const char* KEUZEPLUS_INPUT = "plusKeuze";
const char* BEVESTIGKEUZE_INPUT ="bevestigKeuze";
const char* LAAG = "laag_keuze";
const char* MIDDEN = "midden_keuze";
const char* HOOG = "hoog_keuze";
const char* VOLUME = "volume_keuze";
const char* VOLUME_BEVESTIG = "bevestig_volume";
const char* APssid = "ESP32webradio";
const char* APpswd = "ESP32pswd";
const char* TZ = "tz";
const char* NTP_SERVER = "ntp_server";
const char* STA_SSID = "ssid";
const char* STA_PSWD = "pswd";
const char* ZENDER = "zender";
const char* URL = "url";
const char* ARRAY_MIN = "array_index_min";
const char* ARRAY_PLUS = "array_index_plus";
const char* BEVESTIG_ZENDER = "bevestig_zender";
const char* MIN_INPUT = "min";
const char* PLUS_INPUT = "plus";
const char* BEVESTIG_MP3 ="bevestig_mp3";
const char* h_char = "h";
String songstring =      "                                                                                                                                                                                                        ";
String inputString =     "                                                                                                                                                                                                        ";
String mp3titel =        "                                                                                                                                                                                                        ";      
String zenderFile =      "           ";
String urlFile =         "           ";
String maxurl =          "           ";
String totaal =          "           ";
String streamsong =      "                                                                                                                                                                                                        ";
String mp3_folder =      "                   ";
String songlijst_folder = "                   ";
String mp3test = "mp3";
String ip_1_string = "   ";
String ip_2_string = "   ";
String ip_3_string = "   ";
String ip_4_string = "   ";
String ip_string = "                   ";
String mp3_per_folder = "      ";
String tijdzone_string = "                                                 ";
String ntp_string = "                              ";

String readFile(fs::FS &fs, const char * path){
    File file = fs.open(path);
    if(!file){
      Serial.println("Kan file niet openen om te lezen : ");
      Serial.println(path);
      tft.setTextFont(2);
      tft.setTextColor(TFT_WHITE);
      tft.setCursor(0, tft_pos_int);
      tft.print("Kan file niet openen om te lezen :  ");
      tft.print(path);
      tft_pos_int += 20;
      return("error");
    }
    teller = 0;
    String lees_string = "";
    while(file.available()){
      lees_string += char(file.read());
      teller++;
    }
    file.close();
    return(lees_string);
}

void deleteFile(fs::FS &fs, const char * path){
  fs.remove(path);
}

void appendFile(fs::FS &fs, const char * path, const char * message){
  File file = fs.open(path, FILE_APPEND);
  file.print(message);
  file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Kan file niet openen om te schrijven : ");
    Serial.println(path);
    tft.setTextFont(2);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(0, tft_pos_int);
    tft.print("Kan file niet openen om te schrijven :  ");
    tft.print(path);
    tft_pos_int += 20;
    return;
  }
  file.print(message);
  file.close();
}

void testDir(fs::FS &fs, const char * path){
  File root = fs.open(path);
  if(root){
    dir_bestaat_bool = true;
  }
}

void createDir(fs::FS &fs, const char * path){
  File root = fs.open(path);
  if(root){
    songlijsten = true;
    lijst_maken = false;
    while(1){
      yield();
    }
  }
  else{
    fs.mkdir(path);
  }
}

void file_test(){
  testDir(SD, "/songlijst0");
  if(dir_bestaat_bool){
    songlijst0_bestaat_bool = true;
  }
  else{
    songlijst0_bestaat_bool = false;
  }
  testDir(SD, "/mp3_0");
  if(dir_bestaat_bool){
    mp3_0_bestaat_bool = true;
  }
  else{
    mp3_0_bestaat_bool = false;
  }
}

void audio_showstreamtitle(const char *info){
  if(kiezen == false){
    streamsong = info;
  }
}

void audio_eof_mp3(const char *info){
  mp3_volgend();
}

void files_in_mp3_0(fs::FS &fs, const char * dirname, uint8_t levels){
  File root = fs.open(dirname);
  if(!root){
    Serial.println("Geen mp3_0 folder");
    return;
  }
  File file = root.openNextFile();
  mp3_per_songlijst = 0;
  while(file){
    file = root.openNextFile();
    mp3_per_songlijst ++;
  }
  String(mp3_per_songlijst).toCharArray(hoeveel_mp3, (String(mp3_per_songlijst).length() +1));
  writeFile(SD, "/files", hoeveel_mp3);
}

void maak_lijst(fs::FS &fs, const char * dirname){
  File root = fs.open(dirname);
  if(!root){
    nog_mp3 = false;
    return;
  }
  File file = root.openNextFile();
  while(file){
    songlijst_index =  mp3_aantal / mp3_per_songlijst;
    if(songlijst_index != songlijst_index_vorig){
      songlijst_index_vorig = songlijst_index;
      songlijst_folder = "/songlijst" + String(songlijst_index);
      songlijst_folder.toCharArray(songlijst_dir, (songlijst_folder.length() + 1));
      createDir(SD, songlijst_dir);
    }
    songlijst_folder = "/songlijst" + String(songlijst_index) + "/s" + String(mp3_aantal);
    songlijst_folder.toCharArray(songlijst_dir, (songlijst_folder.length() + 1));
    /*
     * volgende regel veranderd t.o.v. origineel programma
     * file.name() nu alleen filenaam
     * bij origineel ook met foldernaam
     * mp3_folder + "/" +   moeten bijvoegen
     */
    songstring = mp3_folder + "/" + file.name();
    songstring.toCharArray(song, (songstring.length() + 1));
    writeFile(SD, songlijst_dir, song);
    file = root.openNextFile();
    mp3_aantal ++;
  }
}

void mp3_lijst_maken_gekozen(){
  inlezen_begin = millis();
  tft.fillScreen(TFT_BLACK);
  tft.setTextFont(2);
  tft.setTextColor(TFT_GREENYELLOW);
  tft.setCursor(0, 20);
  tft.print("mp3 lijst maken");
  files_in_mp3_0(SD, "/mp3_0", 1);
  mp3_aantal = 0;
  nog_mp3 = true;
  mp3_folder_teller = 0;
  songlijst_index_vorig = -1;
  while(nog_mp3){
    mp3_folder = "/mp3_" + String(mp3_folder_teller);
    inlezen_nu = millis() - inlezen_begin;
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 100);
    tft.print("aantal ingelezen");
    tft.setCursor(120, 100);
    tft.print(": ");
    tft.print(mp3_aantal);
    tft.print(" ");
    tft.print("mp3's");
    tft.setCursor(0, 130);
    tft.print("verstreken tijd");
    tft.setCursor(120, 130);
    tft.print(": ");
    tft.print(inlezen_nu);
    tft.print(" ");
    tft.print("milliseconden");
    mp3_folder.toCharArray(mp3_dir, (mp3_folder.length() + 1));
    maak_lijst(SD, mp3_dir);
    mp3_folder_teller ++;
  }
  String(mp3_aantal).toCharArray(totaal_mp3, (String(mp3_aantal - 1).length() +1));
  writeFile(SD, "/totaal", totaal_mp3);
  int verstreken_tijd = (millis() - inlezen_begin) / 1000;
  tft.setCursor(0, 160);
  tft.print("totaal tijd");
  tft.setCursor(120, 160);
  tft.print(": ");
  tft.print(verstreken_tijd);
  tft.print(" ");
  tft.print("seconden");
  delay(2500);
  lijst_maken = false;
  keuze = -1;
  gekozen = -1;
  gn_keuze = 1;
  netwerk_keuze_int = 2;
  pref.putShort("netwerk", netwerk_keuze_int);
  file_test();
  mp3_gekozen();
}

void mp3_gekozen(){
  inputString = readFile(SD, "/totaal");
  totaal = inputString.substring(0, teller);
  totaalmp3 = totaal.toInt();
  inputString = readFile(SD, "/files");
  mp3_per_folder = inputString.substring(0, teller);
  mp3_per_songlijst = mp3_per_folder.toInt();
  mp3_volgend();
}

void mp3_volgend(){
  mp3_ok = false;
  while(mp3_ok == false){
    volgend = random(totaalmp3);
    songindex = volgend / mp3_per_songlijst;
    songstring = "/songlijst" + String(songindex) + "/s" + String(volgend);
    songstring.toCharArray(songfile, (songstring.length() +1));
    inputString = readFile(SD, songfile);
    inputString.toCharArray(mp3file, inputString.length() + 1);
    mp3_ok = audio.connecttoFS(SD, mp3file);
  }
  songstring = String(mp3file);
  eerste = songstring.indexOf("/");
  tweede = songstring.indexOf("/", eerste + 1);
  mp3titel = songstring.substring(tweede + 1);
  streamsong = mp3titel.substring(0, (mp3titel.length() - 4));
}

void geen_netwerk_mp3_controle(){
  if(gn_keuze == 2){
  if((!songlijst0_bestaat_bool) && (mp3_0_bestaat_bool)){
    lijst_maken = true;
  }
  if((songlijst0_bestaat_bool) && (mp3_0_bestaat_bool)){
    gn_keuze = 1;
    speel_mp3 = true;
  }
  if((!songlijst0_bestaat_bool) && (!mp3_0_bestaat_bool)){
    gn_keuze = 0;
  }
  }
  if(gn_keuze == 1){
    if((songlijst0_bestaat_bool) && (mp3_0_bestaat_bool)){
      speel_mp3 = true;
    }
    if((!songlijst0_bestaat_bool) && (mp3_0_bestaat_bool)){
      lijst_maken = true;
      gn_keuze = 2;
    }
    if((!songlijst0_bestaat_bool) && (!mp3_0_bestaat_bool)){
      gn_keuze = 0;
    }
  }
  if(gn_keuze == 0){
    audio.stopSong();
  }
}

void netwerk_mp3_controle(){
  if(keuze == -2){
  if((!songlijst0_bestaat_bool) &&(mp3_0_bestaat_bool)){
    lijst_maken = true;
  }
  if((songlijst0_bestaat_bool) && (mp3_0_bestaat_bool)){
    keuze = -1;
    gekozen = keuze;
    speel_mp3 = true;
  }
  if(!mp3_0_bestaat_bool){
    keuze = pref.getShort("station");
    webradio = true;
  }
 }
  if(keuze == -1){
    if((!songlijst0_bestaat_bool) &&(mp3_0_bestaat_bool)){
      keuze = -2;
      lijst_maken = true;
    }
    if((songlijst0_bestaat_bool) && (mp3_0_bestaat_bool)){
      gekozen = keuze;
      speel_mp3 = true;
    }
    if(!mp3_0_bestaat_bool){
      keuze = pref.getShort("station");
      webradio = true;
    }
  }
}

void radio_gekozen(){
  memset(url, 0, sizeof(url));
  strcpy(url, urlarray[gekozen]);
  memset(zendernaam, 0, sizeof(zendernaam));
  strcpy(zendernaam, zenderarray[gekozen]);
  audio.connecttohost(url);
}

void schrijf_naar_csv(){
  char terminator = char(0x0a);
  String datastring = "                                                                                                                                                                                  ";
  char datastr[180];
  deleteFile(SD, "/zender_data.csv");
  for(int x = 0; x < MAX_AANTAL_KANALEN; x++){
    datastring = String(zenderarray[x]) + "," + String(urlarray[x]) + String(terminator);
    datastring.toCharArray(datastr, (datastring.length() + 1));
    appendFile(SD, "/zender_data.csv", datastr);
  }
}

/*
 * Inlezen CSV file naar zender en url arry
 */
void lees_CSV(){
  CSV_Parser cp("ss", false, ',');
  if(cp.readSDfile("/zender_data.csv")){
    char **station_naam = (char**)cp[0];  
    char **station_url = (char**)cp[1]; 
    for(row = 0; row < cp.getRowsCount(); row++){
      memset(zenderarray[row], 0, sizeof(zenderarray[row]));
      strcpy(zenderarray[row], station_naam[row]);
      memset(urlarray[row], 0, sizeof(urlarray[row]));
      strcpy(urlarray[row], station_url[row]);
    }
  }
}

/*
 * Copied from TFT_eSPI example files
 */
void touch_calibrate(){
  uint16_t calData[5];
  uint8_t calDataOK = 0;
  if (!SPIFFS.begin()){
    Serial.println("Formating file system");
    SPIFFS.format();
    SPIFFS.begin();
  }
  if (SPIFFS.exists(CALIBRATION_FILE)){
    if (REPEAT_CAL){
      SPIFFS.remove(CALIBRATION_FILE);
    }
    else{
      File f = SPIFFS.open(CALIBRATION_FILE, "r");
      if (f){
        if (f.readBytes((char *)calData, 14) == 14){
          calDataOK = 1;
        }
        f.close();
      }
    }
  }
  if (calDataOK && !REPEAT_CAL) {
    tft.setTouch(calData);
  } 
  else{
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.println("Touch corners as indicated");
    tft.setTextFont(1);
    tft.println();
    if (REPEAT_CAL){
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.println("Set REPEAT_CAL to false to stop this running again!");
    }
    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibration complete!");
    File f = SPIFFS.open(CALIBRATION_FILE, "w");
    if (f){
      f.write((const unsigned char *)calData, 14);
      f.close();
    }
  }
}
/*
 * End copy from TFT-eSPI
 */
 
void touch(){
  detachInterrupt(digitalPinToInterrupt(TOUCH_INT));
  touch_int_bool = true;
}

/*
 * Flushen nog aanwezige interrupt
 */
void dummy(){
}

/*
 * Advanced EC11 rotary encoder processing only one input for one EC11
 */
void volume(){
  if(digitalRead(VOLUME_ENCODER)){
    isr_volume_begin_long = millis();
    tijd_volume_gestart_bool = true;
  }
  else{
    if(tijd_volume_gestart_bool){
      verschil_volume_millis = millis() - isr_volume_begin_long;
      if( verschil_volume_millis < 15){
        volume_plus_bool = true;
      }
      if((verschil_volume_millis > 15) && (verschil_volume_millis < 25)){
        volume_min_bool = true;
      }
      if(verschil_volume_millis > 25){
        volume_switch_bool = true;
      }
      tijd_volume_gestart_bool = false;
    }
  }
}

void station(){
  if(digitalRead(STATION_ENCODER)){
    isr_station_begin_long = millis();
    tijd_station_gestart_bool = true;
  }
  else{
    if(tijd_station_gestart_bool){
      verschil_station_millis = millis() - isr_station_begin_long;
      if( verschil_station_millis < 15){
        station_plus_bool = true;
      }
      if((verschil_station_millis > 15) && (verschil_station_millis < 25)){
        station_min_bool = true;
      }
      if(verschil_station_millis > 25){
        station_switch_bool = true;
      }
      tijd_station_gestart_bool = false;
    }
  }
}

void eth_wifi_keuze(){
  tft.fillScreen(TFT_BLACK);
  tft.setTextFont(2);
  if(netwerk_keuze_int == 0){
    tft.setTextColor(TFT_RED);
  }
  else{
    tft.setTextColor(TFT_YELLOW);
  }
  tft.setCursor((480 - tft.textWidth("Ethernet", 2)) / 2, 120);
  tft.print("Ethernet");
  if(netwerk_keuze_int == 1){
    tft.setTextColor(TFT_RED);
  }
  else{
    tft.setTextColor(TFT_YELLOW);
  }
  tft.setCursor((480 - tft.textWidth("WiFi", 2)) / 2, 160);
  tft.print("WiFi");
  if(netwerk_keuze_int == 2){
    tft.setTextColor(TFT_RED);
  }
  else{
    tft.setTextColor(TFT_YELLOW);
  }
  if(songlijst0_bestaat_bool){
    tft.setCursor((480 - tft.textWidth("mp3 speler", 2)) / 2, 200);
    tft.print("mp3 speler");
  }
}

void wis_tft(){
  tft.fillRect(0, 120, 480, 20, TFT_BLACK);
}

void netwerk_station_schrijf_tft(){
  tft.setTextColor(TFT_SKYBLUE);
  tft.setTextFont(2);
  if(keuze == - 1){
    tft.setCursor((480 - tft.textWidth("mp3 speler", 2)) / 2, 120);
    tft.print("mp3 speler");
  }
  else if(keuze == -2){
    tft.setCursor((480 - tft.textWidth("mp3 lijst maken", 2)) / 2, 120);
    tft.print("mp3 lijst maken");
  }
  else{
    tft.setCursor((480 - tft.textWidth(zenderarray[keuze], 2)) / 2, 120);
    tft.print(zenderarray[keuze]);
  }
}

void gn_netwerk_station_schrijf_tft(){
  tft.setTextColor(TFT_SKYBLUE);
  tft.setTextFont(2);
  if(gn_keuze == 0){
    tft.setCursor((480 - tft.textWidth("stop mp3 speler", 2)) / 2, 120);
    tft.print("stop mp3 speler");
  }
  if(gn_keuze == 1){
    tft.setCursor((480 - tft.textWidth("mp3 speler", 2)) / 2, 120);
    tft.print("mp3 speler");
  }
  if(gn_keuze == 2){
    tft.setCursor((480 - tft.textWidth("mp3 lijst maken", 2)) / 2, 120);
    tft.print("mp3 lijst maken");
  }
}

/*
 * INTERNET RADIO INSTELLINGEN
 */
const char index_html[] = R"rawliteral(
<!DOCTYPE HTML>
<html>
  <head>
    <iframe style="display:none" name="hidden-form"></iframe>
    <title>Internetradio bediening</title>
    <meta name="viewport" content="width=device-width, initial-scale=.85">
    <style>
        div.kader {
          position: relative;
          left: 0px;
          width: 400px;
        }
        div.kader_1{
          position: absolute;
          left : 0px;
          width: 200px;
        }
        div.kader_2 {
          position: absolute;
          left : 80px;
          width: 80px;
        }div.kader_2A {
          position: absolute;
          left : 60px;
          width: 120px;
        }
        div.kader_3 {
          position: absolute;
          left : 160px;
          width: 80px;
        }
        div.kader_4 {
          position: absolute;
          left : 240px;
          width: 80px;
        }
        div.kader_4A {
          position: absolute;
          left : 220px;
          width: 160px;
        }
        div.blanco_10{
          width: auto;
          height: 10px;
        }
        div.blanco_20{
          width: auto;
          height: 20px;
        }
        div.blanco_30{
          width: auto;
          height: 30px;
        }
        div.blanco_40{
          width: auto;
          height: 40px;
        }
        div.blanco_50{
          width: auto;
          height: 50px;
        }
        div.blanco_60{
          width: auto;
          height: 60px;
        }
        div.blanco_80{
          width: auto;
          height: 80px;
        }
    </style>
  </head>
  <body>
    <h3><center> ESP32 internetradio </center></h3>
    <div class="blanco_10">&nbsp;</div>
    <h5><center> %zenderNu% </center></h5>
    <small><p><center>%song%</center></p></small>
    <center>
      <input type="text" style="text-align:center;" value="%selecteren%" size=30>
    </center>
    <div class="blanco_30">&nbsp;</div>
    <form action="/get" target="hidden-form">
      <center>
        <div class="kader">
          <div class="kader_2">
            <center><input type="submit" name="minKeuze" value="   -   " onclick="bevestig()"></center>
          </div>
          <div class="kader_3">
            <center><input type="submit" name="bevestigKeuze" value="OK" onclick="bevestig()"></center>
          </div>
          <div class="kader_4">
            <center><input type="submit" name="plusKeuze" value="   +   " onclick="bevestig()"></center>
          </div>
        </div>
      </center>
    </form>
    <div class="blanco_40">&nbsp;</div>
    <center>
      <div class="kader">
        <div class="kader_2A">
          <center><p><small><b>EQ -40 <-> 6 </b></small></p></center>
        </div>
        <div class="kader_4A">
          <center><p><small><b>Volume 0 <->21</b></small></p></center>
        </div>
      </div>
      </center>
    <div class="blanco_50">&nbsp;</div>
    <form action="/get" target="hidden-form">
    <small>
      <center>
        <labelfor="dummy">L :</label>
        <input type="text" style="text-align:center;" value="%laag_kiezen%"   name="laag_keuze"   size=1>
        &nbsp;&nbsp;
        <labelfor="dummy">M :</label>
        <input type="text" style="text-align:center;" value="%midden_kiezen%" name="midden_keuze" size=1>
        &nbsp;&nbsp;
        <labelfor="dummy">H :</label>
        <input type="text" style="text-align:center;" value="%hoog_kiezen%"   name="hoog_keuze"   size=1>
        &nbsp;&nbsp;
        <labelfor="dummy">V :</label>
        <input type="text" style="text-align:center;" value="%volume_kiezen%" name="volume_keuze" size=1>
      </center>
    </small>
    <div class="blanco_30">&nbsp;</div>
    <center>
      <input type="submit" name="bevestig_volume" value="OK" onclick="bevestig()">
    </center>
    </form>
    <div class="blanco_20">&nbsp;</div>
    <h5><center> Instellen zender en url : %array_index% </center></h5>
    <form action="/get" target="hidden-form">
    <center>
      <input type= "text" style="text-align:center;" value="%zender%" name="zender" size = 30>
    </center>
    <div class="blanco_10">&nbsp;</div>
    <center>
      <input type= "text" style="text-align:center;" value="%url%" name="url" size = 40>
    </center>
    <div class="blanco_30">&nbsp;</div>
    <center>
      <div class="kader">
        <div class="kader_2">
          <center><input type="submit" name="array_index_min" value="   -   " onclick="bevestig()"></center>
        </div>
        <div class="kader_3">
          <center><input type="submit" name="bevestig_zender" value="OK" onclick="ok()"></center>
        </div>
        <div class="kader_4">
        <center><input type="submit" name="array_index_plus" value="   +   " onclick="bevestig()"></center>
        </div>
      </div>
    </center>
    </form>
    <div class="blanco_40">&nbsp;</div>
    <div class="blanco_40">&nbsp;</div>
    <form action="/get" target="hidden-form">
      <center>
        <input type="submit" value="Instellingen" onclick="instellen()"></button>
      </center>
    </form>
    <div class="blanco_80">&nbsp;</div>
    <center>
      <div class="kader">
        <div class="kader_1">
          <h6>thieu-b55 januari 2024</h6>
        </div>
      </div>
    </center>
    <script>
      function bevestig(){
        setTimeout(function(){document.location.reload();},250);
      }
      function instellen(){
        location.assign("/instellen");
      }
      function ok(){
        setTimeout(function(){location.assign("/");},250);
      }
    </script>
  </body>  
</html>
)rawliteral";


/*
 * mp3 speler
 */
const char mp3_html[] = R"rawliteral(
<!DOCTYPE HTML>
<html>
  <head>
    <iframe style="display:none" name="hidden-form"></iframe>
    <title>Internetradio bediening</title>
    <meta name="viewport" content="width=device-width, initial-scale=.85">
    <style>
        div.blanco_40{
          width: auto;
          height: 40px;
        }
    </style>
  </head>
  <body>
    <h4><center><strong>mp3 speler</strong></center></h4>
    <p><small><center>%song%</center></small></p>
    <center>
      <input type="text" style="text-align:center;" value="%selectie%" size=30>
    </center>
      <br>
    <form action="/get" target="hidden-form">
    <center>
      <input type="submit" name="min" value="   -   " onclick="ok()">
      &nbsp;&nbsp;&nbsp;
      <input type="submit" name="plus" value="   +   " onclick="ok()">
      &nbsp;&nbsp;&nbsp;
      <input type="submit" name="bevestig_mp3" value="OK" onclick="ok()">
    </center>
    </form>
    <br>
    <p><small><center><b>EQ -40 <-> 6 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Volume 0 <->21</b></center></small></p>
    <form action="/get" target="hidden-form">
    <small>
    <center>
      <labelfor="dummy">L :</label>
      <input type="text" style="text-align:center;" value="%laag_kiezen%"   name="laag_keuze"   size=1>
      &nbsp;&nbsp;
      <labelfor="dummy">M :</label>
      <input type="text" style="text-align:center;" value="%midden_kiezen%" name="midden_keuze" size=1>
      &nbsp;&nbsp;
      <labelfor="dummy">H :</label>
      <input type="text" style="text-align:center;" value="%hoog_kiezen%"   name="hoog_keuze"   size=1>
      &nbsp;&nbsp;
      <labelfor="dummy">V :</label>
      <input type="text" style="text-align:center;" value="%volume_kiezen%" name="volume_keuze" size=1>
    </center>
    </small>
    <br>
    <center>
      <input type="submit" name="bevestig_volume" value="OK" onclick="ok()">
    </center>
    </form>
    <div class="blanco_40">&nbsp;</div>
    <form action="/get" target="hidden-form">
      <center>
        <input type="submit" value="Instellingen" onclick="instellen()"></button>
      </center>
    </form>
    <script>
      function ok(){
        setTimeout(function(){document.location.reload();},250);
      }
      function instellen(){
        location.assign("/instellen");
      }
    </script>
  </body>  
</html>
)rawliteral";

/*
 * Instellingen
 */
const char instellen_html[] = R"rawliteral(
<!DOCTYPE HTML>
<html>
  <head>
    <iframe style="display:none" name="hidden-form"></iframe>
    <title>instellen</title>
    <meta name="viewport" content="width=device-width, initial-scale=.85">
    <style>
        div.kader {
          position: relative;
          width: 400px;
          height: 12x;
        }
        div.links{
          position: absolute;
          left : 0px;
          width; auto;
          height: 12px;
        }
        div.links_midden{
          position:absolute;
          left:  108px;
          width: auto;
          height: 12px; 
        }
        div.blanco_20{
          width: auto;
          height: 20px;
        }
        div.blanco_40{
          width: auto;
          height: 40px;
        }
        div.blanco_60{
          width: auto;
          height: 60px;
        }
    </style>
  </head>
  <body>
    <h5><center><strong>Tijdzone instellen</strong></center></h5>
    <form action="/get">
    <small>
    <center>
    <div class="kader"><center><input type="text" style="text-align:center;" value="%tijdzone%" name="tz" size=auto></center></div>
    </center>
    </small>
    <div class="blanco_20">&nbsp;</div>
    <center><input type="submit" value="Bevestig" onclick="ok()"></center>
    </form>
    <div class="blanco_20">&nbsp;</div>
    <h5><center><strong>NTP server instellen</strong></center></h5>
    <form action="/get">
    <small>
    <center>
    <div class="kader"><center><input type="text" style="text-align:center;" value="%ntp%" name="ntp_server" size=auto></center></div>
    </center>
    </small>
    <div class="blanco_20">&nbsp;</div>
    <center><input type="submit" value="Bevestig" onclick="ok()"></center>
    </form>
    <div class="blanco_20">&nbsp;</div>
    <h5><center><strong>ESP32 Netwerk instellingen</strong></center></h5>
    <form action="/get">
    <small>
    <center>
      <div class="kader">
        <div class="links"><b>ssid :</b></div>
        <div class="links_midden"><center><input type="text" style="text-align:center;" name="ssid"></center></div>
      </div>
    </center>
    <div class="blanco_40">&nbsp;</div>
    <center>
      <div class="kader">
        <div class="links"><b>pswd :</b></div>
        <div class="links_midden"><center><input type="text" style="text-align:center;" name="pswd"></center></div>
      </div>
    </center>
    </small>
    <div class="blanco_40">&nbsp;</div>
    <center><input type="submit" value="Bevestig" onclick="ok()"></center>
    </form>
    <div class="blanco_20">&nbsp;</div>
    <h5><center>Gewenst IP address (default 192.168.1.222)</center></h5>
    <form action="/get">
    <small>
    <center>
      <div class="kader">
        <center>
          <input type="text" style="text-align:center;" value="%ip_address_1%" name="ip_1_keuze" size=1>
          &nbsp;&nbsp;
          <input type="text" style="text-align:center;" value="%ip_address_2%" name="ip_2_keuze" size=1>
          &nbsp;&nbsp;
          <input type="text" style="text-align:center;" value="%ip_address_3%" name="ip_3_keuze" size=1>
          &nbsp;&nbsp;
          <input type="text" style="text-align:center;" value="%ip_address_4%" name="ip_4_keuze" size=1>
        </center>
      </div>
    </center>
    </small>  
    <div class="blanco_20">&nbsp;</div>
    <center><input type="submit" value="Bevestig" onclick="ok()"></center>
    </form>
    <div class="blanco_40">&nbsp;</div>
    <div class="blanco_40">&nbsp;</div>
    <form action="/get" target="hidden-form">
      <center>
        <input type="submit" value="Hoofdpagina" onclick="internetradio()"></button>
      </center>
    </form>
    <script>
      function ok(){
        setTimeout(function(){document.location.reload();},750);
      }
      function internetradio(){
        location.assign("/");
      }
    </script>
  </body>  
</html>
)rawliteral";

/*
 * processor
 */
String processor(const String& var){
  if(var == "zenderNu"){
    if(gekozen == -2){
      String mp3_lijst = "mp3 lijst maken";
      mp3_lijst.toCharArray(speler, (mp3_lijst.length() + 1));
      return(speler);
    }
    else if(gekozen == -1){
      String mp3_speler = "mp3 speler";
      mp3_speler.toCharArray(speler, (mp3_speler.length() + 1));
      return(speler);
    }
    else{
      return(zenderarray[gekozen]);
    }
  }
  if(var == "song"){
    return(streamsong);
  }
  if(var == "selectie"){
    if(gn_keuze == 0){
      String selectie = "Stop mp3 speler";
      selectie.toCharArray(gn_selectie, (selectie.length() + 1));
      return(gn_selectie);
    }
    if(gn_keuze == 1){
      String selectie = "mp3 speler";
      selectie.toCharArray(gn_selectie, (selectie.length() + 1));
      return(gn_selectie);
    }
    if(gn_keuze == 2){
      String selectie = "Maak mp3 lijst";
      selectie.toCharArray(gn_selectie, (selectie.length() + 1));
      return(gn_selectie);
    }
  }
  if(var == "selecteren"){
    if(keuze == - 2){
      String mp3_lijst = "mp3 lijst maken";
      mp3_lijst.toCharArray(speler, (mp3_lijst.length() + 1));
      return(speler);
    }
    else if((keuze == -1)){     
      String mp3_speler = "mp3 speler";
      mp3_speler.toCharArray(speler, (mp3_speler.length() + 1));
      return(speler);
    }
    else{
      return(zenderarray[keuze]);
    }
  }
  if(var == "laag_kiezen"){
    return(String(laag_gekozen));
  }
  if(var == "midden_kiezen"){
    return(String(midden_gekozen));
  }
  if(var == "hoog_kiezen"){
    return(String(hoog_gekozen));
  }
  if(var == "volume_kiezen"){
    return(String(volume_gekozen));
  }
  if(var == "array_index"){
    return(String(array_index));
  }
  if(var == "zender"){
    return(String(zenderarray[array_index]));
  }
  if(var == "url"){
    return(String(urlarray[array_index]));
  }
  if(var == "folder"){
    String folder = mp3_folder;
    folder.toCharArray(mp3_lijst_folder, (folder.length() + 1));
    return(mp3_lijst_folder);
  }
  if(var == "mp3"){
    String aantal = String(mp3_aantal);
    aantal.toCharArray(mp3_lijst_aantal, (aantal.length() + 1));
    return(mp3_lijst_folder);
  }
  if(var == "tijdzone"){
    return(tijdzone_string);
  }
  if(var == "ntp"){
    return(ntp_string);
  }
  if(var == "ip_address_1"){
    return(String(ip_1_int));
  }
  if(var == "ip_address_2"){
    return(String(ip_2_int));
  }
  if(var == "ip_address_3"){
    return(String(ip_3_int));
  }
  if(var == "ip_address_4"){
    return(String(ip_4_int));
  }
}

/*
 * Input van webpagina
 */
void html_input(){
  server.begin();
  if(netwerk){
    Serial.println(WiFi.localIP());
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/html", index_html, processor);
    });
    server.on("/instellen", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/html", instellen_html, processor);
    });
    server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request){
      String http_zender = "                         ";
      String http_url = "                                                                             ";
      String netwerk = "                         ";
      String paswoord = "                          ";
      bool ssid_ingevuld = false;
      bool pswd_ingevuld = false;
      bool ip_1_bool = false;
      bool ip_2_bool = false;
      bool ip_3_bool = false;
      bool ip_4_bool = false;
      char terminator = char(0x0a);
      if(request->hasParam(KEUZEMIN_INPUT)){
        wachttijd = millis();
        web_kiezen = true;
        keuze--;
        while((urlarray[keuze][0] != *h_char) && (keuze > 0)){
          keuze --;
        }
        if(keuze < -2){
          keuze = MAX_AANTAL_KANALEN - 1;
            while((urlarray[keuze][0] != *h_char) && (keuze > 0)){
              keuze --;
            }
        }
      }
      if(request->hasParam(KEUZEPLUS_INPUT)){
        wachttijd = millis();
        web_kiezen = true;
        keuze++;
        if(keuze > MAX_AANTAL_KANALEN + 1){
          keuze = 0;
        }
        if((keuze > 0) && (keuze < MAX_AANTAL_KANALEN)){
          while((urlarray[keuze][0] != *h_char) && (keuze < MAX_AANTAL_KANALEN)){
            keuze ++;
          }
        }
        if(keuze == MAX_AANTAL_KANALEN){
          keuze = -2;
        }
        if(keuze == MAX_AANTAL_KANALEN + 1 ){
          keuze = -1;
        }
      }
      if(request->hasParam(BEVESTIGKEUZE_INPUT)){
        gekozen_bool = true;
        web_kiezen = false;
        streamsong = "";
        netwerk_mp3_controle();
        if(keuze > -1){
          gekozen = keuze;
          pref.putShort("station", gekozen);
          webradio = true;
        }
      }
      if(request->hasParam(LAAG)){
        laag_keuze = ((request->getParam(LAAG)->value()) + String(terminator)).toInt();
      }
      if(request->hasParam(MIDDEN)){
        midden_keuze = ((request->getParam(MIDDEN)->value()) + String(terminator)).toInt();
      }
      if(request->hasParam(HOOG)){
        hoog_keuze = ((request->getParam(HOOG)->value()) + String(terminator)).toInt();
      }
      if(request->hasParam(VOLUME)){
        volume_keuze = ((request->getParam(VOLUME)->value()) + String(terminator)).toInt();
      }
      if(request->hasParam(VOLUME_BEVESTIG)){
        if((laag_keuze > -41) && (laag_keuze < 7)){
          laag_gekozen = laag_keuze;
          }
          if((midden_keuze > -41) && (midden_keuze < 7)){
            midden_gekozen = midden_keuze;
          }
          if((hoog_keuze > -41) && (hoog_keuze < 7)){
            hoog_gekozen = hoog_keuze;
          }
          if((volume_keuze > -1) && (volume_keuze < 22)){
            volume_gekozen = volume_keuze;
          }
          audio.setVolume(volume_gekozen);
          audio.setTone(laag_gekozen, midden_gekozen, hoog_gekozen);
          pref.putShort("laag", laag_gekozen);
          pref.putShort("midden", midden_gekozen);
          pref.putShort("hoog", hoog_gekozen);
          pref.putShort("volume", volume_gekozen);
        }
      if(request->hasParam(ARRAY_MIN)){
        array_index -= 1;
        if(array_index < 0){
          array_index = MAX_AANTAL_KANALEN - 1;
        }
      }
      if(request->hasParam(ARRAY_PLUS)){
        array_index += 1;
        if(array_index > MAX_AANTAL_KANALEN - 1){
          array_index = 0;
        }
      }
      if(request->hasParam(ZENDER)){
        http_zender = (request->getParam(ZENDER)->value());
      }
      if(request->hasParam(URL)){
        http_url = (request->getParam(URL)->value());
      }
      if(request->hasParam(BEVESTIG_ZENDER)){
        memset(zenderarray[array_index], 0, sizeof(zenderarray[array_index]));
        http_zender.toCharArray(zenderarray[array_index], http_zender.length() + 1);
        memset(urlarray[array_index], 0, sizeof(urlarray[array_index]));
        http_url.toCharArray(urlarray[array_index], http_url.length() + 1);
        schrijf_csv = true; 
      }
      if(request->hasParam(STA_SSID)){
        netwerk = (request->getParam(STA_SSID)->value());
        if((netwerk.length()) > 7){
          netwerk = netwerk + String(terminator);
          netwerk.toCharArray(ssid, (netwerk.length() +1));
          ssid_ingevuld = true;
        }
      }
      if(request->hasParam(STA_PSWD)){
        paswoord = (request->getParam(STA_PSWD)->value());
        if((paswoord.length()) > 7){
          paswoord = paswoord + String(terminator);
          paswoord.toCharArray(password, (paswoord.length() + 1));
          pswd_ingevuld = true;
        }
      }
      if((ssid_ingevuld) && (pswd_ingevuld)){
        ssid_ingevuld = false;
        pswd_ingevuld = false;
        writeFile(SD, "/pswd", password);
        writeFile(SD, "/ssid", ssid);
        Serial.println("Restart over 5 seconden");
        delay(5000);
        ESP.restart();
      }
      if(request->hasParam(TZ)){
        tijdzone_string = (request->getParam(TZ)->value()) + String(terminator);
        tijdzone_string.toCharArray(tijdzone_char, (tijdzone_string.length() +1));
        tijdzone_bool = true;
      }
      if(request->hasParam(NTP_SERVER)){
        ntp_string = (request->getParam(NTP_SERVER)->value()) + String(terminator);
        ntp_string.toCharArray(ntp_char, (ntp_string.length() +1));
        ntp_bool = true;
      }
      if(request->hasParam(IP_1_KEUZE)){
        ip_1_string = (request->getParam(IP_1_KEUZE)->value()) +String(terminator);
        ip_1_bool = true;
      }
      if(request->hasParam(IP_2_KEUZE)){
        ip_2_string = (request->getParam(IP_2_KEUZE)->value()) +String(terminator);
        ip_2_bool = true;
      }
      if(request->hasParam(IP_3_KEUZE)){
        ip_3_string = (request->getParam(IP_3_KEUZE)->value()) +String(terminator);
        ip_3_bool = true;
      }
      if(request->hasParam(IP_4_KEUZE)){
        ip_4_string = (request->getParam(IP_4_KEUZE)->value()) +String(terminator);
        ip_4_bool = true;
      }
      if((ip_1_bool) && (ip_2_bool) && (ip_3_bool) && (ip_4_bool)){
        ip_1_bool = false;
        ip_2_bool = false;
        ip_3_bool = false; 
        ip_4_bool = false;
        pref.putShort("ip_1", ip_1_string.toInt());
        pref.putShort("ip_2", ip_2_string.toInt());
        pref.putShort("ip_3", ip_3_string.toInt());
        pref.putShort("ip_4", ip_4_string.toInt());
        Serial.println("Restart over 5 seconden");
        delay(5000);
        ESP.restart();
      }
    });
  }
  if(!netwerk){
    Serial.println("geen netwerk");
    Serial.println(WiFi.softAPIP());
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/html", mp3_html, processor);
    });
    server.on("/instellen", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/html", instellen_html, processor);
    });
    server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request){
      String netwerk = "                         ";
      String paswoord = "                          ";
      bool ssid_ingevuld = false;
      bool pswd_ingevuld = false;
      bool ip_1_bool = false;
      bool ip_2_bool = false;
      bool ip_3_bool = false;
      bool ip_4_bool = false;
      char terminator = char(0x0a);
      if(request->hasParam(MIN_INPUT)){
        wachttijd = millis();
        web_kiezen = true;
        gn_keuze --;
        if(gn_keuze < 0){
          gn_keuze = 2;
        }
      }
      if(request->hasParam(PLUS_INPUT)){
        wachttijd = millis();
        web_kiezen = true;
        gn_keuze ++;
        if(gn_keuze > 2){
          gn_keuze = 0;
        }
      }
      if(request->hasParam(BEVESTIG_MP3)){
        web_kiezen = false;
        streamsong = "";
        geen_netwerk_mp3_controle();
      }
      if(request->hasParam(LAAG)){
        laag_keuze = ((request->getParam(LAAG)->value()) + String(terminator)).toInt();
      }
      if(request->hasParam(MIDDEN)){
        midden_keuze = ((request->getParam(MIDDEN)->value()) + String(terminator)).toInt();
      }
      if(request->hasParam(HOOG)){
        hoog_keuze = ((request->getParam(HOOG)->value()) + String(terminator)).toInt();
      }
      if(request->hasParam(VOLUME)){
        volume_keuze = ((request->getParam(VOLUME)->value()) + String(terminator)).toInt();
      }
      if(request->hasParam(VOLUME_BEVESTIG)){
        if((laag_keuze > -41) && (laag_keuze < 7)){
          laag_gekozen = laag_keuze;
        }
        if((midden_keuze > -41) && (midden_keuze < 7)){
          midden_gekozen = midden_keuze;
        }
        if((laag_keuze > -41) && (laag_keuze < 7)){
          hoog_gekozen = hoog_keuze;
        }
        if((volume_keuze > -1) && (laag_keuze < 22)){
          volume_gekozen = volume_keuze;
        }
        audio.setVolume(volume_gekozen);
        audio.setTone(laag_gekozen, midden_gekozen, hoog_gekozen);
        pref.putShort("laag", laag_gekozen);
        pref.putShort("midden", midden_gekozen);
        pref.putShort("hoog", hoog_gekozen);
        pref.putShort("volume", volume_gekozen);
      }
      if(request->hasParam(STA_SSID)){
        netwerk = (request->getParam(STA_SSID)->value());
        if((netwerk.length()) > 7){
          netwerk = netwerk + String(terminator);
          netwerk.toCharArray(ssid, (netwerk.length() +1));
          Serial.println(ssid);
          ssid_ingevuld = true;
        }
      }
      if(request->hasParam(STA_PSWD)){
        paswoord = (request->getParam(STA_PSWD)->value());
        if((paswoord.length()) > 7){
          paswoord = paswoord + String(terminator);
          paswoord.toCharArray(password, (paswoord.length() + 1));
          Serial.println(password);
          pswd_ingevuld = true;
        }
      }
      if(request->hasParam(TZ)){
        tijdzone_string = (request->getParam(TZ)->value()) + String(terminator);
        tijdzone_string.toCharArray(tijdzone_char, (tijdzone_string.length() +1));
        tijdzone_bool = true;
      }
      if((ssid_ingevuld) && (pswd_ingevuld)){
        ssid_ingevuld = false;
        pswd_ingevuld = false;
        writeFile(SD, "/pswd", password);
        writeFile(SD, "/ssid", ssid);
        Serial.println("Restart over 5 seconden");
        delay(5000);
        ESP.restart();
      }
      
      if(request->hasParam(IP_1_KEUZE)){
        ip_1_string = (request->getParam(IP_1_KEUZE)->value()) +String(terminator);
        ip_1_bool = true;
      }
      if(request->hasParam(IP_2_KEUZE)){
        ip_2_string = (request->getParam(IP_2_KEUZE)->value()) +String(terminator);
        ip_2_bool = true;
      }
      if(request->hasParam(IP_3_KEUZE)){
        ip_3_string = (request->getParam(IP_3_KEUZE)->value()) +String(terminator);
        ip_3_bool = true;
      }
      if(request->hasParam(IP_4_KEUZE)){
        ip_4_string = (request->getParam(IP_4_KEUZE)->value()) +String(terminator);
        ip_4_bool = true;
      }
      if((ip_1_bool) && (ip_2_bool) && (ip_3_bool) && (ip_4_bool)){
        ip_1_bool = false;
        ip_2_bool = false;
        ip_3_bool = false; 
        ip_4_bool = false;
        pref.putShort("ip_1", ip_1_string.toInt());
        pref.putShort("ip_2", ip_2_string.toInt());
        pref.putShort("ip_3", ip_3_string.toInt());
        pref.putShort("ip_4", ip_4_string.toInt());
        Serial.println("Restart over 5 seconden");
        delay(5000);
        ESP.restart();
      }
    });
  }
}

void setup(){
  delay(2500);
  Serial.begin(115200);
  pinMode(STATION_ENCODER, INPUT);
  pinMode(VOLUME_ENCODER, INPUT);
  pinMode(TOUCH_INT, INPUT);
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);
  digitalWrite(TFT_CS, HIGH);
  pinMode(SD_MISO, INPUT_PULLUP);
  tft.init();
  tft.setRotation(3);
  touch_calibrate();
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_YELLOW);
  tft.setTextFont(4);
  tft.setCursor((480 - tft.textWidth("Internet radio", 4)) / 2, 100);
  tft.print("Internet radio");
  tft.setCursor((480 - tft.textWidth("mp3 speler", 4)) / 2, 140);
  tft.print("mp3 speler");
  tft.setTextColor(TFT_WHITE);
  tft.setTextFont(1);
  tft.setCursor (0, 300);
  tft.print("thieu-b55   januari 2024");
  if(!SD.begin(SD_CS)){
    Serial.println("check SD kaart");
  }
  ntp_string = readFile(SD, "/ntp");
  ntp_string = ntp_string.substring(0, ntp_string.length() - 1);
  tijdzone_string = readFile(SD, "/tz");
  tijdzone_string = tijdzone_string.substring(0, tijdzone_string.length() - 1);
  if(!pref.begin("WebRadio", false)){
    Serial.println("SPIFFS probleem"); 
  }
  //pref.clear();
  if(pref.getString("controle") != "dummy geladen"){
    pref.putShort("netwerk", netwerk_keuze_int);
    pref.putShort("ip_1", ip_1_int);
    pref.putShort("ip_2", ip_2_int);
    pref.putShort("ip_3", ip_3_int);
    pref.putShort("ip_4", ip_4_int);
    pref.putShort("station", 1);
    pref.putShort("volume", 10);
    pref.putShort("laag", 0);
    pref.putShort("midden", 0);
    pref.putShort("hoog", 0);
    pref.putString("controle", "dummy geladen");
  }
  netwerk_keuze_int = pref.getShort("netwerk");
  if(netwerk_keuze_int == 0){
    eth_bool = true;
  }
  if(netwerk_keuze_int == 1){
    wifi_bool = true;
  }
  if(netwerk_keuze_int == 2){
    WiFi.mode(WIFI_AP);
    WiFi.softAP(APssid, APpswd);
    html_input();
    tft_bericht_long = millis();
    while((millis() - tft_bericht_long) < 5000){
      yield();
    }
    gn_keuze = 1;
    gn_keuze_vorig = 1;
    speel_mp3 = true;
  }
  ip_1_int = pref.getShort("ip_1");
  ip_2_int = pref.getShort("ip_2");
  ip_3_int = pref.getShort("ip_3");
  ip_4_int = pref.getShort("ip_4");
  gekozen = pref.getShort("station");
  volume_gekozen = pref.getShort("volume");
  volume_keuze = volume_gekozen;
  laag_gekozen = pref.getShort("laag");
  laag_keuze = laag_gekozen;
  midden_gekozen = pref.getShort("midden");
  midden_keuze = midden_gekozen;
  hoog_gekozen = pref.getShort("hoog");
  hoog_keuze = hoog_gekozen;
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(volume_gekozen);
  audio.setTone(laag_gekozen, midden_gekozen, hoog_gekozen);
  netwerk = false;
  lees_CSV();
  file_test();
   /*
   * ETH.begin() alvorens attachInterruptPin van roatary encoders
   */
  if(eth_bool){
    IPAddress local_IP(ip_1_int, ip_2_int, ip_3_int, ip_4_int);
    IPAddress gateway(192, 168, 1, 1);
    IPAddress subnet(255, 255, 0, 0);
    IPAddress primaryDNS(192, 168, 1, 1);
    IPAddress secondaryDNS(0, 0, 0, 0); 
    ESP32_W5500_onEvent();
    ETH.begin( MISO_GPIO, MOSI_GPIO, SCK_GPIO, CS_GPIO, INT_GPIO, SPI_CLOCK_MHZ, ETH_SPI_HOST);
    ETH.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS);
    eth_verbonden_bool = true;
    delay(5000);
  }
  /*
   * EERST ETH.begin() DAARNA attachInterrupt, bij andere volgorde GEEN ethernet verbinding
   */
  attachInterrupt(TOUCH_INT, touch, FALLING);
  attachInterrupt(STATION_ENCODER, station, CHANGE);
  attachInterrupt(VOLUME_ENCODER, volume, CHANGE);
  /*
   * Kiezen ethernet of WiFi
   */
  eth_wifi_keuze();
  wachttijd = millis();
  while((millis() - wachttijd) < 10000){
    int max_int;
    if((mp3_0_bestaat_bool) && (songlijst0_bestaat_bool)){
      max_int = 2;
    }
    else{
      max_int = 1;
      if(netwerk_keuze_int == 2){
        netwerk_keuze_int = 0;
        pref.putShort("netwerk", netwerk_keuze_int);
        eth_wifi_keuze();
      }
    }
    if(station_plus_bool){
      wachttijd = millis();
      netwerk_keuze_int ++;
      if(netwerk_keuze_int > max_int){
        netwerk_keuze_int = 0;
      }
      eth_wifi_keuze();
      station_plus_bool = false;
    }
    if(station_min_bool){
      wachttijd = millis();
      netwerk_keuze_int --;
      if(netwerk_keuze_int < 0){
        netwerk_keuze_int = max_int;
      }
      eth_wifi_keuze();
      station_min_bool = false;
    }
    if(station_switch_bool){
      pref.putShort("station", random(5));
      if(netwerk_keuze_int != 2){
        pref.putShort("netwerk", netwerk_keuze_int);
        ESP.restart();
      }
      else{
        if((songlijst0_bestaat_bool) && (mp3_0_bestaat_bool)){
          pref.putShort("netwerk", 2);
          ESP.restart();
        }
      }
    }
  }
  /*
   * Ethernet gekozen
   */
  if(eth_bool){
    netwerk = true;
    WiFi.mode(WIFI_AP);
    WiFi.softAP(APssid, APpswd);
    keuze = gekozen;
    keuze_vorig = keuze;
    radio_gekozen();
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_GREEN);
    tft.setTextFont(4);
    tft.setCursor((480 - tft.textWidth(zenderarray[gekozen], 4)) / 2, tft_positie_int);
    tft.print(zenderarray[gekozen]);
    tft.setTextColor(TFT_YELLOW);
    tft.setTextFont(2);
    tft.setCursor((480 - tft.textWidth(streamsong, 2)) / 2, tft_positie_int + 30);
    tft.print(streamsong);
    tft_update_long = millis();
    touch_update_long = millis();
    html_input();
  }
  /*
   * WiFi gekozen
   */
  if(wifi_bool){
    inputString = readFile(SD, "/ssid");
    inputString.toCharArray(ssid, teller);
    //Serial.println(ssid);
    inputString = readFile(SD, "/pswd");
    inputString.toCharArray(password, teller);
    //Serial.println(password);
    delay(2500);
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    netwerk = true;
    wacht_op_netwerk = millis();
    while(WiFi.status() != WL_CONNECTED){
      delay(500);
      if(millis() - wacht_op_netwerk > 15000){
        netwerk = false;
        break;
      }
    }
    if(netwerk){  
      IPAddress subnet(WiFi.subnetMask());
      IPAddress gateway(WiFi.gatewayIP());
      IPAddress dns(WiFi.dnsIP(0));
      IPAddress static_ip(ip_1_int, ip_2_int, ip_3_int, ip_4_int);
      WiFi.disconnect();
      if (WiFi.config(static_ip, gateway, subnet, dns, dns) == false) {
        Serial.println("Configuration failed.");
      }
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid, password);
      wacht_op_netwerk = millis();
      while(WiFi.status() != WL_CONNECTED){
        delay(500);
        if(millis() - wacht_op_netwerk > 15000){
          netwerk = false;
          break;
        }
      }
      keuze = gekozen;
      keuze_vorig = keuze;
      radio_gekozen();
      tft.fillScreen(TFT_BLACK);
      tft.setTextColor(TFT_GREEN);
      tft.setTextFont(4);
      tft.setCursor((480 - tft.textWidth(zenderarray[gekozen], 4)) / 2, tft_positie_int);
      tft.print(zenderarray[gekozen]);
      tft.setTextFont(2);
      tft.setTextColor(TFT_YELLOW);
      tft.setCursor((480 - tft.textWidth(streamsong, 2)) / 2, tft_positie_int + 30);
      tft.print(streamsong);
      tft_update_long = millis();
      touch_update_long = millis();
      html_input();
    }
    else{
      WiFi.mode(WIFI_AP);
      WiFi.softAP(APssid, APpswd);
      tft.fillScreen(TFT_BLACK);
      tft.setTextColor(TFT_RED);
      tft.setTextFont(2);
      tft.setCursor(195, 100);
      tft.print("GEEN INTERNET");
      tft.setCursor(174, 120);
      tft.print("MAAK VERBINDING MET");
      tft.setCursor(209, 160);
      tft.setTextColor(TFT_YELLOW);
      tft.print("NETWERK :");
      tft.setCursor(195, 180);
      tft.setTextColor(TFT_GREEN);
      tft.print("ESP32webradio");
      tft.setCursor(205, 200);
      tft.setTextColor(TFT_YELLOW);
      tft.print("PASWOORD :");
      tft.setCursor(208, 220);
      tft.setTextColor(TFT_GREEN);
      tft.print("ESP32pswd");
      tft.setCursor(222, 240);
      tft.setTextColor(TFT_YELLOW);
      tft.print("IP :");
      tft.setCursor(201, 260);
      tft.setTextColor(TFT_GREEN);
      tft.print("192.168.4.1");
      html_input();
      tft_bericht_long = millis();
      while((millis() - tft_bericht_long) < 10000){
        yield();
      }
    }
  }
}

void loop(){
  if(eerste_loop_bool){
    eerste_loop_bool = false;
    /*
     * tijd instellen op lokale tijd
     */
    if((eth_bool) || (wifi_bool)){
      configTime(0, 0, ntp_string.c_str());
      if(getLocalTime(&timeinfo)){
        setenv("TZ",tijdzone_string.c_str(), 1); 
        tzset();
        tijd_bool = true;
      }
    }
  }
  /*
   * Ethernet verbonden ?
   */
  if(eth_bool){
    if(!ESP32_W5500_eth_connected){
      eth_verbonden_bool = false;
    }
  }
  /*
   * Rotary encoder station
   */
  if(netwerk){
    if(station_plus_bool){
      station_plus_bool = false;
      wachttijd = millis();
      kiezen = true;
      wis_tft();
      keuze++;
      if(keuze > MAX_AANTAL_KANALEN + 1){
        keuze = 0;
      }
      if((keuze > 0) && (keuze < MAX_AANTAL_KANALEN)){
        while((urlarray[keuze][0] != *h_char) && (keuze < MAX_AANTAL_KANALEN)){
          keuze ++;
        }
      }
      if(keuze == MAX_AANTAL_KANALEN){
        keuze = -2;
      }
      if(keuze == MAX_AANTAL_KANALEN + 1 ){
        keuze = -1;
      }
      netwerk_station_schrijf_tft();
    }
    if(station_min_bool){
      station_min_bool = false;
      wachttijd = millis();
      kiezen = true;
      wis_tft();
      keuze--;
      while((urlarray[keuze][0] != *h_char) && (keuze > 0)){
        keuze --;
      }
      if(keuze < -2){
        keuze = MAX_AANTAL_KANALEN - 1;
          while((urlarray[keuze][0] != *h_char) && (keuze > 0)){
            keuze --;
          }
      }
      netwerk_station_schrijf_tft();
    }
    if(station_switch_bool){
      station_switch_bool = false;
      kiezen = false;
      gekozen_bool = true;
      eerste_bool = false;
      streamsong = "";
      netwerk_mp3_controle();
      if(keuze > -1){
        gekozen = keuze;
        pref.putShort("station", gekozen);
        webradio = true;
      }
    } 
  }
  if(!netwerk){
    if(station_plus_bool){
      station_plus_bool = false;
      wachttijd = millis();
      kiezen = true;
      wis_tft();
      gn_keuze ++;
      if(gn_keuze > 2){
        gn_keuze = 0;
      }
      gn_netwerk_station_schrijf_tft();
    }
    if(station_min_bool){
      station_min_bool = false;
      wachttijd = millis();
      kiezen = true;
      wis_tft();
      gn_keuze --;
      if(gn_keuze < 0){
        gn_keuze = 2;
      }
      gn_netwerk_station_schrijf_tft();
    }
    if(station_switch_bool){
      station_switch_bool = false;
      kiezen = false;
      gekozen_bool = true;
      eerste_bool = false;
      streamsong = "";
      gn_keuze_vorig = gn_keuze;
      geen_netwerk_mp3_controle();
    }
  }
  /*
   * Rotary encoder Volume
   */
  if(volume_plus_bool){
    volume_plus_bool = false;
    wachttijd = millis();
    kiezen = true;
    tft.setTextFont(4);
    tft.setCursor(43, 147);
    tft.setTextColor(TFT_BLACK);
    tft.print(volume_gekozen);
    if(volume_gekozen < 21){
      volume_gekozen++;
    }
    volume_bewaren_bool = true;
    audio.setVolume(volume_gekozen);
    tft.setCursor(43, 147);
    tft.setTextColor(TFT_SKYBLUE);
    tft.print(volume_gekozen);
    tft.setTextFont(2);
  }
  if(volume_min_bool){
    volume_min_bool = false;
    wachttijd = millis();
    kiezen = true;
    tft.setTextFont(4);
    tft.setCursor(43, 147);
    tft.setTextColor(TFT_BLACK);
    tft.print(volume_gekozen);
    if(volume_gekozen > 0){
      volume_gekozen--;
    }
    volume_bewaren_bool = true;
    audio.setVolume(volume_gekozen);
    tft.setCursor(43, 147);
    tft.setTextColor(TFT_SKYBLUE);
    tft.print(volume_gekozen);
    tft.setTextFont(2);
  }
  /*
   * Inlezen touchscreen
   */
  if(touch_int_bool){
    t_x = 0;
    t_y = 0; 
    int max_loop = 0;
    pressed = tft.getTouch(&t_x, &t_y, TOUCH_TRESHOLD);
    while((!pressed) && (max_loop < MAX_LOOP)){
      max_loop++;
      pressed = tft.getTouch(&t_x, &t_y, TOUCH_TRESHOLD);
    }
    wachttijd = millis();
    kiezen = true;
    touch_int_bool = false;
    int_bool = true;
    touch_update_long = millis();
    if(eerste_bool){ 
      /*
       * Geluid +
       */
      if((t_x >= 1) && (t_x <= 100) && (t_y >= 1) && (t_y <= 100)){   
        tft.setTextFont(4);
        tft.setCursor(43, 147);
        tft.setTextColor(TFT_BLACK);
        tft.print(volume_gekozen);
        tft.setCursor(43, 147);
        tft.setTextColor(TFT_SKYBLUE);
        if(volume_gekozen < 21){
          volume_gekozen++;
        }
        tft.print(volume_gekozen);
        pref.putShort("volume", volume_gekozen);
        audio.setVolume(volume_gekozen);
        tft.setTextFont(2);
      }
      /*
       * Geluid -
       */
      if((t_x >= 1) && (t_x <= 100) && (t_y >= 220) && (t_y <= 320)){ 
        tft.setTextFont(4);
        tft.setCursor(43, 147);
        tft.setTextColor(TFT_BLACK);
        tft.print(volume_gekozen);
        tft.setCursor(43, 147);
        tft.setTextColor(TFT_SKYBLUE);
        if(volume_gekozen > 0){
          volume_gekozen--;
        }
        tft.print(volume_gekozen);
        pref.putShort("volume", volume_gekozen);
        audio.setVolume(volume_gekozen);
        tft.setTextFont(2);
      }
      if(netwerk){
      /*
       * Preset+
       */
        if((t_x >= 380) && (t_x <= 480) && (t_y >= 1) && (t_y <= 100)){
          wis_tft();
          keuze++;
          if(keuze > MAX_AANTAL_KANALEN + 1){
            keuze = 0;
          }
          if((keuze > 0) && (keuze < MAX_AANTAL_KANALEN)){
            while((urlarray[keuze][0] != *h_char) && (keuze < MAX_AANTAL_KANALEN)){
              keuze ++;
            }
          }
          if(keuze == MAX_AANTAL_KANALEN){
            keuze = -2;
          }
          if(keuze == MAX_AANTAL_KANALEN + 1 ){
            keuze = -1;
          }
          netwerk_station_schrijf_tft();
        }
        /*
         * Preset -
         */
        if((t_x >= 380) && (t_x <= 480) && (t_y >= 220) && (t_y <= 320)){
          wis_tft();
          keuze--;
          while((urlarray[keuze][0] != *h_char) && (keuze > 0)){
            keuze --;
          }
          if(keuze < -2){
            keuze = MAX_AANTAL_KANALEN - 1;
            while((urlarray[keuze][0] != *h_char) && (keuze > 0)){
              keuze --;
            }
          }
          netwerk_station_schrijf_tft();
        }
        /*
         * Preset ok
         */
        if((t_x >= 380) && (t_x <= 480) && (t_y >= 110) && (t_y <= 210)){
          kiezen = false;
          gekozen_bool = true;
          eerste_bool = false;
          streamsong = "";
          netwerk_mp3_controle();
          if(keuze > -1){
            gekozen = keuze;
            pref.putShort("station", gekozen);
            webradio = true;
          }
        } 
      }
      if(!netwerk){
      /*
       * Preset +
       */
        if((t_x >= 380) && (t_x <= 480) && (t_y >= 1) && (t_y <= 100)){
          wis_tft();
          gn_keuze ++;
          if(gn_keuze > 2){
            gn_keuze = 0;
          }
          gn_netwerk_station_schrijf_tft();
        }
        /*
         * Preset -
         */
        if((t_x >= 380) && (t_x <= 480) && (t_y >= 220) && (t_y <= 320)){
          wis_tft();
          gn_keuze --;
          if(gn_keuze < 0){
            gn_keuze = 2;
          }
          gn_netwerk_station_schrijf_tft();
        }
        /*
         * Preset OK
         */
        if((t_x >= 380) && (t_x <= 480) && (t_y >= 110) && (t_y <= 210)){
          kiezen = false;
          gekozen_bool = true;
          eerste_bool = false;
          streamsong = "";
          geen_netwerk_mp3_controle();
        }
      }
    }
  }
  if(((millis() - touch_update_long) > 500) && (int_bool)){
    attachInterrupt(digitalPinToInterrupt(TOUCH_INT), dummy, FALLING);
    detachInterrupt(digitalPinToInterrupt(TOUCH_INT));
    attachInterrupt(digitalPinToInterrupt(TOUCH_INT), touch, FALLING);
    int_bool = false;
  }
  if(!kiezen){
    if(((millis() - tft_update_long) > 10000) || (gekozen_bool)){
      gekozen_bool = false;
      tft_update_long = millis();
      tft_positie_int += tft_positie_plus_int;
      if((tft_positie_int < 20) || (tft_positie_int > 225)){
        tft_positie_plus_int = tft_positie_plus_int * -1;
      }
      if(netwerk){
        tft.fillScreen(TFT_BLACK);
        tft.setTextColor(TFT_GREEN);
        tft.setTextFont(4);
        if(gekozen == - 1){
          tft.setCursor((480 - tft.textWidth("mp3 speler", 4)) / 2, tft_positie_int);
          tft.print("mp3 speler");
          tft.setTextFont(2);
          if(wifi_bool){
            tft.setCursor(0, tft_positie_int + 277);
            tft.print("WiFi IP");
            tft.setCursor(100, tft_positie_int + 277);
            tft.print(": ");
            tft.print(WiFi.localIP());
          }
        }
        else if(gekozen == -2){
          tft.setCursor(188, tft_positie_int);
          tft.print("mp3 lijst maken");
        }
        else{
          tft.setTextFont(4);
          tft.setCursor((480 - tft.textWidth(zenderarray[gekozen], 4)) / 2, tft_positie_int);
          tft.print(zenderarray[gekozen]);
          tft.setTextFont(2);
        }
        tft.setTextColor(TFT_YELLOW);
        tft.setCursor((480 - tft.textWidth(streamsong, 2)) / 2, tft_positie_int + 30);
        tft.print(streamsong);
        if(tijd_bool){
          if(getLocalTime(&timeinfo)){
            tft.setTextColor(TFT_RED);
            tft.setTextFont(4);
            tft.setCursor(206, tft_positie_int + 60);
            tft.print(&timeinfo, "%H:%M");
          }
        }
        if(eth_bool){
          tft.setTextFont(2);
          tft.setTextColor(TFT_GREEN);
          tft.setCursor(0, tft_positie_int + 226);
          tft.print("Local network");
          tft.setCursor(100, tft_positie_int + 226);
          tft.print(": ESP32webradio");
          tft.setCursor(0, tft_positie_int + 243);
          tft.print("pswd");
          tft.setCursor(100, tft_positie_int + 243);
          tft.print(": ESP32pswd");
          tft.setCursor(0, tft_positie_int + 260);
          tft.print("IP");
          tft.setCursor(100, tft_positie_int + 260);
          tft.print(": 192.168.4.1");
          tft.setCursor(0, tft_positie_int + 277);
          tft.setTextColor(TFT_YELLOW);
          tft.print("Ethernet IP");
          tft.setCursor(100, tft_positie_int + 277);
          tft.print(": ");
          if(eth_verbonden_bool){
            tft.print(ETH.localIP());
          }
          else{
            tft.print("niet verbonden");
          }
        }
        if(wifi_bool){
          tft.setTextFont(2);
          tft.setTextColor(TFT_YELLOW);
          tft.setCursor(0, tft_positie_int + 277);
          tft.print("WiFi IP");
          tft.setCursor(100, tft_positie_int + 277);
          tft.print(": ");
          tft.print(WiFi.localIP());
        }
      }
      if(!netwerk){
        tft.fillScreen(TFT_BLACK);
        tft.setTextColor(TFT_GREEN);
        tft.setTextFont(2);
        if(gn_keuze == 0){
          tft.setCursor(188, tft_positie_int);
          tft.print("stop mp3 speler");
        }
        if(gn_keuze == 1){
          /*
          tft.setCursor(205, tft_positie_int);
          tft.print("mp3 speler");
          */
          tft.setTextColor(TFT_GREEN);
          tft.setTextFont(4);
          tft.setCursor((480 - tft.textWidth("mp3 speler", 4)) / 2, tft_positie_int);
          tft.print("mp3 speler");
          tft.setTextFont(2);
          //tft.setTextColor(TFT_GREEN);
          tft.setCursor(0, tft_positie_int + 243);
          tft.print("Local network");
          tft.setCursor(100, tft_positie_int + 243);
          tft.print(": ESP32webradio");
          tft.setCursor(0, tft_positie_int + 260);
          tft.print("pswd");
          tft.setCursor(100, tft_positie_int + 260);
          tft.print(": ESP32pswd");
          tft.setCursor(0, tft_positie_int + 277);
          tft.print("IP");
          tft.setCursor(100, tft_positie_int + 277);
          tft.print(": 192.168.4.1");
        }
        if(gn_keuze == 2){
          tft.setCursor(188, tft_positie_int);
          tft.print("mp3 lijst maken");
        }
        tft.setTextColor(TFT_YELLOW);
        tft.setCursor((480 - tft.textWidth(streamsong, 2)) / 2, tft_positie_int + 30);
        tft.print(streamsong);
      }
    }
  }
  if(kiezen && !eerste_bool){
    eerste_bool = true;
    keuze_vorig = keuze;
    tft.fillScreen(TFT_BLACK);
    tft.fillTriangle(50, 20, 35, 50, 65, 50, TFT_GREEN);
    tft.fillTriangle(50, 300, 35, 270, 65, 270, TFT_GREEN);
    tft.fillTriangle(430, 20, 415, 50, 445, 50, TFT_YELLOW);
    tft.fillRoundRect(412, 145, 30, 30, 5, TFT_SKYBLUE);
    tft.fillTriangle(430, 300, 415, 270, 445, 270, TFT_YELLOW);
    tft.setTextFont(2);
    tft.setTextColor(TFT_SKYBLUE);
    if(netwerk){
      if(keuze == - 1){
        tft.setCursor((480 - tft.textWidth("mp3 speler", 2)) / 2, 120);
        tft.print("mp3 speler");
      }
      else if(keuze == -2){
        tft.setCursor(188, 120);
        tft.print("mp3 lijst maken");
      }
      else{
        tft.setTextFont(2);
        tft.setCursor((480 - tft.textWidth(zenderarray[keuze], 2)) / 2, 120);
        tft.print(zenderarray[keuze]);
      }
    }
    if(!netwerk){
      if(gn_keuze == 0){
        tft.setCursor((480 - tft.textWidth("stop mp3 speler", 2)) / 2, 120);
        tft.print("stop mp3 speler");
      }
      if(gn_keuze == 1){
        tft.setCursor((480 - tft.textWidth("mp3 speler", 2)) / 2, 120);
        tft.print("mp3 speler");
      }
      if(gn_keuze == 2){
        tft.setCursor((480 - tft.textWidth("mp3 lijst maken", 2)) / 2, 120);
        tft.print("mp3 lijst maken");
      }
    }
    tft.setTextFont(4);
    tft.setCursor(43, 147);
    tft.print(volume_gekozen);
    tft.setTextFont(2);
  }
  if(schrijf_csv){
    schrijf_csv = false;
    schrijf_naar_csv();
  }
  if(lijst_maken == true){
    mp3_lijst_maken_gekozen();
  }
  if(speel_mp3 == true){
    speel_mp3 = false;
    mp3_gekozen();
  }
  if(webradio){
    webradio = false;
    pref.putShort("station", gekozen);
    radio_gekozen();
  }
  if(((millis() - wachttijd) > 5000) && (kiezen || web_kiezen)){
    kiezen = false;
    web_kiezen = false;
    keuze = gekozen;
    gn_keuze= gn_keuze_vorig;
    eerste_bool = false;
    if(volume_bewaren_bool){
      volume_bewaren_bool = false;
      pref.putShort("volume", volume_gekozen);
    }
  }
  if(tijdzone_bool){
    tijdzone_bool = false;
    writeFile(SD, "/tz", tijdzone_char);
    tijdzone_string = readFile(SD, "/tz");
    tijdzone_string = tijdzone_string.substring(0, tijdzone_string.length() - 1);
    setenv("TZ",tijdzone_string.c_str(),1); 
    tzset();
  }
  if(ntp_bool){
    ntp_bool = false;
    writeFile(SD, "/ntp", ntp_char);
    ntp_string = readFile(SD, "/ntp");
    ntp_string = ntp_string.substring(0, ntp_string.length() - 1);
    tijdzone_string = readFile(SD, "/tz");
    tijdzone_string = tijdzone_string.substring(0, tijdzone_string.length() - 1);
    configTime(0, 0, ntp_string.c_str());
    if(getLocalTime(&timeinfo)){
      setenv("TZ",tijdzone_string.c_str(), 1); 
      tzset();
      tijd_bool = true;
    }
  }
  audio.loop();
}

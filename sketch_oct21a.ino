
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <WiFiManager.h>

#define TFT_WIDTH  128
#define TFT_HEIGHT 160

String teks = "Ishom, Zuhdi, Ifham, Nadzan, Jauhar";

int indeks = 0;
char Hari[7][12] = { "Ahad", "Senin", "Selasa", "Rabu", "Kamis", "Jum'at", "Sabtu" };  //variabel berisi nama hari

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

byte omm = 99;
bool initial = 1;
byte xcolon = 0;
unsigned int colour = 0;

static uint8_t conv2d(const char* p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}

uint8_t hh = conv2d(__TIME__), mm = conv2d(__TIME__ + 3), ss = conv2d(__TIME__ + 6); // Get H, M, S from compile time

void setup(void) {
  Serial.begin(9600);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(8, 12);
  tft.print("Connecting To Wifi");

  delay(1000);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setCursor(0, 0);
  tft.setTextWrap(true);
  tft.print("Please Connect To Wifi");
  tft.setCursor(0, 10);
  tft.setTextWrap(true);
  tft.print("Wemos D1 R2 Pass(12345678)");
  tft.setCursor(0, 20);
  tft.setTextWrap(true);
  tft.print("Then Set The Device To");
  tft.setCursor(0, 30);
  tft.setTextWrap(true);
  tft.print("Connect To Which Wifi");
  WiFi.mode(WIFI_STA);
  WiFiManager wm;
  bool res;
  res = wm.autoConnect("WEMOS D1 R2", "12345678");  //jika belum terkoneksi ke WiFi manapun maka akan muncul WiFi dengan SSID: "WEMOS D1 R2" dan PASSWORD:"12345678"

  //Cek kondisi jika boolean res tidak ada/WiFi belum terkoneksi
  if (!res) {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_RED, TFT_BLACK);

    tft.setCursor(8, 12);
    tft.print("Waiting Connection");
  } else {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_RED, TFT_BLACK);

    tft.setCursor(8, 12);
    tft.print("Wifi Connected");
  }
  delay(2000);
  timeClient.begin();
  timeClient.setTimeOffset(+7 * 60 * 60);

  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setCursor(0, 110);
  tft.print(teks);

}



void loop() {
  timeClient.update();

  time_t rawTime = timeClient.getEpochTime();
  struct tm *timeInfo = localtime(&rawTime);
  // Mendapatkan waktu

  // Mendapatkan waktu
  int hh = timeClient.getHours();
  int mm = timeClient.getMinutes();
  int ss = timeClient.getSeconds();
  // Mendapatkan nama hari dalam bahasa Indonesia
  int dayIndex = day() - 1; // weekday() mengembalikan nilai 1 hingga 7, maka kurangi 1 untuk indeks array
  String dayName = Hari[weekday() + 1];

  // Mendapatkan tanggal
  int day = timeInfo->tm_mday;
  int month = timeInfo->tm_mon + 1; // tm_mon dimulai dari 0
  int year = 1900 + timeInfo->tm_year; // Tahun 1900 ditambahkan ke tm_year
  // Menampilkan tanggal

  if (ss == 0 || initial) {
    initial = 0;
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.drawCentreString(dayName + ", " + day + " - " + month + " - " + year, 80, 48, 2); // Next size up font 2
  }


  // Update digital time
  byte xpos = 6;
  byte ypos = 0;
  if (omm != mm) { // Only redraw every minute to minimise flicker
    // Uncomment ONE of the next 2 lines, using the ghost image demonstrates text overlay as time is drawn over it
    //    tft.setTextColor(0x39C4, TFT_BLACK);  // Leave a 7 segment ghost image, comment out next line!
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_BLACK, TFT_BLACK); // Set font colour to black to wipe image
    // Font 7 is to show a pseudo 7 segment display.
    // Font 7 only contains characters [space] 0 1 2 3 4 5 6 7 8 9 0 : .
    tft.drawString("88:88", xpos, ypos, 7); // Overwrite the text to clear it
    tft.setTextColor(0x7E0); // Orange
    omm = mm;

    if (hh < 10) xpos += tft.drawChar('0', xpos, ypos, 7);
    xpos += tft.drawNumber(hh, xpos, ypos, 7);
    xcolon = xpos;
    xpos += tft.drawChar(':', xpos, ypos, 7);
    if (mm < 10) xpos += tft.drawChar('0', xpos, ypos, 7);
    tft.drawNumber(mm, xpos, ypos, 7);
  }

  if (ss % 2) { // Flash the colon
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    xpos += tft.drawChar(':', xcolon, ypos, 7);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
  }




}

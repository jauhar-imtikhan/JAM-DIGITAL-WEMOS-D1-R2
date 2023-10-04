#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiManager.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  //deklarasi pin lcd 16x2 I2C

const long utcOffsetSecond = 3600 * 7;  //mengatur waktu lokal dengan memperhitungkan perbedaan waktu UTC sesuai dengan zona waktu

String teks = "Ishom, Zuhdi, Ifham, Nadzan, Jauhar";  //running text

int indeks = 0;  //variabel untuk menentukan awal dari running text

char Hari[7][12] = { "Ahad", "Senin", "Selasa", "Rabu", "Kamis", "Jum'at", "Sabtu" };  //variabel berisi nama hari

WiFiUDP ntpUDP;                                                     //deklarasi library WiFiUDP menjadi ntpUDP
NTPClient WaktuClient(ntpUDP, "id.pool.ntp.org", utcOffsetSecond);  //fungi konek ke ntp server



void setup() {
  Serial.begin(9600);             //memulai serial monitor untuk debug koneksi wifi
  lcd.begin();                    //menyalakan lcd 16x2 I2C
  lcd.setCursor(2.5, 0);          //menentukan letak awal karakter pada lcd
  lcd.print("JAM DIGITAL");       //menghasilkan output "JAM DIGITAL"
  lcd.setCursor(2, 1);            //menentukan letak awal karakter pada lcd
  lcd.print("NTP SERVER");        //menghasilkan output "NTP SERVER"
  delay(4000);                    //delay 4 detik
  lcd.clear(); //menghapus semua karakter pada lcd
  lcd.setCursor(0, 0);            //menentukan letak awal karakter pada lcd
  lcd.print("Menghubungkan Ke");  //menghasilkan output "Menghubungkan Ke"
  lcd.setCursor(0, 1);            //menentukan letak awal karakter pada lcd
  lcd.print("Wifi");  //menghasilkan output "Wifi"
  delay(2000);                    //delay 2 detik
  WiFi.mode(WIFI_STA);            //deklarasi mode WiFi silahkan baca dokumentasi resmi "https://github.com/tzapu/WiFiManager"
  WiFiManager wm;                 //Inisialisasi lokal settingan WiFi. Setelah selesai, tidak akan tersimpan

  bool res;                                         //boolean res untuk menampung fungsi "wm" diatas ⬆️
  res = wm.autoConnect("WEMOS D1 R2", "12345678");  //jika belum terkoneksi ke WiFi manapun maka akan muncul WiFi dengan SSID: "WEMOS D1 R2" dan PASSWORD:"12345678"

  //Cek kondisi jika boolean res tidak ada/WiFi belum terkoneksi
  if (!res) {
    lcd.clear();                    //menghapus semua karakter yang sudah tampil pada lcd
    lcd.setCursor(0, 0);            //menentukan letak awal karakter pada lcd
    lcd.print("Belum Terkoneksi");  //menghasilkan output "Belum Terkoneksi"
    lcd.setCursor(0, 1);            //menentukan letak awal karakter pada lcd
    lcd.print("Ke WiFi");           //menghasilkan output "Ke WiFi"
  } else {
    lcd.clear();                 //menghapus semua karakter yang sudah tampil pada lcd
    lcd.setCursor(0, 0);         //menentukan letak awal karakter pada lcd
    lcd.print("WiFi Berhasil");  //menghasilkan output "WiFi Berhasil"
    lcd.setCursor(0, 1);         //menentukan letak awal karakter pada lcd
    lcd.print("Terhubung");      //menghasilkan output "Terhubung"
  }
  delay(2000);          //delay 2 detik
  lcd.clear();          //menghapus semua karakter yang sudah tampil pada lcd
  WaktuClient.begin();  //WaktuClient berasal dari deklarasi fungsi konek ke ntp server
}

void loop() {

//fungsi membuat runing text
  char str[17];
  str[16] = 0;

  for (int j = 0; j < 16; j++) {
    if (indeks + j < teks.length()) {
      str[j] = teks[indeks + j];
    } else {
      str[j] = 32;
    }
  }

//fungsi menentukan awal running text berada pada kolom "0" baris "1"
  lcd.setCursor(0, 1);
  lcd.print(str);
  indeks++;
  if (indeks == teks.length() - 1) {
    indeks = 0;
    delay(600);
  }

  // fungsi update jam
  WaktuClient.update();

  lcd.setCursor(0, 0); //menentukan letak awal karakter pada lcd
  lcd.print(Hari[WaktuClient.getDay()]); //menghasilkan output nama hari sesuai dengan variabel "Hari" diatas
  lcd.print(",");//menghasilkan output ","
  if (WaktuClient.getHours() <= 9) { lcd.print("0"); } //cek kondisi jika fungsi "WaktuClient.getHours" memiliki value lebih dari sama dengan 9 maka lcd akan menghasilkan output "angka 0"
  lcd.print(WaktuClient.getHours()); //menghasilkan output "jam"
  lcd.print(":"); //menghasilkan output ":"
  if (WaktuClient.getMinutes() <= 9) { lcd.print("0"); } //cek kondisi jika fungsi "WaktuClient.getMinutes" memiliki value lebih dari sama dengan 9 maka lcd akan menghasilkan output "angka 0"
  lcd.print(WaktuClient.getMinutes()); //menghasilkan output "menit"
  lcd.print(":"); //menghasilkan output ":"
  if (WaktuClient.getSeconds() <= 9) { lcd.print("0"); } //cek kondisi jika fungsi "WaktuClient.getSeconds" memiliki value lebih dari sama dengan 9 maka lcd akan menghasilkan output "angka 0"
  lcd.print(WaktuClient.getSeconds()); //menghasilkan output "detik" yang akan berubah setiap 1 detik 

  delay(1000); //delay 1 detik untuk mengupdate kode diatas ⬆️
}



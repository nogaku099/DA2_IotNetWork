#include "DHT.h"
#define DHTPIN 12     // Chân pin cảm biến đang cắm
int ledChin = 9;
#define DHTTYPE DHT11   // DHT 11 
DHT dht(DHTPIN, DHTTYPE);
String ketQuaDo="";
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 
  dht.begin();
  pinMode(ledChin, OUTPUT);// Led nháy cảnh báo màu đỏ
}

void loop() {
 ketQuaDo = docDuLieu();
 Serial.println(ketQuaDo);
}

String docDuLieu()  {
  //Bắt đầu đọc
  float doAm = dht.readHumidity();
  float nhietDo = dht.readTemperature();

  digitalWrite(ledChin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(ledChin, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
 
  char buffer[6]; // Tạo buffer để ép kiểu dữ liệu từ float thành string
  String stringNhietDo = dtostrf(nhietDo, 1, 2, buffer);
  String stringDoAm = dtostrf(doAm, 1, 2, buffer);
  String ketQua=stringNhietDo+";"+stringDoAm;
  
  if (isnan(nhietDo) || isnan(doAm)) {
    Serial.println("Khong the doc du lieu");
  } 
  return ketQua;
}

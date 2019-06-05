#include "DHT.h"
#define DHTPIN 12     // Chân pin cảm biến đang cắm

int ledChin = 9;
int ledTam = 8;
int sensorPin = A0;// chân analog kết nối tới cảm biến LM35
#define DHTTYPE DHT11   // DHT 11 
DHT dht(DHTPIN, DHTTYPE);
String lenh="";
String ketQuaDo="";
void setup() {
  Serial.begin(115200); 
  dht.begin();
  pinMode(ledChin, OUTPUT); // Led nháy lúc đọc dữ liệu màu xanh lá
  pinMode(ledTam, OUTPUT);// Led nháy cảnh báo màu đỏ
}

String docDuLieu()  {
  //Bắt đầu đọc
  float doAm = dht.readHumidity();
  float nhietDo = dht.readTemperature();

  digitalWrite(ledChin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(ledChin, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second

  //Đọc LM35
  float nhietDo35 = analogRead(sensorPin) * 3.3 / 1024.0 * 100.0 - 3 ; 
  
 
  char buffer[6]; // Tạo buffer để ép kiểu dữ liệu từ float thành string
  String stringNhietDo = dtostrf(nhietDo, 1, 2, buffer);
  String stringDoAm = dtostrf(doAm, 1, 2, buffer);
  String stringNhietDo35 = dtostrf(nhietDo35, 1, 2, buffer);
  String ketQua=stringNhietDo+";"+stringDoAm+";"+stringNhietDo35;
  
  if (isnan(nhietDo) || isnan(doAm)) {
    Serial.println("Khong the doc du lieu");
  } 
  return ketQua;
}
void docLenhTuRasp()  {
    //Bắt đầu nhận lệnh
    
    //Nếu có
    if(Serial.available()) {
      lenh=Serial.readStringUntil('\n');
      String state = "";
      for(int i = 0; i <= lenh.length();i++) {
          if(lenh.charAt(i)== 'a' )  { // Bật đèn chân 8
            state = "1";  
            break;
          }else if(lenh.charAt(i)== 'b') { // Tắt đèn chân 8
             state = "0";  
              break;
           }else if(lenh.charAt(i) == 'c') { // Nháy đèn chân 9
              state = "2";
              break;
            }else if(lenh.charAt(i) == 'd') { // Thực hiện đo
              state ="3";
              break;  
            }
          
      }

        //Xử lý lệnh
      if(state.equals("1")) {
        digitalWrite(ledTam, HIGH);
      }
      else if(state.equals("0")) {
        digitalWrite(ledTam, LOW);  
      }
      else if(state.equals("2")) {
        digitalWrite(ledChin, HIGH);
        delay(1000);
        digitalWrite(ledChin, LOW);  
      }
      else if(state.equals("3")) {
        ketQuaDo = docDuLieu();
        guiDuLieuLenRasp(ketQuaDo);  
      }
}
}

void guiDuLieuLenRasp(String duLieu) {
  
   Serial.println(duLieu);
}
void loop() {
  int count =0;
  ketQuaDo = docDuLieu();
  while(count <10){
    count ++;
    delay(1000);
    docLenhTuRasp();  
  }
  guiDuLieuLenRasp(ketQuaDo);
}

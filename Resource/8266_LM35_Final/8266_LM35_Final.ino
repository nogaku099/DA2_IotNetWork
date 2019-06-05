#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiUdp.h>
ESP8266WiFiMulti WiFiMulti;

//Khởi tạo các giá trị mặc định
int sensorPin = A0;// chân analog kết nối tới cảm biến LM35
int led = 14;
long time_ThucHienHam = 0;
WiFiUDP UDPServer;
WiFiUDP UDPClient;
unsigned int UDPPort = 8899;
const int packetSize = 2;
byte packetBuffer[packetSize];
String ketQuaDo="";

void setup() {
    Serial.begin(115200);
    pinMode(led, OUTPUT); // Pin led
    pinMode(LED_BUILTIN, OUTPUT);// pin led builtin
    
    // Bắt đầu kết nối wifi
    const char * ssid = "Phat1";       // Tên wifi
    const char * password = "ThuBamtu1den86!"; // mật khẩu wifi
   
    Serial.println();
    Serial.print("Wait for WiFi... ");
    WiFiMulti.addAP(ssid, password);
    //Set IP tĩnh
    //WiFi.config(IPAddress(192, 168, 1, 220), IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));
    //Kiểm tra xem đã kết nối được hay chưa, nếu chưa thì delay và kết nối lại
    while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP()); // In ra địa chỉ IP của ESP8266
    delay(500);

    UDPServer.begin(UDPPort); //Khởi chạy UDP Server
}
void loop() {

ketQuaDo=docNhietDo();
Serial.println(ketQuaDo);
time_ThucHienHam = millis();
handleUDPServer();
//Serial.println(millis()-time_ThucHienHam);  //Cho hiện thời gian thực hiện
delay(1000);
handleUDPClient(ketQuaDo); //Gọi hàm client UDP để gửi dữ liệu lên Node-red trên Raspberry
}
String docNhietDo(){

  //Mỗi khi đọc nhiệt độ sẽ nháy led builtin
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second

  /*đọc giá trị từ cảm biến LM35
  tính ra giá trị hiệu điện thế (đơn vị Volt) từ giá trị cảm biến
  vì vậy nếu biến voltage là biến lưu hiệu điện thế (đơn vị Volt)
  thì ta chỉ việc nhân voltage cho 100 là ra được nhiệt độ!
  do cảm biến bị chênh lệch (so với DHT11) nên cần trừ hao 3 độ
  */
  float nhietDo = analogRead(sensorPin) * 3.3 / 1024.0 * 100.0 - 3 ; 
  char buffer[6]; // Tạo buffer để ép kiểu dữ liệu từ float thành string
  String ketQua = dtostrf(nhietDo, 1, 2, buffer);
  return ketQua;
}

void handleUDPServer() {

int cb = UDPServer.parsePacket();
  if (cb) {
    UDPServer.read(packetBuffer, packetSize); //Gọi packet đọc từ Raspberry gửi qua
 
    String myData = "";
    long timeprocess = 0;
    timeprocess = millis();
    for(int i = 0; i < packetSize; i++) {
      myData += (char)packetBuffer[i];
    }
    //Serial.println(millis()-timeprocess);
    Serial.println(myData);

    //Xử lý các lệnh từ Raspberry ở đây
    if(myData.charAt(0) == '1'){
        digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(1000);                       // wait for a second
        digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
        delay(1000);                       // wait for a second
     }
    myData="";
    Serial.println("OK");

  }
}

void handleUDPClient(String ketQuaDo){
    const char ip[]="192.168.1.16"; // Địa chỉ IP của Raspberry
    //Ép kiểu kết quả đo từ string sang buffer(dành cho packet) để gửi sang Raspberry
    char buf[100];
    unsigned int len = 24;
    int ketQuaDo_len = ketQuaDo.length() + 1; 
    char replyPacket[ketQuaDo_len]; //Nội dung kết quả đo
    ketQuaDo.toCharArray(replyPacket, ketQuaDo_len);
    //Bắt đầu quá trình tạo và gửi packet sang Raspberry
    UDPClient.beginPacket(ip,UDPPort);
    UDPClient.write(replyPacket);
    UDPClient.endPacket(); //Kết thúc gửi

}


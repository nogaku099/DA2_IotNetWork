int sensorPin = A0;// chân analog kết nối tới cảm biến LM35
//int ledBlink = 12;
String ketQuaDo="";
void setup() {
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);// pin led builtin

}

void loop() {
  ketQuaDo=docNhietDo();
  Serial.println(ketQuaDo);

}

String docNhietDo(){


  //Mỗi khi đọc nhiệt độ sẽ nháy led builtin
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW
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

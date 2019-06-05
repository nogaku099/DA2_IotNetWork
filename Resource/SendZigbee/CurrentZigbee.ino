int currentPin = A0; //Pin of sensor.
int analogValue = 0; //0<x<1023
int ACSoffset = 2500;
int mVperAmp = 185; //180-190mV/A.
double Voltage = 0;
double currentValue = 0.0; //Value -5<x<5
char mess[]="abc"
void setup() {
  Serial.begin(38400);
}

void loop() {
  currentValueRead ();
//  Serial.print("cur,");
//  Serial.print(currentValue,3); //show 3 digit after the dot.
//  Serial.println(",");
  mess[] = Serial.read();
  Serial.print(mess);

  delay(1000);  
}

void currentValueRead () {
  // read the value from the sensor:
  analogValue = analogRead(currentPin);
  // Calculate current.
  Voltage = (5000.0*analogValue)/1024.0; //mV
  currentValue = ((Voltage - ACSoffset) / mVperAmp);
}

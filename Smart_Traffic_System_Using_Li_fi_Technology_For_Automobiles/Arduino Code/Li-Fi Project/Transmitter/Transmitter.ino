#include <DHT.h>
#define vibrationSensor A4
#define LED 3
#define GasSensor 4
#define DHTPIN 5     // what pin we're connected to
#define flameSensor 8
#define alcoholSensor 9
#define trigPin 10
#define echoPin 11
#define IRSensor A5



// Uncomment the type of sensor in use:
#define DHTTYPE DHT11   // DHT 11

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);


void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(vibrationSensor,INPUT);
  pinMode(GasSensor,INPUT);
  pinMode(flameSensor,INPUT);
  pinMode(alcoholSensor,INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(IRSensor, INPUT);
  dht.begin();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("LOOP STart");
  //char c = 'E';
  //displayValue(c);
  //transmitData("G95");
  //Serial.println(getUltraSonicData());
  //Serial.println(getFlameSensorData());
  //Serial.println(getAlcoholSensorData());
  //Serial.println(getVibrationSensorData());
  //Serial.println(getHumiditySensorData());
  //Serial.println(getTemperatureSensorData());
  //Serial.println(getGasSensorData());
  //Serial.println(getIRSensorData());
  //B95AB89G95ABD7F89G9
  //Serial.println(getTransmitData());
  
  String data = getTransmitData();
  Serial.println(data);
  transmitData(data);

  //delay(1000);
  //Serial.println();
}
//************************************************************************************Algorithm********************************************************************************

String getTransmitData(){
  String transmitData = ""+getFlameSensorData()+getAlcoholSensorData()+getIRSensorData()+getUltraSonicData()+getVibrationSensorData()+getHumiditySensorData()+getTemperatureSensorData()+getGasSensorData();
  return transmitData;
}


//***********************************************************************************Li-Fi Transmission*************************************************************************

bool transmitData(String inputString){
  for (int i = 0; i < inputString.length(); i++) {
  displayValue(inputString.charAt(i));
  delay(50);
  }
}

void displayValue(char c){
  if(c == 'A'){
    digitalWrite(LED, LOW);
    delay(100);
  }
  else if(c == 'B'){
    digitalWrite(LED, LOW);
    delay(200);
  }
  else if(c == 'C'){
    digitalWrite(LED, LOW);
    delay(300);
  }
  else if(c == 'D'){
    digitalWrite(LED, LOW);
    delay(400);
  }
  else if(c == 'E'){
    digitalWrite(LED, LOW);
    delay(500);
  }
  else if(c == 'F'){
    digitalWrite(LED, LOW);
    delay(600);
  }
  else if(c == 'G'){
    digitalWrite(LED, LOW);
    delay(700);
  }
  else if(c == 'H'){
    digitalWrite(LED, LOW);
    delay(800);
  }
  else if(c == '0'){
    digitalWrite(LED, LOW);
    delay(900);
  }
  else if(c == '1'){
    digitalWrite(LED, LOW);
    delay(1000);
  }
  else if(c == '2'){
    digitalWrite(LED, LOW);
    delay(1100);
  }
  else if(c == '3'){
    digitalWrite(LED, LOW);
    delay(1200);
  }
  else if(c == '4'){
    digitalWrite(LED, LOW);
    delay(1300);
  }
  else if(c == '5'){
    digitalWrite(LED, LOW);
    delay(1400);
  }
  else if(c == '6'){
    digitalWrite(LED, LOW);
    delay(1500);
  }
  else if(c == '7'){
    digitalWrite(LED, LOW);
    delay(1600);
  }
  else if(c == '8'){
    digitalWrite(LED, LOW);
    delay(1700);
  }
  else if(c == '9'){
    digitalWrite(LED, LOW);
    delay(1800);
  }
  digitalWrite(LED, HIGH);
}
//***************************************************************************Flame Sensor***********************************************************************
String getFlameSensorData(){
  if(digitalRead(flameSensor)==1){
    return "A1";
  }
  return "A0";
}

//***************************************************************************Alcohol Sensor*********************************************************************
String getAlcoholSensorData(){
  if(digitalRead(alcoholSensor)==0){
    return "B1";
  }
  return "B0";
}

//***************************************************************************IR Sensor**************************************************************************

String getIRSensorData(){
  //Serial.println(analogRead(IRSensor));
  if(analogRead(IRSensor)<=400){
    return "C1";
  }
  return "C0"; 
}

//***************************************************************************Ultrasonic Sensor******************************************************************
String getUltraSonicData(){
  String usData="D";
  int cm = readUltraSonicSensor();
  if(cm>=2&&cm<10){
    return usData= usData+"0"+String(cm);
  }
  else if(cm>=10&&cm<100){
    return usData+=String(cm);
  }
  return usData+="00";
}

long readUltraSonicSensor(){
  long duration, inches, cm;
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  delay(100);
  //Serial.println(cm);
  return cm;
}

long microsecondsToInches(long microseconds) {
   return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}

//***************************************************************************Vibration Sensor******************************************************************
String getVibrationSensorData(){
  //Serial.println(analogRead(vibrationSensor));
  if(analogRead(vibrationSensor)>110){
    return "E1";
  }
  return "E0";
}
//***************************************************************************Humidity Sensor********************************************************************

String getHumiditySensorData(){
  float humidity = dht.readHumidity();     // Read humidity
  return "F"+String(int(humidity));
}
//***************************************************************************Temperature Sensor*****************************************************************
String getTemperatureSensorData(){
  float temperature = dht.readTemperature();  // Read temperature
  return "G"+String(int(temperature));
}

//***************************************************************************Gas Sensor************************************************************************
String getGasSensorData(){
  if(digitalRead(GasSensor)==0){
    return "H1";
  }
  return "H0";
}
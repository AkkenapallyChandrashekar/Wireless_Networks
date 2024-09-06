#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define LDR 9


LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x27 for a 16 chars and 2 line display
unsigned long startTime;
unsigned long currentTime;
unsigned long elapsedTime;

bool fireState;
bool alcoholState;
bool IRState;
int distance;
bool vibrationState;
int humidity;
int temperature;
bool gasState;
bool properState = false;

String lcdMessages[8];
String displayData;

void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 baud
  pinMode(LDR,INPUT);
  lcd.init();                      // initialize the lcd 
  lcd.backlight();                 // turn on backlight
}

void loop() {
  //String val = getLDRData();
  //unsigned long duration = pulseIn(LDR, 0); // Measure the pulse length
  //Serial.println("Pulse duration: ");
  //Serial.print(duration);
  Serial.println("loopstart");
  String data = transformData(getLDRData());
  //String data = "H5GB0C030E0F5B0AE3F";
  Serial.println("Data:"+data);
  //displayLCD(data);
  getRecievedData(data);
  Serial.println("Display data:"+displayData);
  /*if(displayData.length()>0){
    displayLCD(displayData);
    delay(1000);
  }*/
  displayLCDMessages();
  //delay(1000);
  //displayLCD("");
}

//************************************************************************Recieved Data Processing**************************************************************************************

void getRecievedData(String data){
  int value=65,count=0,index;
  String singleBit="ABCEH";
  String doubleBit= "DFG";
  displayData="";
  while(value<=72&&count<=8){
    index = data.indexOf(value);
    /*Serial.print("Value:"+value);
    Serial.print(value);
    Serial.print(data.charAt(index));
    Serial.println(index);
    Serial.println();
    Serial.print("Count:_");
    Serial.println(count);*/
    if(index!=-1){
      if(singleBit.indexOf(value)!=-1){
        getSingleBitData(index,data);
      }
      else if(doubleBit.indexOf(value)!=-1){
        getDoubleBitData(index,data);
      }
      count++;
    }
    value++;
    //delay(2000);
  }
  if(count==8){
    properState = true;
  }
}


void getSingleBitData(int index, String data){
  if((index!=data.length()-1)&&(data.charAt(index+1)=='1'))  {
    displayData= displayData+" "+data.charAt(index)+":1";
    setLCDMessage(index,0,data);
  }
  else if((index==data.length()-1)&&(data.charAt(0)=='1')){
    displayData= displayData+" "+data.charAt(0)+":1";
    setLCDMessage(index,0,data);
  }
  //Serial.println("SingleBitDisplayData:"+displayData);
}

void getDoubleBitData(int index, String data){
  if(index==data.length()-1 && checkDigit(0,1,data)){
    setDoubleValue(data,0,1,index);
  }
  else if(index==data.length()-2 && checkDigit(index+1,0,data)){
    setDoubleValue(data,index+1,0,index);
  }
  else if(index!=data.length()-1 && index!=data.length()-2 && checkDigit(index+1,index+2,data)){
    setDoubleValue(data,index+1,index+2,index);
  }
}

void setDoubleValue(String data, int index1, int index2, int index){
  String value;
  value.concat(data.charAt(index1));
  value.concat(data.charAt(index2));
  displayData = displayData+" "+data.charAt(index)+":"+value;
  int val = value.toInt();
  setLCDMessage(index,val,data);
  //Serial.println("DoubleBit Display Data:"+displayData);
}

bool checkDigit(int index1,int index2, String data){
  if((data.charAt(index1)>= 48 && data.charAt(index1)<= 57)&&(data.charAt(index2)>= 48 && data.charAt(index2)<= 57)){
    return true;
  }
  return false;
}

//***********************************************************************LDR Sensor & Li-Fi Reciever************************************************************************************

String getLDRData(){
  String data="";
  unsigned long delay;
  char c;
  while(data.length()<= 18){
    if(startPoint()){
      startTime = millis();
    }
    delay = getLDRDelay();
    c=getChar(delay);
    if(c!='#'){
      data+=c;
    }
  }
  return data;
}

String transformData(String data){
  char c;
  while(isdigit(data.charAt(0))){
    c = data.charAt(0);
    data.remove(0,1);
    data+=c;
  }
  return data;
}

long getLDRDelay(){
  if(digitalRead(LDR)==0){
    currentTime = millis();
    elapsedTime = currentTime-startTime;
    /*if(elapsedTime!=0){
      Serial.println(elapsedTime);
    }*/
    return elapsedTime;
  }
  else{
    getLDRDelay();
  }
}

bool startPoint(){
  if(digitalRead(LDR)==1){
    return true;
  }
  else{
    startPoint();
  }
}

char getChar(unsigned long delay){
  if(delay>50 && delay<110){
    return 'A';
  }
  else if(delay>150 && delay<210){
    return 'B';
  }
  else if(delay>250 && delay<310){
    return 'C';
  }
  else if(delay>350 && delay<410){
    return 'D';
  }
  else if(delay>450 && delay<510){
    return 'E';
  }
  else if(delay>550 && delay<610){
    return 'F';
  }
  else if(delay>650 && delay<710){
    return 'G';
  }
  else if(delay>750 && delay<810){
    return 'H';
  }
  else if(delay>850 && delay<910){
    return '0';
  }
  else if(delay>950 && delay<1010){
    return '1';
  }
  else if(delay>1050 && delay<1110){
    return '2';
  }
  else if(delay>1150 && delay<1210){
    return '3';
  }
  else if(delay>1250 && delay<1310){
    return '4';
  }
  else if(delay>1350 && delay<1410){
    return '5';
  }
  else if(delay>1450 && delay<1510){
    return '6';
  }
  else if(delay>1510 && delay<1610){
    return '7';
  }
  else if(delay>1650 && delay<1710){
    return '8';
  }
  else if(delay>1750 && delay<1810){
    return '9';
  }
  else{
    return '#';
  }
}
//**************************************************************************Generate LCD Messages**********************************************************************************
void displayLCDMessages(){
  for(int i=0;i<8; i++){
    if(lcdMessages[i].length()>0){
      Serial.println(lcdMessages[i]);
      displayLCD(lcdMessages[i]);
      lcdMessages[i]="\0";
      //Serial.print(i);
    }
  }
}

void setLCDMessage(int index, int val,String data){
  if(data.charAt(index)=='A'){
    lcdMessages[0]="Fire Hazzard in Opp Vehicle";
  }
  else if(data.charAt(index)=='B'){
    lcdMessages[1]="Opp Driver is Drunk";
  }
  else if(data.charAt(index)=='C'){
    lcdMessages[2]="Opp Driver is Sleeping";
  }
  else if(data.charAt(index)=='D' && val>1 && val<30){
    lcdMessages[3] = "A object infront of opp vehicle";
  }
  else if(data.charAt(index)=='E'){
    lcdMessages[4]="Mech Issue in Opp Vech";
  }
  else if(data.charAt(index)=='F' && val>50){
    lcdMessages[5]="High Humid in opp vech";
  }
  else if(data.charAt(index)=='G' && val>30){
    lcdMessages[6]="High temp in opp vech";
  }
  else if(data.charAt(index)=='H'){
    lcdMessages[7]="Gas Leakage in Opp Vech";
  }
}

//**********************************************************************************LCD Display*************************************************************************************

void displayLCD(String data){
  String firstLine = getFirstLine(data);
  String secondLine = getSecondLine(data);
  lcd.setCursor(0,0);              // set the cursor to the first column and first row
  lcd.print(firstLine); 
  lcd.setCursor(0,1);
  lcd.print(secondLine);
  delay(2000);
  lcd.clear();
  delay(1000);
}

String getFirstLine(String data){
  String firstLine="";
  for (int i = 0; i < 16; i++) {
    firstLine += data.charAt(i);
  }
  return firstLine;
}

String getSecondLine(String data){
  String secondLine="";
  for (int i = 16; i < data.length(); i++) {
    secondLine += data.charAt(i);
  }
  return secondLine;
}

#include <LiquidCrystal.h>
int rs=7;
int en=8;
int d4=9;
int d5=10;
int d6=11;
int d7=12;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);
int buzzPin=4;
int redPin=5;
int greenPin=6;
int dt1=1;
int dt2=2;
int j;

char pressureString[10];
char fullText[20];
void setup() {
  // put your setup code here, to run once:
pinMode(2, INPUT);   // Connect HX710 OUT to Arduino pin 2
pinMode(3, OUTPUT);  // Connect HX710 SCK to Arduino pin 3
pinMode(13,OUTPUT);//output pin for relay board, this will sent signal to the relay
lcd.begin(16,2);
pinMode(buzzPin, OUTPUT);
pinMode(redPin, OUTPUT);
pinMode(greenPin, OUTPUT);
Serial.begin(9600);
}

void loop() {

  digitalWrite(redPin, LOW);
  float pressureResult =readPressure();
  Serial.println(pressureResult,6);
  delay(5);
 
while (pressureResult>0.0082){
  digitalWrite(13,LOW); // low is to cut the relay
  industryAlert();
  digitalWrite(greenPin, LOW);
  blinkLed();
  dtostrf(pressureResult, 6, 5, pressureString);
  sprintf(fullText, " Pression: %s bar", pressureString);
  scrollText(fullText, 803);
  
  pressureResult =readPressure();
}
  digitalWrite(13,HIGH); //high to continue proving signal and water supply
  lcd.setCursor(0,0);
  lcd.print("Pression :");
  lcd.setCursor(0,1);
  lcd.print(pressureResult,6);
  lcd.print(" bar");
  delay(300);
 lcd.clear();
 digitalWrite(greenPin, HIGH);


}
void blinkLed(){
digitalWrite(redPin, HIGH);
}
void industryAlert() {
  for (j=1;j<=100;j=j+1){
    digitalWrite(buzzPin, HIGH);
    delay(dt1);
    digitalWrite(buzzPin, LOW);
    delay(dt1);
  }
  for (j=1; j<=100;j=j+1){
    digitalWrite(buzzPin, HIGH);
    delay(dt2);
    digitalWrite(buzzPin, LOW);
    delay(dt2);
  }
}
//fonction to read presure

float readPressure() {
  char formattedValue[8];
  // wait for the current reading to finish
  while (digitalRead(2)) {}

  // read 24 bits
  long result = 0;
  for (int i = 0; i < 24; i++) {
    digitalWrite(3, HIGH);
    digitalWrite(3, LOW);
    result = result << 1;
    if (digitalRead(2)) {
      result++;
    }
  }

  // get the 2's complement
  result = result ^ 0x800000;

  // pulse the clock line 3 times to start the next pressure reading
  for (char i = 0; i < 3; i++) {
    digitalWrite(3, HIGH);
    digitalWrite(3, LOW);
  }
  
  float pressure = (result /100000000.0)/14.504 ;
  

  return pressure; // Return the result
}

void scrollText(const char* text, int delayTime) {
  int textLength = strlen(text);
  
  // Display the scrolling part of the text
  for (int i = 0; i <= textLength; i++) {
    //if(i==0)delay(3000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(text+i);
    lcd.setCursor(0,1);
    lcd.print("tank is full");
    
    delay(delayTime);
    if(i==8){
      i=0;

    }
  }
}

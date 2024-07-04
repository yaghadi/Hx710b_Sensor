void setup() {
  // put your setup code here, to run once:
  pinMode(2, INPUT);   // Connect HX710 OUT to Arduino pin 2
  pinMode(3, OUTPUT);  // Connect HX710 SCK to Arduino pin 3
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  float pressureResult =readPressure();
  Serial.print("Pression :");
  Serial.print(",");
  Serial.println(pressureResult,6);
}

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

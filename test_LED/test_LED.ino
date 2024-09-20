int red = 13;
int yellow = 12;
int green = 2;

void setup() {
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);
  // pinMode(greenLED, OUTPUT);
}

void loop() {
  digitalWrite(red, HIGH);
  digitalWrite(green, HIGH);
  digitalWrite(yellow, HIGH);
  delay(3000);
     
  digitalWrite(red, LOW);   
  delay(1000); 
    
  // digitalWrite(greenLED, HIGH); 
  // delay(1000);
    
  // digitalWrite(greenLED, LOW);   
  // delay(1000);   
}
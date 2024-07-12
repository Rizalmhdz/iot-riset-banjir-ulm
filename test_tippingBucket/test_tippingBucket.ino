
#define REED_SWITCH 15           // Reed Switch pin
int totalTip = 0; //variable to store total tip for 1 hour
int prevButtonState = LOW; // set begining state for NO Reedswitch

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) {
    delay(10); // hang out until serial port opens
  } 
  
  pinMode(REED_SWITCH, INPUT_PULLUP);

}

void loop() {
  // put your main code here, to run repeatedly:
  for (;;) {
      tipCounter();
    
   }

}


void tipCounter(){
  int buttonState = digitalRead(REED_SWITCH);
  
  if(buttonState == LOW && prevButtonState == HIGH) // 
  { 
      prevButtonState = LOW;
      totalTip++;
  }

  if(buttonState == HIGH && prevButtonState == LOW) { //reset for next bucket tip or send a secondary virtWrite or slow serial commands
       
       prevButtonState = HIGH;
       Serial.print("total tip : ");
       Serial.println(totalTip);
  }
//   wait a 2 seconds between readings
  delay(10L);
}

void resetCounter(){
  totalTip = 0;
  Serial.println("\nRESETING COUNTER\n");
}

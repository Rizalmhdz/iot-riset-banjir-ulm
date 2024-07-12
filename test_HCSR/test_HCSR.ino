#define JARAK_SENSOR 200//jarak sensor terhadap dasar tanah
#define UltrasonicCalibratedValue 4.24
#define triggerPinHC 32
#define echoPinHC 33

// ============Variabel HC-SR04===========
float durationHC;
float jarakSensorDanAirHCSR04;
float tinggiAirHCSR04;


void setup() {
  // put your setup code here, to run once:  
  Serial.begin(9600);
  while (!Serial) {
    delay(10); // hang out until serial port opens
  }   
  pinMode(triggerPinHC, OUTPUT);
  pinMode(echoPinHC, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  hcsr04();
  delay(2000);
}

void hcsr04() {
  Serial.println("========Pengukuran Sensor HC-SR04========");
  delay(2000);
  digitalWrite(triggerPinHC, LOW);   //Set triggerPin low
  delayMicroseconds(2);       // delay 2 micro second

  digitalWrite(triggerPinHC, HIGH);  // Set triggerPin high
  delayMicroseconds(10);      // triggerPin high selama 10 micro seconds
  digitalWrite(triggerPinHC, LOW);   // triggerPinPin low

  durationHC = pulseIn(echoPinHC, HIGH);   //Pembacaan Echo pin dan durasi
  jarakSensorDanAirHCSR04 = durationHC * 0.034 / 2;
  tinggiAirHCSR04 = JARAK_SENSOR - jarakSensorDanAirHCSR04;
  
  Serial.print("Pengukuran Sensor HC-SR04: ");
  Serial.println(jarakSensorDanAirHCSR04);
  Serial.print("Tinggi Air oleh Sensor HC-SR04:");
  Serial.print(tinggiAirHCSR04);
  Serial.println(" cm");
}



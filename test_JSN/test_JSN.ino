#define triggerPin 26 // JSN
#define echoPin 25 // JSN
#define UltrasonicCalibratedValue 4.24

#define JARAK_SENSOR 200//jarak sensor terhadap dasar tanah

float duration; //waktu tempuh sensor ultrasonik
float jarak; //jarak antara sensor dengan permukaan air
float tinggiAir;

void setup() {
  // put your setup code here, to run once: Serial.begin(9600);
  Serial.begin(9600);
  while (!Serial) {
    delay(10); // hang out until serial port opens
  } 
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  ultrasonicSensor();
  delay(2000);
}

void ultrasonicSensor() {
  digitalWrite(triggerPin, LOW);   //Set triggerPin low
  delayMicroseconds(2);       // delay 2 micro second

  digitalWrite(triggerPin, HIGH);  // Set triggerPin high
  delayMicroseconds(10);      // triggerPin high selama 10 micro seconds
  digitalWrite(triggerPin, LOW);   // triggerPinPin low

  duration = pulseIn(echoPin, HIGH);   //Pembacaan Echo pin dan durasi
  float jarakAwal = duration * 0.034 / 2;
  jarak = jarakAwal + UltrasonicCalibratedValue;   ///Menghitung jarak yg sebenarnya
  tinggiAir = JARAK_SENSOR - jarak - UltrasonicCalibratedValue;
  
  Serial.print("Pengukuran Sensor: ");
  Serial.println(jarak);
  Serial.print("Tinggi Air:");
  Serial.print(tinggiAir);
  Serial.println(" cm");
}
// ================ Libraries ================ 
#include <DHT.h>
#include <AM2320.h>
#include <Wire.h>
/// tambahan RTC + SD CARD
#include "RTClib.h"
// Libraries for SD card
#include "FS.h"
#include "SD.h"
#include <SPI.h>


//================ Define constant variable ================
// ================= PIN MICRO SD CARD ADAPTER
// MISO 19
// MOSI 23
// SCK / CLK 18
// CS 5
#define DHTPIN 4                // DHT22 Pin
#define DHTTYPE DHT22           // DHT22 (AM2302)
#define REED_SWITCH 15           // Reed Switch pin
#define RESET (60 * 60 * 1000L)// Reset tip counter every 1hour
#define INTERVAL (5 * 60 * 1000L)//uploading data to cloud every 1hour
#define JARAK_SENSOR 200//jarak sensor terhadap dasar tanah
#define triggerPin 26 // JSN
#define echoPin 25 // JSN
#define RhCorrectionAM2320 6.5
#define TCorrectionAM2320 0.3
#define RhCorrectionDHT22 6.2
#define TCorrectionDHT22 0.4
#define UltrasonicCalibratedValue 4.24
#define triggerPinHC 32
#define echoPinHC 33
#define SCL1_PIN 21 // RTC
#define SDA1_PIN 22 // RTC
#define SCL2_PIN 27 // AM2320
#define SDA2_PIN 14 // AM2320

// ====== LED PIN
int red = 13;
int yellow = 12;
int green = 2;

// ================ Initialize sensor ================
TwoWire I2C_1 = TwoWire(0);
TwoWire I2C_2 = TwoWire(1);
DHT dht_sensor(DHTPIN, DHTTYPE); // Initialize DHT sensor for normal 16mhz Arduino
AM2320 am_sensor(&I2C_2);

RTC_DS3231 rtc; // Modul Realtime Clock

// ================ set global variable ================
int totalTip = 0; //variable to store total tip for 1 hour
int prevButtonState = LOW; // set begining state for NO Reedswitch
unsigned long last_time = 0L;
unsigned long last_time_reset = 0L; 
float duration; //waktu tempuh sensor ultrasonik
float jarak; //jarak antara sensor dengan permukaan air
float tinggiAir;
float humAM2320;
float tempAM2320;
float humDHT22;
float tempDHT22;
float start;
String mins = "";

// ============Variabel HC-SR04===========
float durationHC;
float jarakSensorDanAirHCSR04;
float tinggiAirHCSR04;

// =============== Initialize task ====================
TaskHandle_t Task1, Task2;


// =================================== SETUP ===================================
void setup(){
  Serial.begin(9600);
  while (!Serial) {
    delay(10); // hang out until serial port opens
  } 
  Wire.begin();
  dht_sensor.begin();
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(REED_SWITCH, INPUT_PULLUP);
  pinMode(triggerPinHC, OUTPUT);
  pinMode(echoPinHC, INPUT);

   pinMode(red, OUTPUT);
   pinMode(green, OUTPUT);
   pinMode(yellow, OUTPUT);

  while (!Serial) {
    delay(1);
  }

  I2C_2.begin(SDA2_PIN, SCL2_PIN, 100000ul);
  // ================= MICRO SD ADAPTER SETUP ==================

  initSDCard();
  
  
  // ====== END BLOCK MICRO SD ADAPTER SETUP ===================
  

  // LOGIC SETUP RTC ==============================

#ifndef ESP8266
  while (!Serial); // wait for serial port to connect. Needed for native USB
#endif

  if (! rtc.begin()) {
     for (int loopCount = 0; loopCount < 6; loopCount++) {
      digitalWrite(red, HIGH); // Turn LED on
      digitalWrite(green, HIGH); // Turn LED on
      delay(250); // Wait for 0.5 seconds
      digitalWrite(red, LOW); // Turn LED off
      digitalWrite(green, LOW); // Turn LED off
      delay(250); // Wait for 0.5 seconds
    }
    Serial.println("Couldn't find RTC");
    delay(5000);
    Serial.println("================== RESTART SYSTEM ===================");
    ESP.restart(); // resrart esp if RTC not available
    delay(5000);
    Serial.flush();
    while (1) delay(10);
   
  } else {
      digitalWrite(red, LOW); // Turn LED on
      digitalWrite(green, HIGH); // Turn LED on
      delay(3000); // Wait for 0.5 seconds
      digitalWrite(red, LOW); // Turn LED off
      digitalWrite(green, LOW); // Turn LED off
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }


    // code untuk menganti jam RTC... uncomment ini (rt.adjucst bla bla bla) dan comment yang rtc.adjuct diatas
    // kemudian set time berdasarkan waktu yang sesuai, kemudian upload.... 
    // setelah waktu sesuai comment lagi code dibawah dan upload ulang
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2024, 7, 28, 22, 29, 0));


  
  DateTime now = rtc.now();
  while(mins == ""){
    mins = String(now.minute());
  }
  Serial.println("Starting minutes at : " + mins);
  start = mins.toFloat();

  // Pisahkan nilai jam, menit
   char jam[3];
  char menit[3];
  sprintf(jam, "%02d", now.hour());
  sprintf(menit, "%02d", now.minute());

  digitalWrite(red, HIGH);
  delay(1000);
  digitalWrite(red, LOW);
  delay(500);

  for (int i = 0; i < String(jam[0]).toInt(); i++) {
    digitalWrite(green, HIGH);
    delay(500);
    digitalWrite(green, LOW);
    delay(500);
  }

  for (int i = 0; i < String(jam[1]).toInt(); i++) {
    digitalWrite(yellow, HIGH);
    delay(500);
    digitalWrite(yellow, LOW);
    delay(500);
  }

  digitalWrite(red, HIGH);
  delay(1000);
  digitalWrite(red, LOW);
  delay(500);

  for (int i = 0; i < String(menit[0]).toInt(); i++) {
    digitalWrite(green, HIGH);
    delay(500);
    digitalWrite(green, LOW);
    delay(500);
  }

  for (int i = 0; i < String(menit[1]).toInt(); i++) {
    digitalWrite(yellow, HIGH);
    delay(500);
    digitalWrite(yellow, LOW);
    delay(500);
  }

  digitalWrite(red, HIGH);
  delay(1000);
  digitalWrite(red, LOW);
  delay(500);


  


  // ============================== END BLOCK RTC ===============================


  //// ================================= TASK SET UP ======================================

  /*Syntax for assigning task to a core:
   xTaskCreatePinnedToCore(
                    coreTask,   // Function to implement the task
                    "coreTask", // Name of the task 
                    10000,      // Stack size in words 
                    NULL,       // Task input parameter 
                    0,          // Priority of the task 
                    NULL,       // Task handle. 
                    taskCore);  // Core where the task should run 
   */ 
   xTaskCreatePinnedToCore(    codeForTask1,    "tipCounter",    5000,      NULL,    2,    &Task1,    0);
   xTaskCreatePinnedToCore(    codeForTask2,    "sensingTemperature",    5000,    NULL,    2,    &Task2,    1);
}

void resetCounter(){
  totalTip = 0;
  Serial.println("\nRESETING COUNTER\n");
  for (int loopCount = 0; loopCount < 6; loopCount++) {
      // Your code to be executed 3 times goes here
      // For example, blinking an LED
      digitalWrite(red, HIGH); // Turn LED on
      digitalWrite(yellow, HIGH); // Turn LED on
      digitalWrite(green, HIGH); // Turn LED on
      delay(250); // Wait for 0.5 seconds
      digitalWrite(red, LOW); // Turn LED off
      digitalWrite(yellow, LOW); // Turn LED off
      digitalWrite(green, LOW); // Turn LED off
      delay(250); // Wait for 0.5 seconds
    }
}

void loop(){
}

void codeForTask1( void * parameter ) {
   for (;;) {
      tipCounter();
    
   }
}


void codeForTask2( void * parameter ) {
   for (;;) {

      if (millis() - last_time >= INTERVAL)  // wrap-around safe test
      {
        Serial.print("                                                            Code is running on Core: ");Serial.println( xPortGetCoreID());
        sensingTemperature();
        ultrasonicSensor();
        hcsr04();
        saveData();
        // request();
        
        last_time += INTERVAL ; // setup for next time
      }

      // if ((start*60*1000L) + millis() - last_time_reset >= RESET)  // wrap-around safe test
      if ((start*60*1000L) + millis() - last_time_reset >= RESET)  // wrap-around safe test
      {
        resetCounter();
        
        last_time_reset += RESET ; // setup for next time
      }
   }
}

void sensingTemperature(){
  dhtSensor();
  amSensor();
}

void amSensor() {
  Serial.println(F("Chip = AM2320"));
  switch(am_sensor.Read()) {
    case 2:
      Serial.println(F("  CRC failed"));
      break;
    case 1:
      Serial.println(F("  Sensor offline"));
      break;
    case 0:
      tempAM2320 = am_sensor.cTemp + TCorrectionAM2320;
      humAM2320 = am_sensor.Humidity - RhCorrectionAM2320;
      Serial.print(F("  Humidity = "));
      Serial.print(humAM2320);
      Serial.println(F("%"));
      Serial.print(F("  Temperature = "));
      Serial.print(tempAM2320);
      Serial.println(F("*C"));
      Serial.println();
      break;
  }  
}

void dhtSensor() {
  tempDHT22 = dht_sensor.readTemperature() - TCorrectionDHT22;
  humDHT22 = dht_sensor.readHumidity() + RhCorrectionDHT22;
  if ( isnan(tempDHT22) || isnan(humDHT22)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.println(F("Chip = DHT22"));
    Serial.print(F("  Humidity = "));
    Serial.print(humDHT22);
    Serial.println(F("%"));  
    Serial.print(F("  Temperature = "));
    Serial.print(tempDHT22);
    Serial.println(F("*C"));
    Serial.println();
  }
}


void tipCounter(){
  int buttonState = digitalRead(REED_SWITCH);
  
  if(buttonState == LOW && prevButtonState == HIGH) // 
  { 
      prevButtonState = LOW;
      totalTip++;
      digitalWrite(red, HIGH); // Turn LED on
      digitalWrite(green, HIGH); // Turn LED on
      delay(250); // Wait for 0.5 seconds
      digitalWrite(red, LOW); // Turn LED off
      digitalWrite(green, LOW); // Turn LED off
      delay(250); // Wait for 0.5 seconds
  }

  if(buttonState == HIGH && prevButtonState == LOW) { //reset for next bucket tip or send a secondary virtWrite or slow serial commands
       
       prevButtonState = HIGH;
       Serial.print("total tip : ");
       Serial.println(totalTip);
  }
//   wait a 2 seconds between readings
  delay(10L);
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

/// ================================ SD CARD LOGIC ===========================

// Initialize SD card
void initSDCard(){
   if (!SD.begin()) {
    Serial.println("Card Mount Failed");
    for (int loopCount = 0; loopCount < 6; loopCount++) {
      // Your code to be executed 3 times goes here
      // For example, blinking an LED
      digitalWrite(red, HIGH); // Turn LED on
      digitalWrite(yellow, HIGH); // Turn LED on
      delay(250); // Wait for 0.5 seconds
      digitalWrite(red, LOW); // Turn LED off
      digitalWrite(yellow, LOW); // Turn LED off
      delay(250); // Wait for 0.5 seconds
    }
    delay(5000);
    Serial.println("================== RESTART SYSTEM ===================");
    ESP.restart(); // resrart esp if RTC not available
    delay(5000);
    Serial.flush();
    return;
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    for (int loopCount = 0; loopCount < 6; loopCount++) {
      // Your code to be executed 3 times goes here
      // For example, blinking an LED
      digitalWrite(red, HIGH); // Turn LED on
      digitalWrite(yellow, HIGH); // Turn LED on
      delay(250); // Wait for 0.5 seconds
      digitalWrite(red, LOW); // Turn LED off
      digitalWrite(yellow, LOW); // Turn LED off
      delay(250); // Wait for 0.5 seconds
    }
    return;
  }
  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
  
  digitalWrite(yellow, HIGH);
  delay(3000);
  digitalWrite(yellow, LOW);
}

// Write to the SD card
void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file) {
    Serial.println("Failed to open file for writing");
    delay(5000);
    Serial.println("================== RESTART SYSTEM ===================");
    ESP.restart(); // resrart esp if RTC not available
    delay(5000);
    Serial.flush();
    return;
  }
  if(file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

// Append data to the SD card
void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file) {
    Serial.println("Failed to open file for appending");
    for (int loopCount = 0; loopCount < 3; loopCount++) {
      digitalWrite(red, HIGH); // Turn LED on
      digitalWrite(yellow, HIGH); // Turn LED on
      delay(250); // Wait for 0.5 seconds
      digitalWrite(red, LOW); // Turn LED off
      digitalWrite(yellow, LOW); // Turn LED off
      delay(250); // Wait for 0.5 seconds
    }
    delay(5000);
    Serial.println("================== RESTART SYSTEM ===================");
    ESP.restart(); // resrart esp if RTC not available
    delay(5000);
    Serial.flush();
    return;
  }
  if(file.print(message)) {
    Serial.println("Message appended");
     for (int loopCount = 0; loopCount < 3; loopCount++) {
      digitalWrite(green, HIGH); // Turn LED on
      digitalWrite(yellow, HIGH); // Turn LED on
      delay(250); // Wait for 0.5 seconds
      digitalWrite(green, LOW); // Turn LED off
      digitalWrite(yellow, LOW); // Turn LED off
      delay(250); // Wait for 0.5 seconds
    }
  } else {
    Serial.println("Append failed");
     for (int loopCount = 0; loopCount < 3; loopCount++) {
      digitalWrite(red, HIGH); // Turn LED on
      digitalWrite(yellow, HIGH); // Turn LED on
      delay(250); // Wait for 0.5 seconds
      digitalWrite(red, LOW); // Turn LED off
      digitalWrite(yellow, LOW); // Turn LED off
      delay(250); // Wait for 0.5 seconds
    }
    delay(5000);
    Serial.println("================== RESTART SYSTEM ===================");
    ESP.restart(); // resrart esp if RTC not available
    delay(5000);
    Serial.flush();
  }
  file.close();
}



// ========================================= END SD CARD LOGIC BLOCK =========================================

// ================= Logic SAVE DATA BACKUP TO MICRO SD ===============================================


void saveData(){
  DateTime now = rtc.now();
  // If the data.txt file doesn't exist
  // Create a file on the SD card and write the data labels
  String file_name = "/"+String(now.year())+"_bulan_"+String(now.month())+".txt";
  const char* file_name1 = file_name.c_str();
  File file = SD.open(file_name1);
  if(!file) {
    Serial.println("File doesn't exist");
    Serial.println("Creating file...");
    writeFile(SD, file_name1, "tanggal, waktu, suhu(AM2320), kelembaban(AM230), suhu(DHT22), kelembaban(DHT22), total_tip, air_sungai(JSN), hasil_perhitungan(JSN), air_sungai(HCSR), hasil_perhitungan(HCSR)\r\n");
  }
  else {
    Serial.println("File already exists");  
  }
  file.close();

  
  String tanggal = now.timestamp(DateTime::TIMESTAMP_DATE);
  String waktu = now.timestamp(DateTime::TIMESTAMP_TIME);

 
  //Concatenate all info separated by commas
  String dataMessage = String(tanggal) + "," + String(waktu) + "," + String(tempAM2320) + "," + String(humAM2320)+ "," + String(tempDHT22)+"," + String(humDHT22)+"," + String(totalTip)+"," + String(tinggiAir)+"," + String(jarak)+"," + String(tinggiAirHCSR04)+"," + String(jarakSensorDanAirHCSR04)+"\r\n";
  Serial.print("Saving data: ");
  Serial.println(dataMessage);

  //Append the data to file
  appendFile(SD, file_name1, dataMessage.c_str());
}
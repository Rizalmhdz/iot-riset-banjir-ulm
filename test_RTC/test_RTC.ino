#include "RTClib.h"
// Libraries for SD card
#include "FS.h"
#include "SD.h"
#include <SPI.h>

#define SCL1_PIN 21 // RTC
#define SDA1_PIN 22 // RTC

float start;
String mins = "";

TwoWire I2C_1 = TwoWire(0);
TwoWire I2C_2 = TwoWire(1);

RTC_DS3231 rtc; // Modul Realtime Clock

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) {
    delay(10); // hang out until serial port opens
  } 
  // ================= MICRO SD ADAPTER SETUP ==================

  initSDCard();

   // LOGIC SETUP RTC ==============================

  #ifndef ESP8266
    while (!Serial); // wait for serial port to connect. Needed for native USB
  #endif

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
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

    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));


  
  DateTime now = rtc.now();
  while(mins == ""){
    mins = String(now.minute());
  }
  Serial.println("Starting minutes at : " + mins);
  start = mins.toFloat();

  // ============================== END BLOCK RTC ===============================


}

void loop() {
  // put your main code here, to run repeatedly:
  saveData();
  delay(5000);

}


/// ================================ SD CARD LOGIC ===========================

// Initialize SD card
void initSDCard(){
   if (!SD.begin()) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
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
}

// Write to the SD card
void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file) {
    Serial.println("Failed to open file for writing");
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
    return;
  }
  if(file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
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
  // String dataMessage = String(tanggal) + "," + String(waktu) + "," + String(tempAM2320) + "," + String(humAM2320)+ "," + String(tempDHT22)+"," + String(humDHT22)+"," + String(totalTip)+"," + String(tinggiAir)+"," + String(jarak)+"," + String(tinggiAirHCSR04)+"," + String(jarakSensorDanAirHCSR04)+"\r\n";
  String dataMessage = String(tanggal) + "," + String(waktu);
  Serial.print("Saving data: ");
  Serial.println(dataMessage);

  //Append the data to file
  appendFile(SD, file_name1, dataMessage.c_str());
}

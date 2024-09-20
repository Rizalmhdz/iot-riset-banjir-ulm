#include "RTClib.h"

#define SCL1_PIN 21 // RTC
#define SDA1_PIN 22 // RTC

int red = 13;
int yellow = 12;
int green = 2;

RTC_DS3231 rtc;

float start;
String mins = "";

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    delay(10);
  } 
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);

  while (!Serial) {
    delay(1);
  }

#ifndef ESP8266
  while (!Serial);
#endif

  if (!rtc.begin()) {
    for (int loopCount = 0; loopCount < 6; loopCount++) {
      digitalWrite(red, HIGH);
      digitalWrite(green, HIGH);
      delay(250);
      digitalWrite(red, LOW);
      digitalWrite(green, LOW);
      delay(250);
    }
    Serial.println("Couldn't find RTC");
    delay(5000);
    ESP.restart();
    Serial.flush();
    while (1) delay(10);
  } else {
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH);
    delay(3000);
    digitalWrite(red, LOW);
    digitalWrite(green, LOW);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  DateTime now = rtc.now();
  while (mins == "") {
    mins = String(now.minute());
  }
  Serial.println("Starting at hours: " + String(now.hour()));
  Serial.println("Starting minutes at : " + mins);
  start = mins.toFloat();

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
}

void loop() {
}

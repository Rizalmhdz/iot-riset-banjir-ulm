#include <AM2320.h>
#include <Wire.h>

#define SCL2_PIN 27 // AM2320
#define SDA2_PIN 14 // AM2320
#define RhCorrectionAM2320 6.5
#define TCorrectionAM2320 0.3

TwoWire I2C_1 = TwoWire(0);
TwoWire I2C_2 = TwoWire(1);

AM2320 am_sensor(&I2C_2);
float humAM2320;
float tempAM2320;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) {
    delay(10); // hang out until serial port opens
  } 
  Wire.begin();
  I2C_2.begin(SDA2_PIN, SCL2_PIN, 100000ul);
}

void loop() {
  // put your main code here, to run repeatedly:
  sensingTemperature();
  delay(5000);

}

void sensingTemperature(){
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


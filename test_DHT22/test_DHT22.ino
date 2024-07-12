#include <DHT.h>
#define DHTPIN 4                // DHT22 Pin
#define DHTTYPE DHT22           // DHT22 (AM2302)

#define RhCorrectionDHT22 6.2
#define TCorrectionDHT22 0.4

DHT dht_sensor(DHTPIN, DHTTYPE); // Initialize DHT sensor for normal 16mhz Arduino

float humDHT22;
float tempDHT22;

void setup() {
  // put your setup code here, to run once:Serial.begin(9600);
    Serial.begin(9600);
  while (!Serial) {
    delay(10); // hang out until serial port opens
  } 
  dht_sensor.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
  sensingTemperature();
  delay(5000);

}

void sensingTemperature(){
  dhtSensor();
  // amSensor();
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


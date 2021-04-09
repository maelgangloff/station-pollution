#include <Arduino.h>


#define BLYNK_USE_DIRECT_CONNECT

#include <BlynkSimpleEsp32_BLE.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <auth.h>

#include <TinyGPS++.h>
#include <SDS011-select-serial.h>

#include <Wire.h>
#include <SPI.h>
#include <SD.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>

#define SD_CS 2
#define DHT22_PIN 4
#define SD_WRITE_LED 13


float p10, p25, t, rh;

SDS011 my_sds(Serial);
File myFile;
TinyGPSPlus gps;
DHT dht(DHT22_PIN, DHT22);
BlynkTimer timer;

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    Blynk.run();
  } while (millis() - start < ms);
}

/*
* Insert une ligne de données dans le fichier CSV:
*   - Latitude
*   - Longitude
*   - Altitude
*   - Température
*   - Hygrométrie relative
*   - Masse par unité de volume d'air de particules fines de 2.5 microns
*   - Masse par unité de volume d'air de particules fines de 10 microns
*/
void writeFile(TinyGPSPlus gps, float t, float h, float &p25, float &p10) {
  char GPSdate[32], GPStime[32];
  sprintf(GPSdate, "%02d/%02d/%02d ", gps.date.day(), gps.date.month(), gps.date.year());
  sprintf(GPStime, "%02d:%02d:%02d ", gps.time.hour(), gps.time.minute(), gps.time.second());

  myFile = SD.open("/data.csv", FILE_APPEND);
  if (myFile) {
    myFile.println(String(GPSdate) + "," +
                   String(GPStime) + "," +
                   String(gps.location.lat(), 5) + "," +
                   String(gps.location.lng(), 5) + "," +
                   String(gps.altitude.meters(), 0) + "," +
                   String(t, 2) + "," +
                   String(h, 2) + "," +
                   String(p25, 2) + "," +
                   String(p10, 2));
    myFile.close();
  }
}

void readAndSend(){
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, rh);
  Blynk.virtualWrite(V2, p25);
  Blynk.virtualWrite(V3, p10);
  if (gps.location.isValid()) {
    writeFile(gps, t, rh, p25, p10);
    Blynk.virtualWrite(V4, HIGH);
  }else{
    Blynk.virtualWrite(V4, LOW);
  }
}

void setup()
{
  Serial.begin(9600); //SDS011
  Serial2.begin(9600); //GPS

  SD.begin(SD_CS);

  dht.begin();

  //Si le fichier CSV n'existe pas, on le crée et on insert les en-têtes
  if (!SD.exists("/data.csv")) {
    myFile = SD.open("/data.csv", FILE_APPEND);
    myFile.println("date,time,lat,lng,alt,temp,hyg,p25,p10");
    myFile.close();
  }
  Blynk.setDeviceName("PROJET ES");
  Blynk.begin(BLYNK_AUTH);

  timer.setInterval(5000L, readAndSend);
}

void loop()
{
  Blynk.run();
  while (Serial2.available()) gps.encode(Serial2.read()); //Traitement des données brutes GPS
  my_sds.read(&p25, &p10);                                //Lecture de la quantité de particules fines dans l'air
  t = dht.readTemperature();                              //Lecture de la température
  rh = dht.readHumidity();                                //Lecture de l'humidité relative
  timer.run();
  smartDelay(100);
}
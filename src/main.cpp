/*
Station mobile de mesure atmosphérique
Mesure de:
  - Latitude
  - Longitude
  - Altitude
  - Température [V0]
  - Hygrométrie relative  [V1]
  - Pression relative     [V6]
  - Masse par unité de volume d'air de particules fines de 2.5 microns  [V2]
  - Masse par unité de volume d'air de particules fines de 10 microns   [V3]
  - Concentration en monoxyde de carbone  [V4]
*/


#include <Arduino.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <auth.h>

#include <TinyGPS++.h>
#include <SDS011-select-serial.h>

#include <Wire.h>
#include <SPI.h>
#include <SD.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>

#include <MQUnifiedsensor.h>

#include <Adafruit_BMP280.h>

#define SD_CS 2
#define DHT22_PIN 4

#define         Board                   ("ESP-32")
#define         Pin                     (33)
#define         Type                    ("MQ-9")
#define         Voltage_Resolution      (3.3)
#define         ADC_Bit_Resolution      (12)
#define         RatioMQ9CleanAir        (9.6)

SDS011 my_sds(Serial);
File myFile;
TinyGPSPlus gps;
DHT dht(DHT22_PIN, DHT22);
BlynkTimer timer;
WidgetLED led1(V5);
MQUnifiedsensor MQ9(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin, Type);
Adafruit_BMP280 bmp; // I2C

float p10, p25, t, rh, press, ppm = 0;


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
*   - Pression relative
*   - Masse par unité de volume d'air de particules fines de 2.5 microns
*   - Masse par unité de volume d'air de particules fines de 10 microns
*   - Concentration en monoxyde de carbone
*/
void writeFile(TinyGPSPlus gps, float t, float h, float &press, float &p25, float &p10, float &ppm) {
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
                   String(press, 0) + "," +
                   String(p25, 2) + "," +
                   String(p10, 2) + "," +
                   String(ppm, 2));
    myFile.close();
  }
}

void readAndSend(){
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, rh);
  Blynk.virtualWrite(V2, p25);
  Blynk.virtualWrite(V3, p10);
  Blynk.virtualWrite(V4, ppm);
  Blynk.virtualWrite(V6, press);
  Serial.println(press);
  if (gps.location.isValid()) {
    writeFile(gps, t, rh, press, p25, p10, ppm);
    led1.on();
  }else{
    led1.off();
  }
}

void setup()
{
  Serial.begin(9600); //SDS011
  Serial2.begin(9600); //GPS

  SD.begin(SD_CS);
  dht.begin();
  bmp.begin(0x76);

  //Si le fichier CSV n'existe pas, on le crée et on insert les en-têtes
  if (!SD.exists("/data.csv")) {
    myFile = SD.open("/data.csv", FILE_APPEND);
    myFile.println("date,time,lat,lng,alt,temp,hyg,press,p25,p10,ppm");
    myFile.close();
  }
  Blynk.begin(BLYNK_AUTH, WIFI_SSID, WIFI_KEY);

  timer.setInterval(5000L, readAndSend);

  MQ9.setRegressionMethod(1);
  MQ9.setA(1000.5); MQ9.setB(-2.186);
  MQ9.init();

  float calcR0 = 0;
  for(int i = 1; i<=10; i ++)
  {
    MQ9.update();
    calcR0 += MQ9.calibrate(RatioMQ9CleanAir);
  }
  MQ9.setR0(calcR0/10);

  MQ9.setA(599.65); MQ9.setB(-2.244); //On reconfigure A et B pour pouvoir mesurer la concentration en CO
}

void loop()
{
  Blynk.run();
  while (Serial2.available()) gps.encode(Serial2.read()); //Traitement des données brutes GPS
  my_sds.read(&p25, &p10);                                //Lecture de la quantité de particules fines dans l'air
  t = dht.readTemperature();                              //Lecture de la température
  rh = dht.readHumidity();                                //Lecture de l'hygrométrie relative
  MQ9.update();
  ppm = MQ9.readSensor();                                 // Lecture de la concentration en CO
  press = bmp.readPressure()/100;

  timer.run();
  smartDelay(100);
}
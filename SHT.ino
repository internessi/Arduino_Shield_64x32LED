/**
 * ReadSHT1xValues
 *
 * Read temperature and humidity values from an SHT1x-series (SHT10,
 * SHT11, SHT15) sensor.
 *
 * Copyright 2009 Jonathan Oxer <jon@oxer.com.au>
 * www.practicalarduino.com
 */

#include <SHT1x.h>
#include <OneWire.h> 
#include <DallasTemperature.h>

// Specify data and clock connections and instantiate SHT1x object
#define dataPin  13
#define clockPin 14
#define ONE_WIRE_BUS 26 

SHT1x sht1x(dataPin, clockPin);
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

void setup()
{
  Serial.begin(115200); // Open serial connection to report values to host

  pinMode       (12, OUTPUT);  
  digitalWrite  (12, HIGH);
  pinMode       (27, OUTPUT);  
  digitalWrite  (27, HIGH);
  pinMode       (25, OUTPUT);  
  digitalWrite  (25, LOW);
  delay(200);
  Serial.println("Starting up");
  
  sensors.begin(); 

  
}

void loop()
{

  
  float temp_c;
  float temp_f;
  float humidity;

  // Read values from the sensor
  temp_c = sht1x.readTemperatureC();
  humidity = sht1x.readHumidity();

  // Print the values to the serial port
  Serial.print("SHT: ");
  Serial.print(temp_c, DEC);
  Serial.print("/ ");
  Serial.print(humidity);
  delay(200);
  sensors.requestTemperatures(); // Send the command to get temperature readings 
  Serial.print("% - DS18B20: "); 
  Serial.print(sensors.getTempCByIndex(0)); 
  Serial.println("C");

  delay(3000);
}

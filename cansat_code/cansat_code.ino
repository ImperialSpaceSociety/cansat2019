#include "MPU9250.h"
#include <MS5611.h>
#include <SPI.h>
#include <SD.h>
MPU9250 IMU(Wire, 0x68);
 
File dataFile;
 
MS5611 baro;
 
int status;
float Sensor_Pres;
float Current_CurrentSensor;
float Voltage_VoltageSensor;
unsigned int i = 0;
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) {
    ;
  }
 
  Serial.println("Initializing SD...");
  if (!SD.begin(4)) {
    Serial.println("SD initialization failed!");
    while (1);
  }
  Serial.println("SD initialized.");
 
  //SD.remove("DATALOG4.TXT");
  //dataFile = SD.open("DATALOG3.TXT", FILE_WRITE);
  //dataFile = SD.open("datalog5.txt", FILE_WRITE);
  //if (dataFile) {
    //dataFile.println("Packet n. - Mission time[s] Pressure[Pa] Temperature[K] Altitude[m] Batt.Voltage[V]");
  //  dataFile.println("Packet n. - Mission time[s] Pressure[Pa] Temperature[K] Altitude[m] Batt.Voltage[V]");
  //  Serial.println("Packet n. - Mission time[s] Pressure[Pa] Temperature[K] Altitude[m] Batt.Voltage[V]");
  //} else {
  //  Serial.println("error opening file!");
  //}
  //dataFile.close();
 
  status = IMU.begin();
 
  baro = MS5611();
  baro.begin();
 
  pinMode(A0, INPUT); //Voltage sensor input
  pinMode(A1, INPUT); //Current sensor input
 
  i = 0;
 
  delay(1000);
}
 
void loop() {
  i++;
  Serial.println(i);
  dataFile = SD.open("datalog5.txt", FILE_WRITE);
 
  IMU.readSensor();
 
  //Pressure Sesor
  Sensor_Pres = baro.getPressure();
 
  // Current sensor
  Current_CurrentSensor = abs(((analogRead(A1) / 1024.0) * 5000 - 2500) / 185);
 
  // Voltage sensor
  Voltage_VoltageSensor = abs(((analogRead(A0) * 5.0) / 1024.0) * 4.684 - 0.05); // ((analogRead(A0) * 5.0) / 1024.0) / (7500 / (30000 + 7500));// / 2.856;
 
  dataFile.print(Sensor_Pres);
  dataFile.print(" ");
  dataFile.print(IMU.getTemperature_C() + 273.15, 2);
  dataFile.print(" ");
  dataFile.println(Voltage_VoltageSensor, 2);
  Serial.print(" ");
  Serial.println(Current_CurrentSensor, 2);
 
 
  dataFile.close();
 
  delay(200);
 
}

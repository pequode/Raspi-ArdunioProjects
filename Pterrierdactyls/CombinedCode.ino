#include "test.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h>
#include <Adafruit_FXOS8700.h>
#include <Adafruit_FXAS21002C.h>

#define BMP_SCK 20
#define BMP_MOSI 31
#define SEALEVELPRESSURE_HPA (1013.25)
// this is the combined code for our first prototype of the rocket design. 


//Why is it that when we move the functions to the bottom of the code, it stops working???

//Start of Accelerometer code
//int ReadAxis(int axisPin);
//void AutoCalibrate(int xRaw, int yRaw, int zRaw);
const int xInput = A1;
const int yInput = A2;
const int zInput = A3;
const int referencePin = A0;
//For Print
float orientationX, orientationY, orientationZ;
float temperature;
float pressure;
float altitude;
float accelerationX, accelerationY, accelerationZ;

// Raw Ranges:
int tolerance = 512;
int xRawMin = tolerance;
int xRawMax = tolerance;
int yRawMin = tolerance;
int yRawMax = tolerance;
int zRawMin = tolerance;
int zRawMax = tolerance;

// Take multiple samples to reduce noise
const int sampleSize = 10;

Adafruit_BMP3XX bmp; // I2C
Adafruit_FXOS8700 accelmag = Adafruit_FXOS8700(0x8700A, 0x8700B);
Adafruit_FXAS21002C gyro = Adafruit_FXAS21002C(0x0021002C);

int ReadAxis(int axisPin){// Read "sampleSize" samples and report the average
  long reading = 0;
  analogRead(axisPin);
  delay(1);
  for (int i = 0; i < sampleSize; i++) reading += analogRead(axisPin);
  return reading/sampleSize;
}
void AutoCalibrate(int xRaw, int yRaw, int zRaw){
  Serial.println("Calibrate");
  if (xRaw < xRawMin) xRawMin = xRaw;
  if (xRaw > xRawMax) xRawMax = xRaw;
  if (yRaw < yRawMin) yRawMin = yRaw;
  if (yRaw > yRawMax) yRawMax = yRaw;
  if (zRaw < zRawMin) zRawMin = zRaw;
  if (zRaw > zRawMax) zRawMax = zRaw;
}


void setup()
{
  Serial.begin(115200);
  while (!Serial);

  Serial.println("BMP388 test and Accel test");

  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);
  }
  // Set up oversampling and filter initialization
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);

  int xRaw = ReadAxis(xInput);
  int yRaw = ReadAxis(yInput);
  int zRaw = ReadAxis(zInput);
  AutoCalibrate(xRaw, yRaw, zRaw);

  if (!accelmag.begin(ACCEL_RANGE_4G)) {
    /* There was a problem detecting the FXOS8700 ... check your connections */
    Serial.println("Ooops, no FXOS8700 detected ... Check your wiring!");
    while (1)
      ;
 //Gyro setup
      Serial.begin(9600);

  /* Wait for the Serial Monitor */
  while (!Serial) {
    delay(1);
  }

  Serial.println("Gyroscope Test");
  Serial.println("");

  /* Initialise the sensor */
  if (!gyro.begin()) {
    /* There was a problem detecting the FXAS21002C ... check your connections
     */
    Serial.println("Ooops, no FXAS21002C detected ... Check your wiring!");
    while (1)
      ;
  }

}
  }
void loop()
{
    //I2C Accelerometer
    sensors_event_t aevent, mevent;
  /* Get a new sensor event */
  accelmag.getEvent(&aevent, &mevent);
  /* Display the accel results (acceleration is measured in m/s^2) */
 accelerationX= (aevent.acceleration.x);
 accelerationY = (aevent.acceleration.y);
  accelerationZ = (aevent.acceleration.z);
    // temp and pressure
    if (! bmp.performReading()) {
      Serial.println("Failed to perform reading :(");
      return;
    }
   temperature = (bmp.temperature);
    pressure = (bmp.pressure / 100.0);
    altitude = (bmp.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(accelerationX,2);
  Serial.println(temperature);

//Gyroscope
/* Get a new sensor event */
  sensors_event_t event;
  gyro.getEvent(&event);

  /* Display the results (speed is measured in rad/s) */
  Serial.print("X: ");
  Serial.print(event.gyro.x);
  Serial.print("  ");
  Serial.print("Y: ");
  Serial.print(event.gyro.y);
  Serial.print("  ");
  Serial.print("Z: ");
  Serial.print(event.gyro.z);
  Serial.print("  ");
  Serial.println("rad/s ");
  delay(500);

}

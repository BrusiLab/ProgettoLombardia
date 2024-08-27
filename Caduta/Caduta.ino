#include <MPU6050.h>
#include <I2Cdev.h>

#define soglia 300
#define laccio 13  //colleghi cavo da 5v a pin

#define STAMPA

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

MPU6050 accelgyro;
//MPU6050 accelgyro(0x69);  // <-- use for AD0 high
//MPU6050 accelgyro(0x68, &Wire1); // <-- use for AD0 low, but 2nd Wire (TWI/I2C) object

int16_t ax, ay, az;
int16_t gx, gy, gz;

bool caduta = false;

int modalita;

void setup() {

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif

  Serial.begin(9600);  //38400

  accelgyro.initialize();

  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  // use the code below to change accel/gyro offset values
  /*
    Serial.println("Updating internal sensor offsets...");
    // -76	-2359	1688	0	0	0
    Serial.print(accelgyro.getXAccelOffset()); Serial.print("\t"); // -76
    Serial.print(accelgyro.getYAccelOffset()); Serial.print("\t"); // -2359
    Serial.print(accelgyro.getZAccelOffset()); Serial.print("\t"); // 1688
    Serial.print(accelgyro.getXGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyro.getYGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyro.getZGyroOffset()); Serial.print("\t"); // 0
    Serial.print("\n");
    accelgyro.setXGyroOffset(220);
    accelgyro.setYGyroOffset(76);
    accelgyro.setZGyroOffset(-85);
    Serial.print(accelgyro.getXAccelOffset()); Serial.print("\t"); // -76
    Serial.print(accelgyro.getYAccelOffset()); Serial.print("\t"); // -2359
    Serial.print(accelgyro.getZAccelOffset()); Serial.print("\t"); // 1688
    Serial.print(accelgyro.getXGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyro.getYGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyro.getZGyroOffset()); Serial.print("\t"); // 0
    Serial.print("\n");
  */

  pinMode(laccio, INPUT);

  Serial.print("Selezionare modalità: ");
  modalita = Serial.read();

  if (modalita == 1) {
    #define bastone
  } else if (modalita == 2){
    #define deambulatore
  } 
}

void loop() {

  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  //accelgyro.getAcceleration(&ax, &ay, &az);
  //accelgyro.getRotation(&gx, &gy, &gz);

#ifdef STAMPA
  // display tab-separated accel/gyro x/y/z values
  Serial.print("a/g:\t");
  Serial.print(ax);
  Serial.print("\t");
  Serial.print(ay);
  Serial.print("\t");
  Serial.print(az);
  Serial.print("\t");
  Serial.print(gx);
  Serial.print("\t");
  Serial.print(gy);
  Serial.print("\t");
  Serial.println(gz);
#endif

#ifdef bastone //sfrutta giroscopio, accelerometro e laccio

  if ((gx < -45 || gx > 45 || gy < -45 || gy > 45) && (ax < -soglia || ax > soglia || ay < -soglia || ay > soglia) && (digitalRead(laccio) == 1)){
    caduta = true;
  }

#endif

#ifdef deambulatore //sfrutta laccio

  if (digitalRead(laccio) == 0) {
    caduta = true;
  }

#endif

  Serial.println(caduta);
  delay(100);
}

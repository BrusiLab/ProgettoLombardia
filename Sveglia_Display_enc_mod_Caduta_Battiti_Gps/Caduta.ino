// MPU control/status vars
bool dmpReady = false;   // set true if DMP init was successful
uint8_t mpuIntStatus;    // holds actual interrupt status byte from MPU
uint8_t devStatus;       // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;     // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;      // count of all bytes currently in FIFO
uint8_t fifoBuffer[64];  // FIFO storage buffer

// orientation/motion vars
Quaternion q;  // [w, x, y, z]         quaternion container

//accelerazione
VectorInt16 aa;       // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;   // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;  // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;  // [x, y, z]            gravity vector

float euler[3];  // [psi, theta, phi]    Euler angle container
float ypr[3];    // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

float gx, gy, gz;
float ax, ay, az;

bool caduta = false;

// packet structure for InvenSense teapot demo
uint8_t teapotPacket[14] = { '$', 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, '\r', '\n' };

// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;  // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
  mpuInterrupt = true;
}

void inizializzaGyro() {
  // initialize device
  Serial.println(F("Initializing I2C devices..."));
  mpu.initialize();
  pinMode(INTERRUPT_PIN, INPUT);

  // verify connection
  Serial.println(F("Testing device connections..."));
  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

  // load and configure the DMP
  Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();

  // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXGyroOffset(90);
  mpu.setYGyroOffset(180);
  mpu.setZGyroOffset(90);
  mpu.setZAccelOffset(1688);  // 1688 factory default for my test chip

  // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
    // Calibration Time: generate offsets and calibrate our MPU6050
    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
    mpu.PrintActiveOffsets();

    // turn on the DMP, now that it's ready
    mpu.setDMPEnabled(true);

    // enable Arduino interrupt detection
    Serial.print(F("Enabling interrupt detection (Arduino external interrupt "));
    Serial.print(digitalPinToInterrupt(INTERRUPT_PIN));
    Serial.println(F(")..."));
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();

    // set our DMP Ready flag so the main loop() function knows it's okay to use it
    dmpReady = true;

    // get expected DMP packet size for later comparison
    packetSize = mpu.dmpGetFIFOPacketSize();

  } else {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)
    Serial.print(F("DMP Initialization failed (code "));
    Serial.print(devStatus);
    Serial.println(F(")"));
    while(true);
  }
}

void verifica_caduta() {

  // display Euler angles in degrees
  mpu.dmpGetQuaternion(&q, fifoBuffer);
  mpu.dmpGetGravity(&gravity, &q);
  mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
  Serial.print("ypr\t");

  emergency();

  gy = ypr[0] * 180 / M_PI;
  Serial.print(gy);
  Serial.print("\t");

  emergency();

  gz = ypr[1] * 180 / M_PI;
  Serial.print(gz);
  Serial.print("\t");

  emergency();

  gx = ypr[2] * 180 / M_PI;
  Serial.println(gx);

  emergency();

#ifdef OUTPUT_READABLE_REALACCEL
  // display real acceleration, adjusted to remove gravity
  mpu.dmpGetQuaternion(&q, fifoBuffer);
  mpu.dmpGetAccel(&aa, fifoBuffer);
  mpu.dmpGetGravity(&gravity, &q);
  mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);

  emergency();

  ax = aaReal.x;
  Serial.print("areal\t");
  Serial.print(aaReal.x);

  emergency();

  ay = aaReal.y;
  Serial.print("\t");
  Serial.print(aaReal.y);

  emergency();

  az = aaReal.z;
  Serial.print("\t");
  Serial.println(aaReal.z);

  emergency();
#endif

  if (modalita == 1) {  //bastone: sfrutta giroscopio, accelerometro e laccio

    if ((gx <= -45 || gy >= 75 || gy <= -70) && digitalRead(laccio) == 1) {
      pericolo = true;
      Serial.println("CADUTOOOOOOOOOOOOOO");
      emergency();
    }

  } else if (modalita == 2) {  //deambulatore: sfrutta laccio

    if (digitalRead(laccio) == 0) {
      pericolo = true;
      emergency();
    }
  }
}
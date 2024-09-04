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
VectorFloat gravity;  // [x, y, z]            gravity vector

float ypr[3];    // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

float gx, gy, gz;
float ax, ay, az;

bool caduta = false;

// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================
void inizializzaGyro() {

  Wire.begin();
  mpu.initialize();
  devStatus = mpu.dmpInitialize();

  // Imposta gli offset dell'MPU6050 (adatta questi valori al tuo modulo specifico)
  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1688); // 1688 factory default per il modulo mine

  // Controlla se l'inizializzazione è andata a buon fine
  if (devStatus == 0) {
    // Abilita DMP
    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
    mpu.PrintActiveOffsets();
    
    mpu.setDMPEnabled(true);

    // Configura interrupt
    mpuIntStatus = mpu.getIntStatus();

    // Configura dimensione pacchetto
    packetSize = mpu.dmpGetFIFOPacketSize();

    // Indica che il DMP è pronto
    dmpReady = true;

  } else {
    // Se c'è un errore di inizializzazione, scrivilo nella seriale
    Serial.print(F("DMP Initialization failed (code "));
    Serial.print(devStatus);
    Serial.println(F(")"));
  }
}

void verifica_caduta() {

  if (!dmpReady) return;

  // Controlla se ci sono pacchetti FIFO disponibili
  fifoCount = mpu.getFIFOCount();

  if (fifoCount == 1024) {
    // Se il FIFO è pieno, svuotalo e segnala l'overflow
    mpu.resetFIFO();
    Serial.println(F("FIFO overflow!"));

  } else if (fifoCount >= packetSize) {
    // Leggi il pacchetto dal FIFO
    mpu.getFIFOBytes(fifoBuffer, packetSize);

    // Estrai quaternione, gravità e ypr
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

    // Stampa i dati sulla seriale per il debug
    Serial.print("Yaw: ");
    Serial.print(ypr[0] * 180/M_PI);
    gx = ypr[0] * 180/M_PI;
    Serial.print("   ");
    Serial.print(gx);

    Serial.print("\t\tPitch: ");
    Serial.print(ypr[1] * 180/M_PI);
    gz = ypr[1] * 180/M_PI;
    Serial.print("   ");
    Serial.print(gz);

    Serial.print("\t\tRoll: ");
    Serial.print(ypr[2] * 180/M_PI);
    gy = ypr[2] * 180/M_PI; 
    Serial.print("   ");
    Serial.println(gy);
  }

  if (modalita == 1) {  //bastone: sfrutta giroscopio, accelerometro e laccio

    if ((gx <= -45 || gy >= 45 || gy <= -25) && digitalRead(laccio) == 1) {
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
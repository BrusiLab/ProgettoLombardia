//Codice copiato dalla libreria con poche modifiche

const byte RATE_SIZE = 4;  //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE];     //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0;  //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;

void inizializzaBattiti() {

  Serial.println("Initializing...");

  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_STANDARD)  //Use default I2C port
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1)
      ;
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");

  particleSensor.setup();                     //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A);  //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0);   //Turn off Green LED
}

void loop() {

  if (irValue >= 50000) { //se rilevato dito

    for (int 120misurazioni = 0; 120misurazioni < 120; 120 misurazioni++) { //esegui 120 misurazioni

      if (irValue >= 50000) {//se rilevato dito

        long irValue = particleSensor.getIR();

        if (checkForBeat(irValue) == true) {
          //We sensed a beat!
          long delta = millis() - lastBeat;
          lastBeat = millis();

          beatsPerMinute = 60 / (delta / 1000.0);

          if (beatsPerMinute < 255 && beatsPerMinute > 20) {
            rates[rateSpot++] = (byte)beatsPerMinute;  //Store this reading in the array
            rateSpot %= RATE_SIZE;                     //Wrap variable

            //Take average of readings
            beatAvg = 0;
            for (byte x = 0; x < RATE_SIZE; x++)
              beatAvg += rates[x];
            beatAvg /= RATE_SIZE;
          }
        }

        Serial.print("IR=");
        Serial.print(irValue);
        Serial.print(", BPM=");
        Serial.print(beatsPerMinute);
        Serial.print(", Avg BPM=");
        Serial.print(beatAvg);

        Serial.println();
      }
    }
  }
}



/*
#define MAX_BRIGHTNESS 255

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
//Arduino Uno doesn't have enough SRAM to store 100 samples of IR led data and red led data in 32-bit format
//To solve this problem, 16-bit MSB of the sampled data will be truncated. Samples become 16-bit data.
uint16_t irBuffer[100];   //infrared LED sensor data
uint16_t redBuffer[100];  //red LED sensor data
#else
uint32_t irBuffer[100];   //infrared LED sensor data
uint32_t redBuffer[100];  //red LED sensor data
#endif

int32_t bufferLength;   //data length
int32_t spo2;           //SPO2 value
int8_t validSPO2;       //indicator to show if the SPO2 calculation is valid
int32_t heartRate;      //heart rate value
int8_t validHeartRate;  //indicator to show if the heart rate calculation is valid

int misurabattiti;

void inizializzaBattiti() {  //inizializzazione del sensore
  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");

  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
}

int iniziorilevazione;

void rileva_salute() {

  particleSensor.check();

  if (particleSensor.available() == false) {

    emergency();
    bufferLength = 100;  //buffer length of 100 stores 4 seconds of samples running at 25sps

    //read the first 100 samples, and determine the signal range
    for (byte i = 0; i < bufferLength; i++) {

      emergency();

      iniziorilevazione = millis();

      while (particleSensor.available() == false && millis() - iniziorilevazione <= 5000) {  //se abbiamo nuovi dati entro 5 sec
        particleSensor.check();                                                              //Check the sensor for new data
        emergency();
      }

      if (millis() - iniziorilevazione <= 5000) {  //se sono stati rilevati nuovi dati elaborali, altrimenti no

        redBuffer[i] = particleSensor.getRed();
        irBuffer[i] = particleSensor.getIR();
        particleSensor.nextSample();  //We're finished with this sample so move to next sample

        Serial.print(F("red="));
        Serial.print(redBuffer[i], DEC);
        Serial.print(F(", ir="));
        Serial.println(irBuffer[i], DEC);
      }
    }

    //ATTENZIONE: QUESTO POTREBBE NON ANDARE PERCHE IO HO INSERITO UN LIMITE DI TEMPO ALLA MISURAZIONE, ALTRIMENTI SI BLOCCAVA SEMPRE A RILEVARE, QUINDI POTREBBE AVERE PROBLEMI AD ELABORARE I DATI CON ALCUNI VALORI MANCANTI
    //calculate heart rate and SpO2 after first 100 samples (first 4 seconds of samples)
    maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);

    if (unavolta == 0) {  //se è la prima volta che si rilevano i battiti dopo i primi 100 rilevane altri 100
      misurabattiti = 0;
    } else {  //altrimenti solo altri 50
      misurabattiti = 2;
    }

    while (misurabattiti < 4) {  //prende 25 misurazioni ogni volta ch ripete il ciclo, 2 volte normalmente, 4 volte la prima volta

      //take 25 sets of samples before calculating the heart rate.
      for (byte i = 75; i < 100; i++) {

        emergency();

        iniziorilevazione = millis();

        while (particleSensor.available() == false && millis() - iniziorilevazione <= 5000) {  //se abbiamo nuovi dati entro 5 sec
          particleSensor.check();                                                              //Check the sensor for new data
          emergency();
        }

        if (millis() - iniziorilevazione <= 5000) {  //se sono stati rilevati nuovi dati elaborali, altrimenti no

          redBuffer[i - 75] = redBuffer[i - 50];  //solo se sono stati rilevati nuovi dati scarta i precedenti
          irBuffer[i - 75] = irBuffer[i - 50];

          digitalWrite(readLED, !digitalRead(readLED));  //Blink onboard LED with every data read

          redBuffer[i] = particleSensor.getRed();
          irBuffer[i] = particleSensor.getIR();
          particleSensor.nextSample();  //We're finished with this sample so move to next sample

          //send samples and calculation result to terminal program through UART
          Serial.print(F("red="));
          Serial.print(redBuffer[i], DEC);
          Serial.print(F(", ir="));
          Serial.print(irBuffer[i], DEC);

          Serial.print(F(", HR="));
          Serial.print(heartRate, DEC);

          Serial.print(F(", HRvalid="));
          Serial.print(validHeartRate, DEC);

          Serial.print(F(", SPO2="));
          Serial.print(spo2, DEC);

          Serial.print(F(", SPO2Valid="));
          Serial.println(validSPO2, DEC);
        }
      }

      emergency();

      //ATTENZIONE: QUESTO POTREBBE NON ANDARE PERCHE IO HO INSERITO UN LIMITE DI TEMPO ALLA MISURAZIONE, ALTRIMENTI SI BLOCCAVA SEMPRE A RILEVARE, QUINDI POTREBBE AVERE PROBLEMI AD ELABORARE I DATI CON ALCUNI VALORI MANCANTI
      //After gathering 25 new samples recalculate HR and SP02
      maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);

      misurabattiti++;
    }
  }
}

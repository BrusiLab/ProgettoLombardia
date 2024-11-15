//Codice copiato dalla libreria con poche modifiche

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

void inizializzaBattiti() { //inizializzazione del sensore
  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST))  //Use default I2C port, 400kHz speed
  {
    Serial.println(F("MAX30105 was not found. Please check wiring/power.")); //messaggio di errore e blocco al codice se inizializzazioen fallisce
    while (1)
      ;
  }

  Serial.println(F("Attach sensor to finger with rubber band. Press any key to start conversion"));

  byte ledBrightness = 60;  //Options: 0=Off to 255=50mA
  byte sampleAverage = 4;   //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2;         //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  byte sampleRate = 100;    //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411;     //Options: 69, 118, 215, 411
  int adcRange = 4096;      //Options: 2048, 4096, 8192, 16384

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);  //Configure sensor with these settings
}

int iniziorilevazione;

void rileva_salute() {

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

    if (millis() - iniziorilevazione <= 5000) { //se sono stati rilevati nuovi dati elaborali, altrimenti no

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

  if (unavolta == 0) {    //se è la prima volta che si rilevano i battiti dopo i primi 100 rilevane altri 100
    misurabattiti = 0;
  } else {                //altrimenti solo altri 50
    misurabattiti = 2;
  }

  while (misurabattiti < 4) { //prende 25 misurazioni ogni volta ch ripete il ciclo, 2 volte normalmente, 4 volte la prima volta

    //take 25 sets of samples before calculating the heart rate.
    for (byte i = 75; i < 100; i++) {

      emergency();

      iniziorilevazione = millis();

      while (particleSensor.available() == false && millis() - iniziorilevazione <= 5000) {  //se abbiamo nuovi dati entro 5 sec
        particleSensor.check();                                                              //Check the sensor for new data
        emergency();
      }

      if (millis() - iniziorilevazione <= 5000) { //se sono stati rilevati nuovi dati elaborali, altrimenti no

        redBuffer[i - 75] = redBuffer[i - 50]; //solo se sono stati rilevati nuovi dati scarta i precedenti
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
//hardaware: + al 5v, - al gnd, altri pin a pin analogici

//valori normali CO: 100 ppm
//valori normali NH3: 50 ppm
//valori normali NO2: 1 ppm
//valori normali ozono: 0.06 ppm 

int co, nh3;
float no2, ozono;

int timeraria(int ogni) {  //timer non bloccante per qualità aria

  static unsigned long inizio = 0;
  static unsigned long passato;
  int uscita = 0;

  emergency();

  passato = millis() - inizio;

  if (passato >= ogni) {
    inizio = millis();
    uscita = 1;
  }

  return uscita;
}

void rileva_aria(){ //aggiorna i dati
  co = map (analogRead(pinCO), 0, 1023, 1, 1000);
  nh3 = map (analogRead(pinNH3), 0, 1023, 1, 500);
  no2 = map (analogRead(pinNO2), 0, 1023, 0.05, 10);
  ozono = map(analogRead(pinozono), 0, 1023, 0.01, 1);
}
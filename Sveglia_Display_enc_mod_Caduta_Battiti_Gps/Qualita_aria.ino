//hardaware: + al 5v, - al gnd, altri pin a pin analogici

//valori normali CO: tra 1 e 4 ppm
//valori normali NH3: 388 in media al giorno
//valori normali NO2: max 0.1 ppm
//valori normali ozono: max 0.05 ppm - media 0.03 ppm

int co, nh3;
float no2, ozono;

void rileva_aria(){
  co = map (analogRead(pinCO), 0, 1023, 1, 1000);
  nh3 = map (analogRead(pinNH3), 0, 1023, 1, 500);
  no2 = map (analogRead(pinNO2), 0, 1023, 0.05, 10);
  ozono = map(analogRead(pinozono), 0, 1023, 0.01, 1);
}
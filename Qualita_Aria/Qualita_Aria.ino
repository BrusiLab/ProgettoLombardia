//hardaware: + al 5v, - al gnd, altri pin a pin analogici

//valori normali CO: tra 1 e 4 ppm
//valori normali NH3: 388 in media al giorno
//valori normali NO2: max 0.1 ppm

#define pinCO A2
#define pinNH3 A1
#define pinNO2 A0

#define valori 1023

int co, nh3;
float no2;

void setup() {
  Serial.begin(9600);
}

void loop() {
  co = map (analogRead(pinCO), 0, valori, 1, 1000);
  nh3 = map (analogRead(pinNH3), 0, valori, 1, 500);
  no2 = map (analogRead(pinNO2), 0, valori, 0.05, 10);

  Serial.print("CO: ");
  Serial.print(co);
  Serial.print(" ppm\t NH3: ");
  Serial.print(nh3);
  Serial.print(" ppm\t NO2: ");
  Serial.print(no2);
  Serial.println(" ppm");

  delay(1000);
}

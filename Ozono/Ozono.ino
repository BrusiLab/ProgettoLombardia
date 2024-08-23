//hardare: + al 5v, - al gnd, A0 a pin analogico, D0 a pin digitale

//valori normali: max 0.05 ppm - media 0.03 ppm

#define pinana A5
#define pindig 7

void setup() {
  Serial.begin(9600);
  pinMode(pindig, INPUT);
}

float ozono = 0;

#define minimo 0.01
#define massimo 1

void loop() {
  
  ozono = map(analogRead(pinana), 0, 1023, minimo, massimo); 
  
  Serial.print("Analogico: ");
  Serial.print(ozono);
  Serial.print("\tDigitale: ");
  Serial.println(digitalRead(pindig));
  delay(500);
}

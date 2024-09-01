  //Hardware buzzer: collego - a gnd e S a pin digitale con pwm

#define disinnesco 8  //disattiva sveglia e allarme
#define emergenza 9   //attiva allarme
#define buzzer 12
#define led 11

bool orario = true;    //attivato a ora sveglia
bool pericolo;  //attivato da puslante emergenza o caduta
bool fermati;   //attivato da disinnesco -> ferma sveglia e allarme

void setup() {

  Serial.begin(9600);
  
  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(disinnesco, INPUT);
  pinMode(emergenza, INPUT);

}

int stop() {
  
  if (digitalRead(disinnesco) == true) {
    fermati = true;
  } else {
    fermati = false;
  }

  return fermati;
}

void stop_bool(){

}

void loop() {

  if (digitalRead(emergenza) == true){
    pericolo = true;
  }

  sveglia();
  allarme();

  digitalWrite(led, LOW);

}

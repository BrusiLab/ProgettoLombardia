//Hardware: collego - a gnd e S a pin digitale con pwm

#define pin 10
#define led 11
#define disinnesco 8

#define frq 1000
#define durata 250

#define scalaalta

#ifdef scalaalta

#define Do 523
#define re 587
#define mi 659
#define fa 699
#define sol 784
#define la 880
#define si 988
#define DO 1047
#define RE 1175
#define MI 1319

#endif

#ifdef scalabassa

#define Do 262
#define re 293
#define mi 330
#define fa 349
#define sol 392
#define la 440
#define si 494
#define DO 523
#define RE 587
#define MI 659

#endif

bool orario = true;
bool pericolo;
bool fermati;

void setup() {
  Serial.begin(9600);
  pinMode(pin, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(disinnesco, INPUT);
}

void suona(int nota, float tempo) {
  tone(pin, nota, durata * tempo);
  delay(durata * 1.3 * tempo);
}

void stop() {
  /*if (digitalRead(disinnesco) == true) {
    fermati = true;
  }*/

  if (Serial.read() == 'a') {
    fermati = true;
  }
}

void sveglia() {

  digitalWrite(led, HIGH);

  suona(MI, 0.5);
  stop();
  digitalWrite(led, LOW);

  suona(RE, 0.5);
  stop();
  digitalWrite(led, HIGH);

  suona(fa, 1);
  stop();
  digitalWrite(led, LOW);

  suona(sol, 1);
  stop();
  digitalWrite(led, HIGH);

  suona(DO, 0.5);
  stop();
  digitalWrite(led, LOW);

  suona(si, 0.5);
  stop();
  digitalWrite(led, HIGH);

  suona(re, 1);
  stop();
  digitalWrite(led, LOW);

  suona(mi, 1);
  stop();
  digitalWrite(led, HIGH);

  suona(si, 0.5);
  stop();
  digitalWrite(led, LOW);

  suona(la, 0.5);
  stop();
  digitalWrite(led, HIGH);

  suona(Do, 1);
  stop();
  digitalWrite(led, LOW);

  suona(mi, 1);
  stop();
  digitalWrite(led, HIGH);

  suona(la, 3);
  stop();
  digitalWrite(led, LOW);
}

void loop() {

  stop();

  while (fermati != true && orario == true) {
    sveglia();
    stop();
  }

  orario = false;
  fermati = false;

  while (fermati != true && pericolo == true) {

    tone(pin, frq, 500);
    stop();
    tone(pin, frq, 500);

    digitalWrite(led, HIGH);
    delay(200);
    stop();
    digitalWrite(led, LOW);
    delay(200);
    stop();

    digitalWrite(led, HIGH);
    delay(200);
    stop();
    digitalWrite(led, LOW);
    delay(200);
    stop();
  }

  pericolo = false;
  fermati = false;
}

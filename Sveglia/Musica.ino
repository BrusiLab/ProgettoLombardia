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

void suona(int nota, float tempo) {
  tone(buzzer, nota, durata * tempo);
  delay(durata * 1.3 * tempo);
}

void sveglia() {

  while (orario == true) {

    digitalWrite(led, HIGH);

    suona(MI, 0.4);
    stop();
    if (fermati) { break; }
    digitalWrite(led, LOW);

    suona(RE, 0.4);
    if (stop()) { break; }
    digitalWrite(led, HIGH);

    suona(fa, 0.9);
    if (stop()) { break; }
    digitalWrite(led, LOW);

    suona(sol, 0.9);
    if (stop()) { break; }
    digitalWrite(led, HIGH);

    suona(DO, 0.4);
    if (stop()) { break; }
    digitalWrite(led, LOW);

    suona(si, 0.4);
    if (stop()) { break; }
    digitalWrite(led, HIGH);

    suona(re, 0.9);
    if (stop()) { break; }
    digitalWrite(led, LOW);

    suona(mi, 0.9);
    if (stop()) { break; }
    digitalWrite(led, HIGH);

    suona(si, 0.4);
    if (stop()) { break; }
    digitalWrite(led, LOW);

    suona(la, 0.4);
    if (stop()) { break; }
    digitalWrite(led, HIGH);

    suona(Do, 0.9);
    if (stop()) { break; }
    digitalWrite(led, LOW);

    suona(mi, 0.9);
    if (stop()) { break; }
    digitalWrite(led, HIGH);

    suona(la, 2.7);
    if (stop()) { break; }
    digitalWrite(led, LOW);

    if (stop()) { break; }
  }

  orario = false;
  fermati = false;
}

void allarme() {

  if (pericolo == true) {

    for (int secondi = 30; secondi > 0; secondi--) {

      tone(buzzer, frq, 190);
      if (stop()) { break; }
      tone(buzzer, frq, 190);
      if (stop()) { break; }

      digitalWrite(led, HIGH);
      delay(130);
      if (stop()) { break; }
      digitalWrite(led, LOW);
      delay(130);
      if (stop()) { break; }

      digitalWrite(led, HIGH);
      delay(130);
      if (stop()) { break; }
      digitalWrite(led, LOW);
      delay(130);
      if (stop()) { break; }

      tone(buzzer, frq, 190);
      if (stop()) { break; }
      tone(buzzer, frq, 190);
      if (stop()) { break; }

      digitalWrite(led, HIGH);
      delay(130);
      if (stop()) { break; }
      digitalWrite(led, LOW);
      delay(130);
      if (stop()) { break; }

      digitalWrite(led, HIGH);
      delay(130);
      if (stop()) { break; }
      digitalWrite(led, LOW);
      delay(130);
      if (stop()) { break; }
    }

    //invia allarme

    while (pericolo == true) {

      tone(buzzer, frq, 500);
      if (stop()) { break; }
      tone(buzzer, frq, 500);

      digitalWrite(led, HIGH);
      delay(200);
      if (stop()) { break; }
      digitalWrite(led, LOW);
      delay(200);
      if (stop()) { break; }

      digitalWrite(led, HIGH);
      delay(200);
      if (stop()) { break; }
      digitalWrite(led, LOW);
      delay(200);
      if (stop()) { break; }
    }

    pericolo = false;
    fermati = false;
  }
}
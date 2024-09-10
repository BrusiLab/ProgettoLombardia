#define frq 1000
#define durata 250

#define scalaalta

#ifdef scalaalta  //suoneria originale nokia

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

#ifdef scalabassa  //suoneria un ottava più bassa

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

bool scattata = false;
bool fermatialla = false;

int stop() {  //rileva se viene premuto pulsante di disinnesco (se si restituisce true)

  if (digitalRead(disinnesco) == true) {
    fermati = true;
  } else {
    fermati = false;
  }

  emergency();

  return fermati;
}

int stopalla() {  //uguale a stop ma da usare con l'allarme perché non contiene "emergency()" che farebbe ripartire l'allamre ogni volta

  if (digitalRead(disinnesco) == true) {
    fermatialla = true;
  } else {
    fermatialla = false;
  }

  return fermatialla;
}

void suona(int nota, float tempo) {  //suona una nota per tot tempo
  tone(buzzer, nota, durata * tempo);
  emergency();
  delay(durata * 1.3 * tempo);
}

void sveglia() {

  if (minuto - 3 >= minutisveglia) {  //dopo 3 minuti dall'ora della sveglia disattiva "scattata" perché tanto non suona più
    scattata = false;
  }

  if (oresveglia == ora && minutisveglia <= minuto - 2 && scattata == false) {  //se è l'ora della sveglia (o fino a due minuti dopo se arduino era impegnato in altro) e non eè ancora suonata la sveglia
    orario = true;                                                              //attiva orario
    scattata = true;
  }

  if (orario == true) {  //se è l'ora della sveglia scrivi a schermo che è ora delle medicine
    display.clearBuffer();

    display.setFont(u8g2_font_timB14_tr);

    display.drawStr(2, 25, "Promemoria");
    display.drawStr(2, 57, "medicine");

    display.sendBuffer();
  }

  while (orario == true) {  //finché non vieni disattiva suona e fai lampeggiare un led

    digitalWrite(led, HIGH);

    suona(MI, 0.4);
    stop();
    if (fermati) { break; }  //se fermati == true esci dal while
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

char rimanenti[3];

void allarme() {

  if (pericolo == true) {  //se una caduta o il pusalnte di emergenza hanno attivato pericolo

    for (int secondi = 30; secondi > 0; secondi--) {  //si hanno 30 secondi per disattivare l'allare prima che sia inviata una mail

      //so che la somma dei dilay non fa 1 secondo a ogni giro ma l'ho cronometrato e così sono 30 sec giusti

      Serial.println(secondi);

      display.clearBuffer();

      display.setFont(u8g2_font_timB14_tr);

      display.drawStr(2, 25, "ALLARME");  //mostra a schermo quanti secondi rimangono
      itoa(secondi, rimanenti, 10);
      display.drawStr(2, 57, rimanenti);
      display.drawStr(22, 57, "s rimanenti");

      display.sendBuffer();

      tone(buzzer, frq, 190);
      if (stopalla()) { break; }
      tone(buzzer, frq, 190);
      if (stopalla()) { break; }

      digitalWrite(led, HIGH);
      delay(130);
      if (stopalla()) { break; }
      digitalWrite(led, LOW);
      delay(130);
      if (stopalla()) { break; }

      digitalWrite(led, HIGH);
      delay(130);
      if (stopalla()) { break; }
      digitalWrite(led, LOW);
      delay(130);
      if (stopalla()) { break; }

      tone(buzzer, frq, 190);
      if (stopalla()) { break; }
      tone(buzzer, frq, 190);
      if (stopalla()) { break; }

      digitalWrite(led, HIGH);
      delay(130);
      if (stopalla()) { break; }
      digitalWrite(led, LOW);
      delay(130);
      if (stopalla()) { break; }

      digitalWrite(led, HIGH);
      delay(130);
      if (stopalla()) { break; }
      digitalWrite(led, LOW);
      delay(130);
      if (stopalla()) { break; }
    }

    if (fermatialla == false) {  //se l'allarme non è stato disattivato invia una mail (codice per mail ancora da scrivere)

      postData = "";  //resetta i dati

      convertidati("mode", "arduino");

      convertidati("battiti", battitistr);
      convertidati("spO2", sp02str);

      itoa(co, costr, 10);
      convertidati("co", costr);
      itoa(nh3, nh3str, 10);
      convertidati("nh3", nh3str);
      no2str = ftoa(no2, 2, 4);
      convertidati("no2", no2str);
      ozonostr = ftoa(ozono, 2, 4);
      convertidati("ozono", ozonostr);

      latstr = ftoa(lat, 6, 10);
      convertidati("lat", latstr);
      lonstr = ftoa(lon, 6, 10);
      convertidati("lon", lonstr);

      convertidati("allarme", "true"); //allare true
      
      client.post(path, contentType, postData);

      display.clearBuffer();

      display.setFont(u8g2_font_timB14_tr);

      display.drawStr(2, 25, "ALLARME");
      display.drawStr(2, 57, "inviato avviso");  //mostra a schermo che è stato inviato l'avviso

      display.sendBuffer();
    }

    while (pericolo == true && fermatialla == false) {  //dopo i 30 secondi cambia ritmo e continua a suonare e lampeggiare finché non si disattiva

      tone(buzzer, frq, 500);
      if (stopalla()) { break; }
      tone(buzzer, frq, 500);

      digitalWrite(led, HIGH);
      delay(200);
      if (stopalla()) { break; }
      digitalWrite(led, LOW);
      delay(200);
      if (stopalla()) { break; }

      digitalWrite(led, HIGH);
      delay(200);
      if (stopalla()) { break; }
      digitalWrite(led, LOW);
      delay(200);
      if (stopalla()) { break; }
    }

    pericolo = false;
    fermati = false;
    fermatialla = false;
  }
}
const byte cuore[32] PROGMEM =  //disegno cuore

  { B00000000, B00000000,
    B00111000, B00011100,
    B01111100, B00111110,
    B11111110, B01111111,
    B11111110, B01111111,
    B11111110, B01111111,
    B11111110, B01111111,
    B11111110, B01111111,
    B11111100, B00111111,
    B11111100, B00111111,
    B11111000, B00011111,
    B11110000, B00001111,
    B11100000, B00000111,
    B11000000, B00000011,
    B10000000, B00000001,
    B00000000, B00000000 };

const byte goccia[32] PROGMEM =  //disegno cuore

  { B00000000, B00000000,
    B10000000, B00000001,
    B10000000, B00000001,
    B11000000, B00000011,
    B11000000, B00000011,
    B11100000, B00000111,
    B11110000, B00001111,
    B11110000, B00001111,
    B11111000, B00011111,
    B11101000, B00011111,
    B11101000, B00011111,
    B11011000, B00011111,
    B11011000, B00011111,
    B11110000, B00001111,
    B11000000, B00000011,
    B00000000, B00000000 };

const byte sole[32] PROGMEM =  //disegno cuore

  { B00000000, B00000000,
    B00100000, B00000100,
    B01000100, B00100010,
    B01001000, B00010010,
    B11010000, B00001011,
    B11100010, B01000111,
    B11111100, B00111111,
    B11110000, B00000111,
    B11110000, B00000111,
    B11111100, B00111111,
    B11100010, B01000111,
    B11010000, B00001011,
    B01001000, B00010010,
    B01000100, B00100010,
    B00100000, B00000100,
    B00000000, B00000000 };

const byte orologio[32] PROGMEM =  //disegno cuore

  { B00000000, B00000000,
    B11000000, B00000011,
    B00110000, B00001100,
    B00001000, B00010000,
    B00000100, B00100000,
    B10000100, B00100000,
    B10000010, B01000000,
    B10000010, B01000000,
    B10000010, B01000000,
    B00000010, B01000001,
    B00000100, B00100010,
    B00000100, B00100000,
    B00001000, B00010000,
    B00110000, B00001100,
    B11000000, B00000011,
    B00000000, B00000000 };

const byte campana[32] PROGMEM =  //disegno cuore

  { B00000000, B00000000,
    B10000000, B00000001,
    B01000000, B00000010,
    B00100000, B00000100,
    B00010000, B00001000,
    B00010000, B00001000,
    B00010000, B00001000,
    B00010000, B00001000,
    B00010000, B00001000,
    B00010000, B00001000,
    B00001000, B00010000,
    B00001000, B00010000,
    B00000100, B00100000,
    B11111100, B00111111,
    B10000000, B00000001,
    B10000000, B00000001 };

const byte calendario[32] PROGMEM =  //disegno cuore

  { B00000000, B00000000,
    B11111100, B00111111,
    B00000100, B00100000,
    B11100100, B00100111,
    B00000100, B00100000,
    B11111100, B00111111,
    B00000100, B00100000,
    B00000100, B00100001,
    B10000100, B00100001,
    B01000100, B00100001,
    B00000100, B00100001,
    B00000100, B00100001,
    B00000100, B00100001,
    B00000100, B00100000,
    B11111100, B00111111,
    B00000000, B00000000 };

char sp02str[4];
char battitistr[4];

byte temperature = 0;
char tempstr[3];
byte humidity = 0;
char humstr[3];

int anno;
int mese;
int giorno;
char gg_settimana[4];
int ora;
int minuto;

char orestr[3];
char minstr[3];
char ggstr[3];
char mesestr[3];
char annostr[3];

int minutisveglia = 61;
char svminstr[3];
int oresveglia = 25;
char svorestr[3];

int premuto() {

  emergency();

  if (!digitalRead(sw)) {
    premutoenc = true;
  } else {
    premutoenc = false;
  }

  return premutoenc;
}

int timertemp(int timer1) {  //timer non bloccante

  static unsigned long t1 = 0;
  static unsigned long dt;
  int ret = 0;

  emergency();

  dt = millis() - t1;

  if (dt >= timer1) {
    t1 = millis();
    ret = 1;
  }

  return ret;
}

int timerdata(int tempodata) {  //timer non bloccante

  static unsigned long tempodata1 = 0;
  static unsigned long deltatmpodata;
  int tot = 0;

  emergency();

  deltatmpodata = millis() - tempodata1;

  if (deltatmpodata >= tempodata) {
    tempodata1 = millis();
    tot = 1;
  }

  return tot;
}

int timerbattiti(int soglia) {  //timer non bloccante

  static unsigned long tempo1 = 0;
  static unsigned long deltatempo;
  int esito = 0;

  emergency();

  deltatempo = millis() - tempo1;

  if (deltatempo >= soglia) {
    tempo1 = millis();
    esito = 1;
  }

  return esito;
}

void visualizza_bastone() {

  display.clearDisplay();
  display.clearBuffer();

  display.setFont(u8g2_font_timB14_tr);

  display.drawStr(2, 20, "MODALITA:");  //scrivi bastone a schermo

  display.drawStr(2, 52, "Bastone");

  display.sendBuffer();

  oldpos = pos;
  pos = enc.read();

  while (oldpos >= pos) {  //se non cambia niente o visualizzeresti ancora bastone

    if (premuto()) {  //se premi imposti quella modalita
      temporanea = 1;
      Serial.println("PIGIATO");
      Serial.println(modalita);
      break;
    } else {
      temporanea = 0;
    }

    oldpos = pos;
    pos = enc.read();
  }
}

void visualizza_deambulatore() {  //come il bastone

  display.clearDisplay();
  display.clearBuffer();

  display.setFont(u8g2_font_timB14_tr);

  display.drawStr(2, 20, "MODALITA:");  //scrivi bastone a schermo

  display.drawStr(2, 52, "Deambulatore");

  display.sendBuffer();

  oldpos = pos;
  pos = enc.read();

  while (oldpos <= pos) {

    if (premuto()) {
      temporanea = 2;
      Serial.println("PIGIATO");
      Serial.println(modalita);
      break;
    } else {
      temporanea = 0;
    }

    oldpos = pos;
    pos = enc.read();
  }
}

void cambia_modalita() {

  Serial.println("cambio modalita");

  while (modalita == 0) {  //finché non selezioni la modalità desiderata

    Serial.println("while");

    oldpos = pos;
    pos = enc.read();

    Serial.println("passo dall'inizio");

    if (oldpos > pos) {  //senso orario
      visualizza_bastone();
    } else if (oldpos < pos) {
      visualizza_deambulatore();
    } else if (oldpos == pos && bast == true) {
      bast = false;
      delay(500);
      visualizza_bastone();
    }

    modalita = temporanea;

    Serial.println(modalita);
  }

  Serial.println(modalita);

  primo = 0;
  batt = true;
}

void visualizza_battiti() {

  display.clearBuffer();

  display.setFont(u8g2_font_timB14_tr);

  display.drawStr(2, 25, "Misurazione");
  display.drawStr(2, 57, "battiti...");

  display.sendBuffer();

  if (unavolta == 0) {
    rileva_salute();
    unavolta++;
  } else {
    rileva_salute();
  }

  display.clearBuffer();

  display.setFont(u8g2_font_timB14_tr);

  display.drawStr(2, 25, "Battiti:");

  display.setFont(u8g2_font_timB18_tr);

  if (heartRate < 30 || heartRate > 250) {
    display.drawStr(75, 25, "--");
  } else if (heartRate >= 100) {
    itoa(heartRate, battitistr, 10);
    display.drawStr(70, 25, battitistr);
  } else {  //se i dati hanno senso
    itoa(heartRate, battitistr, 10);
    display.drawStr(80, 25, battitistr);
  }

  display.drawXBMP(110, 10, 16, 16, cuore);

  display.setFont(u8g2_font_timB14_tr);

  display.drawStr(2, 57, "SpO2:");

  display.setFont(u8g2_font_timB18_tr);

  if (spo2 < 80) {
    display.drawStr(75, 57, "--");
  } else if (spo2 == 100) {  //se i dati hanno senso
    itoa(spo2, sp02str, 10);
    display.drawStr(70, 57, sp02str);
  } else {
    itoa(spo2, sp02str, 10);
    display.drawStr(80, 57, sp02str);
  }

  display.setFont(u8g2_font_TimesNewPixel_tr);

  display.drawStr(112, 55, "%");

  display.sendBuffer();
}

void visualizza_temperatura() {

  sensore_temp.read(&temperature, &humidity, NULL);

  display.clearBuffer();

  display.setFont(u8g2_font_timB18_tr);

  display.drawXBMP(2, 9, 16, 16, sole);

  itoa((int)temperature, tempstr, 10);
  display.drawStr(50, 25, tempstr);

  display.drawStr(100, 25, "C");

  display.drawXBMP(2, 41, 16, 16, goccia);

  itoa((int)humidity, humstr, 10);
  display.drawStr(50, 57, humstr);

  display.drawStr(100, 57, "%");

  Serial.print("Temp: ");
  Serial.println(tempstr);

  Serial.print("Hum: ");
  Serial.println(humstr);

  display.sendBuffer();
}

void visualizza_data() {

  aggiorna_data();

  display.clearBuffer();

  display.setFont(u8g2_font_timB14_tr);

  display.drawXBMP(2, 11, 16, 16, orologio);

  itoa(ora, orestr, 10);
  display.drawStr(19, 25, orestr);
  display.drawStr(37, 23, ":");
  itoa(minuto, minstr, 10);
  display.drawStr(44, 25, minstr);

  display.drawXBMP(66, 11, 16, 16, campana);

  if (oresveglia <= 24) {
    display.clearBuffer();
    display.setFont(u8g2_font_timB14_tr);
    itoa(oresveglia, svorestr, 10);
    display.drawStr(83, 25, svorestr);
    display.drawStr(101, 23, ":");
    itoa(minutisveglia, svminstr, 10);
    display.drawStr(108, 25, svminstr);
    display.sendBuffer();
  } else {
    display.clearBuffer();
    display.setFont(u8g2_font_timB14_tr);
    display.drawStr(83, 25, "--");
    display.drawStr(101, 23, ":");
    display.drawStr(108, 25, "--");
    display.sendBuffer();
  }

  display.drawXBMP(2, 42, 16, 16, calendario);

  itoa(giorno, ggstr, 10);
  display.drawStr(19, 57, ggstr);
  display.drawStr(37, 57, "/");
  itoa(mese, mesestr, 10);
  display.drawStr(43, 57, mesestr);
  display.drawStr(61, 57, "/");
  itoa(anno, annostr, 10);
  display.drawStr(67, 57, annostr);

  display.drawStr(92, 57, gg_settimana);

  display.sendBuffer();
}

void visualizza_ancora_battiti() {

  while (pos >= -10 && pos <= 10) {

    oldpos = pos;
    pos = enc.read();

    if (primo == 0) {
      visualizza_battiti();
      primo++;
      delay(500);
    } else {
      if (timerbattiti(30000)) {
        visualizza_battiti();
      }
    }

    if (premuto()) {
      modalita = 0;
      bast = true;
      cambia_modalita();
    }

    Serial.println(pos);
  }
}

void visualizza_ancora_temperatura() {

  while (pos < -10) {

    oldpos = pos;
    pos = enc.read();

    if (primo == 0) {
      visualizza_ancora_battiti();
    } else {
      if (timertemp(30000)) {
        visualizza_temperatura();
      }
    }

    if (premuto()) {
      modalita = 0;
      bast = true;
      cambia_modalita();
    }
  }
}

void visualizza_ancora_data() {

  while (pos > 10) {

    oldpos = pos;
    pos = enc.read();

    if (primo == 0) {
      visualizza_ancora_battiti();
    } else {
      if (timerdata(1000)) {
        visualizza_data();
      }
    }

    if (premuto()) {
      data = true;
      imposta_sveglia();
    }
  }
}

/*
const byte cuore[32] PROGMEM =  //disegno cuore

  { B00000000, B00000000,
    B00111000, B00011100,
    B01111100, B00111110,
    B11111110, B01111111,
    B11111110, B01111111,
    B11111110, B01111111,
    B11111110, B01111111,
    B11111110, B01111111,
    B11111100, B00111111,
    B11111100, B00111111,
    B11111000, B00011111,
    B11110000, B00001111,
    B11100000, B00000111,
    B11000000, B00000011,
    B10000000, B00000001,
    B00000000, B00000000 };

const byte goccia[32] PROGMEM =  //disegno cuore

  { B00000000, B00000000,
    B10000000, B00000001,
    B10000000, B00000001,
    B11000000, B00000011,
    B11000000, B00000011,
    B11100000, B00000111,
    B11110000, B00001111,
    B11110000, B00001111,
    B11111000, B00011111,
    B11101000, B00011111,
    B11101000, B00011111,
    B11011000, B00011111,
    B11011000, B00011111,
    B11110000, B00001111,
    B11000000, B00000011,
    B00000000, B00000000 };

const byte sole[32] PROGMEM =  //disegno cuore

  { B00000000, B00000000,
    B01000000, B00000100,
    B10011000, B00100100,
    B10100000, B00010010,
    B11000010, B00001011,
    B11101000, B01100111,
    B11110010, B00011111,
    B11111100, B00001111,
    B11110000, B00111111,
    B11111000, B01001111,
    B11100110, B00010111,
    B11010000, B00100011,
    B01001000, B00000101,
    B00100100, B00011001,
    B00100000, B00000010,
    B00000000, B00000000 };

const byte orologio[32] PROGMEM =  //disegno cuore

  { B00000000, B00000000,
    B11000000, B00000011,
    B00110000, B00001100,
    B00001000, B00010000,
    B00000100, B00100000,
    B10000100, B00100000,
    B10000010, B01000000,
    B10000010, B01000000,
    B10000010, B01000000,
    B00000010, B01000001,
    B00000100, B00100010,
    B00000100, B00100000,
    B00001000, B00010000,
    B00110000, B00001100,
    B11000000, B00000011,
    B00000000, B00000000 };

const byte campana[32] PROGMEM =  //disegno cuore

  { B00000000, B00000000,
    B10000000, B00000001,
    B01000000, B00000010,
    B00100000, B00000100,
    B00010000, B00001000,
    B00010000, B00001000,
    B00010000, B00001000,
    B00010000, B00001000,
    B00010000, B00001000,
    B00010000, B00001000,
    B00001000, B00010000,
    B00001000, B00010000,
    B00000100, B00100000,
    B11111100, B00111111,
    B10000000, B00000001,
    B10000000, B00000001 };

const byte calendario[32] PROGMEM =  //disegno cuore

  { B00000000, B00000000,
    B11111100, B00111111,
    B00000100, B00100000,
    B11100100, B00100111,
    B00000100, B00100000,
    B11111100, B00111111,
    B00000100, B00100000,
    B00000100, B00100001,
    B10000100, B00100001,
    B01000100, B00100001,
    B00000100, B00100001,
    B00000100, B00100001,
    B00000100, B00100001,
    B00000100, B00100000,
    B11111100, B00111111,
    B00000000, B00000000 };

char sp02str[4];
char battitistr[4];

byte temperature = 0;
char tempstr[3];
byte humidity = 0;
char humstr[3];

int premuto() {

  emergency();

  if (!digitalRead(sw)) {
    premutoenc = true;
  } else {
    premutoenc = false;
  }

  return premutoenc;
}

int mytimer(int timer1) {  //timer non bloccante

  static unsigned long t1 = 0;
  static unsigned long dt;
  int ret = 0;

  emergency();

  dt = millis() - t1;

  if (dt >= timer1) {
    t1 = millis();
    ret = 1;
  }

  return ret;
}

void visualizza_bastone() {

  display.clearDisplay();
  display.clearBuffer();

  display.setFont(u8g2_font_timB14_tr);

  display.drawStr(2, 20, "MODALITA:");  //scrivi bastone a schermo

  display.drawStr(2, 52, "Bastone");

  display.sendBuffer();

  emergency();

  oldpos = pos;
  pos = enc.read();

  while (oldpos >= pos) {  //se non cambia niente o visualizzeresti ancora bastone

    emergency();

    if (premuto()) {  //se premi imposti quella modalita
      temporanea = 1;
      Serial.println("PIGIATO");
      Serial.println(modalita);
      break;
    } else {
      temporanea = 0;
    }

    oldpos = pos;
    pos = enc.read();
  }
}

void visualizza_deambulatore() {  //come il bastone

  display.clearDisplay();
  display.clearBuffer();

  display.setFont(u8g2_font_timB14_tr);

  display.drawStr(2, 20, "MODALITA:");  //scrivi bastone a schermo

  display.drawStr(2, 52, "Deambulatore");

  display.sendBuffer();

  emergency();

  oldpos = pos;
  pos = enc.read();

  while (oldpos <= pos) {

    emergency();

    if (premuto()) {
      temporanea = 2;
      Serial.println("PIGIATO");
      Serial.println(modalita);
      break;
    } else {
      temporanea = 0;
    }

    oldpos = pos;
    pos = enc.read();
  }
}

void cambia_modalita() {

  Serial.println("cambio modalita");

  while (modalita == 0) {  //finché non selezioni la modalità desiderata

    emergency();

    oldpos = pos;
    pos = enc.read();

    if (oldpos > pos) {  //senso orario
      visualizza_bastone();
    } else if (oldpos < pos) {
      visualizza_deambulatore();
    } else if (oldpos == pos && bast == true) {
      bast = false;
      delay(300);
      emergency();
      visualizza_bastone();
    }

    modalita = temporanea;

    Serial.println(modalita);
  }

  primo = 0;
  batt = true;
}

void visualizza_battiti() {

  if (unavolta == 0) {
    rileva_salute();
    unavolta++;
  } else {
    rileva_salute();
  }

  display.clearBuffer();

  emergency();

  display.setFont(u8g2_font_timB14_tr);

  display.drawStr(2, 25, "Battiti:");

  display.setFont(u8g2_font_timB18_tr);

  if (heartRate < 30 || heartRate > 250) {
    display.drawStr(75, 25, "--");
  } else if (heartRate >= 100) {
    itoa(heartRate, battitistr, 10);
    display.drawStr(70, 25, battitistr);
  } else {  //se i dati hanno senso
    itoa(heartRate, battitistr, 10);
    display.drawStr(80, 25, battitistr);
  }

  display.drawXBMP(110, 10, 16, 16, cuore);

  display.setFont(u8g2_font_timB14_tr);

  display.drawStr(2, 57, "SpO2:");

  display.setFont(u8g2_font_timB18_tr);

  if (spo2 < 80) {
    display.drawStr(75, 57, "--");
  } else if (spo2 == 100) {  //se i dati hanno senso
    itoa(spo2, sp02str, 10);
    display.drawStr(70, 57, sp02str);
  } else {
    itoa(spo2, sp02str, 10);
    display.drawStr(80, 57, sp02str);
  }

  emergency();

  display.setFont(u8g2_font_TimesNewPixel_tr);

  display.drawStr(112, 55, "%");

  display.sendBuffer();
}

void visualizza_temperatura() {

  sensore_temp.read(&temperature, &humidity, NULL);

  emergency();

  display.clearBuffer();

  display.setFont(u8g2_font_timB18_tr);

  display.drawXBMP(2, 11, 16, 16, sole);

  itoa((int)temperature, tempstr, 10);
  display.drawStr(25, 25, tempstr);

  display.drawStr(60, 25, "C");

  display.drawXBMP(2, 43, 16, 16, goccia);

  itoa((int)humidity, humstr, 10);
  display.drawStr(25, 57, humstr);

  display.drawStr(60, 57, "%");

  emergency();

  Serial.print("Temp: ");
  Serial.println(tempstr);

  Serial.print("Hum: ");
  Serial.println(humstr);

  display.sendBuffer();
}

void visualizza_ancora_battiti() {

  while (oldpos >= pos) {

    emergency();

    oldpos = pos;
    pos = enc.read();

    if (primo == 0) {
      visualizza_battiti();
      primo++;
      delay(500);
      emergency();
    } else {
      if (timerbattiti(30000)) {
        visualizza_battiti();
      }
      emergency();
    }

    if (premuto()) {
      emergency();
      modalita = 0;
      bast = true;
      cambia_modalita();
    }
  }
}

void visualizza_ancora_temperatura() {

  while (oldpos <= pos) {

    emergency();

    oldpos = pos;
    pos = enc.read();

    if (primo == 0) {
      visualizza_ancora_battiti();
    } else {
      if (mytimer(10000)) {
        visualizza_temperatura();
      }
      emergency();
    }

    if (premuto()) {
      emergency();
      modalita = 0;
      bast = true;
      cambia_modalita();
    }
  }
}*/
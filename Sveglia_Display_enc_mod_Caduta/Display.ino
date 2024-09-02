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

int ore = 10;
char orestr[3];
int minuti = 55;
char minstr[3];

int giorno = 10;
char ggstr[3];
int mese = 11;
char mesestr[3];
int anno = 2024;
char annostr[3];

int minutisveglia = 30;
char svminstr[3];
int oresveglia = 10;
char svorestr[3];

byte temperature = 0;
char tempstr[3];
byte humidity = 0;
char humstr[3];

int premuto(){

  if(!digitalRead(sw)){
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

  emergency();

  display.setFont(u8g2_font_timB14_tr);

  display.drawStr(2, 20, "MODALITA:");  //scrivi bastone a schermo

  display.drawStr(2, 52, "Bastone");

  display.sendBuffer();

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

  emergency();

  display.setFont(u8g2_font_timB14_tr);

  display.drawStr(2, 20, "MODALITA:");  //scrivi bastone a schermo

  display.drawStr(2, 52, "Deambulatore");

  display.sendBuffer();

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

  emergency();

  display.setFont(u8g2_font_timB14_tr);

  display.drawStr(2, 25, "Battiti:");

  display.setFont(u8g2_font_timB18_tr);

  if (battiti < 30 || battiti > 250) {
    display.drawStr(75, 25, "--");
  } else if (battiti >= 100) {
    itoa(battiti, battitistr, 10);
    display.drawStr(70, 25, battitistr);
  } else {  //se i dati hanno senso
    itoa(battiti, battitistr, 10);
    display.drawStr(80, 25, battitistr);
  }

  display.drawXBMP(110, 10, 16, 16, cuore);

  display.setFont(u8g2_font_timB14_tr);

  display.drawStr(2, 57, "SpO2:");

  display.setFont(u8g2_font_timB18_tr);

  emergency();

  if (sp02 < 80) {
    display.drawStr(75, 57, "--");
  } else if (sp02 == 100) {  //se i dati hanno senso
    itoa(sp02, sp02str, 10);
    display.drawStr(70, 57, sp02str);
  } else {
    itoa(sp02, sp02str, 10);
    display.drawStr(80, 57, sp02str);
  }

  display.setFont(u8g2_font_TimesNewPixel_tr);

  display.drawStr(112, 55, "%");

  display.sendBuffer();
}

void visualizza_temperatura() {

  sensore_temp.read(&temperature, &humidity, NULL);

  display.clearBuffer();

  emergency();

  display.setFont(u8g2_font_timB18_tr);

  display.drawXBMP(2, 9, 16, 16, sole);

  itoa((int)temperature, tempstr, 10);
  display.drawStr(50, 25, tempstr);

  display.drawStr(100, 25, "C");

  display.drawXBMP(2, 41, 16, 16, goccia);

  emergency();

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

  display.clearBuffer();

  emergency();

  display.setFont(u8g2_font_timB14_tr);

  display.drawXBMP(2, 11, 16, 16, orologio);

  itoa(ore, orestr, 10);
  display.drawStr(19, 25, orestr);
  display.drawStr(37, 23, ":");
  itoa(minuti, minstr, 10);
  display.drawStr(44, 25, minstr);

  display.drawXBMP(66, 11, 16, 16, campana);

  emergency();

  itoa(oresveglia, svorestr, 10);
  display.drawStr(83, 25, svorestr);
  display.drawStr(101, 23, ":");
  itoa(minutisveglia, svminstr, 10);
  display.drawStr(108, 25, svminstr);

  display.setFont(u8g2_font_timB18_tr);

  display.drawXBMP(2, 42, 16, 16, calendario);

  emergency();

  itoa(giorno, ggstr, 10);
  display.drawStr(19, 57, ggstr);
  display.drawStr(42, 57, "/");
  itoa(mese, mesestr, 10);
  display.drawStr(49, 57, mesestr);
  display.drawStr(72, 57, "/");
  itoa(anno, annostr, 10);
  display.drawStr(79, 57, annostr);

  display.sendBuffer();
}

void visualizza_ancora_battiti() {

  while (pos >= -10 && pos <= 10) {

    emergency();

    oldpos = pos;
    pos = enc.read();

    if (primo == 0) {
      visualizza_battiti();
      primo++;
      delay(500);
    } else {
      if (mytimer(1000)) {
        visualizza_battiti();
      }
    }

    emergency();

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

    emergency();

    oldpos = pos;
    pos = enc.read();

    if (primo == 0) {
      visualizza_ancora_battiti();
    } else {
      if (mytimer(1000)) {
        visualizza_temperatura();
      }
    }

    emergency();

    if (premuto()) {
      modalita = 0;
      bast = true;
      cambia_modalita();
    }
  }
}

void visualizza_ancora_data() {

  while (pos > 10) {

    emergency();

    oldpos = pos;
    pos = enc.read();

    if (primo == 0) {
      visualizza_ancora_battiti();
    } else {
      if (mytimer(1000)) {
        visualizza_data();
      }
    }

    emergency();

    if (premuto()) {
      modalita = 0;
      data = true;
      cambia_modalita();
    }
  }
}
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

  if (sp02 < 80) {
    display.drawStr(75, 57, "--");
  } else if (sp02 == 100) {  //se i dati hanno senso
    itoa(sp02, sp02str, 10);
    display.drawStr(70, 57, sp02str);
  } else {
    itoa(sp02, sp02str, 10);
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
      if (mytimer(1000)) {
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
      if (mytimer(1000)) {
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
}
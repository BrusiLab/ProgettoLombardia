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

void visualizza_bastone() {

  display.clearDisplay();
  display.clearBuffer();

  display.setFont(u8g2_font_callite24_tr);

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

  display.setFont(u8g2_font_callite24_tr);

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

  primo = 0;
  batt = true;
}

void visualizza_battiti() {

  display.clearBuffer();

  display.setFont(u8g2_font_callite24_tr);

  display.drawStr(2, 20, "Battiti:");

  if (battiti < 30 || battiti > 250) {
    display.drawStr(75, 20, "--");
  } else if (battiti >= 100) {
    itoa(battiti, battitistr, 10);
    display.drawStr(70, 20, battitistr);
  } else {  //se i dati hanno senso
    itoa(battiti, battitistr, 10);
    display.drawStr(75, 20, battitistr);
  }

  display.drawXBMP(110, 5, 16, 16, cuore);

  display.drawStr(2, 52, "Sp02:");

  if (sp02 < 80) {
    display.drawStr(75, 52, "--");
  } else if (sp02 == 100) {  //se i dati hanno senso
    itoa(sp02, sp02str, 10);
    display.drawStr(70, 52, sp02str);
  } else {
    itoa(sp02, sp02str, 10);
    display.drawStr(75, 52, sp02str);
  }

  display.setFont(u8g2_font_TimesNewPixel_tr);

  display.drawStr(112, 52, "%");

  display.sendBuffer();
}

void visualizza_temperatura() {

  sensore_temp.read(&temperature, &humidity, NULL);

  display.clearBuffer();
  display.setFont(u8g2_font_callite24_tr);

  itoa((int)temperature, tempstr, 10);
  display.drawStr(2, 52, tempstr);

  display.setFont(u8g2_font_TimesNewPixel_tr);
  display.drawStr(38, 52, "C");

  display.setFont(u8g2_font_callite24_tr);

  itoa((int)humidity, humstr, 10);
  display.drawStr(64, 52, humstr);

  display.setFont(u8g2_font_TimesNewPixel_tr);
  display.drawStr(100, 52, "%");

  Serial.print("Temp: ");
  Serial.println(tempstr);

  Serial.print("Hum: ");
  Serial.println(humstr);

  display.sendBuffer();
}

void visualizza_ancora_battiti() {

  while (oldpos >= pos) {

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
    
    if (premuto()) {
      modalita = 0;
      bast = true;
      cambia_modalita();
    }
  }
}

void visualizza_ancora_temperatura() {

  while (oldpos <= pos) {

    oldpos = pos;
    pos = enc.read();

    if (primo == 0) {
      visualizza_ancora_battiti();
    } else {
      if (mytimer(1000)) {
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
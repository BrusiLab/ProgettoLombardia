
char settimana[7][4] = { //giorni della settimana (bisogna cominciare l'arrey con il giorno della settimana in cui è partito per la prima volta il sensore)
  "dom", "lun", "mar", "mer", "gio", "ven", "sab"
};

void inizializza_data() { //inizializzat sensore

  if (!rtc.isrunning()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //usa data e ora attuali
    //rtc.adjust(DateTime(anno, mese, giorno, ora, minuti, secondi))
  }
}

void aggiorna_data() { //aggiorna data e ora

  DateTime now = rtc.now();

  anno = now.year()-2000; //togli 2000 all'ann per avere una scrittura più compatta (24 anziché 2024)
  mese = now.month();
  giorno = now.day();

  for (int primo_gg = 0; primo_gg < 4; primo_gg++) {
    gg_settimana[primo_gg] = settimana[now.dayOfTheWeek()][primo_gg]; //copia il giorno della settimana in una veriabile che andrà visualizzata sul display
  }

  ora = now.hour();
  minuto = now.minute();
}

bool premuteore = false;
long positivo;

void imposta_sveglia() {

  Serial.println("imposto sveglia");

  delay(500); //senza delay la pressione dell'encoder arriva anche nel codice dopo

  while (!premuto()) { //finché non si preme l'encoder

    while (!premuteore) { //finché non si preme l'encoder (però dopo averlo premuto una volta non rientra più qui finché non si richiama di nuovo imposta_sveglia)

      if (premuto()) {
        premuteore = true;
      }

      oldpos = pos;
      pos = enc.read();

      //se la posizione è negativa cambia il segno (arduino non fa il resto delle divisioni con numeri negtivi)

      if (pos != oldpos) {
        Serial.println(pos);
        if (pos <= 0) {
          positivo = -pos;
        } else {
          positivo = pos;
        }
      }

      //ogni intervallo di 8 corrisponde a un'ora, anche se superi le 23 torni a 00 perché il resto della divisione rientra nella prima categoria
      //finché non superi le ore 23 (posizione 200) il resto della divisione corrisponde alla posizione, poi ricomincia

      if (positivo % 200 >= 0 && positivo % 200 < 8) {
        oresveglia = 0;
      } else if (positivo % 200 >= 8 && positivo % 200 < 16) {
        oresveglia = 1;
      } else if (positivo % 200 >= 16 && positivo % 200 < 24) {
        oresveglia = 2;
      } else if (positivo % 200 >= 32 && positivo % 200 < 32) {
        oresveglia = 3;
      } else if (positivo % 200 >= 40 && positivo % 200 < 48) {
        oresveglia = 4;
      } else if (positivo % 200 >= 48 && positivo % 200 < 56) {
        oresveglia = 5;
      } else if (positivo % 200 >= 56 && positivo % 200 < 64) {
        oresveglia = 6;
      } else if (positivo % 200 >= 64 && positivo % 200 < 72) {
        oresveglia = 7;
      } else if (positivo % 200 >= 72 && positivo % 200 < 80) {
        oresveglia = 8;
      } else if (positivo % 200 >= 80 && positivo % 200 < 88) {
        oresveglia = 9;
      } else if (positivo % 200 >= 88 && positivo % 200 < 96) {
        oresveglia = 10;
      } else if (positivo % 200 >= 96 && positivo % 200 < 104) {
        oresveglia = 11;
      } else if (positivo % 200 >= 104 && positivo % 200 < 112) {
        oresveglia = 12;
      } else if (positivo % 200 >= 112 && positivo % 200 < 120) {
        oresveglia = 13;
      } else if (positivo % 200 >= 120 && positivo % 200 < 128) {
        oresveglia = 14;
      } else if (positivo % 200 >= 128 && positivo % 200 < 136) {
        oresveglia = 15;
      } else if (positivo % 200 >= 136 && positivo % 200 < 144) {
        oresveglia = 16;
      } else if (positivo % 200 >= 144 && positivo % 200 < 152) {
        oresveglia = 17;
      } else if (positivo % 200 >= 152 && positivo % 200 < 160) {
        oresveglia = 18;
      } else if (positivo % 200 >= 160 && positivo % 200 < 168) {
        oresveglia = 19;
      } else if (positivo % 200 >= 168 && positivo % 200 < 176) {
        oresveglia = 20;
      } else if (positivo % 200 >= 176 && positivo % 200 < 184) {
        oresveglia = 21;
      } else if (positivo % 200 >= 184 && positivo % 200 < 192) {
        oresveglia = 22;
      } else if (positivo % 200 >= 192 && positivo % 200 < 200) {
        oresveglia = 23;
      }

      //ogni volta visualizza l'ora che stiamo impostando come sveglia

      display.clearBuffer();

      display.setFont(u8g2_font_timB14_tr);

      display.drawStr(2, 25, "Imposta sveglia");

      display.drawXBMP(2, 42, 16, 16, campana);

      display.setFont(u8g2_font_timB18_tr);

      itoa(oresveglia, svorestr, 10);
      display.drawStr(39, 57, svorestr);
      display.drawStr(64, 55, ":");
      itoa(minutisveglia, svminstr, 10);
      display.drawStr(73, 57, svminstr);

      display.sendBuffer();

      //per confermare l'ora e passare alla selezione dei minuti premere l'encoder
    }

    delay(500); //solito delay per dare tempo di rilasciare la pressione dell'encoder

    oldpos = pos;
    pos = enc.read();

    //se la posizione è negativa cambia il segno (arduino non fa il resto delle divisioni con numeri negtivi)

    if (pos != oldpos) {
      Serial.println(pos);
      if (pos <= 0) {
        positivo = -pos;
      } else {
        positivo = pos;
      }
    }

    //scala i minuti di 15 in 15

    if (positivo % 32 >= 0 && positivo % 32 < 8) {
      minutisveglia = 0;
    } else if (positivo % 32 >= 8 && positivo % 32 < 16) {
      minutisveglia = 15;
    } else if (positivo % 32 >= 16 && positivo % 32 < 24) {
      minutisveglia = 30;
    } else if (positivo % 32 >= 24 && positivo % 32 < 32) {
      minutisveglia = 45;
    }

    //visualizza i mintui che stiamo impostando

    display.clearBuffer();

    display.setFont(u8g2_font_timB14_tr);

    display.drawStr(2, 25, "Imposta sveglia");

    display.drawXBMP(2, 42, 16, 16, campana);

    display.setFont(u8g2_font_timB18_tr);

    itoa(oresveglia, svorestr, 10);
    display.drawStr(39, 57, svorestr);
    display.drawStr(64, 55, ":");
    itoa(minutisveglia, svminstr, 10);
    display.drawStr(73, 57, svminstr);

    display.sendBuffer();
  }

  delay(500); //solito delay per dare tempo di rilasciare la pressione dell'encoder

  primo = 0;  //dopo torna a visualizzare i battiti
  batt = true;
}
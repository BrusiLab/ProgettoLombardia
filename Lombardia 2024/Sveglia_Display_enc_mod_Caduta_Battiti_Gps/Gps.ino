//copiato da libreria

float lat = 0.0;
float lon = 0.0;
unsigned long age;

int timergps(int ognitot) {  //timer non bloccante pe gps

  static unsigned long partenza = 0;
  static unsigned long trascorso;
  int voila = 0;

  emergency();

  trascorso = millis() - partenza;

  if (trascorso >= ognitot) {
    partenza = millis();
    voila = 1;
  }

  return voila;
}

void posizione(){

  display.clearBuffer();

  display.setFont(u8g2_font_timB14_tr); //mentre rileva la posizione mostra a schermo che lo stai facendo (altrimenti l'anziano non capisce perché non funziona l'encoder)

  display.drawStr(2, 25, "Rilevazione");
  display.drawStr(2, 57, "posizione...");

  display.sendBuffer();
  
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  for (unsigned long start = millis(); millis() - start < 1000;) {

    emergency();

    while (ss.available()) {

      char c = ss.read();

      if (gps.encode(c)) {
        newData = true;
      }
    }
  }

  if (newData == true) {

    emergency();

    gps.f_get_position(&lat, &lon, &age);

    Serial.print("LAT=");
    Serial.println(lat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : lat, 6);

    Serial.print(" LON=");
    Serial.println(lon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : lon, 6);

    Serial.print(" SAT=");
    Serial.println(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
  }    

  gps.stats(&chars, &sentences, &failed);

  if (chars == 0){
    emergency();
    Serial.println("** No characters received from GPS: check wiring **");
  }
}
//hardware gnd a ground, vcc a 5v, sda a sda, scl a scl

#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

int battiti = 100;  //valore casuale per prova
char battitistr[4];
int sp02 = 99;  //valore casuale per prova
char sp02str[4];

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

/*COMANDI LIBRERIA

clearBuffer(); //toglie buffer grafici
sendBuffer();  //invia disegni ??
clearDisplay(); //cancella tutto
drawStr(x, y, "ciao"); //scrive
drawXBMP(x,y, dimensionex ,dimensioney, matrice); //disegno
*/

void setup() {

  Serial.begin(9600);
  display.begin();
}

void loop() {

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

  delay(100);
}

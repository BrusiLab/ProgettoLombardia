//hardware gnd a ground, vcc a 5v, sda a sda, scl a scl

//hardare enc: pin ai pin dichiarati, + a 5V e - a gnd
//numeri scalano di 2 per ogni passo --> + senso antiorario, - senso orario

#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <Encoder.h>

#include <Wire.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include <SimpleDHT.h>

#define clock 2
#define encdt 3
#define sw 4
#define temp 13

SimpleDHT11 sensore_temp(temp);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0, /* reset=*/U8X8_PIN_NONE);
Encoder enc(clock, encdt);

int battiti = 100;  //valore casuale per prova
char battitistr[4];
int sp02 = 99;  //valore casuale per prova
char sp02str[4];

long oldpos;
long pos = 1;

bool batt = true;
bool tmp = false;
bool bast = false;

bool premutoenc = false;

byte temperature = 0;
char tempstr[3];
byte humidity = 0;
char humstr[3];

int modalita = 0;
int temporanea = 0;
int primo = 0;

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

void setup() {

  Serial.begin(9600);

  display.begin();

  pinMode(sw, INPUT_PULLUP);

  cambia_modalita();
  
}

void loop() {

  oldpos = pos;
  pos = enc.read();

  if (oldpos == pos) {  //se non cambio schermata visualizzo quella che c'era prima aggiornando i dati

    if (batt == true) {
      visualizza_ancora_battiti();
    } else if (tmp == true) {
      visualizza_ancora_temperatura();
    }
  }

  batt = false;
  tmp = false;

  if (oldpos > pos) {  //senso orario

    visualizza_battiti();
    batt = true;

  } else if (oldpos < pos) {

    visualizza_temperatura();
    tmp = true;

  } else if (premuto()) {

    modalita = 0;
    bast = true;
    cambia_modalita();
  }
}

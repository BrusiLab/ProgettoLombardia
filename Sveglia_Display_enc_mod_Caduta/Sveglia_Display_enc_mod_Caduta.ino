//hardware gnd a ground, vcc a 5v, sda a sda, scl a scl

//hardare enc: pin ai pin dichiarati, + a 5V e - a gnd
//numeri scalano di 2 per ogni passo --> + senso antiorario, - senso orario

#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <Encoder.h>

#include <Wire.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include <SimpleDHT.h>
#include "MPU6050_6Axis_MotionApps20.h"
#include <I2Cdev.h>

#define clock 2
#define encdt 3
#define sw 4
#define temp 13

#define disinnesco 8  //disattiva sveglia e allarme
#define emergenza 9   //attiva allarme
#define buzzer 10
#define led 11

#define laccio 13  //colleghi cavo da 5v a pin

#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards

MPU6050 mpu;
SimpleDHT11 sensore_temp(temp);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0, /* reset=*/U8X8_PIN_NONE);
Encoder enc(clock, encdt);

int battiti = 100;  //valore casuale per prova
char battitistr[4];
int sp02 = 99;  //valore casuale per prova
char sp02str[4];

long oldpos;
long pos = 1;

int secondi;
int unavolta = 0;

bool batt = true;
bool tmp = false;
bool bast = false;
bool data = false;

bool premutoenc = false;

int modalita = 0;
int temporanea = 0;
int primo = 0;

bool orario;            //attivato a ora sveglia
bool pericolo = false;  //attivato da pulsante emergenza o caduta
bool fermati;           //attivato da disinnesco -> ferma sveglia e allarme

void emergency(){
  if (digitalRead(emergenza) == true) {
    pericolo = true;
  }
  allarme();
}

void setup() {

  Serial.begin(9600);

  display.begin();

  pinMode(sw, INPUT_PULLUP);

  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(disinnesco, INPUT);
  pinMode(emergenza, INPUT);

  pinMode(laccio, INPUT);

  Wire.begin();

  inizializzaGyro();

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
    } else if (data == true) {
      visualizza_ancora_data();
    }
  }

  batt = false;
  tmp = false;
  data = false;

  Serial.println(pos);

  if (pos >=-10 && pos <= 10) {  //senso orario

    visualizza_battiti();
    batt = true;

  } else if (pos < - 10) {

    visualizza_temperatura();
    tmp = true;

  } else if (pos > 10){

    visualizza_data();
    data = true;

  } else if (premuto()) {

    modalita = 0;
    bast = true;
    cambia_modalita();
  }

  emergency();

  verifica_caduta();

  sveglia();
  allarme();

  digitalWrite(led, LOW);
}

//hardware gnd a ground, vcc a 5v, sda a sda, scl a scl

//hardare enc: pin ai pin dichiarati, + a 5V e - a gnd
//numeri scalano di 2 per ogni passo --> + senso antiorario, - senso orario

//Encoder
#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <Encoder.h>
//I2C
#include <Wire.h>
//Display
#include <Arduino.h>
#include <U8g2lib.h>
//Temperatura e umidità
#include <SimpleDHT.h>
//Giroscopio e accelerometro
#include "MPU6050_6Axis_MotionApps20.h"
#include <I2Cdev.h>
//Gps
#include <SoftwareSerial.h>
#include <TinyGPS.h>
//Battiti
#include "MAX30105.h"
#include "spo2_algorithm.h"

//Encoder
#define clock 3
#define encdt 4
#define sw 5
//Temperatura e umidità
#define temp 6
//Pulsanti
#define disinnesco 7  //disattiva sveglia e allarme
#define emergenza 8   //attiva allarme
//Sveglie
#define buzzer 9
#define led 10
//Caduta
#define laccio 13  //colleghi cavo da 5v a pin
#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards
//Gps
#define TX 1
#define RX 0
//Ossigenazione
#define pulseLED 11 //Must be on PWM pin
#define readLED 12 //Blinks with each data read
//Qualità aria
#define pinCO A2
#define pinNH3 A1
#define pinNO2 A0
#define pinozono A3

MPU6050 mpu;

SimpleDHT11 sensore_temp(temp);

U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

Encoder enc(clock, encdt);

TinyGPS gps;
SoftwareSerial ss(RX, TX);

MAX30105 particleSensor;

long oldpos;
long pos = 1;

int secondi;
int unavolta = 0;

bool batt = true;
bool tmp = false;
bool bast = false;
bool data = false;

int modalita = 0;
int temporanea = 0;
int primo = 0;

bool premutoenc = false;

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
  ss.begin(9600);
  display.begin();

  pinMode(sw, INPUT_PULLUP);

  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(disinnesco, INPUT);
  pinMode(emergenza, INPUT);

  pinMode(laccio, INPUT);

  pinMode(pulseLED, OUTPUT);
  pinMode(readLED, OUTPUT);

  Wire.begin();

  inizializzaBattiti();

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

  emergency();

  batt = false;
  tmp = false;
  data = false;

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

  if(timergps(600000)){
    posizione();
  }
  
  if(timeraria(600000)){
    rileva_aria();
  }
  
  sveglia();
  allarme();

  digitalWrite(led, LOW);
}

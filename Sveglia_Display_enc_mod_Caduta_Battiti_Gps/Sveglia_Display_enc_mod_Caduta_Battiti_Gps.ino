//Encoder
#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <Encoder.h>
//I2C
#include <Wire.h>
#include <SPI.h>
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
//Data e ora
#include <RTClib.h>

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
//Display
#define display_addr 0x3C
//Caduta
#define laccio 13          //colleghi cavo da 5v a pin
#define INTERRUPT_PIN 2    //use pin 2 on Arduino Uno & most boards
#define MPU6050_addr 0x69  //modificare indirizzo da 0x68 a 0x69 collegando ad0 a 5v
//Gps
#define TX 1
#define RX 0
//Ossigenazione
#define pulseLED 11  //Must be on PWM pin
#define readLED 12   //Blinks with each data read
//Qualità aria
#define pinCO A2
#define pinNH3 A1
#define pinNO2 A0
#define pinozono A3

//Giroscopio
MPU6050 mpu;
//sensore temperatura e umidità
SimpleDHT11 sensore_temp(temp);
//display
U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);
//encoder
Encoder enc(clock, encdt);
//gps
TinyGPS gps;
SoftwareSerial ss(RX, TX);
//sensore di battiti
MAX30105 particleSensor;
//orologio
RTC_DS1307 rtc;

long oldpos;          //posizione precdente encoder
long pos = 1;         //attuale posizione encoder

int secondi;          //variabile per countdown prima di inviare mail di allarme
int unavolta = 0;     //varibaile per prendere più misurazione per i battiti la prima volta

bool batt = true;     //variabili usate per visualizzare sempre la stessa schermata se non si muove encoder
bool tmp = false;
bool bast = false;
bool data = false;

int modalita = 0;     //modalità -> 1 = bastone, 2 = deambulatore
int temporanea = 0;   //stesso valore di modalità
int primo = 0;        //variabile usata per tornare alla schermata dei battiti sul display dopo aver cambiato modalità o impostato sveglia

bool premutoenc = false;

bool orario = false;  //attivato a ora sveglia
bool pericolo = false;//attivato da pulsante emergenza o caduta
bool fermati;         //attivato da disinnesco -> ferma sveglia e allarme

int anno;             //anno corrente
int mese;             //mese corrente
int giorno;           //giorno corrente
char gg_settimana[4]; //arrey contenente giorni della settimana
int ora;              //ora corrente
int minuto;           //minuto corrente

char orestr[3];       //stringhe delle variabili (il display accetta solo stringhe)
char minstr[3];
char ggstr[3];
char mesestr[3];
char annostr[3];

int minutisveglia = 0;    //minuti a cui è impostata la sveglia
char svminstr[3];
int oresveglia = 25;      //ore a cui è impostata la sveglia (inizializzato a 25 quando non c'è nessuna sveglia)
char svorestr[3];

void emergency() {        //funzione che rileva se viene schiacciato il pulsante di emergenza e richiama l'allarme (spammata ovunque)
  
  if (digitalRead(emergenza) == true) { //se viene schiacciato il pulsante di emergenza -> attiva pericolo
    pericolo = true;
  }
  allarme();
}

void setup() {

  Serial.begin(9600); //inizializzazione della seriale a 9600 baud

  while (!Serial)     //attendi finché non si è inizializzata la seriale
    ;

  Wire.begin();       //inizializza la comunicazione I2C per il sensore di battiti, il display, il giroscopio e l'orologio

  ss.begin(9600);     //inizializza la software serial per il gps a 9600 baud

  display.begin();    //inizializza il display

  pinMode(sw, INPUT_PULLUP);  //pin che rileva se l'encoder viene premuto

  pinMode(buzzer, OUTPUT);    //pin buzzer
  pinMode(led, OUTPUT);       //pin led
  pinMode(disinnesco, INPUT); //pulsante per disattivare allarmi e sveglie
  pinMode(emergenza, INPUT);  //pulsante di emergenza

  pinMode(laccio, INPUT);     //laccio per controllare se si è attaccati al dispositivo

  pinMode(pulseLED, OUTPUT);  //pin per il sensore di battiti
  pinMode(readLED, OUTPUT);

  inizializzaBattiti();       //inizializzazione sensore di battiti

  inizializzaGyro();          //inizializzazione giroscopio

  inizializza_data();         //inizializza orologio

  cambia_modalita();          //visualizza la schermata in cui selezionare la modalità
}

void loop() {

  oldpos = pos;       //associa la posizione dell'encoder a oldpos
  pos = enc.read();   //aggiorna la posizione dell'encoder

  if (oldpos == pos) {  //se non cambio schermata visualizzo quella che c'era prima 

    if (batt == true) {
      visualizza_ancora_battiti();
    } else if (tmp == true) {
      visualizza_ancora_temperatura();
    } else if (data == true) {
      visualizza_ancora_data();
    }
  }

  emergency();    //questa funzione sarà spammata nel codice in modo da rilevare quando viene premuto il pulante

  batt = false;   //azzera le variabili che indicano che schermata si stava visualizzando
  tmp = false;
  data = false;

  if (pos >= -10 && pos <= 10) {  //se la posizione dell'encder si trova tra -10 e 10 visualizza la schermata con battiti e ossigenazione

    visualizza_battiti();         //visualizza la scherata dei battiti
    batt = true;                  //attiva la booleana per tenere il memoria che si stanno visualizzando i battiti

  } else if (pos < -10) {

    visualizza_temperatura();
    tmp = true;

  } else if (pos > 10) {

    visualizza_data();
    data = true;

  } else if (premuto()) {       //se l'encoder viene premuto si va alla schermata per cambiare modalità 
                                //(in teoria il codice passa raramente di qui perché si svolge tutto nelle funzioni visua_ancora_---)
    modalita = 0;
    bast = true;                //modalità torna a 0 e si attiva bast per visualizzare prima la modalità bastone
    cambia_modalita();
  }

  emergency();

  verifica_caduta();            //questa funzione verifica un eventuale caduta ogni volta che arduino ripete il loop

  if (timergps(60000)) {        //rileva la posizione ogni minuto (timer non bloccante)
    posizione();
  }

  if (timeraria(60000)) {       //rileva la qualità dell'aria ogni minuto (timer non bloccante)
    rileva_aria();
  }

  sveglia();                    //richiama le funzioni sveglia e allarme che si attivano solo se è l'ora della sveglia o una caduta o il pulsante di emergenza hanno attivato un pericolo
  allarme();

  digitalWrite(led, LOW);       //spegni il led perché a volte rimane acceso dopo gli allarmi
}

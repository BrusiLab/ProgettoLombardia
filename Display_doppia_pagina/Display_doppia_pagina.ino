//hardware gnd a ground, vcc a 5v, sda a sda, scl a scl

//hardare enc: pin ai pin dichiarati, + a 5V e - a gnd
//numeri scalano di 2 per ogni passo --> + senso antiorario, - senso orario

#include <Wire.h>
#include <Encoder.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define clock 2
#define dt 3
#define sw 4

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C  //se non va cambialo in 0x3D

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Encoder enc(clock, dt);

int battiti = 90;  //valore casuale per prova
int sp02 = 99;     //valore casuale per prova

long oldpos;
long pos = 1;

bool batt = false;
bool oss = false;

static const unsigned char cuore[] =  //disegno cuore
  { 0b00000000, 0b00000000,
    0b00011100, 0b00111000,
    0b00111110, 0b01111100,
    0b01111111, 0b11111110,
    0b01111111, 0b11111110,
    0b01111111, 0b11111110,
    0b01111111, 0b11111110,
    0b01111111, 0b11111110,
    0b00111111, 0b11111100,
    0b00111111, 0b11111100,
    0b00011111, 0b11111000,
    0b00001111, 0b11110000,
    0b00000111, 0b11100000,
    0b00000011, 0b11000000,
    0b00000001, 0b10000000,
    0b00000000, 0b00000000 };

void setup() {

  Serial.begin(9600);

  /*if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("Error");
    while (true)
      ;
  }

  display.display();
  display.clearDisplay();*/

  pinMode(sw, INPUT_PULLUP);
}

void visualizzabattiti() {

  display.clearDisplay();
  display.setTextSize(8);
  display.setTextColor(1);

  display.drawBitmap(0, 24, cuore, 16, 16, 1);

  display.setCursor(20, 0);

  if (battiti < 30 || battiti > 250) {
    display.print("--");
  } else {  //se i dati hanno senso
    display.print(battiti);
  }

  display.display();
}

void visualizzasp02() {

  display.clearDisplay();
  display.setTextSize(8);
  display.setTextColor(1);

  display.print("Sp02: ");

  if (sp02 < 80) {
    display.print("--");
  } else {  //se i dati hanno senso
    display.print(sp02);
  }

  display.print("%");

  display.display();
}

void loop() {

  oldpos = pos;
  pos = enc.read();

  if (oldpos == pos) {
    if (batt == true) {
      visualizzabattiti();
    } else if (oss == true) {
      visualizzasp02();
    }
  }

  batt = false;
  oss = false;

  if (oldpos > pos) {  //senso orario
    visualizzabattiti();
    batt = true;
  } else if (oldpos < pos) {
    visualizzasp02();
    oss = true;
  }
}
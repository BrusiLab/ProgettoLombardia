//hardware gnd a ground, vcc a 5v, sda a sda, scl a scl

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET -1 
#define SCREEN_ADDRESS 0x3C //se non va cambialo in 0x3D

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int battiti = 90; //valore casuale per prova
int sp02 = 99;    //valore casuale per prova

static const unsigned char cuore[] = //disegno cuore
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

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("Error");
    while(true); 
  }

  display.display();
  display.clearDisplay();

}

void loop() {

  display.clearDisplay();
  display.setTextSize(4); //se è piccolo scrivere 4 al posto di due (dovrebberp starci 2 righe sul display)
  display.setTextColor(WHITE);

  display.drawBitmap(0, 16, cuore, 16, 16, 1); //se non funziona scrivere WHITE o INVERSE al posto di 1
                                              //se è troppo attaccato al bordo modificare le coordinate (i primi due parametri sono x e y da scrivere in pixel)

  display.setCursor(40,0);  //lascai un po' di spazio dopo il cuore

  if(battiti < 30 || battiti > 250){  
    display.print("--");
  } else {                              //se i dati hanno senso
    display.print(battiti);
  }
  
  display.display();

  display.setTextSize(4); //se è piccolo scrivere 4 al posto di due (dovrebberp starci 2 righe sul display)
  display.setTextColor(WHITE);

  display.setCursor(0,40);  //vai a capo (se coordinate non giuste modificare)
  display.print("Sp02: ");

  if(sp02 < 80){
    display.print("--");
  } else {                  //se i dati hanno senso
    display.print(sp02);
  }

  display.print("%");
  
  display.display();

}

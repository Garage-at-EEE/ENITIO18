#include <Adafruit_NeoPixel.h>

#define bigGap 300
#define DEBUG 1// 1 for DEBUG, 0 for PLAY

int initialReading[16];
int newReading[16];
int NeoPixelPin = 4;
int BuzzerPin = 3;
int LazerPin = 5;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(3, NeoPixelPin, NEO_GRB);

void setup() {

  strip.begin();
  strip.show();

  pinMode(LazerPin, OUTPUT); //on laser
  digitalWrite(LazerPin, HIGH);

  Serial.begin(9600);

  green(); //initial colour
  for (int i = 22; i < 53; i+=2) {
    pinMode(i, INPUT_PULLUP); //5V to LDR
  }





  for (int j = 0; j < 16; j++) {
    initialReading[j] = analogRead(j); //set base value
  }
}

void loop() {
  if (isTrip() ) {


    digitalWrite(LazerPin, LOW);
    annoyingTrigger();
  }
}

bool isTrip () {
  for (int k = 0; k < 16; k++) {
    newReading[k] = analogRead(k);
#if DEBUG
    Serial.print("Taking Reading ");
    Serial.print(k);
    Serial.print("\t");
    Serial.print(initialReading[k]);
    Serial.print("\t");
    Serial.println(newReading[k]);
#endif

    if (newReading[k] - initialReading[k] > bigGap || initialReading[k] - newReading[k] > bigGap) {
#if DEBUG
      Serial.print("TRIPPED GAME LOST");
#endif

      return true;
    }
  }
  return false;
}

void annoyingTrigger() {


  while (1) {
    for (int i = 0; i < strip.numPixels(); i++)
      strip.setPixelColor(i, strip.Color(5, 0, 0));
    strip.show();//On red
    
    for (int hz = 440; hz < 1000; hz++) { //annoying buzzer
      tone(BuzzerPin, hz, 50);
      delay(1);
    }
    noTone(BuzzerPin);
    for (int i = 0; i < strip.numPixels(); i++)
      strip.setPixelColor(i, strip.Color(0, 0, 0));
    strip.show();//Off red 
    
    for (int hz = 440; hz < 1000; hz++) { //annoying buzzer
      tone(BuzzerPin, hz, 50);
      delay(1);
    } noTone(BuzzerPin);


  }
}

void green() {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 5, 0));
    delay(1000);
    strip.show();
  }

}

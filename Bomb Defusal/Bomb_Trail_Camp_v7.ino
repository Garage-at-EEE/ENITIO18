/*This version buzzer works when timer reaches 0
 * Switch is included
 * AnalogRead for redSwitch
 * Screen of timer only displayed when redSwitch is flipped
 * Change 4th resistor to 120k ohm
 * Timelimit:30min
 */
#define w1 A0
#define w2 A1
#define w3 A2
#define w4 A3
#define b1 3
#define b2 4
#define b3 5
#define b4 6
#define buzzer 7
#define redled1 8 // for button
#define greenled1 9
#define redled2 10  // for wire
#define greenled2 11
#define CLK 12
#define DATA 13
#define redSwitch A4
#define switchLed A5


#define numberOfSeconds(_time_) ((_time_ / 1000) % 60)
#define numberOfMinutes(_time_) ((((_time_) / 1000) /60) % 60)

#include <TM1637Display.h>

int wireReading1 = 0;
int wireReading2 = 0;
int wireReading3 = 0;
int wireReading4 = 0;
int greenledCount = 0;
bool isBothGameFin = false;
bool hasGameStarted = false;

int pressCount = 0; //initialise to zero
int posCount = 0;
int combination[] = {1, 4, 4, 3}; //reference combination
int userInput[] = {0, 0, 0, 0}; //4 elements in the array

int switchRead;
int ledRead;

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 1000;

uint8_t OFF[] = {0, 0, 0, 0};
uint8_t END[] = {B00111111, B00111111, B00111111, B00111111};

TM1637Display display(12, 13);  //(CLK,DATA)

unsigned long timeLimit = 1800000; //30min

void setup() {
  // put your setup code here, to run once:
  pinMode(greenled1, OUTPUT);
  pinMode(redled1, OUTPUT);

  pinMode(greenled2, OUTPUT);
  pinMode(redled2, OUTPUT);

  pinMode(w1, INPUT_PULLUP);
  pinMode(w2, INPUT_PULLUP);
  pinMode(w3, INPUT_PULLUP);
  pinMode(w4, INPUT_PULLUP);

  pinMode(b1, INPUT_PULLUP);
  pinMode(b2, INPUT_PULLUP);
  pinMode(b3, INPUT_PULLUP);
  pinMode(b4, INPUT_PULLUP);

  pinMode(redSwitch, INPUT_PULLUP);
  pinMode(switchLed, OUTPUT);

  pinMode(buzzer, OUTPUT);
  
  lastDebounceTime = millis();

  //set brightness
  display.setBrightness(0x0c);
  //clear the display
  display.setSegments(OFF);

  digitalWrite(redled1, LOW);
  digitalWrite(greenled1, LOW);
  digitalWrite(redled2, LOW);
  digitalWrite(greenled2, LOW);
  Serial.begin(9600);
  
}

void loop() {
  switchRead = analogRead(redSwitch);
  
  /* This part control the led light on the red switch
   * It also checks if the switch for the game is triggered
   */
  if (switchRead < 100) {
    ledRead = 900;
    hasGameStarted = true;

  } else if (switchRead > 900)  {
    ledRead = 0;
  }
  analogWrite(switchLed, ledRead);

if (hasGameStarted) {
  checkStatusOfGame();
    if (isBothGameFin) {
      display.showNumberDec(100);  // show the next location

    } else {
      countdown();
    }

    wireReading1 = analogRead(w1);
    wireReading2 = analogRead(w2);
    wireReading3 = analogRead(w3);
    wireReading4 = analogRead(w4);

    int buttonReading1 = digitalRead(b1);
    int buttonReading2 = digitalRead(b2);
    int buttonReading3 = digitalRead(b3);
    int buttonReading4 = digitalRead(b4);


    //code for button
    if ((millis() - lastDebounceTime) > debounceDelay) {
      if (!buttonReading1) {  //if b1 is pressed
        lastDebounceTime = millis();
        userInput[pressCount] = 1;
        Serial.print('1');
        pressCount++;
      }
      else if (!buttonReading2) {  //if b1 is pressed
        lastDebounceTime = millis();
        userInput[pressCount] = 2;
        Serial.print('2');
        pressCount++;
      }
      else if (!buttonReading3) {  //if b1 is pressed
        lastDebounceTime = millis();
        userInput[pressCount] = 3;
        Serial.print('3');
        pressCount++;
      }
      else if (!buttonReading4) {  //if b1 is pressed
        lastDebounceTime = millis();
        userInput[pressCount] = 4;
        Serial.print('4');
        pressCount++;
      }

    }

    if (pressCount == 4) { //pressCount start from zero
      for (int i = 0; i <= 3; i++) {
        Serial.print(userInput[i]);
      }
      if (isArrayCorrect()) {
        digitalWrite(greenled1, HIGH);
        Serial.println("Correct Array!");
      } else {
        digitalWrite(redled1, HIGH);
        timeLimit = timeLimit - 30000; //deduct 30s
        resetForButton();
      }

    }
    //code for wire
    if ( (49 < wireReading1) && (wireReading1 < 151) && (199 < wireReading2) && (wireReading2 < 301) && (499 < wireReading3) && (wireReading3 < 601) && (699 < wireReading4) && (wireReading4 < 801)) {
      Serial.println("Correct Wire Combination!");
      digitalWrite(greenled2, HIGH);
      digitalWrite(redled2, LOW);

    } else {
      //Serial.println("Wrong Wire Combination!");
      digitalWrite(redled2, HIGH);
      digitalWrite(greenled2, LOW);

    }
  }

}

void countdown() {
  unsigned long timeRemaining = timeLimit - millis();

  int seconds = numberOfSeconds (timeRemaining);
  int minutes = numberOfMinutes(timeRemaining);

  //display seconds in the last two places
  display.showNumberDecEx(seconds, 0, false, 2, 2);
  //display minutes in the first two places
  display.showNumberDecEx(minutes, 0x80 >> 3, false, 2, 0);

  //Update the time remaining
  timeRemaining = timeLimit - millis();

  if (timeRemaining< 1000) {
    tone(buzzer,1000);
    display.setSegments(END);
  }
}

bool checkStatusOfGame() {
  if ((digitalRead(greenled1) == HIGH) && (digitalRead(greenled2) == HIGH)) {
    isBothGameFin = true;

  } else {
    isBothGameFin = false;

  }
}

bool isArrayCorrect() {
  Serial.println();
  Serial.println("Checking...");
  Serial.print("User Input: ");
  for (int i = 0; i <= 3; i++) {
    Serial.print(userInput[i]);
  }

  for (int i = 0; i <= 3; i++) {
    if (userInput[i] != combination[i]) {
      Serial.println("FAIL");
      for (int i = 0; i <= 3; i++) {
        userInput[i] = 0;
      }
      return false;
    }
  }
  Serial.println("SUCCESS");

  return true;
}

void resetForButton() {
  digitalWrite(redled1, LOW);
  digitalWrite(greenled1, LOW);
  for (int i = 0; i <= 3; i++) {
    userInput[i] = 0;
  }
  pressCount = 0;
  posCount = 0;
}

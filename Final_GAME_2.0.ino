#include <Adafruit_CircuitPlayground.h>
#include <Wire.h>
#include <SPI.h>

volatile int score = 0; //score.

int myDelay = 500; //player control and difficulty.
int player;

float midi[127];
int A_four = 440; //440 hz
int song[4][2] = {
    {82, 400},  // song[0][0]], song[0][1]]
    {81, 400},  // song[1][0]], song[1][1]]
    {80, 400},  // song[2][0]], song[2][1]]
    {79, 900}  // song[3][0]], song[3][1]]
  };
const uint8_t spBRAVO[]         PROGMEM = {0x61,0x5A,0xBA,0xC2,0xDD,0x62,0x85,0xD6,0xE8,0x15,0x59,0xB1,0x97,0x9A,0x30,0xD5,0xBC,0x85,0xDF,0xA8,0x63,0x0F,0xE9,0x50,0xE5,0xA7,0xCA,0x6E,0x22,0x5D,0x57,0xEF,0x72,0x97,0xB3,0x2A,0x6D,0x74,0x15,0xE9,0xBA,0x3A,0xF6,0x66,0xE8,0x3E,0xD4,0x5C,0x65,0xD7,0x31,0x2D,0x95,0x54,0xBB,0x8B,0xDF,0xD9,0xAE,0xB1,0xA1,0xAC,0x0E,0x51,0x3F,0xE7,0xB6,0x14,0xD2,0x35,0x4E,0xEE,0xFB,0x5E,0x77,0xB3,0x7B,0xDF,0x19,0x2C,0x7D,0xEC,0xE9,0x2F,0x73,0x05,0xDF,0x19,0x2C,0x7D,0xF8,0xF3,0xFF};
const uint8_t spPAPA[]          PROGMEM = {0x0A,0x70,0x4A,0xB5,0xA5,0x45,0x55,0x84,0x49,0xCC,0x93,0x66,0xD7,0x19,0x26,0x4B,0x4E,0x96,0xDD,0x44,0xBA,0xAE,0xBE,0xD9,0xCC,0x10,0x28,0x42,0xB9,0x8B,0xC8,0x06,0x60,0x80,0xF1,0xE9,0xAB,0xCA,0xA6,0x23,0xD4,0x36,0xDF,0xE1,0x8C,0x55,0x74,0x86,0x6B,0x9F,0xB1,0x67,0xBD,0xE1,0xE6,0xBB,0xDB,0x97,0x53,0x45,0x88,0xCF,0xAE,0xDF,0xFF,0x03};

int buttonLeft = 4;
int buttonRight = 5;
const int switchPin = 7;

volatile bool switchFlag = 0;
volatile bool buttonLeftPressed = 0;
volatile bool buttonRightPressed = 0;
bool switchVal = 0;


void setup() {
  CircuitPlayground.begin();
  pinMode(switchPin, INPUT_PULLUP);
  pinMode(buttonLeft, INPUT_PULLDOWN); //CircuitPlayground actually sets the pins automatically, but for good practice.
  pinMode(buttonRight, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(switchPin), onOFF, CHANGE);
  attachInterrupt(digitalPinToInterrupt(buttonLeft), fun, RISING);
  attachInterrupt(digitalPinToInterrupt(buttonRight), notfun, RISING);
  switchVal = digitalRead(switchPin);
  generateMIDI();
  Serial.begin(9600);
  }


void loop() {

if(switchFlag) {
  delay(5);
  switchVal = digitalRead(switchPin);
  switchFlag = 0;
}

  if(switchVal) {

    CircuitPlayground.setPixelColor(4, 0xFFA500);
    CircuitPlayground.setPixelColor(5, 0xFFA500);

   for(player = 0; player < 10; player++) {
      if(player == 5 && buttonLeftPressed || player == 6 && buttonLeftPressed) {
        score++;
        Serial.print("Your score is: ");
        Serial.println(score);
        myDelay = myDelay/1.25;
        buttonLeftPressed = 0;
    }
      else if(player != 5 && buttonLeftPressed || player != 6 && buttonLeftPressed) {
        lose();
        buttonLeftPressed = 0;

    }
    CircuitPlayground.setPixelColor(player, 0, 255, 0);
    delay(myDelay);
    CircuitPlayground.setPixelColor(player, 0, 0, 0);

      CircuitPlayground.setPixelColor(4, 0xFFA500);
      CircuitPlayground.setPixelColor(5, 0xFFA500);
  }
    if(score == 10) {
      CircuitPlayground.speaker.say(spBRAVO);
      CircuitPlayground.speaker.say(spPAPA);
      score++;
      Serial.print("Here's a free level: ");
      Serial.println(score);
    }
  }

  if(!switchVal) {
    CircuitPlayground.clearPixels();
    myDelay = 500;
    score = 0;
  }

  if(buttonRightPressed) {
    score = 0;
    myDelay = 500;
    Serial.println("Game has been reset. Your score count is 0.");
    buttonRightPressed = 0;
  }
}


void lose() {
  CircuitPlayground.setPixelColor(4, 255, 0, 0);
  CircuitPlayground.setPixelColor(5, 255, 0, 0);
  Serial.println("You lose! Your score count is now 0! ");
  for(int i = 0; i < sizeof(song) / sizeof(song[0]); i++) // Calculate how many rows are in the array using: sizeof(song) / sizeof(song[0])
  {
    CircuitPlayground.playTone(midi[song[i][0]], song[i][1]);
    //Serial.println(midi[song[i][0]]);
    delay(1);
  }
  delay(1500);
  myDelay = 500;
  score = 0;
}

void fun() {
  buttonLeftPressed = 1;

}

void notfun() {
  buttonRightPressed = 1;
}

void onOFF() {
  switchFlag = 1;
}

void generateMIDI()
{
  for (int x = 0; x < 127; ++x)
  {
    midi[x] = (A_four / 32.0) * pow(2.0, ((x - 9.0) / 12.0));
    Serial.println(midi[x]);
  }
}
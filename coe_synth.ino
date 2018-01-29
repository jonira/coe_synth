#include "lib/TimerOne.h"

#define inputPin A5
#define outputSignalP 3
#define pot A1
#define pot2 A2
#define pot3 A3

#define b1P 2
#define b2P 3

#define DUMP

void setup() {
  // put your setup code here, to run once:
#ifdef DUMP
  Serial.begin(115200);
#endif
  Timer1.initialize(16 * 3);
  Timer1.attachInterrupt(makeSound);     
  pinMode(outputSignalP, OUTPUT);
  pinMode(11, OUTPUT);

  pinMode(b1P, INPUT_PULLUP);
  pinMode(b2P, INPUT_PULLUP);
  
  TCCR2A = _BV(COM2A1) /* | _BV(COM2B1) */ | _BV(WGM20);
  TCCR2B = _BV(CS20);
  OCR2A = 180;
  // OCR2B = 50;
}

struct Sound {
  int k;
  int d;
  int id;
  unsigned char type;
  unsigned typeBeingSelected;
  int i;
  int count;
  int b1;
  int b2;
};

struct Sound snd;

void sound() { //ound(float k, int d, int id) {  
//  for (int i = 0; i < 1000; i++) {
  if (snd.i < 1000) {
    OCR2A = constrain( snd.i & 511 > 255 ? 512 - snd.i % (int)snd.k * 50 : 0 , 0, 255);
    ++snd.count;
    if (snd.count > snd.id/6) {
      snd.count = 0;
      snd.i++;
    }
  } else {
//    OCR2A = (snd.count >> (snd.d & 3)) & 1 ? 255 : 0;
    ++snd.count;
    if (snd.count >= 62 * snd.d) {
      snd.i = 0; 
    }
  }
}

void sound2() { //ound(float k, int d, int id) {  
//  for (int i = 0; i < 1000; i++) {
  if (snd.i < 500) {
    OCR2A = constrain( snd.i & 511 > 255 ? 317 * (snd.i * snd.k) : 512 - snd.id , 0, 255);
    ++snd.count;
    if (snd.count > snd.id/6) {
      snd.count = 0;
      snd.i++;
    }
  } else {
//    OCR2A = (snd.count >> (snd.d & 3)) & 1 ? 255 : 0;
    ++snd.count;
    if (snd.count >= 262 * snd.d) {
      snd.i = 0; 
    }
  }
}

void sound3() {
  OCR2A = snd.i > 255 ? 512 - snd.i : snd.i;
  snd.i++;
  if (snd.i > 511) snd.i = 0;
}

const unsigned pitches[] = {
   238, 225, 212, 200, 189, 178, 168, 159, 150, 142, 134, 126, 119
};

void sound4() {
  // k [0-511] selects note
  // d [0-255] selects speed (not yet)
  // id [0-511] unused
  // count is current sample time
  // i is wave pos of current sound, range [0,b1*2[
  // b1 is currently playing sound pitch
  
  ++snd.count;
  ++snd.i;
  if (snd.i > snd.b1 * 2) {
    // note switch
    snd.i = 0;
    snd.b1 = pitches[snd.k / 43];
  }
  OCR2A = snd.i > snd.b1 ? 0 : 255;
}

#define SND_TYPES 4

void makeSound() {
  if (snd.b1 == LOW) {
    if (snd.typeBeingSelected == 0) {
      memset(&snd, 0, sizeof(snd)); // reset sound state
      ++snd.type;
      snd.type %= SND_TYPES;
    }
    snd.typeBeingSelected = 1;
  } else {
    snd.typeBeingSelected = 0;
  }

  switch (snd.type) {
    case 0: sound(); break;
    case 1: sound2(); break;
    case 2: sound3(); break;
    case 3: sound4(); break;
  }
}

#ifdef DUMP
#define ar(x) dump(analogRead(x))
#define dr(x) dump(1000 * digitalRead(x))

int dump(int value) {
  Serial.print(value);
  Serial.print(" ");
  return value;
}
#else
#define ar(x) analogRead(x)
#define dr(x) digitalRead(x)
#endif

void loop() {
  // put your main code here, to run repeatedly:
//int input = analogRead(inputPin);
//Serial.println(input);
 //Serial.println(input * 5. / 1023);
//Serial.println(analogRead(pot));
 
 //OCR2A = 0;
// delay(1);
// OCR2A = 255;
// delay(4); 
while(true) {
  snd.k = ar(pot) / 2;
  snd.d = ar(pot2) / 4;
  snd.id = ar(pot3) / 2;
  snd.b1 = dr(b1P);
  snd.b2 = dr(b2P);
  //sound(potv / 2, potv2 / 4, potv3 /2);
  #ifdef DUMP
  Serial.println();
  #endif
}
//sound(1., 1);
//sound(2., 2);
//sound(3., 3);
//sound(0.1, 1);
//sound(0.5, 1);
//sound(1., 1);
//sound(2., 1);
//sound(3., 1);


// for(int i = 0; i < 32767; i++) {
//   OCR2A = constrain( i % 512 > 255 ? 512 - i % 512 : i % 512 , 0, 255) / 2 * 2;
//   delayMicroseconds(160);
//    //Serial.println(OCR2A - 127);
// }

}

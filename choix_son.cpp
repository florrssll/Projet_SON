#include <Audio.h>
#include <Serial.h>

// Code Faust généré 
#include "mydsp.h"

AudioPlaySdWav           playWav;
AudioEffectFaust         faustEffect; // Objet Faust généré
AudioOutputI2S           audioOutput;
AudioConnection           patchCord1(playWav, 0, faustEffect, 0);
AudioConnection           patchCord2(faustEffect, 0, audioOutput, 0);

const char* sons[] = {"son1.wav", "son2.wav", "son3.wav"};
int indexSon = 0;

void setup() {
  Serial.begin(9600);
  AudioMemory(10);
  // Initialisation de l'audio et de Faust
}

void loop() {
  if (Serial.available() > 0) {
    char choix = Serial.read();
    if (choix == '1') indexSon = 0;
    else if (choix == '2') indexSon = 1;
    else if (choix == '3') indexSon = 2;
    playWav.play(sons[indexSon]);
  }
}

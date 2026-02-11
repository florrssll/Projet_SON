#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "code_faust.h" 

// 1. OBJETS AUDIO
AudioPlaySdWav           playWav;       // Lecteur SD à la place de waveform
code_faust               faustEffect;   // Ton effet Faust
AudioOutputI2S           audioOutput;   // Sortie physique
AudioControlSGTL5000     audioShield;   // Contrôle du shield

// 2. MIXEUR (Optionnel mais conseillé)
// La carte SD sort souvent en stéréo (0 et 1), on les mixe pour l'effet Faust mono
AudioMixer4              monoMix;
AudioConnection          patch1(playWav, 0, monoMix, 0);
AudioConnection          patch2(playWav, 1, monoMix, 1);
AudioConnection          patch3(monoMix, 0, faustEffect, 0);
AudioConnection          patch4(faustEffect, 0, audioOutput, 0);
AudioConnection          patch5(faustEffect, 0, audioOutput, 1);

// Pin CS pour la carte SD (10 est le standard du Shield Audio)
#define SDCARD_CS_PIN    10

void setup() {
  Serial.begin(115200); // Vitesse augmentée pour Processing
  AudioMemory(60);      // Plus de mémoire pour la SD

  audioShield.enable();
  audioShield.volume(0.6);

  // Initialisation de la carte SD
  SPI.setMOSI(7); 
  SPI.setSCK(14);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    while (1) {
      Serial.println("Erreur : Carte SD introuvable !");
      delay(500);
    }
  }

  // Gains du mixeur
  monoMix.gain(0, 0.5);
  monoMix.gain(1, 0.5);

  // Initialisation Faust
  faustEffect.setParamValue("Wear", 0.5); 
  faustEffect.setParamValue("Noise", 0.1);
}

void loop() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    
    // On lance le fichier correspondant à la touche
    if (cmd == '1') {
      playWav.play("dab.WAV");
      Serial.println("Lecture: dab.WAV");
    } 
    else if (cmd == '2') {
      playWav.play("jazz.WAV");
      Serial.println("Lecture: jazz.WAV");
    } 
    else if (cmd == '3') {
      playWav.play("piano.WAV");
      Serial.println("Lecture: piano.WAV");
    }
  }
}
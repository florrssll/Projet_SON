#include <Audio.h>
#include <SD.h>
#include <SerialFlash.h>
#include "premiere_version.h" // Ton fichier Faust

// Objets audio
AudioPlaySdWav           playWav;
premiere_version         faustEffect; // Ton effet Faust
AudioOutputI2S           audioOutput;
AudioControlSGTL5000     audioShield;

// Connexions audio
AudioConnection          patchCord1(playWav, 0, faustEffect, 0);
AudioConnection          patchCord2(playWav, 1, faustEffect, 1);
AudioConnection          patchCord3(faustEffect, 0, audioOutput, 0);
AudioConnection          patchCord4(faustEffect, 1, audioOutput, 1);

// Liste des sons
const char* sons[] = {"piano.wav", "drums-bass.wav", "jazz.wav"};
int indexSon = 0;

void setup() {
  Serial.begin(9600);

  // Initialise la mémoire audio
  AudioMemory(20);

  // Initialise la carte SD
  if (!(SD.begin(BUILTIN_SDCARD))) {
    Serial.println("Échec de l'initialisation de la carte SD");
    while (1);
  }

  // Initialise le shield audio
  audioShield.enable();
  audioShield.volume(0.8);

  // Initialise les paramètres Faust (exemple)
  faustEffect.setParamValue("/mydsp/gain", 0.7);
}

void loop() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    if (cmd >= '1' && cmd <= '3') { // Sélection du son
      indexSon = cmd - '1'; // Convertit '1' en 0, '2' en 1, etc.
      playWav.play(sons[indexSon]);
    } else if (cmd == 'g') { // Réglage du gain
      float value = Serial.parseFloat();
      faustEffect.setParamValue("/mydsp/gain", value);
    }
    // Ajoute d'autres paramètres ici
  }
}



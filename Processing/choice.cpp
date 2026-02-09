#include <Audio.h>
#include "premiere_version.h" // Ton fichier Faust

// Objets audio
AudioSynthWaveform       waveform;      // Génère une sinusoïde (pour test)
premiere_version         faustEffect;   // Ton effet Faust
AudioOutputI2S           audioOutput;   // Sortie audio
AudioControlSGTL5000     audioShield;   // Contrôle du shield audio

// Connexions audio
AudioConnection          patchCord1(waveform, 0, faustEffect, 0);
AudioConnection          patchCord2(faustEffect, 0, audioOutput, 0);

void setup() {
  Serial.begin(9600);
  AudioMemory(10);

  // Initialise le shield audio
  audioShield.enable();
  audioShield.volume(0.8);

  // Configure la sinusoïde (pour test)
  waveform.begin(WAVEFORM_SINE);
  waveform.frequency(440);
  waveform.amplitude(0.5);

  // Initialise les paramètres Faust
  faustEffect.setParamValue("/premiere_version/gain", 0.7);
}

void loop() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    if (cmd >= '1' && cmd <= '3') {
      // Change la fréquence de la sinusoïde en fonction du son sélectionné (optionnel)
      float freq = 220.0 * (cmd - '0'); // Exemple : 220Hz, 440Hz, 660Hz
      waveform.frequency(freq);
      Serial.print("Son sélectionné : ");
      Serial.println(cmd);
    } else if (cmd == 'g') {
      // Réglage du gain
      float value = Serial.parseFloat();
      faustEffect.setParamValue("/premiere_version/gain", value);
      Serial.print("Gain réglé à : ");
      Serial.println(value);
    }
  }
}

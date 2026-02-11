#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include "EP_45.h"
#include "LP_33.h"

// ================= CONFIG =================
#define SD_CS 10

// ================= AUDIO OBJECTS =================
AudioPlaySdWav       playWav;
EP_45                ep45;
LP_33                lp33;
AudioMixer4          modeMixer;
AudioOutputI2S       i2sOut;
AudioControlSGTL5000 sgtl5000;

// ================= CONNECTIONS =================

// Digital
AudioConnection patch1(playWav, 0, modeMixer, 0);

// EP
AudioConnection patch2(playWav, 0, ep45, 0);
AudioConnection patch3(ep45, 0, modeMixer, 1);

// LP
AudioConnection patch4(playWav, 0, lp33, 0);
AudioConnection patch5(lp33, 0, modeMixer, 2);

// Output
AudioConnection patch6(modeMixer, 0, i2sOut, 0);
AudioConnection patch7(modeMixer, 0, i2sOut, 1);

// ================= VARIABLES =================
const char* currentFile = "piano.wav";

// ================= SETUP =================
void setup() {

  Serial.begin(9600);   // même vitesse que Processing
  AudioMemory(80);

  sgtl5000.enable();
  sgtl5000.volume(0.7);

  SPI.setMOSI(11);
  SPI.setMISO(12);
  SPI.setSCK(13);

  if (!SD.begin(SD_CS)) {
    while (1) {
      Serial.println("SD initialization failed!");
      delay(1000);
    }
  }

  Serial.println("SD OK");

  // Mode initial = DIGITAL
  modeMixer.gain(0, 1); // digital
  modeMixer.gain(1, 0); // EP
  modeMixer.gain(2, 0); // LP
}

// ================= LOOP =================
void loop() {

  if (Serial.available() > 0) {
    char cmd = Serial.read();

    // ----------- CHOIX MUSIQUE -----------
    if (cmd == '1') {
      currentFile = "dab.wav";
      playWav.play(currentFile);
    }
    else if (cmd == '2') {
      currentFile = "jazz.wav";
      playWav.play(currentFile);
    }
    else if (cmd == '3') {
      currentFile = "piano.wav";
      playWav.play(currentFile);
    }

    // ----------- CHOIX MODE -----------
    else if (cmd == '4') { // EP
      modeMixer.gain(0, 0);
      modeMixer.gain(1, 1);
      modeMixer.gain(2, 0);
      Serial.println("Mode: EP 45");
    }
    else if (cmd == '5') { // LP
      modeMixer.gain(0, 0);
      modeMixer.gain(1, 0);
      modeMixer.gain(2, 1);
      Serial.println("Mode: LP 33");
    }
    else if (cmd == '6') { // DIGITAL
      modeMixer.gain(0, 1);
      modeMixer.gain(1, 0);
      modeMixer.gain(2, 0);
      Serial.println("Mode: DIGITAL");
    }
  }

  // Relancer si fini
  if (!playWav.isPlaying()) {
    playWav.play(currentFile);
    delay(10);
  }
}

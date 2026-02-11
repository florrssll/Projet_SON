#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include "mode.h"

// ================= CONFIG =================
#define SD_CS 10

// ================= AUDIO OBJECTS =================
AudioPlaySdWav       playWav;
mode                 faustMode;   
AudioOutputI2S       i2sOut;
AudioControlSGTL5000 sgtl5000;

// ================= CONNECTIONS =================

AudioConnection patch1(playWav, 0, faustMode, 0);
AudioConnection patch2(faustMode, 0, i2sOut, 0);
AudioConnection patch3(faustMode, 0, i2sOut, 1);

// ================= VARIABLES =================
const char* currentFile = "piano.wav";

// ================= SETUP =================
void setup() {

  Serial.begin(9600);   // même vitesse que Processing
  AudioMemory(60);

  sgtl5000.enable();
  sgtl5000.volume(0.6);

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
  faustMode.setParamValue("mode", 0);
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
      faustMode.setParamValue("mode", 1);
      Serial.println("Mode: EP 45");
    }
    else if (cmd == '5') { // LP
      faustMode.setParamValue("mode", 2);
      Serial.println("Mode: LP 33");
    }
    else if (cmd == '6') { // DIGITAL
      faustMode.setParamValue("mode", 0);
      Serial.println("Mode: DIGITAL");
    }
  }
}

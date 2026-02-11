#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>

#include "amelioration.h"

// ================= CONFIG =================
#define SD_CS 10
#define PIN_BYPASS 0

// ================= AUDIO OBJECTS =================
AudioPlaySdWav       playWav;
amelioration         faustEP;
AudioMixer4          modeMixer;
AudioOutputI2S       i2sOut;
AudioControlSGTL5000 sgtl5000;

// ================= CONNECTIONS =================
// Digital path
AudioConnection patch1(playWav, 0, modeMixer, 0);

// Vinyl path
AudioConnection patch2(playWav, 0, faustEP, 0);
AudioConnection patch3(faustEP, 0, modeMixer, 1);

// Output
AudioConnection patch4(modeMixer, 0, i2sOut, 0);
AudioConnection patch5(modeMixer, 0, i2sOut, 1);

// ================= VARIABLES =================
Bounce bypassBtn(PIN_BYPASS, 10);
bool vinylMode = true;
bool lastButtonState = HIGH;

const char* currentFile = "piano.wav";

// ================= SETUP =================
void setup() {

  Serial.begin(115200);   
  AudioMemory(60);

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

  pinMode(PIN_BYPASS, INPUT_PULLUP);

  // Mode initial = VINYL
  modeMixer.gain(0, 0); // digital OFF
  modeMixer.gain(1, 1); // vinyl ON
}

// ================= LOOP =================
void loop() {

  // ----------- COMMANDE DEPUIS PROCESSING -----------
  if (Serial.available() > 0) {
    char cmd = Serial.read();

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
  }

  // ----------- TOGGLE BOUTON PHYSIQUE -----------
  bypassBtn.update();
  bool currentState = digitalRead(PIN_BYPASS);

  if (lastButtonState == HIGH && currentState == LOW) {
    vinylMode = !vinylMode;

    if (vinylMode) {
      modeMixer.gain(0, 0); // digital OFF
      modeMixer.gain(1, 1); // vinyl ON
    } else {
      modeMixer.gain(0, 1); // digital ON
      modeMixer.gain(1, 0); // vinyl OFF

    }
  }

  lastButtonState = currentState;
}

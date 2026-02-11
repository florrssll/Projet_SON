#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>

#include "code_faust.h"   // Ton DSP Faust Teensy

// ================= AUDIO OBJECTS =================
AudioPlaySdWav       playWav;
code_faust           faustEP;
AudioOutputI2S       i2sOut;
AudioControlSGTL5000 sgtl5000;

// ================= CONNECTIONS =================
AudioConnection patch1(playWav, 0, faustEP, 0);
AudioConnection patch2(faustEP, 0, i2sOut, 0);
AudioConnection patch3(faustEP, 0, i2sOut, 1);

// ================= BOUTON =================
constexpr int PIN_BYPASS = 0;
Bounce bypassBtn(PIN_BYPASS, 10);

bool vinylMode = true;
bool lastButtonState = HIGH;

// Variable pour mémoriser le dernier son choisi
const char* currentFile = "piano.wav"; 

#define SD_CS 10

// ================= UTILITY =================
static inline float adcNorm(uint16_t v, uint16_t maxv) {
  return (float)v / (float)maxv;
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);
  AudioMemory(60);

  // Audio Shield
  sgtl5000.enable();
  sgtl5000.volume(0.7);

  SPI.setMOSI(11);
  SPI.setMISO(12);
  SPI.setSCK(13);

  // SD init
  if (!SD.begin(SD_CS)) {
    while (1) {
      Serial.println("SD initialization failed!");
      delay(1000);
    }
  }

  Serial.println("SD OK");

  // Initialisation
  playWav.play(currentFile);

  pinMode(PIN_BYPASS, INPUT_PULLUP);
  analogReadResolution(12);

  faustEP.setParamValue("Wear", 1.0f);
  faustEP.setParamValue("Noise", 0.05f);
  faustEP.setParamValue("Bypass", 0.0f);
}


void loop() {

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

  bypassBtn.update();
  const uint16_t ADC_MAX = 4095;

  // ----------- Toggle bouton -----------
  bool currentState = digitalRead(PIN_BYPASS);
  if (lastButtonState == HIGH && currentState == LOW) {
    vinylMode = !vinylMode;
  }
  lastButtonState = currentState;

  // ----------- Potentiomètres -----------
  float wearVal  = adcNorm(analogRead(A0), ADC_MAX);
  float noiseVal = adcNorm(analogRead(A2), ADC_MAX) * 0.3f;

  // ----------- Paramètres Faust ----------
  faustEP.setParamValue("Wear", wearVal);
  faustEP.setParamValue("Noise", noiseVal);
  faustEP.setParamValue("Bypass", vinylMode ? 0.0f : 1.0f);

  // ----------- MODIFICATION : Relancer le fichier actuel si fini ----------
  if (!playWav.isPlaying()) {
    playWav.play(currentFile);
    delay(10);
  }

  delay(5);
}
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

// 
#define SD_CS 10

// ================= UTILITY =================
static inline float adcNorm(uint16_t v, uint16_t maxv) {
  return (float)v / (float)maxv;
}

// ================= SETUP =================
void setup() {

  Serial.begin(115200);
  AudioMemory(40);

  // Audio Shield
  sgtl5000.enable();
  sgtl5000.volume(0.7);

  // SPI pins (sécurise sur Teensy 4.0)
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

  playWav.play("piano.wav");
  delay(10);

  pinMode(PIN_BYPASS, INPUT_PULLUP);
  analogReadResolution(12);

  faustEP.setParamValue("Wear", 1.0f);
  faustEP.setParamValue("Noise", 0.05f);
  faustEP.setParamValue("Bypass", 0.0f);
}

// ================= LOOP =================
void loop() {

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

  // ----------- Relancer si fini ----------
  if (!playWav.isPlaying()) {
    playWav.play("piano.wav");
    delay(10);
  }

  delay(5);
}
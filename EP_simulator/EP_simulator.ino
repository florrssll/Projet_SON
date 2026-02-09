#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <Bounce.h>

#include "code_faust.h"   // DSP Faust Teensy

// ---------------- AUDIO OBJECTS ----------------
AudioInputUSB        usbIn;
code_faust           faustEP;   // ← PLUS mydsp
AudioOutputI2S       i2sOut;
AudioControlSGTL5000 sgtl5000;

// -------------- AUDIO PATCH CORDS --------------
AudioMixer4 monoMix;

AudioConnection patch1(usbIn, 0, monoMix, 0);
AudioConnection patch2(usbIn, 1, monoMix, 1);
AudioConnection patch3(monoMix, 0, faustEP, 0);
AudioConnection patch4(faustEP, 0, i2sOut, 0);
AudioConnection patch5(faustEP, 0, i2sOut, 1);

// ------------------- UI -------------------
constexpr int PIN_BYPASS = 0;
Bounce bypassBtn(PIN_BYPASS, 10);

static inline float adcNorm(uint16_t v, uint16_t maxv) {
  return (float)v / (float)maxv;
}

void setup() {
  Serial.begin(115200);
  AudioMemory(40);

  // Audio Shield
  sgtl5000.enable();
  sgtl5000.volume(0.6);

  // Mix L/R USB → mono
  monoMix.gain(0, 0.5);
  monoMix.gain(1, 0.5);

  pinMode(PIN_BYPASS, INPUT_PULLUP);
  analogReadResolution(12);

  // Valeurs initiales
  faustEP.setParamValue("Wear", 1.0f);
  faustEP.setParamValue("Noise", 0.05f);
  faustEP.setParamValue("Bypass", 0.0f);

  sgtl5000.volume(0.9);
  sgtl5000.lineOutLevel(29);

}

void loop() {
  bypassBtn.update();

  const uint16_t ADC_MAX = 4095;

  // Pot A0 → Wear
  float wearVal = adcNorm(analogRead(A0), ADC_MAX);
  faustEP.setParamValue("Wear", wearVal);

  // Pot A2 → Noise
  float noiseVal = adcNorm(analogRead(A2), ADC_MAX) * 0.3f;
  faustEP.setParamValue("Noise", noiseVal);

  // Bouton → Bypass
  int bypassState = (digitalRead(PIN_BYPASS) == LOW) ? 1 : 0;
  faustEP.setParamValue("Bypass", (float)bypassState);

  delay(5);
}

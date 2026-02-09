#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// Inclusion du fichier généré par Faust
#include "mydsp.h" 

// 1. Création des composants de la chaîne audio
AudioInputI2S            lineIn;         
faust_ep                 myEP;           // Ton moteur Faust
AudioOutputI2S           headphones;      
AudioControlSGTL5000     sgtl5000;       

// 2. Connexions (Patch Bay)
// On connecte l'entrée gauche au moteur Faust, puis le moteur aux deux oreilles
AudioConnection          patch1(lineIn, 0, myEP, 0); 
AudioConnection          patch2(myEP, 0, headphones, 0);
AudioConnection          patch3(myEP, 0, headphones, 1);

void setup() {
  Serial.begin(9600);
  
  // 3. Initialisation de la mémoire et du shield
  AudioMemory(20); // Alloue de la mémoire pour les délais (Wow)
  sgtl5000.enable();
  sgtl5000.volume(0.6); // Volume général de sortie
  sgtl5000.inputSelect(AUDIO_INPUT_LINEIN); // Sélectionne l'entrée Line-In
  
  // Configuration des boutons
  pinMode(0, INPUT_PULLUP); // Bouton Bypass
}

void loop() {
  // --- LECTURE DU POTENTIOMÈTRE 1 (WEAR) ---
  // On lit A0, on normalise (0.0 à 1.0)
  float wearVal = analogRead(A0) / 1023.0;
  // On envoie à Faust (le nom "Wear" doit être identique au hslider Faust)
  myEP.setParamValue("Wear", wearVal);

  // --- LECTURE DU POTENTIOMÈTRE 2 (NOISE) ---
  // On lit A2, on normalise
  float noiseVal = analogRead(A2) / 1023.0;
  myEP.setParamValue("Noise", noiseVal);

  // --- LECTURE DU BOUTON (BYPASS) ---
  // Rappel : INPUT_PULLUP inverse la logique (LOW = pressé)
  int bypassState = (digitalRead(0) == LOW) ? 1 : 0;
  myEP.setParamValue("Bypass", bypassState);

  // --- OPTIONNEL : DEBUG DANS LA CONSOLE ---
  /*
  Serial.print("Wear: "); Serial.print(wearVal);
  Serial.print(" | Noise: "); Serial.println(noiseVal);
  */

  delay(10); // Petite pause pour la stabilité des lectures analogiques
}
import controlP5.*;
import processing.serial.*;

ControlP5 cp5;
Serial port;

void setup() {
  size(500, 200); // Crée une fenêtre de 500x200 pixels

  // Initialise ControlP5 pour les boutons
  cp5 = new ControlP5(this);

  // Affiche la liste des ports série disponibles
  printArray(Serial.list());

  // Initialise le port série (remplace "COM3" par le port de ta Teensy)
  // Utilise le premier port série disponible
  port = new Serial(this, Serial.list()[0], 9600);

  // Boutons pour choisir les musiques
  cp5.addButton("Drums")
     .setPosition(50, 50)
     .setSize(100, 40)
     .onRelease(new CallbackListener() {
       public void controlEvent(CallbackEvent theEvent) {
         port.write('1'); // Envoie '1' pour "Drums-Bass"
       }
     });

  cp5.addButton("Jazz")
     .setPosition(180, 50)
     .setSize(100, 40)
     .onRelease(new CallbackListener() {
       public void controlEvent(CallbackEvent theEvent) {
         port.write('2'); // Envoie '2' pour "Jazz"
       }
     });

  cp5.addButton("Piano")
     .setPosition(310, 50)
     .setSize(100, 40)
     .onRelease(new CallbackListener() {
       public void controlEvent(CallbackEvent theEvent) {
         port.write('3'); // Envoie '3' pour "Piano"
       }
     });
}

void draw() {
  background(220); // Fond de la fenêtre
}

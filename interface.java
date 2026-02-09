import controlP5.*;
import processing.serial.*;

ControlP5 cp5;
Serial port;

void setup() {
  size(500, 300);
  cp5 = new ControlP5(this);

  // Boutons pour choisir le son
  cp5.addButton("Drums_bass sound")
     .setPosition(50, 50)
     .onRelease(() -> port.write('1'));

  cp5.addButton("Piano sound")
     .setPosition(150, 50)
     .onRelease(() -> port.write('2'));

  cp5.addButton("Jazz sound")
     .setPosition(250, 50)
     .onRelease(() -> port.write('3'));

  // Curseur pour le gain
  cp5.addSlider("Gain")
     .setPosition(50, 150)
     .setRange(0, 1)
     .onChange((event) -> {
       port.write('g'); // Envoie 'g' pour indiquer que c'est le gain
       port.write(float(event.getValue())); // Envoie la valeur du gain
     });

  // Initialise le port série (remplace "COM3" par ton port)
  port = new Serial(this, "COM3", 9600);
}

void draw() {
  background(220);
}


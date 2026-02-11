import controlP5.*;
import processing.serial.*;
int btnW = 100;
int btnH = 40;
int espacement = 10;
color b1, b2, c1, c2;

ControlP5 cp5;
Serial port;

void setup() {
  size(700, 200); // Crée une fenêtre de 500x200 pixels
  
  b1 = color(255);
  b2 = color(0);
  c1 = color(0, 102, 153);
  c2 = color(0, 200, 50);  
  
  // Initialise ControlP5 pour les boutons
  cp5 = new ControlP5(this);

  // Affiche la liste des ports série disponibles
  String[] serialPorts = Serial.list();
  printArray(Serial.list());
  
  if (serialPorts.length > 0) {
    port = new Serial(this, serialPorts[0], 9600);
  } else {
    println("Aucun port série disponible. Branchez votre Teensy.");
  }

  // Initialise le port série (remplace "COM3" par le port de ta Teensy)
  // Utilise le premier port série disponible
  port = new Serial(this, Serial.list()[0], 9600);

  // Boutons pour choisir les musiques
  cp5.addButton("Drums")
     .setPosition(width/2-btnW/2, height/2-btnH/2)
     .setSize(btnW, btnH)
     .setColorBackground(color(0, 45, 90)) // Couleur du bouton au repos
     .setColorForeground(color(0, 116, 217)) // Couleur quand la souris passe dessus
     .setColorActive(color(255, 128, 0))
     .onRelease(new CallbackListener() {
       public void controlEvent(CallbackEvent theEvent) {
         port.write('1'); // Envoie '1' pour "Drums-Bass"
       }
     });

  cp5.addButton("Opera")
     .setPosition(width/2 - btnW/2 - btnW - espacement, height/2 - btnH/2)
     .setSize(btnW, btnH)
     .setColorBackground(color(0, 45, 90)) // Couleur du bouton au repos
     .setColorForeground(color(0, 116, 217)) // Couleur quand la souris passe dessus
     .setColorActive(color(148, 0, 211))
     .onRelease(new CallbackListener() {
       public void controlEvent(CallbackEvent theEvent) {
         port.write('2'); // Envoie '2' pour "Opera"
       }
     });

  cp5.addButton("Piano")
     .setSize(100, 40)
     .setPosition(width/2 - btnW/2 + btnW + espacement, height/2 - btnH/2)
     .setColorBackground(color(0, 45, 90)) // Couleur du bouton au repos
     .setColorForeground(color(0, 116, 217)) // Couleur quand la souris passe dessus
     .setColorActive(color(255, 128, 0))
     .onRelease(new CallbackListener() {
       public void controlEvent(CallbackEvent theEvent) {
         port.write('3'); // Envoie '3' pour "Piano"
       }
     });

  cp5.addButton("EP")
     .setSize(100, 40)
     .setPosition(width/2 - btnW/2 - espacement - btnW, height/2+ 40)
     .setColorBackground(color(0, 45, 90)) // Couleur du bouton au repos
     .setColorForeground(color(0, 116, 217)) // Couleur quand la souris passe dessus
     .setColorActive(color(255, 128, 0))
     .onRelease(new CallbackListener() {
       public void controlEvent(CallbackEvent theEvent) {
         port.write('4'); // Envoie '4' pour "Mode Vinyl"
       }
     });

  cp5.addButton("LP")
     .setSize(100, 40)
     .setPosition(width/2 - btnW/2, height/2+40)
     .setColorBackground(color(0, 45, 90)) // Couleur du bouton au repos
     .setColorForeground(color(0, 116, 217)) // Couleur quand la souris passe dessus
     .setColorActive(color(255, 128, 0))
     .onRelease(new CallbackListener() {
       public void controlEvent(CallbackEvent theEvent) {
         port.write('5'); // Envoie '5' pour "Mode LP"
       }
     });
     
  cp5.addButton("Digital")
     .setPosition(width/2 + 150, height/2+50)
     .setSize(100, 40)
     .setPosition(width/2 - btnW/2 + btnW + espacement, height/2+40)
     .setColorBackground(color(0, 45, 90)) // Couleur du bouton au repos
     .setColorForeground(color(0, 116, 217)) // Couleur quand la souris passe dessus
     .setColorActive(color(255, 128, 0))
     .onRelease(new CallbackListener() {
       public void controlEvent(CallbackEvent theEvent) {
         port.write('6'); // Envoie '6' pour "Digital"
       }
     });
}

void draw() {
  
  //Background
  drawGradientBackground();
  
  // Config texte
  fill(100, 100, 255);              
  textAlign(CENTER);      
  
// Titre 
  textSize(24);           
  text("SIMULATEUR VINYL", width/2, 40); 


  textSize(14);          
  text("Choisissez la musique que vous souhaitez traiter :", width/2, 60);

}

void drawGradientBackground() {
  // Dégradé horizontal de gauche à droite
  for (int i = 0; i < width; i++) {
    float inter = map(i, 0, width, 0, 1);
    color c = lerpColor(b1, b2, inter);
    stroke(c);
    line(i, 0, i, height);
  }
}

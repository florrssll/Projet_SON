import controlP5.*;
import processing.serial.*;
int btnW = 100;
int btnH = 40;
int espacement = 10;

ControlP5 cp5;
Serial port;

void setup() {
  size(700, 200); // Crée une fenêtre de 500x200 pixels
  
  // Initialise ControlP5 pour les boutons
  cp5 = new ControlP5(this);

  // Affiche la liste des ports série disponibles
  printArray(Serial.list());

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

  cp5.addButton("Jazz")
     .setPosition(width/2 - btnW/2 - btnW - espacement, height/2 - btnH/2)
     .setSize(btnW, btnH)
     .setColorBackground(color(0, 45, 90)) // Couleur du bouton au repos
     .setColorForeground(color(0, 116, 217)) // Couleur quand la souris passe dessus
     .setColorActive(color(255, 128, 0))
     .onRelease(new CallbackListener() {
       public void controlEvent(CallbackEvent theEvent) {
         port.write('2'); // Envoie '2' pour "Jazz"
       }
     });

  cp5.addButton("Piano")
     .setPosition(width/2 + 150, height/2)
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
}

void draw() {
  background(220);
  // Config texte
  fill(100, 100, 255);              
  textAlign(CENTER);      
  
// Titre 
  textSize(24);           
  text("SIMULATEUR VINYL", width/2, 40); 


  textSize(14);          
  text("Choisissez la musique que vous souhaitez traiter :", width/2, 60);

}

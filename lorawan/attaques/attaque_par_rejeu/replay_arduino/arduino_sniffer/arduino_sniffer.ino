#include <SPI.h>
#include <LoRa.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRaWAN Sniffer");

  if (!LoRa.begin(868.3E6)) {  // Fréquence 868,1 MHz pour l'Europe
    Serial.println("Erreur lors de l'initialisation de LoRa !");
    while (1);
  }

  // Configurer le spreading factor et la bande passante
  LoRa.setSyncWord(0x34);
  LoRa.setSpreadingFactor(7);  // SF7
  LoRa.setSignalBandwidth(125E3);  // 125 kHz
  LoRa.setCodingRate4(5);  // Coding Rate 4/5

  Serial.println("En attente de messages LoRaWAN...");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.println("\nPaquet reçu !");

    // Afficher la taille du paquet et le RSSI
    Serial.print("Taille du paquet : ");
    Serial.println(packetSize);
    Serial.print("RSSI : ");
    Serial.println(LoRa.packetRssi());

    // Afficher les données brutes en hexadécimal
    Serial.print("Données reçues (hex) : ");
    while (LoRa.available()) {
      uint8_t byte = LoRa.read();
      Serial.print(byte < 16 ? "0" : "");
      Serial.print(byte, HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
}

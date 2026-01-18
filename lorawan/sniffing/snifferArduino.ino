#include <SPI.h>
#include <LoRa.h>

#define NSS 10
#define RESET 9
#define DIO0 2

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("Initialisation du module LoRa en mode écoute...");
  LoRa.setPins(NSS, RESET, DIO0);

  if (!LoRa.begin(868E6)) {
    Serial.println("Erreur de démarrage LoRa !");
    while (1);
  }

  // Configuration Spreading Factor et bande passante
  LoRa.setSpreadingFactor(7);      // SF7 
  LoRa.setSignalBandwidth(125E3);  // Bande passante de 125 kHz
  LoRa.setCodingRate4(5);          // Coding Rate à 4/5

  Serial.println("Sniffing LoRa actif. En attente de paquets...");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.print("Paquet reçu (taille ");
    Serial.print(packetSize);
    Serial.println(" octets) : ");

    while (LoRa.available()) {
      byte data = LoRa.read();
      Serial.print("0x");
      if (data < 0x10) Serial.print("0");
      Serial.print(data, HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
}

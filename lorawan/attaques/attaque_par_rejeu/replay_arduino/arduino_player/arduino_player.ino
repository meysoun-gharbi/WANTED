#include <SPI.h>
#include <LoRa.h>

// Paquet à rejouer (19 octets pour un join_request)
//uint8_t packet[] = {
//  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // AppEUI
//  0x57, 0x2A, 0x71, 0x99, 0x49, 0xD5, 0xB3, 0x70,  // DevEUI
//  0xDD, 0x7C                                        // DevNonce
//};

// paquet data up 40 31 CC 9E 01 00 14 02 02 78 B7 11 B3 A6 55 E6 4B 
uint8_t packet[] = {
  0x40, 0x31, 0xCC, 0x9E, 0x01, 0x00, 0x14, 0x02, 0x02,
  0x78, 0xB7, 0x11, 0xB3, 0xA6, 0x55, 0xE6, 0x4B
};

int packetSize = sizeof(packet);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Replayer");

  if (!LoRa.begin(868.3E6)) {  // Fréquence 868,3 MHz
    Serial.println("Erreur lors de l'initialisation de LoRa !");
    while (1);
  }

  // Configurer le spreading factor et la bande passante
  LoRa.setSpreadingFactor(7);  // SF7
  LoRa.setSignalBandwidth(125E3);  // 125 kHz
  LoRa.setCodingRate4(5);  // Coding Rate 4/5

  Serial.println("Prêt à rejouer le paquet en boucle...");
}

void loop() {
  //Serial.println("Envoi du paquet...");
  Serial.print("Rejeu de la trame : ");
  
  // Affichage hexadécimal pour votre console série
  for (int i = 0; i < packetSize; i++) {
    if (packet[i] < 0x10) Serial.print("0");
    Serial.print(packet[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
  // Incrémenter le compteur de trames (FCnt)
  //packet[6]++;  // Incrémente l'octet de poids faible
  //if (packet[6] == 0) {
    //packet[7]++;  // Incrémente l'octet de poids fort si nécessaire
  //}

  // Envoyer le paquet
  LoRa.beginPacket();
  LoRa.write(packet, packetSize);
  LoRa.endPacket();

  delay(4000);  // Attendre 4 secondes avant de renvoyer le paquet
}

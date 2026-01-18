/*
   RadioLib SX127x Receive Example

   This example listens for LoRa transmissions using SX127x LoRa modules.
   To successfully receive data, the following settings have to be the same
   on both transmitter and receiver:
    - carrier frequency
    - bandwidth
    - spreading factor
    - coding rate
    - sync word
    - preamble length

   Other modules from SX127x/RFM9x family can also be used.

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/

// Inclure la bibliothèque
#include <RadioLib.h>

// SX1276 a les connexions suivantes :
// NSS pin:   10
// DIO0 pin:  2
// RESET pin: 9
// DIO1 pin:  3
SX1276 lora = new Module(10, 2, 9, 6);

void setup() {
  Serial.begin(115200);
  Serial.print(F("START "));

  // Configuration du module LoRa :
  // - Fréquence porteuse : 868.3 MHz
  // - Bande passante : 125 kHz
  // - Facteur d'étalement : 7
  // - Taux de codage : 5
  // - Sync word : 0x34 (réservé pour le réseau LoRaWAN)
  // - Puissance de sortie : 17 dBm (valeurs acceptées entre -3 et 17 dBm)
  // - Longueur du préambule : 8 symboles
  // - Gain de l'amplificateur : 0 (contrôle automatique du gain)
  int state = lora.begin(868.3, 125.0, 7, 5, 0x34, 17, 8, 0);

  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("SUCCESS!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true); // Boucle infinie en cas d'échec
  }
}

void loop() {
  // Envoi d'un message LoRa
  int state = lora.transmit("JammingJammingJammingJammingJammingJammingJammingJamming");

  if (state == RADIOLIB_ERR_NONE) {
    // Le paquet a été transmis avec succès
    Serial.println(F("JAM_SUCCESS"));

  } else if (state == RADIOLIB_ERR_PACKET_TOO_LONG) {
    // Le paquet est trop long (plus de 256 octets)
    Serial.println(F("ERR_PACKET_TOO_LONG"));

  } else if (state == RADIOLIB_ERR_TX_TIMEOUT) {
    // Temps d'attente écoulé avant la fin de la transmission
    Serial.println(F("ERR_TX_TIMEOUT"));

  } else {
    // Une autre erreur s'est produite
    Serial.print(F("ERR "));
    Serial.println(state);
  }
}

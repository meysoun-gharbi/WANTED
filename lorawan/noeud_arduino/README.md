# LoRaWAN Sketch avec LMIC pour un noeud Arduino

Ce répertoire décrit la configuration et l'utilisation du framework LMIC pour connecter une carte Arduino à un réseau LoRaWAN via OTAA. Ce qui nous sera utile pour réaliser l'attaque.

---

## Étapes suivies

### 1. Téléchargement de la librairie LMIC
Nous avons téléchargé la version recommandée de la librairie LMIC depuis le dépôt suivant :  
[https://github.com/mcci-catena/arduino-lmic](https://github.com/mcci-catena/arduino-lmic)

### 2. Inclusion de la librairie dans l’environnement Arduino
- Ouvrir l’IDE Arduino.
- Aller dans **Sketch > Include Library > Add .ZIP Library…**
- Ajouter la librairie téléchargée.
- Il faut bien faire attention à la région selectionnée dans project_config/lmic_project_config.h


### 3. Utilisation d’un exemple
- Dans l’IDE Arduino, naviguer vers **File > Examples > MCCI LoRaWAN LMIC library > ttn-otaa**.
- Charger l’exemple **ttn-otaa.ino**.

### 4. Configuration des clés
- Modifier les clés dans le sketch pour correspondre aux paramètres de l’application configurée dans ChirpStack :
  - **DevEUI** : format Little Endian.
  - **AppEUI** : format Little Endian.
  - **AppKey** : format Big Endian.



### 5. Configuration du Pin Mapping
Adapter la configuration des broches pour la carte utilisée. Voici le pin mapping utilisé :  

```cpp
// Pin mapping
const lmic_pinmap lmic_pins = {
    .nss = 10,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 9,
    .dio = {2, 6, 7},
};

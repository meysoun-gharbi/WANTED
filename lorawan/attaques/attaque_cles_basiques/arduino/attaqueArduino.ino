#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>

uint8_t currentDevEUIIndex = 0; // Index pour les DevEUI
uint8_t currentAppKeyIndex = 0; // Index pour les AppKey

static const u1_t PROGMEM APPEUI[8]={ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);}

static const u1_t PROGMEM DEVEUI[][8] = {
    //{0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11}, // suppression de cette ligne pour notre exemple, car un réseau étranger semble avoir ces identifiants
    {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA}, // AAAAAAAAAAAAAAAA
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 0000000000000000
    {0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF}, // 1234567890ABCDEF
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // FFFFFFFFFFFFFFFF
    {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}, // 0101010101010101
    {0xAB, 0xCD, 0xEF, 0xAB, 0xCD, 0xEF, 0xAB, 0xCD}, // ABCDEFABCDEFABCD
};
void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI[currentDevEUIIndex], 8);}


static const u1_t PROGMEM APPKEY[][16] = {
    {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11}, // 11111111111111111111111111111111
    {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA}, // AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 00000000000000000000000000000000
    {0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF, 0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF}, // 1234567890ABCDEF1234567890ABCDEF
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, // FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
    {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}, // 01010101010101010101010101010101
    {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88}, // 11223344556677881122334455667788
    {0xAB, 0xCD, 0xEF, 0xAB, 0xCD, 0xEF, 0xAB, 0xCD, 0xAB, 0xCD, 0xEF, 0xAB, 0xCD, 0xEF, 0xAB, 0xCD}  // ABCDEFABCDEFABCDEFABCDEFABCDEFAB
};
void os_getDevKey (u1_t* buf) {  memcpy_P(buf, APPKEY[currentAppKeyIndex], 16);}

static uint8_t mydata[] = "Hello, world!";
static osjob_t sendjob;


const unsigned TX_INTERVAL = 5;

// Pin mapping
const lmic_pinmap lmic_pins = {
    .nss = 10,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 9,
    .dio = {2, 6, 7},
};

void printHex2(unsigned v) {
    v &= 0xff;
    if (v < 16)
        Serial.print('0');
    Serial.print(v, HEX);
}

// Déclaration d'un tableau pour les DevEUI et AppKey actuels
uint8_t currentDevEUI[8];
uint8_t currentAppKey[16];

void onEvent (ev_t ev) {
    Serial.print(os_getTime());
    Serial.print(": ");
    switch(ev) {
        case EV_SCAN_TIMEOUT:
            Serial.println(F("EV_SCAN_TIMEOUT"));
            break;
        case EV_BEACON_FOUND:
            Serial.println(F("EV_BEACON_FOUND"));
            break;
        case EV_BEACON_MISSED:
            Serial.println(F("EV_BEACON_MISSED"));
            break;
        case EV_BEACON_TRACKED:
            Serial.println(F("EV_BEACON_TRACKED"));
            break;
        case EV_JOINING:
            Serial.println(F("EV_JOINING"));
            break;
        case EV_JOINED:
            Serial.println(F("EV_JOINED"));
            {
              u4_t netid = 0;
              devaddr_t devaddr = 0;
              u1_t nwkKey[16];
              u1_t artKey[16];
              LMIC_getSessionKeys(&netid, &devaddr, nwkKey, artKey);
              Serial.print("netid: ");
              Serial.println(netid, DEC);
              Serial.print("devaddr: ");
              Serial.println(devaddr, HEX);
              Serial.print("AppSKey: ");
              for (size_t i=0; i<sizeof(artKey); ++i) {
                if (i != 0)
                  Serial.print("-");
                printHex2(artKey[i]);
              }
              Serial.println("");
              Serial.print("NwkSKey: ");
              for (size_t i=0; i<sizeof(nwkKey); ++i) {
                      if (i != 0)
                              Serial.print("-");
                      printHex2(nwkKey[i]);
              }
              Serial.println();
            }
            // Disable link check validation (automatically enabled
            // during join, but because slow data rates change max TX
	    // size, we don't use it in this example.
            LMIC_setLinkCheckMode(0);
            break;

        case EV_JOIN_FAILED:
            Serial.println(F("EV_JOIN_FAILED"));
            break;
        case EV_REJOIN_FAILED:
            Serial.println(F("EV_REJOIN_FAILED"));
            break;
        case EV_TXCOMPLETE:
            Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
            if (LMIC.txrxFlags & TXRX_ACK)
              Serial.println(F("Received ack"));
            if (LMIC.dataLen) {
              Serial.print(F("Received "));
              Serial.print(LMIC.dataLen);
              Serial.println(F(" bytes of payload"));
            }
            // Schedule next transmission
            os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
            break;
        case EV_LOST_TSYNC:
            Serial.println(F("EV_LOST_TSYNC"));
            break;
        case EV_RESET:
            Serial.println(F("EV_RESET"));
            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            Serial.println(F("EV_RXCOMPLETE"));
            break;
        case EV_LINK_DEAD:
            Serial.println(F("EV_LINK_DEAD"));
            break;
        case EV_LINK_ALIVE:
            Serial.println(F("EV_LINK_ALIVE"));
            break;

        case EV_TXSTART:
            Serial.println(F("EV_TXSTART"));
            break;
        case EV_TXCANCELED:
            Serial.println(F("EV_TXCANCELED"));
            break;
        case EV_RXSTART:
            /* do not print anything -- it wrecks timing */
            break;
        case EV_JOIN_TXCOMPLETE: ////////C'EST CA QU'ON VEUT////////
            Serial.println(F("EV_JOIN_TXCOMPLETE: no JoinAccept"));
            // Passer à la prochaine combinaison de clés
            currentAppKeyIndex++; // Passer à l'AppKey suivant
            if (currentAppKeyIndex >= sizeof(APPKEY) / sizeof(APPKEY[0])) {
              // Si on a essayé tous les AppKey, passer au DevEUI suivant
              currentAppKeyIndex = 0;
              currentDevEUIIndex++; // Passer au DevEUI suivant
              if (currentDevEUIIndex >= sizeof(DEVEUI) / sizeof(DEVEUI[0])) {
                Serial.println(F("Toutes les combinaisons ont échoué !"));
                // Arrêter ici si toutes les combinaisons ont été testées
              }
            }

            Serial.println(F("Tentative avec la combinaison suivante :"));
            

            // Afficher le DevEUI actuel
            Serial.print(F("DevEUI Index actuel: "));
            Serial.println(currentDevEUIIndex);

            // Afficher l'AppKey actuel
            Serial.print(F("AppKey Index actuel: "));
            Serial.println(currentAppKeyIndex);

            // Mettre à jour les clés pour la prochaine tentative
            memcpy_P(currentDevEUI, DEVEUI[currentDevEUIIndex], 8);
            memcpy_P(currentAppKey, APPKEY[currentAppKeyIndex], 16);

            LMIC_reset(); // Réinitialiser le module pour la nouvelle combinaison
            do_send(&sendjob); // Relancer une tentative de transmission

            break;

        default:
            Serial.print(F("Unknown event: "));
            Serial.println((unsigned) ev);
            break;
    }
}

void do_send(osjob_t* j){
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("OP_TXRXPEND, not sending"));
    } else {
        // Prepare upstream data transmission at the next possible time.
        LMIC_setTxData2(1, mydata, sizeof(mydata)-1, 0);
        Serial.println(F("Packet queued"));
    }
    // Next TX is scheduled after TX_COMPLETE event.
}

void setup() {
    Serial.begin(9600);
    Serial.println(F("Starting"));

    #ifdef VCC_ENABLE
    // For Pinoccio Scout boards
    pinMode(VCC_ENABLE, OUTPUT);
    digitalWrite(VCC_ENABLE, HIGH);
    delay(1000);
    #endif

    // LMIC init
    os_init();
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();

    // Afficher la première combinaison de clés dans le Serial Monitor
    Serial.println(F("Tentative avec la première combinaison :"));
    
    // Afficher le DevEUI de la première combinaison
    Serial.print(F("DevEUI Index actuel: "));
    Serial.println(currentDevEUIIndex);


    // Afficher l'AppKey de la première combinaison
    Serial.print(F("AppKey Index actuel: "));
    Serial.println(currentAppKeyIndex);
    Serial.println();

    // Démarrer le job d'envoi (envoi automatique démarre aussi OTAA)
    do_send(&sendjob);
}

void loop() {
    os_runloop_once();
}

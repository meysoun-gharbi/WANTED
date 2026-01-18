Pour réaliser cette attaque, nous nous sommes inspiré des exemples disponibles sur [arduino-LoRa](https://github.com/sandeepmistry/arduino-LoRa).

# Partie Capture de paquets

### Analyse des données capturées avec l'Arduino

Voici un exemple de données capturées :

```
00 00 00 00 00 00 00 00 00 57 2A 71 99 49 D5 B3 70 DD 7C 93 36 E4 C9
```

#### 1. **Identification du join_request (MHDR)**
   - Les **3 premiers bits** du premier octet (`00`) indiquent le type de message. Ici, `000` correspond à un **join_request**.

#### 2. **AppEUI**
   - Les **8 octets suivants** représentent l'**AppEUI** (Identifiant de l'application).
   - Dans ce cas, l'AppEUI est `00 00 00 00 00 00 00 00`, ce qui signifie qu'il est configuré à une valeur par défaut ou non définie.

#### 3. **DevEUI (en little-endian)**
   - Les **8 octets suivants** représentent le **DevEUI** (Identifiant du dispositif), transmis en **little-endian**.
   - Les octets capturés sont : `57 2A 71 99 49 D5 B3 70`.
   - En **big-endian** (ordre standard), cela donne : `70 B3 D5 49 99 71 2A 57`. Ce qui correspond à la configuration de notre noeud

#### 4. **DevNonce**
   - Les **2 octets suivants** représentent le **DevNonce**, une valeur aléatoire générée par le dispositif pour ce **join_request**.
   - Ici, le DevNonce est `DD 7C`.

#### 5. **MIC**
   - Les **4 derniers octets** (`93 36 E4 C9`) concernent le MIC.
   - Calculé avec AES-CMAC pour l'intégrité

---

### Résumé

Les données capturées correspondent bien à un **join_request** LoRaWAN, avec les champs suivants :

- **MHDR** : `00` (join_request)
- **AppEUI** : `00 00 00 00 00 00 00 00` (valeur par défaut ou non définie)
- **DevEUI** : `70 B3 D5 49 99 71 2A 57` (en big-endian)
- **DevNonce** : `DD 7C` (valeur aléatoire)
- **MIC** : `93 36 E4 C9` (vérification de l'intégrité)

---

### **Autres captures**  

Nous avons ensuite capturé d'autres paquets :  

```
40 E2 F1 54 00 00 00 00 02 6F BD 26 07 55 5E 51 5A BA 78 C1 AE E8 76 D0 23 15 FA 
40 E2 F1 54 00 00 03 00 02 86 4F A9 79 3E C1 42 97 4E 06 F4 F4 1E E2 75 BC 79 BD 
40 E2 F1 54 00 00 05 00 02 BF AE FF F4 7D 78 9A A3 0E 61 44 A7 39 90 93 9E 25 E2
```

Grâce au **MHDR** de ces paquets (`40`), nous pouvons vérifier qu’il s’agit bien de **Unconfirmed Data Up**.  

---

# **Partie rejeu de paquets**  

Nous avons d’abord réalisé un rejeu de paquets très simplifié en envoyant en boucle un **Unconfirmed Data Up** capturé plus haut. Cette approche fonctionne, puisque nous parvenons à sniffer ces paquets à l’aide de l’outil **Lorattack**.  

Cependant, **ces paquets ne sont pas visibles sur l’application ChirpStack**.  

Cela est logique, car ChirpStack vérifie l’intégrité des paquets via **deux méthodes** :  
1. **Le compteur de trames (FCnt)** : il doit s’incrémenter pour éviter les rejeux.  
2. **Le MIC (Message Integrity Code)** : il est unique pour chaque trame et garantit son intégrité.  

En prenant un des paquets capturés en exemple :  

```
40 E2 F1 54 00 00 **05 00** 02 BF AE FF F4 7D 78 9A A3 0E 61 44 A7 39 90 **93 9E 25 E2**
```

- Le **compteur de trames (FCnt)** est **`05 00`** (incrémenté par rapport aux trames précédentes).  
- Le **MIC** est **`93 9E 25 E2`**, garantissant l’intégrité du message.  

En **modifiant ces deux éléments**, nous pourrions ainsi faire valider nos paquets rejoués par ChirpStack.

---

Incrément du compteur de trames : ok
Recalcul du MIC : plus compliqué, en effet il faut utiliser AES-CMAC avec la NwkSKey que nous ne sommes pas sensé connaître en tant qu'attaquants.


---  

# Automatisation  

Dans le répertoire **arduino_total_sniffer**, un code permet de combiner en un seul programme ce que nous faisons séparément dans **arduino_sniffer** et **arduino_player**. Dans cet exemple, nous capturons un paquet de type *Unconfirmed Data Up* et le rejouons immédiatement après en incrémentant son compteur de trames.  

Ce rejeu fonctionne bien, car en utilisant le sniffing avec **lorattack**, nous pouvons confirmer que les paquets sont bien retransmis. 
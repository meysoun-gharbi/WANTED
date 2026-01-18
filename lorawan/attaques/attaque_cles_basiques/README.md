# Scénario d'attaque LoRaWAN

## Contexte
Dans un réseau LoRaWAN agricole utilisant une version obsolète de LoRaMAC (comme la 1.0.x), plusieurs failles de sécurité sont présentes :  
1. **Ancienne version du protocole** : LoRaMAC 1.0.x utilise moins de clés de sécurité, ce qui le rend vulnérable par rapport à la version 1.1.x.  
2. **Identifiants faibles et prévisibles** : Des DevEUI, et AppKey comme `000...`, `111...`, ou `123...` sont faciles à deviner, facilitant les attaques par force brute.  

Ces vulnérabilités rendent le réseau facile à attaquer, et c'est cette faiblesse présente sur le choix des identifiants que nous allons exploiter.

---

## Méthodologie de l'attaque
Il faut pour cette attaque, forcer, à la fois le DevEUI et l'AppKey, qui nous permettent d'établir la connexion avec la passerelle,et ensuite de pouvoir envoyer des données falsifiées.

---

## Rappel des clés de sécurité dans LoRaMAC 1.0.x en utilisant OTAA
- **AppKey** : Sécurise l’activation OTAA et génère les clés de session.
- **AppSKey** : Chiffre/déchiffre les données utilisateur.
- **NwkSKey** : Assure l’intégrité des messages MAC.

---

## Contre-mesures et recommandations
1. **Mettre à jour vers LoRaMAC 1.1.x** pour bénéficier des améliorations de sécurité.
2. **Utiliser des clés cryptographiques complexes et aléatoires**, difficiles à deviner.
3. **Changer régulièrement les clés du réseau** pour limiter l’exposition en cas de fuite.

---

# AVEC PYCOM

## Temps de calcul
- Dans le cas du code présent, nous avons énuméré 8 AppKey, ainsi que 6 DevEUI, ce qui fait un total de 48 combinaisons possibles.
De plus, nous avons configuré un timeout allant jusqu'à 10 secondes si la connexion échoue, afin de bien laisser le temps à une potentielle connexion réussie.
Si aucune combinaison ne permet de rejoindre le réseau, ou si nous allons jusqu'au bout des possibilités, alors nous devrons attendre un total de 8 minutes (480 secondes), et ce, en négligeant les potentielles latences.

- Cependant dans la réalité nous sommes plutot proche des 12 minutes nécessaires au test de la totalité des combinaisons.



---

# AVEC ARDUINO

## Temps de calcul
- Nous utilisons **le même nombre de clés** que dans le cas de Pycom, soit **48 combinaisons possibles** (8 AppKey × 6 DevEUI).  
- Chaque tentative de connexion prend environ **15 secondes** (incluant le timeout pour laisser le temps à une potentielle connexion).  
- **Temps total estimé** pour tester toutes les combinaisons : **12 minutes environ**.  



# Améliorations et limites  

## Améliorations possibles  
- **Étendre la bibliothèque de clés** pour tester un plus grand nombre de combinaisons courantes utilisées dans les déploiements vulnérables.  
- **Optimiser les délais d'attente** pour réduire le temps total d'exécution sans compromettre la fiabilité des tests.  

## Limites de l'attaque  
1. **Temps d’exécution croissant** : Plus on ajoute de clés, plus le temps de brute-force augmente exponentiellement.  
   - Chaque test doit respecter un délai (~10-15s) pour laisser la connexion s'établir.  

2. **Absence de ciblage précis** : L’attaque ne permet pas de choisir un réseau spécifique.  
   - Lors des tests, nous avons pu rejoindre un réseau en utilisant des identifiants faibles comme :  
     - **DevEUI** : `1111111111111111`  
     - **AppKey** : `11111111111111111111111111111111`  
     - Ou encore : `AAAAAAAAAAAAAAAA` et `AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA`  
   - Ces valeurs n'étaient **pas dans notre banc de test**, ce qui montre qu’un réseau avec de telles configurations était accessible.  

**Conclusion** : Bien que l'attaque soit efficace contre des identifiants faibles, son efficacité dépend fortement du contexte et du nombre de clés testées.  


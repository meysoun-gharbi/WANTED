Ce répertoire contient les codes destinés aux **nœuds Pycom**, à flasher via **Pymakr**. Il comprend deux scripts principaux :

1. **message_pycom.py** : Ce script envoie un message une seule fois. Il permet de tester la communication de base entre un nœud et un serveur avec une **App Key aléatoire** pour assurer une sécurité minimale.
   
2. **simulation_capteur_humidité.py** : Ce script simule un capteur d'humidité qui envoie des données toutes les 5 secondes. Il génère des valeurs d'humidité et les transmet à des intervalles réguliers, en utilisant également une **App Key aléatoire** pour une communication sécurisée.

Ces deux scripts ont pour objectif de mettre en place une **communication sécurisée de base**, en utilisant des clés d’application générées de manière aléatoire.

## Ajout futur

Un second capteur d'humidité sera simulé, ce qui ajoutera un **deuxième nœud légitime** au réseau. Cependant, pour celui-ci, une **négligence de sécurité** sera introduite en utilisant une **App Key basique** (composée uniquement de chiffres `0`). Cette configuration servira à démontrer les risques liés à l’utilisation de clés faibles et à l'importance d'une gestion rigoureuse des clés de sécurité.


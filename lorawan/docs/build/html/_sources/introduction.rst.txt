Introduction
============

Objectif du projet
******************

Ce projet a pour but d’expérimenter différentes attaques sur un réseau LoRaWAN, notamment :

- **Sniffing** : interception des paquets LoRaWAN transmis sur le réseau.
- **Brouillage** : perturbation du signal pour empêcher la communication.
- **Rejeu de paquets** : retransmission de paquets interceptés pour tromper le réseau.
- **Usurpation d’identité** : imitation d’un nœud légitime pour envoyer des données falsifiées.

Ce document décrit la mise en place de ces attaques ainsi que leur reproduction dans le cadre du projet.

Infrastructure LoRaWAN
======================

Matériel utilisé
****************

- **Passerelle LoRaWAN** fournie par l’encadrant, incluant :
  
  - Raspberry Pi 3 Model B
  - Module concentrateur LoRa IC880A

- **Serveur réseau ChirpStack** préconfiguré sur la passerelle.

Logiciel utilisé
****************

ChirpStack
----------

ChirpStack est utilisé comme serveur réseau LoRaWAN. Pour accéder à son interface web et configurer l’application :

1. Se connecter au réseau WiFi de la passerelle.
2. Ouvrir un navigateur et entrer l’URL : `http://192.168.0.1:8080/`.

Noeud légitime
==============

Matériel utilisé
****************

- **LoPy4 Pycom**

Logiciel utilisé
****************

Pymakr via VS Code
------------------

Mise en place :

1. Installer l’extension **Pymakr** sur **VS Code**.
2. Brancher le **LoPy4 Pycom** au PC via USB.
3. Synchroniser le LoPy4 avec le projet.
4. Flasher le code souhaité :

   - Sélectionner **"Upload on device"**.
   - Exécuter le script avec **"Run on device"** dans l’onglet Pymakr.

Code utilisé
------------

Les scripts sont disponibles dans le répertoire **`noeud_pycom`** du dépôt Git.

- **Fichier principal** : `simulation_capteur_humidité.py`
  
  - Ce fichier simule un capteur d’humidité.
  - Les clés cryptographiques peuvent être modifiées directement dans le code.
 

Configuration ChirpStack 
=========================
1- Connecter au réseau de ChirpStack 
- SSID: ChirpStackAP-d5b60e
- Password: ChirpStackAP

2- Configurer la paserelle dans ChirpStack 

- Il est nécessaire d’enregistrer le Gateway ID du passerelle dans ChirpStack afin que le serveur puisse l’authentifier, la reconnaître et router correctement les données des nœuds.
- Le Gateway ID est l'addresse MAC du Raspberry Pi, on peut l'extraire:
 	-lier le Raspberry Pi à un moniteur via un HDMI. Un clavier et une souris via un USB sur les ports du Raspberry pi.
 	
 	-Dans la console, taper: "ip link"
 	
 	-La MAC est la valeur après "link/ether"
 	
 	-Transformer la MAC en Gateway EUI64 en supprimant les ":" et insérant "fffe" au milieu 

3- Configuration du device

- On peut extraire le devEUI du device à partir du terminal Pymakr après ">>>" insérer le code suivant:
	>>> from network import LoRa
	>>> import ubinascii
	>>> lora = LoRa(mode=LoRa.LORAWAN, region=LoRa.EU868)
	>>> print(ubinascii.hexlify(lora.mac().upper())

- Après la génération du "Application Key" dans l'onglet "OTAA Keys" du device, il faut bien la changer dans les codes qui se trouvent dans "noeud_pycom".

- Après l'insértion du bonne clé dans le code, il faut le téléverser à nouveau dans le noeud lopy4.

	-Si l'Upload échoue, supprimer manuellement le contenu de flash du lopy dans le terminal "Pymakr" après ">>>":
	
	>>> import os
	>>> os.remove('main.py')
	
	-Puis, forcer le nouveau code avec:
	
	>>> import main
	

4- Si la passerelle et le device sont correctement configurés, on voit dans l'onglet "LoRaWANFrames" du ChirpStack un JoinRequest et un JoinAccept, confirmant l’activation OTAA. Ensuite apparaissent des UnconfirmedDataUp, envoyés par le capteur sans ACK pour économiser la batterie, ne pas saturer le réseau et pouvant être exploités pour une attaque par rejeu.

Noeud attaquant
===============

Matériel utilisé
****************

- **Arduino Dragino**

Logiciel utilisé
****************

Arduino IDE
-----------

Ou directement via un terminal Linux.

Bibliothèques utilisées
-----------------------

Pour ce projet, nous utilisons plusieurs bibliothèques Arduino compatibles avec LoRaWAN :

- **LoRa** de **Sandeep Mistry** (v0.8.0)
- **MCCI LoRaWAN LMIC** de **Terry Moore** (v5.0.1)
- **RadioLib** de **Jan Gromes** (v7.1.2)

from network import LoRa
import socket
import time
import binascii

# 1. Initialisation - On essaie de forcer la classe B si le firmware le permet
lora = LoRa(mode=LoRa.LORAWAN, region=LoRa.EU868)

# Clés pour l'OTAA
join_eui = binascii.unhexlify('0000000000000000')
app_key = binascii.unhexlify('ad6c170e0068fadd9767b90dec1651dc')

# Lancement du Join
lora.join(activation=LoRa.OTAA, auth=(join_eui, app_key), timeout=0)

while not lora.has_joined():
    time.sleep(2.5)
    print('Attente Join OTAA...')

print('Join réussi !')

# 2. Configuration du Socket pour forcer la communication
# Le simple fait d'envoyer un paquet après le Join oblige le serveur
# et le noeud à échanger les commandes MAC de configuration.
s = socket.socket(socket.AF_LORA, socket.SOCK_RAW)
try:
    s.setsockopt(0x8000, 1, 1) # SOL_LORA = 0x8000, IPPROTO_LORA = 1
except:
    print("Note: setsockopt a échoué, mais on continue...")

print('Envoi du paquet de synchronisation...')
s.setblocking(True)
s.send(bytes([0x01])) # Ce paquet "porte" les requêtes MAC invisibles
s.setblocking(False)

print('Requête envoyée. Le noeud attend maintenant le Beacon sur 869.525 MHz.')

# Boucle infinie pour maintenir le noeud actif
while True:
    time.sleep(10)
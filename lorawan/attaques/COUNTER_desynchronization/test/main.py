from network import LoRa
import socket
import binascii
import time
import pycom

# Désactiver le clignotement bleu
pycom.heartbeat(False)

# 1. Initialisation LoRaWAN v1.0.2
lora = LoRa(mode=LoRa.LORAWAN, region=LoRa.EU868)

# 2. Paramètres ABP
dev_addr = binascii.unhexlify('004dbc5d') 
nwk_swkey = binascii.unhexlify('5e43652a2ef251b1372d73e62e6719b1')
app_swkey = binascii.unhexlify('63042e3bbaaf652adb8f0f286319b50f')

# 3. Activation ABP
lora.join(activation=LoRa.ABP, auth=(dev_addr, nwk_swkey, app_swkey))

# Création du socket
s = socket.socket(socket.AF_LORA, socket.SOCK_RAW)
s.setsockopt(socket.SOL_LORA, socket.SO_DR, 5)

print("--- Noeud LoPy4 ABP Demarre ---")

while True:
    payload = "Hum:15%"
    print("Envoi Uplink...")
    
    s.setblocking(True)
    s.send(payload)
    s.setblocking(False)
    
    # LED Vert Flash
    pycom.rgbled(0x007f00) 
    time.sleep(0.5)
    pycom.rgbled(0x000000)
    
    print("Message envoye. Attente 15s.")
    time.sleep(15)
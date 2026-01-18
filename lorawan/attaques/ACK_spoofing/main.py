
from network import LoRa
import socket
import time
import ubinascii
import machine

# Initialise LoRa in LORAWAN mode.
# Please pick the region that matches where you are using the device:
# Asia = LoRa.AS923
# Australia = LoRa.AU915
# Europe = LoRa.EU868
# United States = LoRa.US915
lora = LoRa(mode=LoRa.LORAWAN, region=LoRa.EU868)

# create an OTAA authentication parameters, change them to the provided credentials
app_eui = ubinascii.unhexlify('0000000000000000') #app key non trouvée
app_key = ubinascii.unhexlify('a9230f79e57f0f4cd4acbd4b2250e702')
#uncomment to use LoRaWAN application provided dev_eui
dev_eui = ubinascii.unhexlify('70B3D54995CA5785')

# Uncomment for US915 / AU915 & Pygate
# for i in range(0,8):
#     lora.remove_channel(i)
# for i in range(16,65):
#     lora.remove_channel(i)
# for i in range(66,72):
#     lora.remove_channel(i)

# join a network using OTAA (Over the Air Activation)
#uncomment below to use LoRaWAN application provided dev_eui
#lora.join(activation=LoRa.OTAA, auth=(app_eui, app_key), timeout=0)
lora.join(activation=LoRa.OTAA, auth=(dev_eui, app_eui, app_key), timeout=0)

# wait until the module has joined the network
while not lora.has_joined():
    time.sleep(2.5)
    print('Not yet joined...')

print('Joined the network')
# create a LoRa sock
s = socket.socket(socket.AF_LORA, socket.SOCK_RAW)

# set the LoRaWAN data rate
s.setsockopt(socket.SOL_LORA, socket.SO_DR, 5)
# IMPORTANT : Mode CONFIRMED pour attendre un ACK
s.setsockopt(socket.SOL_LORA, socket.SO_CONFIRMED, True)

# make the socket blocking
# (waits for the data to be sent and for the 2 receive windows to expire)

msg_count = 0
while True:
    print("\n[{}] Tentative d'envoi...".format(time.time()))
    
    s.setblocking(True)
    s.settimeout(15) # On laisse le temps à la puce de faire ses tentatives
    
    try:
        s.send("ALERTE")
        # On repasse en non-bloquant pour lire
        s.setblocking(False)
        data = s.recv(64)
        if data:
            print("ACK REÇU")
        else:
            # Si le send est fini mais pas de data, c'est l'échec MAC
            print(" MAC_ERR : Pas d'ACK après les retransmissions.")
            
    except Exception as e:
        print("[-] Cycle terminé (Erreur ou Timeout)")

    time.sleep(20)

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

print('Joined')
# create a LoRa sock
s = socket.socket(socket.AF_LORA, socket.SOCK_RAW)

# set the LoRaWAN data rate
s.setsockopt(socket.SOL_LORA, socket.SO_DR, 5)

# make the socket blocking
# (waits for the data to be sent and for the 2 receive windows to expire)

# ===== 2. VARIABLES DE DÉTECTION =====
attack_count = 0
normal_count = 0
total_energy_impact = 0




while (True):
    print("Envoi Uplink")
    s.setblocking(True)
    s.settimeout(None)

    # send some data
    tx_start = time.ticks_ms()
    s.send("Humidity : 15%".encode('utf-8'))
    s.setblocking(False)

    
    
    
    # make the socket non-blocking
    # (because if there's no data received it will block forever...)

    # get any data received (if any...)
    rx_start = time.ticks_ms()
    data = None
    while time.ticks_diff(time.ticks_ms(), rx_start) < 3000:
        try: 
            data = s.recv(64)
            if data and len(data) > 0:
                break
        except:
            pass
        time.sleep_ms(5)

    rx_end = time.ticks_ms()
    duration = time.ticks_diff(rx_end, rx_start)
        

    
    print("Fenêtre d'écoute terminée en : {} ms".format(duration))
    
    if duration > 3000: 
        attack_count += 1
        print("*" * 30)
        print("!!! ALERTE ENERGY DRAIN !!!")
        print("Surconsommation : La radio est restée active {} ms".format(duration))
        if data:
            print("Données suspectes reçues : ", ubinascii.hexlify(data))
        else:
            print("Aucune donnée valide, mais la fenêtre a été forcée par l'attaquant.")
        print("*" * 30)
    time.sleep(10) #intervalle de 5s



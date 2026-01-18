#!/bin/bash
cd "$(dirname "$0")"
# Définir les valeurs par défaut
PORT="/dev/ttyACM0"  # Port série par défaut dans notre cas
BAUDRATE="9600"  # Débit par défaut
OUTPUT_FILE="sniffed_data.txt"
INO_FILE="arduino_sniffer.ino"

# Afficher une &ide explicative
usage() {
    echo "Usage: $0 [-p PORT] [-b BAUDRATE] [-o OUTPUT_FILE] [-h]"
    echo "  -p PORT       Spécifier le port série (ex: /dev/ttyUSB0)"
    echo "  -b BAUDRATE   Spécifier le débit en bauds (ex: 115200)"
    echo "  -o FICHIER    Nom du fichier de sortie (par défaut: sniffed_data.txt)"
    echo "  -h            Afficher cette aide"
    exit 1
}

# Lire les options passées en argument
while getopts "p:b:o:h" opt; do
  case ${opt} in
    p ) PORT="$OPTARG" ;;
    b ) BAUDRATE="$OPTARG" ;;
    o ) OUTPUT_FILE="$OPTARG" ;;
    h ) usage ;;
    * ) usage ;;
  esac
done

# Vérifier si minicom est installé
if ! command -v minicom &> /dev/null; then
    echo "Erreur : minicom n'est pas installé. Installer avec : sudo apt install minicom"
    exit 1
fi

# Étape 1 : Compiler et téléverser le code Arduino
echo "Téléversement du code de sniffing sur l'Arduino..."
arduino --upload "$INO_FILE" --port "$PORT"

if [ $? -ne 0 ]; then
    echo "❌ Échec du téléversement !"
    exit 1
fi

echo "✅ Téléversement réussi !"

# Étape 2 : Lancer minicom et enregistrer les paquets
echo "Démarrage du sniffing, enregistrement dans $OUTPUT_FILE..."
minicom -D "$PORT" -b "$BAUDRATE" -C "$OUTPUT_FILE"

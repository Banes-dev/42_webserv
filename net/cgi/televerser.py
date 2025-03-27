#!/usr/bin/python3
import os
import sys

# 📂 Répertoire de stockage des fichiers
UPLOAD_DIR = "/home/gschwart/webserv/42_webserv/net/picture"

# 📏 Lire la taille des données envoyées
try:
    content_length = int(os.environ.get("CONTENT_LENGTH", "0"))
except ValueError:
    content_length = 0

# 📥 Lire les données brutes de STDIN
data = sys.stdin.buffer.read(content_length)

# 🔍 Vérifier si les données sont valides
if not data or b"Content-Disposition" not in data:
    print("Content-Type: text/html\n")
    print("<html><body><p>Erreur: Aucun fichier reçu.</p></body></html>")
    sys.exit(1)

# 📌 Détecter le boundary
lines = data.split(b"\r\n")

# 🎯 Trouver le nom du fichier
filename = None
file_start_index = None
for i, line in enumerate(lines):
    if b'filename="' in line:
        filename = line.split(b'filename="')[1].split(b'"')[0].decode()
        filename = os.path.basename(filename)  # Sécuriser le nom du fichier
        file_start_index = i + 2  # L'image commence après "Content-Type"
        break

if not filename or file_start_index is None:
    print("Content-Type: text/html\n")
    print("<html><body><p>Erreur: Aucun nom de fichier trouvé.</p></body></html>")
    sys.exit(1)

# 📌 Construire le chemin du fichier
filepath = os.path.join(UPLOAD_DIR, filename)

temp = data.split(b"\n")
l = len(temp)
i = 0
zorro = b""
for z in temp:
    i += 1
    print(i, "\n", z, "\n")
    att = b""
    wer = b""
    if i > 4 and i < l - 1:
        yeho = z.split(b"\n")
        for li in yeho:
            att += li
        uoa = att.split(b"\r")
        for oi in uoa:
            wer += oi
        zorro += wer

print(zorro)

# 📥 Extraire uniquement les données binaires de l'image
file_data = b"\r\n".join(lines)  # Supprime la dernière ligne boundary

try:
    with open(filepath, "wb") as f:
        f.write(zorro)
except Exception as e:
    print("Content-Type: text/html\n")
    print(f"<html><body><p>Erreur lors de l'enregistrement du fichier: {str(e)}</p></body></html>")
    sys.exit(1)

# ✅ Réponse de succès
print("Content-Type: text/html\n")
print("<html><body>")
print(f"<p>Fichier <strong>{filename}</strong> televerser avec succes!</p>")
print(f"<p>Stocké dans : {filepath}</p>")
print("</body></html>")
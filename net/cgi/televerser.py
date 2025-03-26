#!/usr/bin/python3
import os
import sys

# 📂 Répertoire où stocker l'image
UPLOAD_DIR = "/net/picture"

# 📏 Lire la taille des données envoyées
try:
    content_length = int(os.environ.get("CONTENT_LENGTH", "0"))
except ValueError:
    content_length = 0

limit = os.environ.get("CONTENT_TYPE")
debut = limit.split("=")
util = debut[1]
# 📥 Lire les données brutes de STDIN
data = sys.stdin.buffer.read(content_length)

# 🔍 Vérifier si les données sont valides
if not data or b"Content-Disposition" not in data:
    print("Content-Type: text/html\n")
    print("<html><body><p>Erreur: Aucun fichier reçu.</p></body></html>")
    sys.exit(1)

# 📌 Détecter le boundary
print(data)
test = data.split(b"\n")
print(test[4]) #ok
lines = data.split(b"\r\n")
hello = test[4] + test[5]
print(test[6])
print(hello)
# 🎯 Trouver le nom du fichier
filename = None
for line in lines:
    if b'filename="' in line:
        filename = line.split(b'filename="')[1].split(b'"')[0].decode()
        filename = os.path.basename(filename)  # Sécuriser
        break

if not filename:
    print("Content-Type: text/html\n")
    print("<html><body><p>Erreur: Aucun nom de fichier trouvé.</p></body></html>")
    sys.exit(1)

#filepath = os.path.join(UPLOAD_DIR, filename)
filepath = "/home/gschwart/webserv/42_webserv/net/picture/9008217_orig.png"

print(filepath)
#with open(filepath, "wb") as f:
#    f.write(hello)  # Enlever le dernier saut de ligne
string = r"b'\x89PNG\r\n\x1a'"
with open(filepath,"wb") as file:
  file.write(eval(string))

# ✅ Réponse de succès
print("Content-Type: text/html\n")
print()
print("<html><body>")
print(f"<p>Fichier <strong>{filename}</strong> téléversé avec succès!</p>")
print(f"<p>Stocké dans : {filepath}</p>")
print("</body></html>")
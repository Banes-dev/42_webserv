#!/usr/bin/python3
import os
import sys

def read_multipart():
    content_length = int(os.environ.get("CONTENT_LENGTH", 0))
    if content_length > 0:
        return sys.stdin.buffer.read(content_length)
    return None

# Définir le répertoire où les fichiers seront enregistrés
UPLOAD_DIR = "/net/cd/data/picture"
os.makedirs(UPLOAD_DIR, exist_ok=True)

# Lire le contenu du fichier envoyé
data = read_multipart()

if data:
    limit = data.split(b"\r\n")[0]
    parts = data.split(limit)[1:-1]  # Ignorer la première et dernière partie
    
    for part in parts:
        headers, file_content = part.split(b"\r\n\r\n", 1) # separe les entetes du contenu du fichier
        headers = headers.decode()
        if "filename=\"" in headers:
            filename = headers.split("filename=\"")[1].split("\"")[0] # extrait le nom du fichier
            filename = os.path.basename(filename)  # Sécuriser le nom du fichier
            file_path = os.path.join(UPLOAD_DIR, filename)
            
            with open(file_path, "wb") as f:
                f.write(file_content.rsplit(b"\r\n", 1)[0])  #ecrit et Supprime le dernier retour à la ligne
            
            print("Content-Type: text/html\n")
            print("<html><body>")
            print(f"<p>Fichier <strong>{filename}</strong> téléversé avec succès!</p>")
            print("</body></html>")
            break
else:
    print("Content-Type: text/html\n")
    print("<html><body><p>Aucun fichier reçu.</p></body></html>")

# Suppression d'un fichier si un paramètre 'delete' est passé
def delete_file():
    query_string = os.environ.get("QUERY_STRING", "")
    if query_string.startswith("delete="):
        filename = os.path.basename(query_string.split("=", 1)[1]) # securisation du chemin
        file_path = os.path.join(UPLOAD_DIR, filename)
        if os.path.exists(file_path):
            os.remove(file_path)
            print("Content-Type: text/html\n")
            print("<html><body>")
            print(f"<p>Fichier <strong>{filename}</strong> supprimé avec succès!</p>")
            print("</body></html>")
        else:
            print("Content-Type: text/html\n")
            print("<html><body><p>Fichier introuvable.</p></body></html>")

if "QUERY_STRING" in os.environ and "delete=" in os.environ["QUERY_STRING"]:
    delete_file()

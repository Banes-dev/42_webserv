#!/usr/bin/python3
import os
import cgi

query-string = os.getenv("QUERY-STRING")
form = cgi.FieldStorage()
name = form.getvalue('name', 'unknown')
commentaire = form.getvalue('commentaire', 'vide')

print ("Content-type:text/html; charset=UTF-8")
print ()

a = """
<!DOCTYPE html>
    <html>
    <head><title>Hello word</title></head>
    <body>
        <p>Hello {name} </p>
        <p>1er CGI</p>
        <p>{commentaire}</p>
    </body>
    </html>
"""
print(a)

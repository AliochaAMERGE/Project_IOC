# Project_IOC
Projet d'IOC M1 SAR 2021

Objectif : 

Controler un esp32 depuis un navigateur sur pc, en passant par un serveur sur raspberry3.

TODO :

- Sur le raspberry

  - creer un serveur local sur le raspberry en python
    - creer un site vitrine sur ce serveur 
    - creer une base de donnée sqlite simple contenant une unique table, pour l'historique des capteurs (date, capteur, valeur)
    - javascript : actualisation constante des valeurs d'un capteur
    - ouvrir le serveur pour un acces non local

- Sur l'ESP32
  - brancher Led et ou photorésistance
  - connecter au raspberry via wifi

- Sur le pc
  - rien pour le moment, on ouvrira juste l'url du serveur (local dans un premier temps)
  - configurer l'environnement pour travailler sur le raspberry en ssh.

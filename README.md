# Project_IOC
Projet d'IOC M1 SAR 2021

Objectif : 

Controler un esp32 depuis un navigateur sur pc, en passant par un serveur sur raspberry3.

TODO :

projet MQTT

raspberry ssh : 192.168.1.46

- Sur le raspberry

  - creer un serveur local sur le raspberry en python
    - lecture d'un capteur et affichage de sa valeurs
    - écriture sur un capteur
    - creer un site vitrine sur ce serveur 
      - nous y afficherons au début les valeurs des capteurs apres une requete.
    - creer une base de donnée sqlite simple contenant une unique table, pour l'historique des capteurs (date, capteur, valeur)
    - javascript : actualisation constante des valeurs d'un capteur
    - ouvrir le serveur pour un acces non local
  
- Sur l'ESP32

  - brancher Led et ou photorésistance, et ou bouton (tout ce qu'on a en fait)
  - connecter au wifi : https://techtutorialsx.com/2017/04/24/esp32-connecting-to-a-wifi-network/
  - connecter au raspberry en reseau local via le wifi

- Sur le pc

  - configurer l'environnement pour travailler sur le raspberry en ssh.
  - rien d'autre pour le moment, on ouvrira juste l'url du serveur (local dans un premier temps)


Docs :

MQTT project explanation and examples :
  https://cumulocity.com/guides/device-sdk/introduction/

Similar project using MQTT : 
   https://create.arduino.cc/projecthub/BnBe_Club/mqtt-communication-with-the-nano-33-iot-wemos-d1-boards-5f7f0e?ref=tag&ref_id=mqtt&offset=4
  
MQTT & esp32 :
 https://www.youtube.com/watch?v=5tG3JXFYrUo

libraire platform io pour MQTT : PubSub `pio lib install 89`

MQTT on raspberry
   https://appcodelabs.com/introduction-to-iot-build-an-mqtt-server-using-raspberry-pi

https://www.valvers.com/open-software/arduino/esp32-mqtt-tutorial/

https://randomnerdtutorials.com/esp32-mqtt-publish-subscribe-arduino-ide/

http://www.steves-internet-guide.com/mqtt-websockets/


TODO : 

dans le code serveur, mettre les ip et mdp en parametre, ou dans un fichier à part
javascript : graphe des données d'il y a un certain temps
Vérifié que la db ne se rempli pas trop, et au besoin regulé automatiquement
ajouter un bouton pour allumé eteindre la led (sur un autre topic : esp32/input i.e.)
nettoyer et commenter nos ajouts (surtout dans le rapport en fait)
ajouter des jolies couleurs sur la page de garde
jauge pour la luminosité ?

commencer le rapport avec les différentes étapes et raisonnement





# Page de garde

# table des matieres


# introduction et format du projet 

Comment on voulait faire les trucs initialement, et une introduction du sujet
faire un shéma de quoi est ou (le mqtt et server sur raspberry), esp tier avec tel periph

# ESP32

le code sur l'esp32 et ses limites (deconnection, meme si je crois que ça vient du fait qu'il recoive trop de message, on devrait tenter avec deux topic)


# Mise en place du broker

installation, mosquitto, et ou est il

# website

ce que l'on voulait faire, quel outils etc, rapidement

## installation du serveur Django

pourquoi django, et rapide mise en place

## création de la base de donnée

pourquoi tel données, et format

## connection entre la base de donnée et l'esp via le broker

quel méthodes, et ou sont elles appelée

## partie java script et frontend

le joli

# conclusion

# sources

Création d'un environnement python approprié + création d'un server avec python et Django

https://www.youtube.com/watch?v=rA4X73E_HV0

- activate required python environnement
pyenv activate DjangoWebsite@3.7.3 

- install requirements  
pip install -r requirements/dev.txt

- start server
python manage.py runserver

pour le diagramme quand on aura récupérer les données
https://startbootstrap.com/template/sb-admin
https://www.youtube.com/watch?v=rzEXSEkS8gM



JS part : 
https://www.freecodecamp.org/news/how-to-make-your-first-javascript-chart/

pour créer le serveur py - base en 10 mminutes
https://www.youtube.com/watch?v=ZsJRXS_vrw0 
https://www.youtube.com/watch?v=PqeAvFf_HDI minimum, but work well

Gérer les querries dans le serveur django 
https://docs.djangoproject.com/en/3.2/topics/db/queries/

Libraire django mqtt
https://github.com/ehooo/django_mqtt


run with : ./manage.py runserver --noreload


https://www.youtube.com/watch?v=B4Vmm3yZPgc

https://canvasjs.com/docs/charts/how-to/creating-dynamic-charts/





/****** DELETE ****/
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

heure d'activité ? (lum > val définie)

commencer le rapport avec les différentes étapes et raisonnement

/****** FIN DELETE ****/


localhost/app/index
paho mqtt 
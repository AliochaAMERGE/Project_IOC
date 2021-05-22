# IOC [MU4IN109] - Micro projet

Réalisé par :

- **Haitham OUERSIGHNI M1 SAR**
- **Aliocha AMERGÉ M1 SAR**

Lien du projet : https://github.com/AliochaAMERGE/Project_IOC

Ce projet a été réalisé dans le cadre de l'UE IOC (MU4IN109) du master 1 Informatique mention SAR de *Sorbonne Université*.

<div style="page-break-after: always;"></div>

# Table of contents

- [IOC [MU4IN109] - Micro projet](#ioc--mu4in109----micro-projet)
- [Table of contents](#table-of-contents)
- [Introduction et format du projet](#introduction-et-format-du-projet)
  * [Les consignes :](#les-consignes--)
  * [Nos objectifs :](#nos-objectifs--)
  * [Matériel utilisé :](#mat-riel-utilis---)
- [Mise en place du broker](#mise-en-place-du-broker)
- [ESP32](#esp32)
- [website](#website)
  * [installation du serveur Django](#installation-du-serveur-django)
  * [création de la base de donnée](#cr-ation-de-la-base-de-donn-e)
  * [connection entre la base de donnée et l'esp via le broker](#connection-entre-la-base-de-donn-e-et-l-esp-via-le-broker)
  * [partie java script et frontend](#partie-java-script-et-frontend)
- [conclusion](#conclusion)
- [pour aller plus loin](#pour-aller-plus-loin)
- [sources](#sources)


# Introduction et format du projet 

## Les consignes :

&emsp;Le micro-projet consiste à mettre en place un serveur HTTP sur une rasperrypi. 
Ce serveur devra afficher des données envoyées par l'ESP32, et permettre d'envoyer des messages à afficher sur l'écran OLED de l'ESP32.
Les données envoyées seront les valeurs de la photorésistance.

## Nos objectifs :

&emsp;Au cours de ce micro-projet, nous voulons faire communiquer un ESP32 avec un raspberrypi, et ce au travers d'un broker MQTT.
L'objectif est donc de récupérer les valeurs de la photorésistance de l'ESP32, les envoyées au broker MQTT implémenter sur le raspberrypi,
qui à son tour, transfère les données sur le site web implémenter également sur le raspberrypi.



## Matériel utilisé :

&emsp;Au cours de ce micro-projet, nous utiliserons :
- Un [ESP32 + LoRa](https://randomnerdtutorials.com/ttgo-lora32-sx1276-arduino-ide/) avec un écran OLED intégré (modèle TTGO LoRa32 SX1276 OLED). 

- Un [raspberry pi 3 modèle B](https://www.raspberrypi.org/products/raspberry-pi-3-model-b/) avec [Raspberry Pi OS](https://www.raspberrypi.org/software/operating-systems/#raspberry-pi-os-32-bit) (anciennement Raspbian) installé.

- Un serveur utilisant le framework python [Django](https://www.djangoproject.com/)

- Un broker MQTT [Mosquitto](https://mosquitto.org/)

Nous vous remercions pour nous avoir mis tout ce matériel à disposition.

# Mise en place du broker

## Qu'est ce qu'un broker MQTT ?

&emsp;MQTT (message Queuing Telemetry Transport) est un protocole de messagerie qui fonctionne sur le principe de souscription / publication. [[1]](https://projetsdiy.fr/mosquitto-broker-mqtt-raspberry-pi/)
Concrêtement, le broker MQTT fait office d'intérmédiaire entre différents clients. Lorsque qu'un client s'**abonne** (subscribe) sur un **topic**, le broker lui transmettra tous les messages **publié** (publish) par d'autre client sur ce **même topic**. Les topics sont liés au broker, il n'y aura pas de conflit si nous utilisons deux fois le même topic sur deux brokers différents, ces deux topics seront distincts.

![MQTT-example](/img/MQTT.png)
*Fonctionnement d'un broker* : nous avons deux clients (4 et 5) abonnés sur deux topic */data/A* et */data/B* et trois clients (1, 2 et 3) publiant des messages sur les topics */data/A* et */data/b*. 
Le broker transférerant tous les messages du client 1 et 3 aux client 4 et 5, et tous les messages des clients 2 et 3 au client 4.

Dans ce micro-projet, le schéma sera le suivant : 

![](/img/ourMQTT.png)

Le raspberry héberge le broker MQTT et le site web. L'ESP32 produit les données via sa photorésistance.
Les données de la photorésistance sont envoyé au broker via le topic esp32/output (valeurs entre 0 et 4000).
L'utilisateur à la possibilité d'allumer ou éteindre la LED depuis le site web, les messages seront envoyé sur le topic /esp32/input et contiendront les valeurs "on" ou "off".

## Installation du broker MQTT sur la raspberry

&emsp;Nous utilisons un broker MQTT [Mosquitto](https://mosquitto.org/) intégré au raspberrypi.

Source pour reproduire l'installation : 
https://appcodelabs.com/introduction-to-iot-build-an-mqtt-server-using-raspberry-pi

- 1. Install the mosquitto MQTT Broker 

`sudo apt install mosquitto mosquitto-clients`

- 2. Enable the mosquitto broker
    
Enable the broker and allow it to auto-start after reboot using the following command:
`sudo systemctl enable mosquitto`

- 3. Subscribe to the MQTT Topic Locally

In the existing terminal, subscribe to the `test/message` topic:

terminal 1: 
`mosquitto_sub -h localhost -t "test/message"`

terminal 2: 
`mosquitto_pub -h localhost -t "test/message" -m "Hello, world"`

Hello, world apparait sur le Terminal 1


# ESP32

le code sur l'esp32 et ses limites (deconnection, meme si je crois que ça vient du fait qu'il recoit trop de message, on devrait tenter avec deux topic) c'est tenter avec deux topics et c'est toujours cassé

# website

ce que l'on voulait faire, quel outils etc, rapidement, c'est l'intro de la partie

## installation du serveur Django

pourquoi django, et rapide mise en place

## création de la base de donnée

pourquoi tel données, et format

## connection entre la base de donnée et l'esp via le broker

quel méthodes, et ou sont elles appelée
pourquoi avons nous ajouter les données direct dans la bdd, et pas au niveau des messages

NB : on a du mqtt c++ python et js

ATTENTION, erreur si reception d'un message non composé de nombre

## partie java script et frontend

le joli

# conclusion

les connaissances que nous avons acquise

# pour aller plus loin 

Ce que nous aurions pu faire si plus de temps / de matos, et comment l'aurons nous fait rapidement

TODO : expliquer plus en détails comment le faire dans la mesure du possible

Ajouter un menu pour ajouter plusieurs esp32, et faire une table par esp32, ou un identifiant et les mettres dans la meme table et gérer plus efficacement les adresses ip des serveurs et ou broker.

Gérer des requetes pour récupérer la moyenne des xx dernieres données, ou des données au cours des xx derniere heure (sql)

faire en sorte que les données du graphique restent inchangées lorsqu'il n'y a pas de nouvelles valers dans la BD

utiliser le BP pour activer desactiver le transfert des données sur le broker

gérer d'autre capteurs que les photorésistance

utiliser le bluetooth plutot que le wifi, pour plus de portabilité

dans la chart, utiliser les dates plutot que rien actuellement

# sources

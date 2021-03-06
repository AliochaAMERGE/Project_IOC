  <img align ="left" height="80" width="200"  src="/img/Sciences_SU-removebg-preview.png">
  <img  align="right" height="80" width="200" src="/img/peri.png">

<center> <h1>IOC [MU4IN109] - Micro projet</h1> </center>

Réalisé par :

- **Haitham &emsp; OUERSIGHNI &emsp; M1 SAR**
- **Aliocha&emsp;&emsp;AMERGÉ &emsp;&emsp;&emsp; M1 SAR**

Lien du projet : *https://github.com/AliochaAMERGE/Project_IOC*

Ce projet a été réalisé dans le cadre de l'UE IOC (MU4IN109) du master 1 Informatique mention SAR de *Sorbonne Université* sous la direction de Monsieur Franck Wajsburt.

<div style="page-break-after: always;"></div>

# Table of contents

- [Table of contents](#table-of-contents)
    - [Instructions concernant ce projet :](#instructions-concernant-ce-projet-)
- [Introduction et format du projet](#introduction-et-format-du-projet)
  - [Les consignes :](#les-consignes-)
  - [Matériel utilisé :](#matériel-utilisé-)
- [Mise en place du broker MQTT](#mise-en-place-du-broker-mqtt)
  - [Qu'est ce qu'un broker MQTT ?](#quest-ce-quun-broker-mqtt-)
  - [Installation du broker MQTT sur le raspberrypi](#installation-du-broker-mqtt-sur-le-raspberrypi)
- [ESP32](#esp32)
  - [Déroulement de l'algorithme](#déroulement-de-lalgorithme)
    - [**Notre implémentation :**](#notre-implémentation-)
  - [Connection au broker MQTT](#connection-au-broker-mqtt)
    - [Connection WiFi](#connection-wifi)
    - [Connection au broker MQTT](#connection-au-broker-mqtt-1)
  - [esp32/output](#esp32output)
  - [esp32/input](#esp32input)
  - [Limites de l'implémentation](#limites-de-limplémentation)
  - [Résumé](#résumé)
- [Website](#website)
  - [Objectifs](#objectifs)
  - [Implémentation du serveur Django](#implémentation-du-serveur-django)
    - [Pourquoi Django ?](#pourquoi-django-)
    - [Création du serveur Django](#création-du-serveur-django)
  - [Implémentation d'une base de donnée](#implémentation-dune-base-de-donnée)
  - [connection entre la base de donnée et l'esp32 via le broker MQTT](#connection-entre-la-base-de-donnée-et-lesp32-via-le-broker-mqtt)
    - [esp32/output](#esp32output-1)
    - [esp32/input](#esp32input-1)
  - [Frontend](#frontend)
    - [HTML et CSS](#html-et-css)
    - [Recupération des valeur](#recupération-des-valeur)
      - [Passage d'une valeurs en paramètre du rendu](#passage-dune-valeurs-en-paramètre-du-rendu)
      - [Passage d'une valeurs via une route dédiée](#passage-dune-valeurs-via-une-route-dédiée)
    - [Live data](#live-data)
- [Conclusion](#conclusion)
- [Pour aller plus loin](#pour-aller-plus-loin)
- [Remerciement](#remerciement)


### Instructions concernant ce projet :

Pour tester ce projet chez vous, vous aurez besoin de :
  - Pyenv (installation détaillée dans le rapport)
  - Un IDE supportant la programmation Arduino (PlatformIO, ArduinoIDE) ainsi que certaines libraires (détaillées dans le rapport).
  - Un ESP32 ou un Arduibno
  - Une connection WiFi fonctionnelle (5 GHz non supporté)
  - Un raspberrypi (ou un pc / serveur) pouvant héberger le serveur et le broker MQTT

- Modification nécéssaire :

- Création d'un fichier : `esp32/include/WiFiCredentials.h`
  ```cpp
    const char *SSID = "xxxxxxx";
    const char *WiFiPassword = "xxxxxxxx";
  ```
- Création d'un fichier : `esp32/include/MQTTCredentials.h`
  ```cpp
    const char* mqtt_server = "xxx.xxx.x.xx";
    const int mqtt_port = 1883;  // port par défaut
  ```

- Modification des fichers [*website/myapp/mqtt_output.py*](website/myapp/mqtt_output.py) et [*website/myapp/views.py*](website/myapp/views.py) en modifiant les valeurs de `mqtt_server` et `mqtt_port`.

&emsp;Pour lancer le serveur : 

- Dans le dossier [website](website/), exécuter la commande `./manage.py runserver --noreload`. Le serveur se lancera sur l'adresse `localhost:8000`.
Dans une optique de test, il est possible d'ajouter des données manuellement à la base de donnée via la route "/admin".
Attention à ne pas laisser le serveur et l'esp32 connecté trop longtemps, la base de donnée pourrait atteindre une taille indésirée.

&emsp;Au cours de ce rapport, de nombreux lien hypertexte sont présents, ainsi que des images et un formatage particulier.
Pour plus de confort, il est *fortement* conseillé de lire ce rapport sur https://github.com/AliochaAMERGE/Project_IOC.

Merci pour tout, et bon courage dans la lecture de ce rapport.


# Introduction et format du projet 

## Les consignes :

&emsp;Ce micro-projet consiste à faire communiquer un ESP32 et un serveur HTTP au travers d'un broker MQTT.
&emsp;Nous devons donc mettre place un serveur HTTP sur un rasperrypi. Ce serveur affichera des données envoyées depuis l'ESP32 et permettra à son tour d'envoyer des messages et d'autre commande à afficher sur l'écran OLED de l'ESP32.
Les données envoyées par l'ESP32 seront les valeurs de la photorésistance.

## Matériel utilisé :

&emsp;Au cours de ce micro-projet, nous utiliserons :
- Un [ESP32 + LoRa](https://randomnerdtutorials.com/ttgo-lora32-sx1276-arduino-ide/) avec un écran OLED intégré (modèle TTGO LoRa32 SX1276 OLED). 

- Un [raspberry pi 3 modèle B](https://www.raspberrypi.org/products/raspberry-pi-3-model-b/) avec [Raspberry Pi OS](https://www.raspberrypi.org/software/operating-systems/#raspberry-pi-os-32-bit) (anciennement Raspbian) installé.

- Un serveur utilisant le framework python [Django](https://www.djangoproject.com/)

- Un broker MQTT [Mosquitto](https://mosquitto.org/)

# Mise en place du broker MQTT

## Qu'est ce qu'un broker MQTT ?

&emsp;MQTT (message Queuing Telemetry Transport) est un protocole de messagerie qui fonctionne sur le principe de souscription / publication. [[1]](https://projetsdiy.fr/mosquitto-broker-mqtt-raspberry-pi/)
Concrêtement, le broker MQTT fait office d'intérmédiaire entre différents clients. Lorsque qu'un client s'**abonne** (subscribe) sur un **topic**, le broker lui transmettra tous les messages **publiés** (publish) par d'autre clients sur ce **même topic**. Les topics sont liés au broker, il n'y aura pas de conflit si nous utilisons deux fois le même topic sur deux brokers différents, ces derniers seront distincts.

![MQTT-example](/img/MQTT.png)

&emsp;*Fonctionnement d'un broker* : nous avons deux clients (4 et 5) abonnés sur deux topic */data/A* et */data/B* et trois clients (1, 2 et 3) publiant des messages sur les topics */data/A* et */data/b*. 
Le broker transfère tous les messages du client 1 et 3 aux client 4 et 5 (topic : */data/A*), et tous les messages des clients 2 et 3 au client 4 (topic */data/B*).

Dans notre micro-projet, le schéma sera le suivant : 

![](/img/ourMQTT.png)

&emsp;Le raspberrypi héberge le broker MQTT et le site web. L'ESP32 produit des données via sa photorésistance.
Les données de la photorésistance sont envoyées au broker via le topic `esp32/output` (valeurs : entier entre 0 et 4000).
L'utilisateur à la possibilité d'effectuer différentes actions pré-configurées sur l'ESP32 (allumer ou éteindre la LED, afficher un message sur l'écran OLED) directement depuis le site web. Nous utiliserons le topic `esp32/input`.

## Installation du broker MQTT sur le raspberrypi

&emsp;Nous utilisons un broker MQTT [Mosquitto](https://mosquitto.org/) intégré au raspberrypi.

- *Source pour reproduire l'installation :*

https://appcodelabs.com/introduction-to-iot-build-an-mqtt-server-using-raspberry-pi

- 1. Installation de Broker MQTT Mosquitto (sur Ubuntu / Débian)

`sudo apt install mosquitto mosquitto-clients`

- 2. Activation du broker mosquitto
    
Active le broker et l'autorise à se lancer automatiquement après avoir redémarrer le raspberrypi
`sudo systemctl enable mosquitto`

Si la connection est refusée lors d'un abonnement ou une publication, il peut être nécéssaire de redémarrer le démon mosquitto :
`mosquitto -d`

- 3. *Subscribe* à un topic MQTT (localement)

Nous effectuerons ces commandes sur deux terminaux distincts :
Sur le terminal 1, abonnement au topic `test/message` :
`mosquitto_sub -h localhost -t "test/message"`

Sur le terminal 2, nous publierons "Hello World" sur le topic `test/message` : 
`mosquitto_pub -h localhost -t "test/message" -m "Hello, world"`

> Hello, world apparait sur le Terminal 1

&emsp;Notre broker Mosquitto est opérationnel, mais pour le moment il n'est accessible que localement. Dans le cadre du projet, nous le laisserons tel quel, mais pour une utilisation plus poussée, il sera nécéssaire d'ouvrir les ports pour un accès depuis l'extérieur (par exemple si nous hébergons le site via un service de cloud distant)

&emsp;Notre broker MQTT étant installé sur le raspberry, il nous faut récupérer l'adresse de ce dernier pour pouvoir y accéder (même localement il est nécéssaire de le différencier d'autres appareils).

- Pour ce faire nous récupérons l'adresse avec :

`hostname -I`

- Si le broker est installé sur Windows, il faudra utilisé la commande : 

`ipconfig`

Cette adresse sera utile à tout moment lorsque nous voudrons converser avec le broker MQTT.


# ESP32

&emsp;Nos objectifs sur l'esp32 sont de gérer deux topics : `esp32/output` duquel nous enverrons les données de la photorésistance, et `esp32/input` duquel nous recevrons des *commandes* ayant différentes actions.

Nous utiliserons [Visual Studio Code](https://code.visualstudio.com/) avec l'extension intégrant l'IDE [PlatformIO](https://platformio.org/) pour réaliser toute la partie sur l'ESP32.
Cette extension est extrêmement compléte et bien documentée, la gestion des librairies est simplifiée, et toutes les opérations sont intuitives et ou documentée.

Nous aurons besoin d'importer 3 librairies :
- PubSub : pour gérer la communication avec le broker
`pio lib install "knolleary/PubSubClient"`
- Adafruit GFX Library : librarie graphique
`pio lib install "adafruit/Adafruit GFX Library"`
- Adafruit SSD1306 : librairie de driver oled SSD1306 pour des écrans monochrome 128x64 et 128x32
`pio lib install "adafruit/Adafruit SSD1306"`

Nous utiliserons également les libraires suivantes :
`Arduino.h`, `WiFi.h`, `SPI.h` et `Wire.h`

## Déroulement de l'algorithme

- **Programmation sur Arduino (esp32) :**

&emsp;La programmation sur esp32 fonctionne de la même façon que la programmation sur Arduino :
Le code nécéssite deux méthodes : `void setup();` et `void loop();`

  - `void setup();`

&emsp;La méthode sera appelée une fois au démarrage de l'esp32, elle sert notamment à l'initiatlisation des variables, à la connection à un réseau, ou toutes actions que nous ne réalisons qu'une fois.
Nous pouvons appeler d'autre méthodes dans le `setup()`, ces méthodes ne seront executée qu'une unique fois.

  - `void loop();`

&emsp;La méthode sera appelée périodiquement, c'est en quelque sorte un `while(true){ loop() }`. Nous réalisons les différentes actions périodique de notre code dans cette méthode. 
Nous pourrons appeler d'autre méthodes dans le `loop()`, ces méthodes seront exécutée séquentiellement jusqu'à arrêt du programme.

Les programmes Arduino peuvent en gérénal être réprésenté sous la forme de diagramme d'état, en voici un exemple :

![automate](img/mermaid_automate-removebg-preview.png)

*Ici, nous avons 2 méthodes appelées dans le setup(), et 3 méthodes dans le loop().*

### **Notre implémentation :**

Fichier : [*esp32/src/main.cpp](esp32/src/main.cpp)

- **Les variables**

```cpp
// Le broker
WiFiClient espClient;
PubSubClient client(espClient);
// l'écran
Adafruit_SSD1306 display
```

- **setup()**

```cpp
void setup(){
  // Connection au WiFi
  ConnectWifi();
  // Connection au broker MQTT & subscribe aux topics désirés
  ConnectMQTT();
  // et c'est tout, les autres fonctionnalités sont gérée dans ConnectMQTT().
}
```

- **loop()**

```cpp
void loop(){
  // se reconnecter au broker en cas de déconnection
  if (!client.connected()) {
    reconnect();
  }
  // l'attente de réception des messages du broker
  client.loop();

  /**** Envoie des messages sur esp32/output ****/
  /*    explicité dans une prochaine partie    */
}
```

## Connection au broker MQTT

### Connection WiFi

&emsp;Afin de nous connecter au broker MQTT, nous aurons tout d'abord besoin de nous connecter au WiFi.
Pour cela, nous aurons besoin de la librairie `WiFi.h` :
Fichier : [*esp32/include/WifiConfig.h*](esp32/include/WifiConfig.h)

```cpp
void connectWifi(void) {
  // connection en fonction des identifiants et mot de passe fournis
  WiFi.begin(SSID, WiFiPassword);
  // Attente bloquante tant que la connection n'a pas abouti
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}
```

Les identifiants (SSID) et mot de passe (WiFiPassword) seront indiqués dans le fichier */esp32/include/WiFiCredentials.h* contenant ces deux variables :

```cpp
const char *SSID = "xxxxxxx";
const char *WiFiPassword = "xxxxxxxx";
```

**ATTENTION : Le fichier */esp32/include/WiFiCredentials.h* devra être créé par l'utilisateur, et modifier en cas de changement de réseau WiFi !**

### Connection au broker MQTT

Une fois la connection WiFi établie, nous pouvons nous connecter au broker MQTT.
De la même manière que le WiFi, les informations de connection seront stockées dans un fichier */esp32/include/MQTTCredentials.h* contenant : 
```cpp
const char* mqtt_server = "xxx.xxx.x.xx";
const int mqtt_port = 1883;  // port par défaut
```

Fichier : [*esp32/include/MQTTConfig.h*](esp32/include/MQTTConfig.h)

```cpp
// Connection au serveur 
client.setServer(mqtt_server, mqtt_port);
// Gestion du callback
client.setCallback(callback);
// abonnnement au topic esp32/input
client.subscribe("esp32/input")
```

La méthode `setCallback()` prend en paramètre un pointeur sur fonction et indiquera la procédure à réaliser en cas de réception d'un message. Nous expliciterons cette méthode ultérieurement.

La souscription sera également explicitée par la suite.

En cas de déconnection au serveur, la méthode `void reconnect()` est appelée : 
```cpp
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/input");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
```
Nous relançons tout simplement la connection au serveur, ainsi que les abonnements aux topics nécéssaire.

A ce niveau, nous avons établis une connection au WiFi et au broker MQTT, mais nous ne recevons ni n'envoyons de données vers ce dernier, c'est ce que nous verrons par la suite.

Nous mettons maintenant en place deux topics, un pour les messages *entrants*, et un pour les messages *sortants*.

## esp32/output

Ce topic concerne les données *sortantes* de l'esp32 vers le site web, c'est à dire dans notre implémentation, les valeurs captée par la photorésistance.
Cette valeurs est lu toutes les secondes, puis envoyé au broker :

```cpp
long now = millis();
if (now - lastMsg > 1000) {
  lastMsg = now;
  lumos = analogRead(PHOTORESISTANCE_PIN);

  /* Convert the value to a char array */
  char tempString[8];
  dtostrf(lumos, 1, 2, tempString);
  Serial.print("value = ");
  Serial.println(tempString);

  client.publish("esp32/output", tempString);
}
```
&emsp;Nous verifions si le délai d'une seconde depuis le dernier envoie est atteint (d'une manière très similaire au waitFor() vu au cours du TME 3).
Ensuite nous récupérons la valeurs de la photorésistance avec `analogRead(byte pin)`. Cette valeurs est comprise entre 0 et 4000 (du plus clair au plus sombre). Elle est ensuite convertie en une chaine de caractère, puis publiée sur le topic `esp32/ouput`.

**NB** : Nous n'utilisons pas la fonction `delay(1000)` car cela provoque une attente bloquante ce qui empêche l'appel à d'autre méthodes ("met en pause le *loop()*").

## esp32/input

&emsp;Ce topic concerne les données *entrantes* vers l'esp32 depuis le site web, ce sont des *commandes* depuis le site web.
Pour le moment, nous avons 3 commandes mise en place :
- "LedOn" : allume la Led (*LEDBUILTIN*)
- "LedOff" : éteint la Led (*LEDBUILTIN*)
- un autre message : affiche le contenu du message sur l'écran OLED (le message à une taille maximale)

Ces commandes sont gérées dans la fonction `callback()` précedemment énoncée :

```cpp
void callback(char* topic, byte* message, unsigned int length) {
  
  // permet de traduire le message (tableau de byte) en String
  String messageTemp = "";
  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }
  pinMode(LED_BUILTIN, OUTPUT);

  // Si nous recevons un message du topic esp32/input
  if (String(topic) == "esp32/input") {
    // si message est la commande "LedOn"
    if (String(messageTemp) == "LedOn") {
      // on allume la LED
      digitalWrite(LED_BUILTIN, HIGH);
    } else
    // si message est la commande "LedOff"
     if (String(messageTemp) == "LedOff") {
      // on éteint la LED
      digitalWrite(LED_BUILTIN, LOW);
    } else {
      // on affiche le contenu du message sur l'écran Oled
      display.clearDisplay();       // efface le display
      display.setCursor(20, 10);    // place le curseur
      display.println(messageTemp); // écrit le message dans le buffer
      display.display();            // affiche le contenu du buffer
    }
  }
}
```
&emsp;L'abonnement au topic `esp32/input` se fait à la connection et / ou à la reconnection au broker MQTT.

&emsp;Le code est plutot explicite, lors de la récetion d'un message, nous le convertissions en String, si le topic du message est `esp32/input`, nous le traitons.
Cette verification est utile si nous nous abonnons à plusieurs topics (ce qui n'est pas le cas dans notre implémentation).
Nous passons le mode du pin concernant la Led en OUTPUT afin de pouvoir écrire une valeurs (HIGH ou LOW), si nous recevons "LedOn" nous allumons la Led, et inversement si nous recevons "LedOff". 
Si nous recevons un autre message, ce dernier sera afficher sur l'écran (ne fonctionne que pour des messages relativement court pour le moment, mais cela est facilement modifiable en fonction des besoins).

<img src="img/OledExample.jpg" width=400/>

Pour utiliser l'écran, nous avons besoin de réaliser plusieurs opérations sur ce dernier au préalable qui sont expliqués dans le lien suivant : 
https://mansfield-devine.com/speculatrix/2019/01/ttgo-esp32-oled-display/

## Limites de l'implémentation

- Problème : 

&emsp;Cette implémentation montre tout de même quelques défauts. Tout d'abord, il est nécéssaire de rentrer les identifiants et mots de passe du WiFi et du broker "*en dur*" à chaque changement de réseau WiFi ou de broker. En cas de partage du code, ces données apparaissent en clair et ne sont pas criptées, ce qui peux provoquer un risque d'un point de vue sécurité.

- Idée de résolution :

&emsp;Résolution temporaire, nous demandons aux utilisateurs de créer le fichier contenant leurs identifiants, cela n'est viable que dans une phase de test.
&emsp;Résolution plus performante : Il faudrait chiffré les identifiants en utilisant une clé de chiffrement propre aux utilisateurs, mais cela demande d'implémenter un algorithme suplémentaire pour déchiffrer ces données, propre à chaque personne, ce qui ne rend pas le code plus facilement distribuable.
Nous pourrions également faire une demande d'argument à l'exécution, ce qui serait plus simple mais non persistant.

- Problème :

&emsp;Nous faisons face à des déconnections intempestives ainsi que des retards dans les envoies / réceptions de messages en lien avec le broker.

- Idée de résolution :

&emsp;Celà est peut-être lié à un problème de performance matériel, auquel cas le seul changement possible est de changer de modèle d'ESP32. Sinon, le problème est lié à une instabilité du réseau (ce qui est improbable dans les conditions de test).

## Résumé

&emsp;L'implémentation sur l'esp32 est terminée.
Nous avons connecté l'esp32 au WiFi et au broker MQTT ainsi que deux topic, un pour les messages entrants (`esp32/input`), un pour les messages sortants(`esp32/ouput`).
&emsp;Le topic pour les messages sortants concerne les valeurs de la photorésistance, le topic concernant les messages entrants concerne des commandes traitées par l'esp32.
Ces commandes sont  { "LedOn" : allume la Led, "LedOff" : éteint la Led, autre : affiche la commande sur l'écran.

# Website

&emsp;Nous attaquons la partie le plus conséquente de ce micro-projet, et celle qui nous a demandé le plus de temps, et le plus de lecture de documentation. Nous n'expliciterons pas toutes les étapes de notre cheminemant afin de créer le serveur Web car la rapport serait bien trop long (ce qui est déja le cas). Nous indiquerons donc dans la mesure du possible la documentation / les tutoriels utilisé afin de reproduire les étapes, et nous prendrons au maximum appuie sur le code.

## Objectifs

&emsp;Nous voulons faire un serveur HTTP récupérant les données émises par l'esp32 via le broker MQTT. Nous voulons également pouvoir envoyé des commandes via un formulaire, sur l'ESP32, toujours au travers du broker.

Pour ce faire nous avons décidé d'utiliser le [framework Django](https://www.djangoproject.com/)

## Implémentation du serveur Django

"*Django is a high-level Python Web framework that encourages rapid development and clean, pragmatic design. Built by experienced developers, it takes care of much of the hassle of Web development, so you can focus on writing your app without needing to reinvent the wheel. It’s free and open source.*" - djangoproject.com

### Pourquoi Django ?

Nous avons choisis d'utiliser Django pour plusieurs raisons :

- C'est un framework reconnu mondialement : 

Apprendre à utiliser un serveur Django est beaucoup demandé, et nous sera toujours utile d'un point de vue professionnel, et personnel si nous voulons complexifier notre serveur.

- Disposant de hautes performances : 

Nous faisions face à des soucis de performance et de forte utilisation des ressources matérielles lors de la mise en place d'un serveur HTTP *basics*. Ces problèmes n'ont pas été apperçu avec le serveur Django.

- Et d'une facilité d'impléméntation déconcertante : 

Au début de ce projet, nous pensions que le site web serait un *support* et que le partie importante du code était sur l'esp32, nous nous étions lourdement trompé. Le framework étant implémenter en python, déstiné au *grand public*, et disposant d'une documentation riche (ainsi que d'une grande variété de ressources tierces en ligne), l'implémentation du serveur simplifiée au possible et extrémement versatile en fonction de nos besoins.

"*Django : The web framework for perfectionists with deadlines.*"

### Création du serveur Django

La création du serveur Django se base grandement sur cette playlist youtube par [HackerShack](http://www.thehackershack.com/) : https://www.youtube.com/embed/videoseries?list=PL39pssg07dpDJas1vxb7Dyw5f8SkAw6c-

- Mise en place d'un environnement virtuel python :

*Pourquoi* ? Afin de faciliter les dépendances, les librairies, leurs versions, et la version de python.
Python 2 et python 3 n'étant pas retro-compatible, il est parfois nécessaire de forcer un environnement pour éviter tout problème de compatibilité.

Pour créer un environnement virtuel, nous utiliserons [*pyenv*](https://github.com/pyenv/pyenv) ([lien pour l'installation sous Ubuntu](https://www.liquidweb.com/kb/how-to-install-pyenv-on-ubuntu-18-04/)) suivi de [*pyenv virtualenv*](https://github.com/pyenv/pyenv-virtualenv) ([lien pour l'installation sous Ubuntu](https://www.liquidweb.com/kb/how-to-install-pyenv-virtualenv-on-ubuntu-18-04/)).

Nous créons un environnement que nous appelerons `DjangoWebsite@3.7.3` sous python 3.7.3.

Nous ajouterons également un dossier `/requierements/` contenant deux fichiers : `dev.txt` et `prod.txt` afin de gérer les dépendances du serveur à l'aide de la commande `pip install -r dev.txt`.

- Mise en place du serveur Django

Nous installons ensuite la librairie Django avec `pip install Django`. Sans oublier d'ajouter la nouvelle dépendance à `prod.txt`.

&emsp;Nous créons ensuite un projet Django avec la commande `django-admin startproject pioc_website` ce sera notre site web. Au sein de ce site, nous créons une application que sera propre à ce projet, nous l'avons appeler *myapp* (le nom aurai pu être mieux choisis) `./manage.py startapp myapp`. 
&emsp;Nous migrons ensuite l'application que nous venons de créer : `./manage.py migrate`. Les migrations sont la manière par laquelle Django propage des modifications que nous apportons à des modèles (ajout d’un champ, suppression d’un modèle, etc.) dans un schéma de base de données.[[*](https://docs.djangoproject.com/fr/3.2/topics/migrations/)]

Nous obtenons l'arborescence suivante :
```
.
├── manage.py
├── myapp
├── pioc_website
└── requirements
    ├── dev.txt
    └── prod.txt
```
D'autre étapes sont nécessaires pour ajouter nos applications nouvellement crées au serveur Django, elles sont très bien expliquée sur la [vidéo suivante](https://www.youtube.com/watch?v=PqeAvFf_HDI).

- Arborescence détaillée du serveur Django :
```
.
├── db.sqlite3  ~ la base de donnée expliqué plus tard dans le rapport
├── manage.py   ~ gestion du serveur, ex : "./manage.py runserver" pour démarrer le serveur
├── myapp       ~ application énoncée ci-dessus, les opérations sont réalisé ici
│   ├── admin.py  ~ gestion administrateur de l'app, auto générér non modifié
│   ├── apps.py   ~ paramètre de l'app, auto générér non modifié
│   ├── __init__.py  ~ programme python exécuter au démarrage du serveur, 
|   |                               explicité plus tard dans le rapport
│   ├── migrations   ~ dossier auto générer gérant les migrations
│   │   └── [...]  
│   ├── models.py    ~ création des tables pour la base de donnée
│   ├── mqtt_input.py  ~ programme appelé dans __init__ 
│   ├── __pycache__  ~ cache
│   │   └── [...]
│   ├── templates
│   │   └── index.html
│   ├── tests.py  ~ Programme pour test, non utilisé
│   ├── urls.py   ~ Gestion des routes du serveur (en partant de la route de myapp)
│   └── views.py  ~ Gestion des rendus du serveur associé aux routes
├── pioc_website   ~ Le serveur Django ~
│   ├── asgi.py  
│   ├── __init__.py  ~ même chose que myapp, non exploit
│   ├── __pycache__  ~ cache
│   │   └── [...] 
│   ├── settings.py   ~ paramètre du serveur, nous indiquerons les app et les paths utile au serveur
│   ├── urls.py   ~ route du serveur, nous définirons myapp comme la route principale
│   └── wsgi.py
├── requirements   ~ dépendances du serveur
│   ├── dev.txt
│   └── prod.txt
└── static     ~ Dossier contenant les élements statics pour les rendus (images, CSS, javascripts)
    └── [...]
```

Pour lancer le serveur, nous utiliserons : `./manage.py runserver` 

Tout d'abord, nous ajoutons une route vers *myapp*, ce sera la route par défaut :

Dans le fichier [pioc_website/urls.py](/website/pioc_website/urls.py), nous ajouterons `path("", include("myapp.urls"))` dans la liste des *urlpatterns*.

Dans myapp, nous ajoutons la *view* index à la route par défaut.
Dans le fichier [myapp/urls.py](/website/myapp/urls.py), nous ajouterons `path("", views.index, name="index")` dans la liste des *urlpatterns*.

Dans le fichier [myapp/views.py](website/myapp/views.py), nous ajoutons une méthode afin de générer le rendu, nous appelerons un fichier `index.html` que nous ajouterons dans le dossier `myapp/template/`.
Le contenu de ce fichier sera explicité plus tard.

Si nous lançons le serveur, à l'adresse `localhost:8000`, le contenu de `index.html` devrait s'afficher.

La base de notre serveur est en place.

## Implémentation d'une base de donnée

&emsp;Notre prochaine étape consiste à mettre en place une base de donnée dans le serveur Django.
Cette base de donnée permettra de garder en mémoire les données reçu du broker MQTT, et ce même après avoir éteint le serveur.
&emsp;De plus lors de la réception d'un message du broker MQTT, la donnée sera directement ajoutée dans la base de donnée. Et ensuite des requêtes vers cette base de donnée seront effectuée pour récupérer des valeurs et les affichées sur le site, mais nous verrons cela plus tard.

Notre base de donnée est formée d'une unique table contenant les attributs suivant : 
- Un identifiant unique et implicite s'incrémentant à chaque nouvelle entité.
- Une chaine de caractère *id_esp* symbolisant l'identifiant de l'esp d'où provient la valeurs
    (ne travaillant avec un unique esp, nous l'avons identifié par esp-test, mais nous pourrons récupérer l'identifiant via le broker à terme)
- Une date de publication *pub_date* si nous voulons réaliser une requête sur une interval de date, mais ce type de requête n'est pas encore implémenter
- Un entier symbolisant la valeurs de la photorésistance

Cette table a été défini dans le fichier [*/myapp/models.py*](website/myapp/models.py) de la manière suivante :

```py
class Data(models.Model):
    # Identifiant générer implicitement
    # identifiant propre à l'esp (en cas de connection de plusieurs esp)
    id_esp = models.CharField(max_length=200)
    # date de la donnée
    pub_date = models.DateTimeField(auto_now_add=True)
    # valeurs de la donnée (0,4000)
    value = models.IntegerField()
```

&emsp;Cette table s'appelle `Data` et étend `django.db.Model`. Les identifiants sont générer implicitement de manière unique, et les dates sont indiquées lors de la création d'une nouvelle entité dans la table.

Nous avons besoin de migré la table après création comme indiqué dans la [vidéo suivante](https://www.youtube.com/watch?v=PqeAvFf_HDI) précédement utilisée.

Les nouvelles entités sont crées à la réception d'un message du broker MQTT, et sont utilisées lors de l'affichage de la page HTML, ces deux aspects seront explicités ultérieurement.

- Avantages :
  - pérsistance des données
  - accès faciliter au travers de requetes ([queries](https://docs.djangoproject.com/fr/3.2/topics/db/queries/))
  - Opérations supplémentaires plus facilement implémentable (Par exemple nous voulons la moyenne des valeurs de la photorésistance entre 9h et 19h en semaine, nous pourrons le faire avec une requête SQL les données étant deja en place)
  - administration de ces données facilitée grâce au framework Django : accès via `localhost:8000/admin`

<img src="img/DjangoAdmin.png" alt="DjangoAdmin" width="600"/>

*Présentation de la page d'administration du serveur Django, Datas étant notre table.*

- Inconvénients :
  - La table ne se vide pas automatiquement, mais cela peut etre implémenter si nous avons des données plus conséquentes. Cela peut etre problématique si le serveur est lancé en permanence, il créé une nouvelle entité toutes les seconds.

Notre base de donnée étant opérationnelle, comme la remplissons nous ?

## connection entre la base de donnée et l'esp32 via le broker MQTT

&emsp;Nous avons maintenant le coeur de notre site opérationnel, il nous faut maintenant récupérer les données de notre broker MQTT, et les ajouter dans la base de donnée. Pour celà nous utiliserons la librarie python [*paho-mqtt*](https://pypi.org/project/paho-mqtt/).
Nous devons donc d'abord installer la libraire :
`pip install paho-mqtt`

Puis l'ajouter dans la liste des *requierements*.

### esp32/output

Nous voulons lancer la connection au démarrage du serveur, la reception des messages se faisant dans la *back end*. C'est à ce moment que [/myapp/\_\_init__.py](website/myapp/__init__.py) fait sont entrée.

Les méthodes appelés dans *\_\_init__* sont lancée au démarrage du serveur, et ce quoi qu'il arrive.

Nous avons d'abord créé un fichier [myapp/output.py](website/myapp/output.py) pour gérer les données entrante sur le serveur (c'est à dire sortante de l'esp32 sur le topic *esp32/output*).

ATTENTION : Le fichier \_\_init__.py s'éxecute très "tôt" au lancement du serveur, et ce même avant que toutes les applications soient chargée. Cela provoquera un problème lors que nous voulons importer le modèle de notre table (*models.py*) dans \_\_init.__py. Le models n'étant pas initialisé, nous obtenons une erreur `Apps aren't loaded yet`/

```py
# after conenecting to the MQTT broker
def on_connect(client, userdata, flags, rc):
    from .models import Data  # fix Apps aren't loaded yet

    client.subscribe(topic="esp32/output")

# upon message receipt
def on_message(client, userdata, msg):
    # fix Apps aren't loaded yet
    from .models import Data 
    # decode the message in order to use it (byte array -> String(utf-8))
    temp = msg.payload.decode("utf-8")
    # create a new entity in the table Data
    Data.objects.create(id_esp="esp-test", value=float(msg.payload.decode("utf-8")))

# create a new MQTT client
client = mqtt.Client()
# execute on_connect when connected
client.on_connect = on_connect
# execute on_message when receiving a message
client.on_message = on_message

# inititate connection
client.connect("192.168.1.46", 1883)

```

Nous faisons l'import de la Table *Data* dans la méthode car au lancement du serveur, les apps ne sont pas chargée de suite, il en est de même pour la table et de ce fait, l'import n'était pas reconnu. De ce fait l'import se fera lors de la connection, qui se fera forcément après que le serveur soit totalement opérationnel et que toutes les ressources soient chargées.

&emsp;Lors de la connection au broker, nous nous abonnons au topic `esp32/output` pour recevoir les données de l'esp32.
&emsp;Lors de la réception d'un message, nous créons une nouvelle entité dans notre table avec la valeurs récupérée du message. L'identifiant de l'esp32 est entré "*en dur*" pour le moment, mais nous pouvons utilisé le champs *userdata* pour avoir un identifiant plus cohérent.

L'adresse du broker est également entrée "*en dur*", il serait fortement envisageable de la paramétrée au lancement du serveur Django via des arguments, ou une lecture de fichier.

Dans le fichier [/myapp/\_\_init__.py](website/myapp/__init__.py), nous appelons `mqtt_output.client.loop_start()` afin de lancer la boucle d'attente des messages.

Nous noterons la grande similarité des méthodes entre le code sur l'esp32 et celui ci : `on_message` <-> `callback`; `client.loop_start()` <-> `client.loop()` etc ...

Notre base de donnée se rempli au fur et à mesure de la réception des messages.

- Limites de notre implémentation :

Si un message arrive sur le topic `esp32/output` contenant autre chose qu'un entier, une erreur de conversion sera lancée.

Attention, à partir de ce moment nous avons besoin d'ajouter le flag `--noreload` lors du lancement du serveur. Autrement le serveur essayais de s'abonner au topic à nouveau, et nous recevions tous les messages deux fois.

`./manage.py runserver -noreload`

### esp32/input

&emsp;Nous voulons à présent envoyer des données depuis le site web vers l'esp32, pour cela nous utiliserons le topic `esp32/input`.

Nous avons essayer d'implémenter cette fonctionnalité en Javascript, initialement avec un switch "On" "Off" allumant et éteignant la Led.

Pour ce faire nous utilisions la librairie paho-mqtt, mais cette fois-ci en javascript : 

```js
jQuery(document).ready(function($) {
  var client;
  // broker ip adress
  var host = "162.168.1.46";
  // broker port
  var port = 1883;

  // after conenecting to the MQTT broker
  function onConnect(){
      console.log("Connected");
      // set topic
      message.destinationName = "esp32/input";
      // first turn off the led
      client.send("LedOff");
  }
  // inititate connection
  function MQTTconnect(){
      console.log("connecting to "+host + " "+ port);
      // new connection
      client = new Paho.MQTT.Client(host,port,"clientJS");
      var option = {
          timeout: 3,
          onSuccess: onConnect,
      };
      // connect
      client.connect(option);
  }

  MQTTconnect();

  // react to the button, has been changed to a form
  $('#switchLed').click(function() {
      if($('#switchLed').is(':checked')){
          // if switch is on, turn on the led 
          document.getElementById('actualLed_val').innerHTML = 'On';
          client.send("LedOn");
      }else{
          // if switch is off, turn off the led
          document.getElementById('actualLed_val').innerHTML = 'Off';
          client.send("LedOff");
      }
  });
});
```
&emsp;Nous remarquons qu'une fois de plus les méthodes sont similaire aux précédentes implémentations.
Malheuresement cette implémentation s'est révélée infructueuse:
En effet, la connection établie pour l'envoie de message était "*anonyme*", cette dernière était refusée par le broker MQTT. Et même en changeant la configuration du broker Mosquitto (via `mosquitto.conf`), en spécifiant que nous acceptions les connections anonymes, les connections au broker MQTT étaient tout de même refusée et nous n'arrivions pas à résoudre le problème.

&emsp;Nous avons donc décider d'implémenter cette fonctionnalité en envoyant une requête "POST" depuis un [formulaire](https://docs.djangoproject.com/fr/2.2/topics/forms/) sur la page HTML. Cette requête étant ensuite récupérée et traitée dans [/myappp/views.py](website/myapp/views.py).
```html
<!-- Formulaire pour esp32/input -->
<form method="post">
    {%csrf_token%}
    <label for="input">Veuillez entrer une commande : </label>
    <input class="input_msg" id="esp32_input" type="text" name="esp32_input" placeholder="exemple : LedOn">
    <input class="submit" type="submit" value="OK">
</form>
```

![](img/CommandeExample.png)

Cette commande affichera ce super message sur notre écran :

<img src="img/OledExample.jpg" width=400/>

Lorsque l'utilisateur entre un message (une commande) dans le formulaire, une requete "*POST*" est envoyée, cette dernières est récupérée dans [/myappp/views.py](website/myapp/views.py) :

```py
def index(request):
  # si une donnée est entrée dans le formulaire  
  if request.method == 'POST' and 'esp32_input' in request.POST:
      # etablie une connection temporaire et publie le contenu de la requete dessus
      publish.single("esp32/input", request.POST.get("esp32_input"), hostname="192.168.1.46")
  # charge le template
  template = loader.get_template("index.html")
  # récupère la derniere valeurs de la base de donnée
  data = Data.objects.order_by("-pub_date").all()[0]
  # affiche le rendu
  return render(request, "index.html", {f"value": data.value})
```

&emsp;Vu que nous n'avons pas besoin de maintenir une connection constante avec le broker,  les envoies de commandes étant isolés, nous utilisons `publish.single()`. Cela permettra d'établir une connection d'envoyer le message et de se déconnecter juste après.

- Inconvénients :
  - Lors d'un envoie de commande, un nouveau rendu de la page est réalisé ce qui provoque une *actualisation*.
  - Si nous avons plusieurs esp32 sur la meme route, nous aurons besoin d'indiqué l'identifiant de l'esp concerné par la commande, ou d'utilisé plusieurs routes.

- Idée de fonctionnalité :
  - Ajouter une table supplémentaire contenant l'historique des commandes.
  - Ajouter un champs permettant de choisir vers quel ESP32 nous voulons envoyé la commande

## Frontend

Nous attaquons maintenant la partie visible de l'iceberg qu'est notre projet. Le *front end*.

Nous n'utiliserons qu'une route, et un unique fichier HTML. Toutes les autres routes seront des *utilitaires*.

### HTML et CSS

Fichier : [*index.html*](website/myapp/templates/index.html)
Ce fichier contiendra l'ensemble des données affichée sur notre page Web tel que vous pouvez la voir.

Pour cela nous avons utilisé principalement deux langages qui sont évidemment nul autres que HTML, CSS ainsi que JavaScript

La découverte de la partie front-end à été aussi intérressante qu'enrichissante.

<img align="right" width="300" height="340" src="/img/front_design.png" alt="front_design">

Nous avons découper le code en plusieurs section "div" selon les différentes partie du site.

Les différentes parties concernent :
- Le header (*haut de page*) avec le titre et les logos
- Des sections :
  - Le sommaire 
  - La valeur actuelle de la photoresistance
  - Un formulaire d'envoie de message (*commande*)
  - Un graphique dynamique
- Le Footer (*bas de page*)

&emsp;L'utilisation d'ID à été utile afin de retrouver certaines valeurs lors de l'implémentation du javascript dans le but d'afficher la valeur de la photoresistance par exemple :
```js
var element = document.getElementById("current_value")
```
Ici nous stockons dans la variable `element` la valeur pointer par l'id `current_value` ce qui nous permettra par la suite de la modifier dynamiquement dans la partie Graphique.

<img src="img/partie1.png" width=400/>
<br/>
<img src="img/LiveDataIOC.gif" width=400/>
<br/>
<img src="img/partie2.png" width=400/>

*Représentaton de notre page web*.

### Recupération des valeur

Nous affichons différentes valeurs dans la page HTML, ces valeurs proviennent de la base de donnée et sont soumises à des changements, nous devons donc nous assurer qu'elles restent autant que possible à jours. Nous avons utilisé deux méthodes afin de récupérer les valeurs de la base de donnée :

#### Passage d'une valeurs en paramètre du rendu

Tout d'abord nous affichons les dernières valeurs de la photorésistance sur le site : 

![](img/photovalue.png)

Cette valeurs la dernière valeurs en date de la base de donnée. Au lancement du site elle est récupérée et passé en paramètre lors du rendu. 

Fichier [myapp/views.py](website/myapp/views.py) :
```py
def index(request):
  # si requete POST provenant du formulaire
  if request.method == 'POST' and 'esp32_input' in request.POST:
    # publication de la commande
    publish.single("esp32/input", request.POST.get("esp32_input"), hostname="192.168.1.46")
  template = loader.get_template("index.html")
  # nous récupérons la dernière valeurs de la base de donnée.
  data = Data.objects.order_by("-pub_date").all()[0]
  return render(request, "index.html", {f"value": data.value})
```
Cette valeurs est ensuite récupérée et affichée : 
```html
<div class="actualVal_content">
    <p class="actualVal_txt">Voici la valeur actuelle de notre photoresistance : </p>
    <!-- Valeurs passée en paramètre du rendu -->
    <p id="current_value" class="actualVal_val">{{value}}</p>
</div>
```

#### Passage d'une valeurs via une route dédiée

Nous voulons maintenant récupérér plusieurs valeurs, et ce sans avoir besoin de générer un nouveau rendu à chaque fois.
&emsp;Pour faire celà, nous créons une nouvelle route que nous appelerons "/data", contenant une *JacksonResponse*.
Soit une réponse au format JSON des 50 dernières valeurs de notre base de donnée.
Fichier [*myapp/views.py](website/myapp/views.py) :

```py
def get_data(request):
  data = list(Data.objects.order_by("-pub_date").values()[:50]) 
  return JsonResponse(data, safe=False)
```
![](img/JsonResponse.png)

### Live data

Nous utilisons ces valeurs afin d'afficher une charte évolutive à l'aide de [CanvasJs](CanvasJS.com).
```html
<script type="text/javascript">
  var element = document.getElementById("current_value")
  window.onload = function () {
    // Notre data points
    dps = [];
    // Nous récupérons les données au format JSON depuis la route "/data"
    $.getJSON('/data', function(data) {
        // Valeurs initiales affichée sur le graphe
        for (let index = 10; index >= 0; index--) {
            dps.push({x: 10-index, y: data[index].value});
        }
    }); 
    // Format du graphe
    var chart = new CanvasJS.Chart("chartContainer",{
        theme: "light1",
        title :{  text: "Live Data"  },
        axisX: {  title: "" },
        axisY: {	title: "" },
        data: [{
            type: "line",
            dataPoints : dps,
            color :"purple"
        }]
    });
    // Afficher le graphge
    chart.render();
    var xVal = 10 + 1;
    var yVal = 15;	
    var updateInterval = 1000;
    // Mise à jour du graphe toutes les secondes 
    // (en lien avec la fréquence d'envoie de message sur l'ESP32)
    var updateChart = function () {
      // Nous récupérons les nouvelles données
      $.getJSON('/data', function(data) {
        // ajout de cette donnée dans notre liste de point
        dps.push({x: xVal, y: data[0].value});
        // Mise à jour de la valeurs de la photorésistance 
        element.innerHTML = data[0].value;
      }); 
      xVal++;
      // Décalage du graphe et réaffichage avec la nouvelle valeurs
      if (dps.length >  10 ) {
          dps.shift();				
      }
      chart.render();		
    };
    setInterval(function(){updateChart()}, updateInterval); 
  }
</script>
<script type="text/javascript" src="https://canvasjs.com/assets/script/canvasjs.min.js"></script>
```

![LiveData](img/LiveDataIOC.gif)

Nous avons également un joli bouton qui ne fait rien :)

# Conclusion

![](/img/ourMQTT.png)

Au cours de ce projet, nous avons mis en place un serveur MQTT gérant deux topics : `esp32/output` et `esp32/input`.

- esp32/output : Les valeurs renvoyées par la photorésistance, récupérée par le serveur. Elles sont ajoutée à une base de donnée, puis sont afficher sur une page web, notamment au travers d'un graphique.

- esp32/input : Les valeurs sont générée par un formulaire sur le serveur Web, puis sont envoyées sur l'ESP32. La donnée est ensuite traitée, si une commande est reconnu, elle est traitée, sinon le message est affiché sur l'écran OLED.

Au cours de ce projet, nous avons pu utiliser une myriade d'**outils**, de **langages**, et d'**API**.
Nous avons découvert le fonctionnement et l'utilité d'un **Broker MQTT** au travers de **Mosquitto**.
Pour l'**esp32**, nous avons consolidé nos acquis des TME précédents.
Pour le serveur **Django**, nous avons pu découvrir un Framework extrement **complet**, **polyvalent** et **simple** d'utilisation. Ce framework est beaucoup utilisé dans le monde professionel, et nous y être intéréssé ne peut que nous être bénéfique.
Nous avons fortement consolidé nos bases en **python**, et découvert / perfectionné nos langages **HTML**, **CSS**.
Nous avons également effleuré le champs des possibles de **javascript**.

Nous avons également passé en revu une quantité de documentations et de forums inconcevable ce qui nous a permis de nous rendre compte de la richesse et de la complexité de ces outils.

Et ces découvertes vont jusqu'au rapport, avec la découverte de **Mermaid** un outils de génération de graphe et diagramme intégrable à Markdown.

Nous avons eu la chance de travailler sur un projet **utile** pour **nous**, que nous pourrons reprendre et étendre à l'avenir, nous avons **appris** énormément, et nous sommes presque triste de devoir rendre ce projet sans avoir pû ajouter toutes les fonctionnalités que nous aurions voulu. Mais toutes bonnes choses ont une fin.
 
Nous arrivons à la fin de ce rapport ..

Voici une liste non exaustives d'idées que nous avons eu, de fonctionnalité que nous pourrions implémenter ultérieuerement, que nous n'avons pas fait par manque de temps, de connaissances ou d'idée d'utilisation.

# Pour aller plus loin 

- Un menu pour ajouter plusieurs ESP32 :

Un menu dans notre site web pour gérer plusieurs esp32 identifié par "id_esp" dans la base de donnée, avec des graphiques / courbes propres à chaque esp32, avec une évolution dynamique du site en fonction de nouvel arrivage d'esp32.

- Complexification des requêtes sur la base de donnée :

Ajouter plus de fonctionnalités concernant la base de donnée, par exemple une requete récupérérant les données au cours des xx dernières heures. Possibilité d'intégration d'un calendrier, et d'afficher la moyenne par jours.

- Perfectionnenement du graphique :

Modifier la charte javascript pour qu'elle reste inchangée si aucune nouvelle valeurs sont arrivée (déconnection de l'esp), et si plusieurs valeurs arrivent en même temps (retard de message), que ces valeurs soient prisent en compte (actuellement seule la dernière valeurs de la base de donnée est affichée toutes les secondes). 
Ajout éventuelle d'une échelle de temps dans l'axe des abscisses respectant les spécifications énoncée ci-dessus.

- Activation du transfert de donnée :

Utiliser un bouton pressoir pour activer / désactiver le transfert de donnée vers le broker MQTT. Si nous voulons arréter le transfert, nous devons débrancher l'esp actuellement.

- Gestion d'autres capteurs :

Pouvoir gérer dynamiquement la base de donnée en fonction des capteurs présent sur l'esp32.
Exemple : Si nous ajoutons un capteur d'humidité, cette valeurs sera ajoutée dans la table avec un nouvel attribut lors de la détection de ce nouveau capteur. Et ajout de différentes routes au serveur Web pour gérer ces capteurs.

- Implémentation du Bluetooth (BLE) :

Rendre l'esp32 plus portatif en ajoutant une connection via Bluetooth pour ne pas avoir besoin de se connecter au WiFi. L'esp32 pourrait fonctionner sur batterie et transmettre les données sur d'autre support (le serveur Web dépendant également d'internet)

# Remerciement

Nous sommes vraiment à la fin de ce rapport cette fois-ci.

Nous tenions particulièrement à remercier Monsieur Franck Wajsburt pour cette UE.
Malgré les conditions particulière vous nous avez proposer des cours, des TP et un projet passionnant. Le nombre de choses retenu est juste incroyable et ont été vraiment enrichissante. Nous avons pris un réel plaisir à faire ce projet, et nous espérons que nous avons réussi à vous le montrer au travers de ce projet, et que ce dernier n'a pas été trop long à lire.
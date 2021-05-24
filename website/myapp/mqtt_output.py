import sys
import paho.mqtt.client as mqtt

# après connection au broker MQTT
def on_connect(client, userdata, flags, rc):
    from .models import Data  # fix Apps aren't loaded yet

    client.subscribe(topic="esp32/output")


# à la réception d'un message
def on_message(client, userdata, msg):
    # fix Apps aren't loaded yet
    from .models import Data

    # decode the message in order to use it (byte array -> String(utf-8))
    temp = msg.payload.decode("utf-8")
    # create a new entity in the table Data
    Data.objects.create(id_esp="esp-test", value=(4000-float(msg.payload.decode("utf-8"))))
    print("topic : %s" % msg.topic, file=sys.stderr)


# create a new MQTT client
client = mqtt.Client()
# execute on_connect when connected
client.on_connect = on_connect
# execute on_message when receiving a message
client.on_message = on_message

# inititae connection
client.connect("192.168.1.46", 1883)
print("connected to mqtt server", file=sys.stderr)

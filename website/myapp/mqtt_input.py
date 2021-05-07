import sys
import paho.mqtt.client as mqtt


def on_connect(client, userdata, flags, rc):
    from .models import Data  # fix Apps aren't loaded yet

    client.subscribe(topic="esp32/output")


def on_message(client, userdata, msg):
    # fix Apps aren't loaded yet'
    from .models import Data 
    temp = msg.payload.decode("utf-8")
    Data.objects.create(id_esp="esp-test", value=float(msg.payload.decode("utf-8")))
    print("topic : %s" % msg.topic, file=sys.stderr)


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("192.168.1.46", 1883)
print("connected to mqtt server", file=sys.stderr)

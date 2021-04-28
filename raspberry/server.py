#!/usr/bin/env python3

import paho.mqtt.client as mqtt

mqtt_server = '192.168.1.46'
mqtt_port = 1883
mqtt_topic = 'esp32/output'
mqtt_client_id = 'python_server'

 
def on_connect(client, userdata, flags, rc):
    print('connected (%s)' % client._client_id)
    client.subscribe(topic=mqtt_topic, qos=0)


def on_message(client, userdata, message):
    print('topic : %s' % message.topic)
    print('payload : %s' % message.payload)


def main():
    client = mqtt.Client(client_id=mqtt_client_id, clean_session=False)
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(host=mqtt_server, port=mqtt_port)
    client.loop_forever()


if __name__ == '__main__':
    main()

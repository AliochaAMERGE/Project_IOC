import paho.mqtt.client


def on_connect(client, userdata, flags, rc):
    print('connected (%s)' % client._client_id)
    client.subscribe(topic='esp32/output', qos=2)


def on_message(client, userdata, message):
    print('topic : %s' % message.topic)
    print('payload : %s' % message.payload)


def main():
    client = paho.mqtt.client.Client(
        client_id='tems_subs', clean_session=False)
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(host='192.168.0.15', port=1883)
    client.loop_forever()


if __name__ == '__main__':
    main()

## Installation de mosquitto

Source : 
https://appcodelabs.com/introduction-to-iot-build-an-mqtt-server-using-raspberry-pi

- 1. Install the mosquitto MQTT Broker

`sudo apt install mosquitto mosquitto-clients`

- 2. Enable the mosquitto broker
    
Enable the broker and allow it to auto-start after reboot using the following command:
`sudo systemctl enable mosquitto`

- 3. Subscribe to the MQTT Topic Locally

In the existing terminal, subscribe to the test/message topic:

terminal 1:
`mosquitto_sub -h localhost -t "test/message"`

terminal 2:
`mosquitto_pub -h localhost -t "test/message" -m "Hello, world"`

Hello, world apparait sur le Terminal 1



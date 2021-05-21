jQuery(document).ready(function($) {
    // var client;
    // var reconnectTimeout = 2000;
    // var host = "192.168.1.46";
    // var port = 1883;

    // function onConnect(){
    //     console.log("Connected");
    //     message.destinationName = "esp32/input";
    //     client.send("off");
    // }

    // function MQTTconnect(){
    //     console.log("connecting to "+host + " "+ port);
    //     client = new Paho.MQTT.Client(host,port,"clientJS");
    //     var option = {
    //         timeout: 3,
    //         onSuccess: onConnect,
    //     };
    //     client.connect(options);
    // }

    // MQTTconnect();

    $('#switchLed').click(function() {
        if($('#switchLed').is(':checked')){
            document.getElementById('actualLed_val').innerHTML = 'On';
            // client.send("on");
            // appeler code python switch("on");
        }else{
            document.getElementById('actualLed_val').innerHTML = 'Off';
            // client.send("off");
            // appeler code python switch("off");
        }
    });
 });
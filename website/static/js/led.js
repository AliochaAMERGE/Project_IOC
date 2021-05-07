jQuery(document).ready(function($) {
    $('#switchLed').click(function() {
        if($('#switchLed').is(':checked')){
            document.getElementById('actualLed_val').innerHTML = 'On';
        }else{
            document.getElementById('actualLed_val').innerHTML = 'Off';
        }
    });
 });
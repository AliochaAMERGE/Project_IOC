from django.shortcuts import render
from django.http import HttpResponse
from django.template import loader
from django.http import JsonResponse
from django.template import RequestContext
import paho.mqtt.publish as publish


from .models import Data


def index(request):
    # si requete POST provenant du formulaire
    if request.method == 'POST' and 'esp32_input' in request.POST:
        # publication de la commande
        publish.single("esp32/input", request.POST.get("esp32_input"), hostname="192.168.0.15")
    template = loader.get_template("index.html")
    # nous récupérons la dernière valeurs de la base de donnée.
    data = Data.objects.order_by("-pub_date").all()[0]
    return render(request, "index.html", {f"value": data.value})


def get_data(request):
    data = list(Data.objects.order_by("-pub_date").values()[:50]) 
    return JsonResponse(data, safe=False)

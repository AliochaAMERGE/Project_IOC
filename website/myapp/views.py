from django.shortcuts import render
from django.http import HttpResponse
from django.template import loader
from django.http import JsonResponse


from .models import Data


def index(request):
    template = loader.get_template("index.html")
    data = Data.objects.order_by("-pub_date").all()[0]
    return render(request, "index.html", {f"value": data.value})


def get_data(request):
    data = list(Data.objects.order_by("-pub_date").values()[:50]) # change with date
    print(data)
    return JsonResponse(data, safe=False)

def auto_update(request):
    data = Data.objects.order_by("-pub_date")
    
    date = []
    value = []
    
    for dataset in data:
        date.append(dataset.pub_date)
        value.append(dataset.value)
        
    date = date[:25]
    value = value[:25]
    data.reverse()
    value.reverse()
    
    context = {
        'date' : date,
        'value' : value,
    }
    
    return JsonResponse(context)

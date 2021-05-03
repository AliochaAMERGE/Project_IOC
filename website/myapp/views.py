from django.shortcuts import render
from django.http import HttpResponse
from django.template import loader

from .models import Data


def index(request):
    template = loader.get_template('index.html')
    data = Data.objects.order_by('-pub_date').all()[0]
    return render(request, 'index.html', {f"value": data.value})
    # return HttpResponse(f"value: {data.value}")

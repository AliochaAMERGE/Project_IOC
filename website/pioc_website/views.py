from django.shortcuts import render


def index(request : HttpRequest) -> HttpResponse:
    return render(request, "index.html")


def about(request: HttpRequest) -> HttpResponse:
    return render(request, "about.html")


def contact(request: HttpRequest) -> HttpResponse:
    return render(request, "contact.html")

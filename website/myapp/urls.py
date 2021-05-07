from django.urls import path

from . import views

urlpatterns = [
    path("", views.index, name="index"),
    path("data", views.get_data, name="data"),
    path("data_updated", views.auto_update, name="data_updated"),
]

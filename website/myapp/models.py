from django.db import models

class Data(models.Model):
    # identifiant propre à l'esp (en cas de connection de plusieurs esp)
    id_esp = models.CharField(max_length=200)
    # date de la donnée
    pub_date = models.DateTimeField(auto_now_add=True)
    # valeurs de la donnée (0,4000)
    value = models.IntegerField()
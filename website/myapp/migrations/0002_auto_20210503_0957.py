# Generated by Django 3.0.7 on 2021-05-03 09:57

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('myapp', '0001_initial'),
    ]

    operations = [
        migrations.AlterField(
            model_name='data',
            name='pub_date',
            field=models.DateTimeField(auto_now_add=True),
        ),
    ]

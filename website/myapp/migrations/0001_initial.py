# Generated by Django 3.0.7 on 2021-05-03 09:49

from django.db import migrations, models


class Migration(migrations.Migration):

    initial = True

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='Data',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('id_esp', models.CharField(max_length=200)),
                ('pub_date', models.DateField(verbose_name='date published')),
                ('value', models.IntegerField()),
            ],
        ),
    ]

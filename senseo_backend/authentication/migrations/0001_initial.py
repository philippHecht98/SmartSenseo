# Generated by Django 4.1.4 on 2022-12-09 12:44

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    initial = True

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='RFID',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('rfid', models.BinaryField()),
            ],
        ),
        migrations.CreateModel(
            name='Tokens',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('user_name', models.CharField(max_length=20)),
                ('cup_size', models.CharField(choices=[('S', 'SIMPLE'), ('D', 'DOUBLE')], max_length=1)),
                ('rfid', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='authentication.rfid')),
            ],
        ),
        migrations.CreateModel(
            name='CoffeeAuthenication',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('timestamp', models.DateTimeField(auto_now_add=True)),
                ('token', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='authentication.tokens')),
            ],
        ),
    ]
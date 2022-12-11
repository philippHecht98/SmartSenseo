from django.db import models

# Create your models here.
from django.db import models

class Configuration(models.Model):
    rfid = models.CharField(max_length=20, primary_key=True)

    SIZE = (('S', 'SINGLE'), ('D', 'DOUBLE'))
    cup_size = models.CharField(max_length=1, choices=SIZE)

    def __str__(self) -> str:
        return super().__str__()


class CoffeeEntry(models.Model):    
    config = models.ForeignKey(Configuration, on_delete=models.CASCADE)
    timestamp = models.DateTimeField(auto_now_add=True, auto_now=False)

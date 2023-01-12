from django.db import models

# Create your models here.
from django.db import models

class CoffeeEntry(models.Model):    
    rfid = models.CharField(max_length=50, blank=False)
    timestamp = models.DateTimeField(auto_now_add=True, auto_now=False)

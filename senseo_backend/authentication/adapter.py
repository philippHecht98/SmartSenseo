from .models import CoffeeEntry
from django.db.models import Count

class DatabaseAdapter():

    def add_new_coffee_entry(rfid):
        CoffeeEntry.objects.create(rfid=rfid)

    def view_coffee_entries(lower_border):
        return list(CoffeeEntry.objects
            .values('rfid')
            .annotate(sum=Count('rfid'))
            .filter(timestamp>= lower_border)
            )

        
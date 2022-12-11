from .models import Configuration, CoffeeEntry

class DatabaseAdapter():

    def add_new_configuration(rfid, cup_size):
        Configuration.objects.create(rfid=rfid, cup_size=cup_size)

    def add_new_coffee_entry(rfid):
        configuration = DatabaseAdapter.get_configuration(rfid)
        print(configuration)
        CoffeeEntry.objects.create(
            config=configuration)

    def check_rfid_exists(rfid):
        object = Configuration.objects.filter(rfid=rfid)

        return object.exists()

    def get_configuration(rfid):
        configuration = Configuration.objects.filter(rfid=rfid)[0]
        return configuration
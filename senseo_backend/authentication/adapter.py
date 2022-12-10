from .models import Configuration

class DatabaseAdapter():

    def create_new_configuration():
        pass

    def add_new_configuration(rfid, cup_size):
        Configuration.objects.create(rfid=rfid, cup_size=cup_size)

    def add_new_coffee_entry():
        pass

    def check_rfid_exists(rfid):
        object = Configuration.objects.filter(rfid=rfid)
        print(object.exists())
        return object.exists()

    def get_configuration(rfid):
        configuration = Configuration.objects.filter(rfid=rfid)[0]
        return configuration
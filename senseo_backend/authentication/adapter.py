from .models import RFID
from .serializers import RFIDSerializer

class DatabaseAdapter():

    def create_new_configuration():
        pass

    def add_new_rfid(rfid):
        serializer = RFIDSerializer({'rfid': rfid})
        if serializer.is_valid():
            serializer.save()
            return DatabaseAdapter.get_rfid_entry(rfid)
        else:
            return serializer.errors

    def add_new_coffee_entry():
        pass

    def check_rfid_is_new(rfid):
        DatabaseAdapter.get_rfid_entry(rfid).exists()

    def get_configuration(rfid):
        pass

    def get_rfid_entry(rfid):
        return RFID.objects.filter(rfid=rfid)
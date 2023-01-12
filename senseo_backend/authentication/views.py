from rest_framework.views import APIView
from rest_framework.response import Response
from rest_framework import status

from .adapter import DatabaseAdapter

import datetime
import json
import base64

# add a new coffe for a rfid token
class CoffeeEntryAPI(APIView):

    def post(self, request, *args, **kwargs):
        try:
            rfid = request.data.get('rfid')        

            DatabaseAdapter.add_new_coffee_entry(rfid)
            return Response(status=status.HTTP_201_CREATED)

        except ValueError:
            return Response(status=status.HTTP_400_BAD_REQUEST)

    
    # get all coffe entries grouped by rfid from given time till now
    def get(self, request, *args, **kwargs):

        lower_border = request.data.get('lower_border')
        print(datetime.datetime.strptime(
                lower_border, '%Y-%m-%d %H:%M:%S'))
        data = DatabaseAdapter.view_coffee_entries(
            datetime.datetime.strptime(
                lower_border, '%Y-%m-%d %H:%M:%S')
            )
        results = []
        for entry in data:
            results.append({
                'rfid': entry['rfid'],
                'sum': entry['sum']
                })
        return Response(data=results, status=status.HTTP_200_OK)
from django.shortcuts import render
from rest_framework.views import APIView
from rest_framework.response import Response
from rest_framework import status
from .serializers import RFIDSerializer

from .models import Tokens
from .adapter import DatabaseAdapter

class RFIDAPI(APIView):
    def post(self, request, *args, **kwargs):
        rfid = request.data.get('rfid')

        response_data = None
        if DatabaseAdapter.check_rfid_is_new(rfid):
            id = DatabaseAdapter.add_new_rfid(rfid)
            response_data = {'status': 'new', 'id': id}
        else:
            DatabaseAdapter.get_configuration()
            response_data = {'status': 'exists'}
        return Response(data=response_data, status=status.HTTP_200_OK)

        #return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)

class ConfigurationAPI(APIView):
    def post(self, request, *args, **kwargs):
        pass
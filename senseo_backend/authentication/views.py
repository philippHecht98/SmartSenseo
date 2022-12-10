from django.shortcuts import render
from django.core import serializers
from rest_framework.views import APIView
from rest_framework.response import Response
from rest_framework import status

from .models import Configuration
from .adapter import DatabaseAdapter

class RFIDAPI(APIView):

    # get if the rfid token is already in the database
    def get(self, request, *args, **kwargs):
        rfid = request.data.get('rfid')

        response_data = None
        if not DatabaseAdapter.check_rfid_exists(rfid):
            response_data = {'status': 'new'}
        else:
            response_data = {'status': 'exists'}
        return Response(data=response_data, status=status.HTTP_200_OK)

        #return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)


class ConfigurationAPI(APIView):

    # create a new configuration for a given rfid
    def post(self, request, *args, **kwargs):
        rfid = request.data.get('rfid')

        if DatabaseAdapter.check_rfid_exists():
            return Response(status=status.HTTP_208_ALREADY_REPORTED)

        cup_size = request.data.get('cup_size')
        DatabaseAdapter.add_new_configuration(rfid, cup_size)
        return Response(status=status.HTTP_201_CREATED)

    # get the configuration of a given rfid
    def get(self, request, *args, **kwargs):
        rfid = request.data.get('rfid')

        if not DatabaseAdapter.check_rfid_exists(rfid):
            return Response(status=status.HTTP_404_NOT_FOUND)

        configuration = DatabaseAdapter.get_configuration(rfid)
        print(configuration)
        return Response(
            data={'cup_size': configuration.cup_size},
            status=status.HTTP_200_OK
        )
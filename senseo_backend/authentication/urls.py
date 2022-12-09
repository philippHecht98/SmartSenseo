from django.contrib import admin
from django.urls import path
from . import views


urlpatterns = [
    path('rfid', views.RFIDAPI.as_view()),
    path('conf', views.ConfigurationAPI.as_view())
]

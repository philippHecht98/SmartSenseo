from django.contrib import admin
from django.urls import path
from . import views


urlpatterns = [
    path('coffee', views.CoffeeEntryAPI.as_view())
]

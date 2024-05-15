# admin.py

from django.contrib import admin
from .models import Campsite, Guest, Reservation, Service, Payment

admin.site.register(Campsite)
admin.site.register(Guest)
admin.site.register(Reservation)
admin.site.register(Service)
admin.site.register(Payment)

# Copyright (c) Timur Iskhakov.
# Distributed under the terms of the MIT License.


from django.conf.urls import url
from django.contrib import admin

from ads import views


urlpatterns = [
    url(r'^admin/', admin.site.urls),
    url(r'^$', views.index),
]

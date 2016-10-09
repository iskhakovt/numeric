# Copyright (c) Timur Iskhakov.
# Distributed under the terms of the MIT License.


from django.conf.urls import url
from django.contrib import admin

from ads import views


urlpatterns = [
    url(r'^admin/', admin.site.urls),
    url(r'^$', views.index),
    url(r'^problem/', views.index),
    url(r'^result/', views.index),
    url(r'^api/problems/$', views.api_problems),
    url(r'^api/problem/(?P<problemName>[-\w]+)/$', views.api_problem),
    url(r'^api/query/(?P<problemName>[-\w]+)/$', views.api_query),
    url(r'^api/result/(?P<id>[\w]{32})/$', views.api_result),
]

# Copyright (c) Timur Iskhakov.
# Distributed under the terms of the MIT License.


from django.shortcuts import render

from .docker import Docker


def index(request):
    docker = Docker()
    docker.run.delay()
    return

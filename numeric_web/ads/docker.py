# Copyright (c) Timur Iskhakov.
# Distributed under the terms of the MIT License.


from __future__ import absolute_import

from numeric_web.celery import app
from celery.contrib.methods import task_method


class Docker:
    def __init__(self):
        pass

    @app.task(filter=task_method)
    def run(self, docker, *args):
        pass

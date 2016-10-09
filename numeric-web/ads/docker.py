# Copyright (c) Timur Iskhakov.
# Distributed under the terms of the MIT License.


from __future__ import absolute_import

from timeit import default_timer

from numeric_web.celery import app
from celery.contrib.methods import task_method


class Docker:
    def __init__(self, query, func, args):
        self.query = query
        self.func = func
        self.args = args

    @app.task(filter=task_method)
    def run(self, docker, *args):
        start = default_timer()
        res = self.func(self.args)
        end = default_timer()

        self.query.time = end - start
        self.query.result = res
        self.query.save()

# Copyright (c) Timur Iskhakov.
# Distributed under the terms of the MIT License.


from __future__ import absolute_import

from celery.contrib.methods import task_method
import json
from timeit import default_timer
from traceback import format_exc

from numeric_web.celery import app
from numeric_web.settings import DEBUG


class Docker:
    def __init__(self, query, func, args):
        self.query = query
        self.func = func
        self.args = args

    def fail(self, err):
        self.query.status = 'FL'
        self.query.result = {'error': err}
        self.query.save()

    @app.task(filter=task_method, bind=True)
    def run(docker, self):
        start = default_timer()

        try:
            res = self.func(self.args)
        except Exception as err:
            self.fail(format_exc() if DEBUG else str(err))
            return

        end = default_timer()

        self.query.status = 'OK'
        self.query.time = end - start
        self.query.result = res
        self.query.save()

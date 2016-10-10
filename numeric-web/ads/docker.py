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

    def fail(self, err):
        self.query.status = 'FL'
        self.query.result = {'error', str(err)}
        self.query.save()

    @app.task(filter=task_method, bind=True)
    def run(docker, self):
        start = default_timer()
        
        try:
            res = self.func(self.args)
        except Exception as err:
            self.fail(err)
            return

        end = default_timer()

        self.status = 'OK'
        self.query.time = end - start
        self.query.result = res
        self.query.save()
